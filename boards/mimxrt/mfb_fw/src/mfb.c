/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mfb.h"
#include "microseconds.h"
#include "port_flexspi_info.h"
#include "mfb_nor_flash.h"
#if MXIC_DEVICE_SERIES
#include "mfb_nor_flash_mxic.h"
#endif
#if ISSI_DEVICE_SERIES
#include "mfb_nor_flash_issi.h"
#endif
#if MICRON_DEVICE_SERIES
#include "mfb_nor_flash_micron.h"
#endif
#if WINBOND_DEVICE_SERIES
#include "mfb_nor_flash_winbond.h"
#endif
#if ADESTO_DEVICE_SERIE
#include "mfb_nor_flash_adesto.h"
#endif
#include "fsl_flexspi.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern const uint32_t customLUT_ISSI_Quad[CUSTOM_LUT_LENGTH];
extern const uint32_t customLUT_ISSI_Octal[CUSTOM_LUT_LENGTH];
extern const uint32_t customLUT_MXIC_Octal[CUSTOM_LUT_LENGTH];
extern const uint32_t customLUT_MICRON_Quad[CUSTOM_LUT_LENGTH];
extern const uint32_t customLUT_MICRON_Octal[CUSTOM_LUT_LENGTH];
extern const uint32_t customLUT_WINBOND_Quad[CUSTOM_LUT_LENGTH];
extern const uint32_t customLUT_ADESTO_Quad[CUSTOM_LUT_LENGTH];

extern status_t flexspi_nor_get_jedec_id(FLEXSPI_Type *base, uint32_t *jedecId);
extern status_t flexspi_nor_set_dummy_cycle(FLEXSPI_Type *base, uint8_t dummyCmd);
extern status_t flexspi_nor_enable_octal_mode(FLEXSPI_Type *base);
extern status_t flexspi_nor_enable_quad_mode(FLEXSPI_Type *base);
extern void flexspi_nor_flash_init(FLEXSPI_Type *base, const uint32_t *customLUT, flexspi_read_sample_clock_t rxSampleClock);
/*******************************************************************************
 * Variables
 ******************************************************************************/
#if MFB_FLASH_MEMCPY_PERF_ENABLE
static uint8_t s_nor_read_buffer[FLASH_PAGE_SIZE];
#endif

uint8_t s_flashBusyStatusPol    = 0;
uint8_t s_flashBusyStatusOffset = 0;
uint8_t s_flashQuadEnableCfg    = 0;
uint8_t s_flashEnableOctalCmd   = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
flexspi_device_config_t deviceconfig = {
    .flexspiRootClk       = 27400000,
    .flashSize            = 0x2000, /* 64Mb/KByte */
    .CSIntervalUnit       = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval           = 2,
    .CSHoldTime           = 3,
    .CSSetupTime          = 3,
    .dataValidTime        = 2,
    .columnspace          = 0,
    .enableWordAddress    = 0,
    .AWRSeqIndex          = NOR_CMD_LUT_SEQ_IDX_WRITE,
    .AWRSeqNumber         = 1,
    .ARDSeqIndex          = NOR_CMD_LUT_SEQ_IDX_READ,
    .ARDSeqNumber         = 1,
    .AHBWriteWaitUnit     = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
    .AHBWriteWaitInterval = 0,
};

const uint32_t customLUTCommonMode[CUSTOM_LUT_LENGTH] = {
    /*  Normal read */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 0] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,      kFLEXSPI_1PAD, 0x03, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Read status register */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,      kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_1PAD, 0x04),

    /* Write Enable */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,      kFLEXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0X00),

    /* Read ID */
    [4 * NOR_CMD_LUT_SEQ_IDX_READID] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,      kFLEXSPI_1PAD, 0x9F, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_1PAD, 0x04),

    /*  Dummy write, do nothing when AHB write command is triggered. */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_STOP,     kFLEXSPI_1PAD, 0x00, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),
};

int mfb_printf(const char *fmt_s, ...)
{
#if MFB_DEBUG_LOG_INFO_ENABLE
    PRINTF(fmt_s);
#endif
    
    return 0;
}

void mfb_flash_memcpy_perf_test(bool isFirstTime)
{
#if MFB_FLASH_MEMCPY_PERF_ENABLE
    if (isFirstTime)
    {
        microseconds_init();
    }

    uint64_t startTicks = microseconds_get_ticks();
    uint64_t totalSize = (8UL*1024*1024);
    uint32_t loopMax = totalSize / MFB_FLASH_ACCESS_REGION_SIZE;
    uint32_t unitSize = sizeof(s_nor_read_buffer);
    uint32_t idxMax = MFB_FLASH_ACCESS_REGION_SIZE / unitSize;
    /* Read 8MB data from flash to test speed */
    for (uint32_t loop = 0; loop < loopMax; loop++)
    {
        /* Min NOR Flash size is 64KB */
        for (uint32_t idx = 0; idx < idxMax; idx++)
        {
            memcpy(s_nor_read_buffer, (uint8_t*)(EXAMPLE_FLEXSPI_AMBA_BASE + idx * unitSize), unitSize);
        }
    }
    uint64_t totalTicks = microseconds_get_ticks() - startTicks;
    uint32_t microSecs = microseconds_convert_to_microseconds(totalTicks);
    uint32_t kBps = (totalSize / 1024) * 1000000 / microSecs;
    mfb_printf("MFB: Flash to RAM memcpy speed: %dKB/s.\r\n", kBps);

    if (!isFirstTime)
    {
        microseconds_shutdown();
    }
#endif
}

void mfb_jump_to_application(uint32_t vectorStartAddr)
{
#if MFB_APP_JUMP_ENABLE
    mfb_printf("\r\nMFB: Jump to Application code at 0x%x.\r\n", EXAMPLE_FLEXSPI_AMBA_BASE + MFB_APP_IMAGE_OFFSET);
    mfb_printf("-------------------------------------\r\n");
    static uint32_t s_resetEntry = 0;
    static uint32_t s_stackPointer = 0;
    s_resetEntry = *(uint32_t *)(vectorStartAddr + 4);
    s_stackPointer = *(uint32_t *)vectorStartAddr;
    // Turn off interrupts.
    __disable_irq();

    // Set the VTOR.
    SCB->VTOR = vectorStartAddr;

    // Memory barriers for good measure.
    __ISB();
    __DSB();

    // Set main stack pointer and process stack pointer.
    __set_MSP(s_stackPointer);
    __set_PSP(s_stackPointer);

    // Jump to application entry point, does not return.
    static void (*s_entry)(void) = 0;
    s_entry = (void (*)(void))s_resetEntry;
    s_entry();
#endif
}

uint32_t mfb_decode_common_capacity_id(uint8_t capacityID)
{
    uint32_t memSizeInBytes = 0;
    //| ISSI QuadSPI       |  MXIC OctalSPI     |  Micron QuadSPI    |
    //| ISSI OctalSPI      |  MXIC QuadSPI U    |GigaDevice QuadSPI Q|
    //| MXIC QuadSPI R/L/V |                    |                    |
    //| Winbond QuadSPI    |                    |                    |
    //| Micron OctalSPI    |                    |                    |
    //| GigaDevice QuadSPI |                    |                    |
    //| GigaDevice OctalSPI|                    |                    |
    //|Adesto QuadSPI SL/QL|                    |                    |
    //|---------------------------------------------------------------
    //| 09h - 256Kb        |                    |                    |
    //| 10h - 512Kb        |                    |                    |
    //| 11h - 1Mb          |                    |                    |
    //| 12h - 2Mb          |                    |                    |
    //| 13h - 4Mb          |                    |                    |
    //| 14h - 8Mb          |                    |                    |
    //| 15h - 16Mb         |                    |                    |
    //| 16h - 32Mb         |                    |                    |
    //| 17h - 64Mb         |  37h - 64Mb        |   17h - 64Mb       |
    //| 18h - 128Mb        |  38h - 128Mb       |   18h - 128Mb      |
    //| 19h - 256Mb        |  39h - 256Mb       |   19h - 256Mb      |
    //| 1ah - 512Mb        |  3ah - 512Mb       |   20h - 512Mb      |
    //| 1bh - 1Gb          |  3bh - 1Gb         |   21h - 1Gb        |
    //| 1ch - 2Gb          |  3ch - 2Gb         |   22h - 2Gb        |
    if (capacityID <= 0x09)
    {
        capacityID += 6;
    }
    else if (capacityID <= 0x1c)
    {
        // Do Nothing
    }
    else if (capacityID <= 0x22)
    {
        capacityID -= 6;
    }
    else
    {
        capacityID &= 0x1F;
    }
    memSizeInBytes = 1 << capacityID;
    return memSizeInBytes;
}

uint32_t mfb_decode_adesto_capacity_id(uint8_t capacityID)
{
    uint32_t memSizeInBytes = 0;
    //|Adesto QuadSPI      | Adesto QuadSPI EU  |
    //|------------------------------------------
    //|                    | 10h - 1Mb          |
    //| 03h - 2Mb          | 11h - 2Mb          |
    //| 04h - 4Mb          | 14h - 4Mb          |
    //| 05h - 8Mb          |                    |
    //| 06h - 16Mb         |                    |
    //| 07h - 32Mb         |                    |
    //| 08h - 64Mb         |                    |
    //| 09h - 128Mb        |                    |
    if (capacityID <= 0x09)
    {
        capacityID += 15;
    }
    else if (capacityID == 0x10)
    {
        capacityID = 0x11;
    }
    else if (capacityID == 0x11)
    {
        capacityID = 0x12;
    }
    else if (capacityID == 0x14)
    {
        capacityID = 0x13;
    }
    memSizeInBytes = 1 << capacityID;
    return memSizeInBytes;
}

void mfb_show_mem_size(uint8_t capacityID, bool isAdesto)
{
#if MFB_DEBUG_LOG_INFO_ENABLE
    uint32_t flashMemSizeInKB;
    if (isAdesto)
    {
        mfb_printf("MFB: Flash Density Code: 0x%x", capacityID);
        flashMemSizeInKB = mfb_decode_adesto_capacity_id(capacityID)/ 0x400;
    }
    else
    {
        mfb_printf("MFB: Flash Capacity ID: 0x%x", capacityID);
        flashMemSizeInKB = mfb_decode_common_capacity_id(capacityID)/ 0x400;
    }
    if (flashMemSizeInKB <= 0x400)
    {
        mfb_printf(" -- %dKB.\r\n", flashMemSizeInKB);
    }
    else
    {
        mfb_printf(" -- %dMB.\r\n", flashMemSizeInKB / 0x400);
    }
#endif
}

void mfb_main(void)
{
    status_t status;
    uint32_t jedecID = 0;
    uint8_t manufacturerID = 0;
    uint8_t memoryTypeID = 0;
    uint8_t capacityID = 0;

    mfb_printf("\r\n-------------------------------------\r\n");
    mfb_printf("MFB: i.MXRT multi-flash boot solution.\r\n");
    cpu_show_clock_source();
    
    /* Init FlexSPI pinmux */
    flexspi_port_switch(EXAMPLE_FLEXSPI, FLASH_PORT, kFLEXSPI_2PAD);
    flexspi_pin_init(EXAMPLE_FLEXSPI, FLASH_PORT, kFLEXSPI_2PAD);

    /* Move FlexSPI clock to a stable clock source */ 
    flexspi_clock_init(EXAMPLE_FLEXSPI, kFlexspiRootClkFreq_30MHz);
    /* Init FlexSPI using common LUT */ 
    flexspi_nor_flash_init(EXAMPLE_FLEXSPI, customLUTCommonMode, kFLEXSPI_ReadSampleClkLoopbackInternally);
    mfb_printf("\r\nMFB: FLEXSPI module is initialized to 1bit SDR normal read mode.\r\n");
    flexspi_show_clock_source(EXAMPLE_FLEXSPI);

    /* Get JEDEC ID. */
#if MFB_FLASH_FAKE_JEDEC_ID_ENABLE
    jedecID = MICRON_OCTAL_FLASH_JEDEC_ID;
#else
    status = flexspi_nor_get_jedec_id(EXAMPLE_FLEXSPI, &jedecID);
    if (status != kStatus_Success)
    {
        mfb_printf("MFB: Get Flash Vendor ID failed.\r\n");
    }
    else
#endif
    {
        bool isOctalFlash = false;
        bool isValidVendorId = true;
        bool isFirstTime = true;
        bool isAdestoDevice = false;
        uint8_t dummyValue = 0;
        manufacturerID = jedecID & 0xFF;
        memoryTypeID = (jedecID >> 8) & 0xFF;
        capacityID = (jedecID >> 16) & 0xFF;
        uint32_t flashMemSizeInByte = mfb_decode_common_capacity_id(capacityID);
        mfb_flash_memcpy_perf_test(isFirstTime);
        isFirstTime = false;
        mfb_printf("MFB: Flash Manufacturer ID: 0x%x", manufacturerID);
        switch (manufacturerID)
        {
#if MXIC_DEVICE_SERIES
            // MXIC
            case 0xc2:
                {
                    mfb_printf(" -- MXIC Serial Flash.\r\n");
                    mfb_printf("MFB: Flash Memory Type ID: 0x%x", memoryTypeID);
                    switch (memoryTypeID)
                    {
                        case 0x20:
                            mfb_printf(" -- MX25L/MX66L/MX25V QuadSPI 3.3V Series.\r\n");
                            break;
                        case 0x25:
                            mfb_printf(" -- MX25U/MX66U QuadSPI 1.8V Series.\r\n");
                            break;
                        case 0x28:
                            mfb_printf(" -- MX25R QuadSPI 1.8-3.3V Series.\r\n");
                            break;
                        case 0x75:
                            mfb_printf(" -- MX77L QuadSPI 3.3V Series.\r\n");
                            break;
                        case 0x80:
                            isOctalFlash = true;
                            mfb_printf(" -- MX25UM/MX66UM OctalSPI 1.8V Series.\r\n");
                            break;
                        case 0x81:
                            isOctalFlash = true;
                            mfb_printf(" -- MX25UM51345G OctalSPI 1.8V Series.\r\n");
                            break;
                        case 0x83:
                            isOctalFlash = true;
                            mfb_printf(" -- MX25UM25345G OctalSPI 1.8V Series.\r\n");
                            break;
                        case 0x84:
                            isOctalFlash = true;
                            mfb_printf(" -- MX25UW51345G OctalSPI 1.8V Series.\r\n");
                            break;
                        case 0x85:
                            isOctalFlash = true;
                            mfb_printf(" -- MX25LM/MX66LM OctalSPI 3.3V Series.\r\n");
                            break;
                        // Missing MX25LW51245G, MX66LW1G45G, MX66LW2G45G
                        // Missing MX25UW6445G, MX66UW12845G, MX25UW25645G, MX25UW51245G, MX66UW1G45G, MX66UW2G45G
                        // Missing MX25UW6345G, MX66LW12345G, MX66UW25345G      done                 , MX66UW2G345G
                        default:
                            mfb_printf(" -- Unsupported Series.\r\n");
                            break;
                    }
                    mfb_show_mem_size(capacityID, isAdestoDevice);
#if MXIC_DEVICE_MX25UM51345
                    if (isOctalFlash)
                    {
                        flexspi_root_clk_freq_t rootClkFreq = kFlexspiRootClkFreq_100MHz;
                        flexspi_pin_init(EXAMPLE_FLEXSPI, FLASH_PORT, kFLEXSPI_8PAD);
                        flexspi_clock_init(EXAMPLE_FLEXSPI, rootClkFreq);
                        /* Update root clock */
                        deviceconfig.flexspiRootClk = flexspi_get_clock(EXAMPLE_FLEXSPI);
                        deviceconfig.flashSize = flashMemSizeInByte / 0x400;
                        s_flashBusyStatusPol    = MXIC_FLASH_BUSY_STATUS_POL;
                        s_flashBusyStatusOffset = MXIC_FLASH_BUSY_STATUS_OFFSET;
                        s_flashEnableOctalCmd   = MXIC_OCTAL_FLASH_ENABLE_DDR_CMD;
                        /* Re-init FlexSPI using custom LUT */
                        flexspi_nor_flash_init(EXAMPLE_FLEXSPI, customLUT_MXIC_Octal, kFLEXSPI_ReadSampleClkExternalInputFromDqsPad);
                        // Set dummy cycle of Flash
                        if (rootClkFreq == kFlexspiRootClkFreq_200MHz)
                        {
                            dummyValue = MXIC_OCTAL_FLASH_SET_DUMMY_CMD;
                            flexspi_nor_set_dummy_cycle(EXAMPLE_FLEXSPI, MXIC_OCTAL_FLASH_SET_DUMMY_CMD);
                        }
                        /* Enter octal mode. */
                        status = flexspi_nor_enable_octal_mode(EXAMPLE_FLEXSPI);
                    }
#endif
                    break;
                }
#endif // MXIC_DEVICE_SERIES

#if ISSI_DEVICE_SERIES
            // ISSI
            case 0x9d:
                {
                    mfb_printf(" -- ISSI Serial Flash.\r\n");
                    mfb_printf("MFB: Flash Memory Type ID: 0x%x", memoryTypeID);
                    switch (memoryTypeID)
                    {
                        case 0x40:
                            mfb_printf(" -- IS25LQ/IS25LP QuadSPI 3.3V Series.\r\n");
                            break;
                        case 0x60:
                            mfb_printf(" -- IS25LP/IS25LE QuadSPI 3.3V Series.\r\n");
                            break;
                        case 0x70:
                            mfb_printf(" -- IS25WP/IS25WJ/IS25WE QuadSPI 1.8V Series.\r\n");
                            break;
                        case 0x5A:
                            isOctalFlash = true;
                            mfb_printf(" -- IS25LX OctalSPI 3.3V Series.\r\n");
                            break;
                        case 0x5B:
                            isOctalFlash = true;
                            mfb_printf(" -- IS25WX OctalSPI 1.8V Series.\r\n");
                            break;
                        default:
                            mfb_printf(" -- Unsupported Series.\r\n");
                            break;
                    }
                    mfb_show_mem_size(capacityID, isAdestoDevice);
#if ISSI_DEVICE_IS25WP064A
                    if (!isOctalFlash)
                    {
                        flexspi_pin_init(EXAMPLE_FLEXSPI, FLASH_PORT, kFLEXSPI_4PAD);
                        flexspi_clock_init(EXAMPLE_FLEXSPI, kFlexspiRootClkFreq_100MHz);
                        /* Update root clock */
                        deviceconfig.flexspiRootClk = flexspi_get_clock(EXAMPLE_FLEXSPI);
                        deviceconfig.flashSize = flashMemSizeInByte / 0x400;
                        s_flashBusyStatusPol    = ISSI_FLASH_BUSY_STATUS_POL;
                        s_flashBusyStatusOffset = ISSI_FLASH_BUSY_STATUS_OFFSET;
                        s_flashQuadEnableCfg    = ISSI_FLASH_QUAD_ENABLE;
                        /* Re-init FlexSPI using custom LUT */
                        flexspi_nor_flash_init(EXAMPLE_FLEXSPI, customLUT_ISSI_Quad, kFLEXSPI_ReadSampleClkLoopbackFromDqsPad);
                        /* Enter quad mode. */
                        status = flexspi_nor_enable_quad_mode(EXAMPLE_FLEXSPI);
                    }
#endif
#if ISSI_DEVICE_IS25WX256
                    if (isOctalFlash)
                    {
#if MFB_FLASH_FORCE_LOOPBACK_DQS
                        flexspi_root_clk_freq_t rootClkFreq = kFlexspiRootClkFreq_30MHz;
#else
                        flexspi_root_clk_freq_t rootClkFreq = kFlexspiRootClkFreq_166MHz;
#endif
                        flexspi_pin_init(EXAMPLE_FLEXSPI, FLASH_PORT, kFLEXSPI_8PAD);
                        flexspi_clock_init(EXAMPLE_FLEXSPI, rootClkFreq);
                        /* Update root clock */
                        deviceconfig.flexspiRootClk = flexspi_get_clock(EXAMPLE_FLEXSPI);
                        deviceconfig.flashSize = flashMemSizeInByte / 0x400;
                        s_flashBusyStatusPol    = ISSI_FLASH_BUSY_STATUS_POL;
                        s_flashBusyStatusOffset = ISSI_FLASH_BUSY_STATUS_OFFSET;
                        s_flashEnableOctalCmd   = ISSI_OCTAL_FLASH_ENABLE_DDR_CMD;
                        /* Re-init FlexSPI using custom LUT */
#if MFB_FLASH_FORCE_LOOPBACK_DQS
                        flexspi_nor_flash_init(EXAMPLE_FLEXSPI, customLUT_ISSI_Octal, kFLEXSPI_ReadSampleClkLoopbackFromDqsPad);
#else
                        flexspi_nor_flash_init(EXAMPLE_FLEXSPI, customLUT_ISSI_Octal, kFLEXSPI_ReadSampleClkExternalInputFromDqsPad);
                        // Set dummy cycle of Flash
                        if (rootClkFreq == kFlexspiRootClkFreq_200MHz)
#endif
                        {
                            dummyValue = ISSI_OCTAL_FLASH_SET_DUMMY_CMD;
                            flexspi_nor_set_dummy_cycle(EXAMPLE_FLEXSPI, ISSI_OCTAL_FLASH_SET_DUMMY_CMD);
                        }
                        /* Enter octal mode. */
#if !MFB_FLASH_FORCE_LOOPBACK_DQS
                        status = flexspi_nor_enable_octal_mode(EXAMPLE_FLEXSPI);
#endif
                    }
#endif
                    break;
                }
#endif // ISSI_DEVICE_SERIES

#if WINBOND_DEVICE_SERIES
            // Winbond
            case 0xef:
                {
                    mfb_printf(" -- Winbond Serial Flash.\r\n");
                    mfb_printf("MFB: Flash Memory Type ID: 0x%x", memoryTypeID);
                    switch (memoryTypeID)
                    {
                        case 0x30:
                            mfb_printf(" -- W25X DualSPI 3.3V Series.\r\n");
                            break;
                        case 0x40:
                            mfb_printf(" -- W25QxxxDV/FV/BV/CL/JV(-IQ/JQ) QuadlSPI 3.3V Series.\r\n");
                            break;
                        case 0x60:
                            mfb_printf(" -- W25QxxxFW/EW/NW(-IQ/IN) QuadlSPI 1.8V Series.\r\n");
                            break;
                        case 0x70:
                            mfb_printf(" -- W25QxxxJV(-IM/JM) QuadlSPI 3.3V Series.\r\n");
                            break;
                        case 0x80:
                            mfb_printf(" -- W25QxxxJW/NW(-IM) QuadlSPI 1.8V Series.\r\n");
                            break;
                        // Missing W25H, W25M, W25R
                        // Missing xxxJL, xxxDW, xxxRV
                        default:
                            mfb_printf(" -- Unsupported Series.\r\n");
                            break;
                    }
                    mfb_show_mem_size(capacityID, isAdestoDevice);
#if WINBOND_DEVICE_W25Q128JW
                    if (!isOctalFlash)
                    {
                        flexspi_pin_init(EXAMPLE_FLEXSPI, FLASH_PORT, kFLEXSPI_4PAD);
                        flexspi_clock_init(EXAMPLE_FLEXSPI, kFlexspiRootClkFreq_133MHz);
                        /* Update root clock */
                        deviceconfig.flexspiRootClk = flexspi_get_clock(EXAMPLE_FLEXSPI);
                        deviceconfig.flashSize = flashMemSizeInByte / 0x400;
                        s_flashBusyStatusPol    = WINBOND_FLASH_BUSY_STATUS_POL;
                        s_flashBusyStatusOffset = WINBOND_FLASH_BUSY_STATUS_OFFSET;
                        s_flashQuadEnableCfg    = WINBOND_FLASH_QUAD_ENABLE;
                        /* Re-init FlexSPI using custom LUT */
                        flexspi_nor_flash_init(EXAMPLE_FLEXSPI, customLUT_WINBOND_Quad, kFLEXSPI_ReadSampleClkLoopbackFromDqsPad);
                        /* Enter quad mode. */
                        status = flexspi_nor_enable_quad_mode(EXAMPLE_FLEXSPI);
                    }
#endif
                    break;
                }
#endif // WINBOND_DEVICE_SERIES

#if MICRON_DEVICE_SERIES
            // Micron
            case 0x20:
                {
                    mfb_printf(" -- Micron Serial Flash.\r\n");
                    mfb_printf("MFB: Flash Memory Type ID: 0x%x", memoryTypeID);
                    switch (memoryTypeID)
                    {
                        case 0xBA:
                            mfb_printf(" -- MT25QL QuadSPI 3.3V Series.\r\n");
                            break;
                        case 0xBB:
                            mfb_printf(" -- MT25QU QuadSPI 1.8V Series.\r\n");
                            break;
                        case 0x5A:
                            isOctalFlash = true;
                            mfb_printf(" -- MT35XL OctalSPI 3.3V Series.\r\n");
                            break;
                        case 0x5B:
                            isOctalFlash = true;
                            mfb_printf(" -- MT35XU OctalSPI 1.8V Series.\r\n");
                            break;
                        default:
                            mfb_printf(" -- Unsupported Series.\r\n");
                            break;
                    }
                    mfb_show_mem_size(capacityID, isAdestoDevice);
#if MICRON_DEVICE_MT25QL256
                    if (!isOctalFlash)
                    {
                        flexspi_pin_init(EXAMPLE_FLEXSPI, FLASH_PORT, kFLEXSPI_4PAD);
                        flexspi_clock_init(EXAMPLE_FLEXSPI, kFlexspiRootClkFreq_100MHz);
                        /* Update root clock */
                        deviceconfig.flexspiRootClk = flexspi_get_clock(EXAMPLE_FLEXSPI);
                        deviceconfig.flashSize = flashMemSizeInByte / 0x400;
                        s_flashBusyStatusPol    = MICRON_FLASH_BUSY_STATUS_POL;
                        s_flashBusyStatusOffset = MICRON_FLASH_BUSY_STATUS_OFFSET;
                        /* Re-init FlexSPI using custom LUT */
                        flexspi_nor_flash_init(EXAMPLE_FLEXSPI, customLUT_MICRON_Quad, kFLEXSPI_ReadSampleClkLoopbackFromDqsPad);
                        /* No need to enable quad mode for micron device. */
                    }
#endif
#if MICRON_DEVICE_MT35XU512
                    if (isOctalFlash)
                    {
                        flexspi_root_clk_freq_t rootClkFreq = kFlexspiRootClkFreq_200MHz;
                        flexspi_pin_init(EXAMPLE_FLEXSPI, FLASH_PORT, kFLEXSPI_8PAD);
                        flexspi_clock_init(EXAMPLE_FLEXSPI, rootClkFreq);
                        /* Update root clock */
                        deviceconfig.flexspiRootClk = flexspi_get_clock(EXAMPLE_FLEXSPI);
                        deviceconfig.flashSize = flashMemSizeInByte / 0x400;
                        s_flashBusyStatusPol    = MICRON_FLASH_BUSY_STATUS_POL;
                        s_flashBusyStatusOffset = MICRON_FLASH_BUSY_STATUS_OFFSET;
                        s_flashEnableOctalCmd   = MICRON_OCTAL_FLASH_ENABLE_DDR_CMD;
                        /* Re-init FlexSPI using custom LUT */
                        flexspi_nor_flash_init(EXAMPLE_FLEXSPI, customLUT_MICRON_Octal, kFLEXSPI_ReadSampleClkExternalInputFromDqsPad);
                        // Set dummy cycle of Flash
                        if (rootClkFreq == kFlexspiRootClkFreq_200MHz)
                        {
                            dummyValue = MICRON_OCTAL_FLASH_SET_DUMMY_CMD;
                            flexspi_nor_set_dummy_cycle(EXAMPLE_FLEXSPI, MICRON_OCTAL_FLASH_SET_DUMMY_CMD);
                        }
                        /* Enter octal mode. */
                        status = flexspi_nor_enable_octal_mode(EXAMPLE_FLEXSPI);
                    }
#endif
                    break;
                }
#endif // MICRON_DEVICE_SERIES

#if GIGADEVICE_DEVICE_SERIE
            // GigaDevice
            case 0xc8:
                {
                    mfb_printf(" -- GigaDevice Serial Flash.\r\n");
                    mfb_printf("MFB: Flash Memory Type ID: 0x%x", memoryTypeID);
                    switch (memoryTypeID)
                    {
                        case 0x40:
                            // GD25D DualSPI
                            mfb_printf(" -- GD25Q/GD25B/GD25S QuadSPI 3.3V Series.\r\n");
                            break;
                        case 0x42:
                            mfb_printf(" -- GD25VQ/GD25VE QuadSPI 2.5V Series.\r\n");
                            break;
                        case 0x47:
                            mfb_printf(" -- GD55B QuadSPI 3.3V Series.\r\n");
                            break;
                        case 0x60:
                            // GD25LD DualSPI
                            mfb_printf(" -- GD25LE/GD25LQ QuadSPI 1.8V Series.\r\n");
                            break;
                        case 0x63:
                            mfb_printf(" -- GD25LF/GD55LF QuadSPI 1.8V Series.\r\n");
                            break;
                        case 0x65:
                            // GD25WD DualSPI
                            mfb_printf(" -- GD25W/GD55W QuadSPI 1.8-3.3V Series.\r\n");
                            break;
                        case 0x66:
                            mfb_printf(" -- GD25LT/GD55LT QuadSPI 1.8V Series.\r\n");
                            break;
                        case 0x67:
                            mfb_printf(" -- GD25LB/GD55LB QuadSPI 1.8V Series.\r\n");
                            break;
                        case 0x68:
                            mfb_printf(" -- GD25LX/GD55LX OctalSPI 1.8V Series.\r\n");
                            break;
                        // Missing GD25F, GD25LR, GD25T, GD25R
                        // Missing GD25X,
                        default:
                            mfb_printf(" -- Unsupported Series.\r\n");
                            break;
                    }
                    mfb_show_mem_size(capacityID, isAdestoDevice);
#if GIGADEVICE_DEVICE_GD25Q64C

#endif
                    break;
                }
#endif // GIGADEVICE_DEVICE_SERIE

#if ADESTO_DEVICE_SERIE
            // Adesto
            case 0x1f:
                {
                    mfb_printf(" -- Adesto Serial Flash.\r\n");
                    if (memoryTypeID != 0x42)
                    {
                        capacityID = memoryTypeID & 0x1F;
                        memoryTypeID = (memoryTypeID & 0xE0) >> 5;
                        mfb_printf("MFB: Flash Family Code: 0x%x", memoryTypeID);
                    }
                    else
                    {
                        mfb_printf("MFB: Flash Memory Type ID: 0x%x", memoryTypeID);
                    }
                    switch (memoryTypeID)
                    {
                        case 0x00:
                            mfb_printf(" -- AT25EU QuadSPI 1.8-3.3V Series.\r\n");
                            break;
                        case 0x01:
                            mfb_printf(" -- AT25DQ QuadSPI 2.5V Series.\r\n");
                            break;
                        case 0x02:
                            mfb_printf(" -- AT25FF/AT25XE/AT25XV QuadSPI 1.8-3.3V Series.\r\n");
                            break;
                        case 0x04:
                            mfb_printf(" -- AT25SF/AT25QF QuadSPI 3.3V Series.\r\n");
                            break;
                        // Only this type is same as other vendors
                        case 0x42:
                            mfb_printf(" -- AT25SL/AT25QL QuadSPI 1.8V Series.\r\n");
                            break;
                        default:
                            mfb_printf(" -- Unsupported Series.\r\n");
                            break;
                    }
                    if (memoryTypeID != 0x42)
                    {
                        isAdestoDevice = true;
                        mfb_show_mem_size(capacityID, isAdestoDevice);
                        flashMemSizeInByte = mfb_decode_adesto_capacity_id(capacityID);
                    }
                    else
                    {
                        mfb_show_mem_size(capacityID, isAdestoDevice);
                        flashMemSizeInByte = mfb_decode_common_capacity_id(capacityID);
                    }
#if ADESTO_DEVICE_AT25SF128A
                    if (!isOctalFlash)
                    {
                        flexspi_pin_init(EXAMPLE_FLEXSPI, FLASH_PORT, kFLEXSPI_4PAD);
                        flexspi_clock_init(EXAMPLE_FLEXSPI, kFlexspiRootClkFreq_133MHz);
                        /* Update root clock */
                        deviceconfig.flexspiRootClk = flexspi_get_clock(EXAMPLE_FLEXSPI);
                        deviceconfig.flashSize = flashMemSizeInByte / 0x400;
                        s_flashBusyStatusPol    = ADESTO_FLASH_BUSY_STATUS_POL;
                        s_flashBusyStatusOffset = ADESTO_FLASH_BUSY_STATUS_OFFSET;
                        s_flashQuadEnableCfg    = ADESTO_FLASH_QUAD_ENABLE;
                        /* Re-init FlexSPI using custom LUT */
                        flexspi_nor_flash_init(EXAMPLE_FLEXSPI, customLUT_ADESTO_Quad, kFLEXSPI_ReadSampleClkLoopbackFromDqsPad);
                        /* Enter quad mode. */
                        status = flexspi_nor_enable_quad_mode(EXAMPLE_FLEXSPI);
                    }
#endif
                    break;
                }
#endif // ADESTO_DEVICE_SERIE

            default:
                mfb_printf("\r\nMFB: Unsupported Manufacturer ID\r\n");
                isValidVendorId = false;
                break;
        }
        if (isValidVendorId)
        {
            mfb_printf("\r\nMFB: FLEXSPI module is initialized to multi-I/O fast read mode.\r\n");
            flexspi_show_clock_source(EXAMPLE_FLEXSPI);
            if (status != kStatus_Success)
            {
                if (isOctalFlash)
                {
                    mfb_printf("MFB: Flash failed to Enter Octal I/O DDR mode.\r\n");
                }
                else
                {
                    mfb_printf("MFB: Flash failed to Enter Quad I/O SDR mode.\r\n");
                }
            }
            else
            {
                if (dummyValue)
                {
                    mfb_printf("MFB: Flash dummy cycle is set to %d.\r\n", dummyValue);
                }
                if (isOctalFlash)
                {
                    mfb_printf("MFB: Flash entered Octal I/O DDR mode.\r\n");
                }
                else
                {
                    mfb_printf("MFB: Flash entered Quad I/O SDR mode.\r\n");
                }
                mfb_flash_memcpy_perf_test(isFirstTime);
                mfb_jump_to_application(EXAMPLE_FLEXSPI_AMBA_BASE + MFB_APP_IMAGE_OFFSET);
            }
        }
    }
    mfb_printf("-------------------------------------\r\n");
}
