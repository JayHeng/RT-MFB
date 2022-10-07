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
extern const uint32_t customLUT_ISSI[CUSTOM_LUT_LENGTH];
extern const uint32_t customLUT_MXIC[CUSTOM_LUT_LENGTH];

extern status_t flexspi_nor_get_jedec_id(FLEXSPI_Type *base, uint32_t *jedecId);
extern status_t flexspi_nor_enable_octal_mode(FLEXSPI_Type *base);
extern status_t flexspi_nor_enable_quad_mode(FLEXSPI_Type *base);
extern void flexspi_nor_flash_init(FLEXSPI_Type *base, const uint32_t *customLUT, flexspi_read_sample_clock_t rxSampleClock);
/*******************************************************************************
 * Variables
 ******************************************************************************/
#if MFB_FLASH_SPEED_TEST_ENABLE
static uint8_t s_nor_read_buffer[256];
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
#if MFB_LOG_INFO_ENABLE
    PRINTF(fmt_s);
#endif
    
    return 0;
}

#if MFB_FLASH_SPEED_TEST_ENABLE
void mfb_flash_speed_test(void)
{
    uint64_t startTicks = microseconds_get_ticks();
    /* Read 8MB data from flash to test speed */
    for (uint32_t loop = 0; loop < 16 * 8; loop++)
    {
        /* Min NOR Flash size is 64KB */
        for (uint32_t idx = 0; idx < 256; idx++)
        {
            memcpy(s_nor_read_buffer, (uint8_t*)(EXAMPLE_FLEXSPI_AMBA_BASE + idx * sizeof(s_nor_read_buffer)), sizeof(s_nor_read_buffer));
        }
    }
    uint64_t totalTicks = microseconds_get_ticks() - startTicks;
    uint32_t microSecs = microseconds_convert_to_microseconds(totalTicks);
    mfb_printf("MFB: Flash to RAM memcpy speed: %dKB/s.\r\n", (8UL*1024*1000000)/microSecs);
}
#endif

void mfb_jump_to_application(uint32_t vectorStartAddr)
{
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
}

uint32_t mfb_decode_capacity_id(uint8_t capacityID)
{
    uint32_t memSizeInBytes = 0;
    //| ISSI QuadSPI    |  MXIC OctalSPI
    //|---------------------------------
    //| 09h - 256Kb     |
    //| 10h - 512Kb     |
    //| 11h - 1Mb       |
    //| 12h - 2Mb       |
    //| 13h - 4Mb       |
    //| 14h - 8Mb       |
    //| 15h - 16Mb      |
    //| 16h - 32Mb      |
    //| 17h - 64Mb      |  37h - 64Mb
    //| 18h - 128Mb     |  38h - 128Mb
    //| 19h - 256Mb     |  39h - 256Mb
    //| 1ah - 512Mb     |  3ah - 512Mb
    //| 1bh - 1Gb       |  3bh - 1Gb
    //| 1ch - 2Gb       |  3ch - 2Gb
    if (capacityID <= 0x09)
    {
        memSizeInBytes = 1 << (capacityID + 6);
    }
    else if (capacityID >= 0x10)
    {
        memSizeInBytes = 1 << (capacityID & 0x1F);
    }
    return memSizeInBytes;
}

void mfb_show_mem_size(uint8_t capacityID)
{
    mfb_printf("MFB: Flash Capacity ID: 0x%x", capacityID);
    uint32_t flashMemSizeInKB = mfb_decode_capacity_id(capacityID)/ 0x400;
    if (flashMemSizeInKB <= 0x400)
    {
        mfb_printf(" -- %dKB.\r\n", flashMemSizeInKB);
    }
    else
    {
        mfb_printf(" -- %dMB.\r\n", flashMemSizeInKB / 0x400);
    }
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

    /* Move FlexSPI clock to a stable clock source */ 
    flexspi_clock_init(kFlexspiRootClkFreq_30MHz);
    /* Init FlexSPI using common LUT */ 
    flexspi_nor_flash_init(EXAMPLE_FLEXSPI, customLUTCommonMode, kFLEXSPI_ReadSampleClkLoopbackInternally);
    mfb_printf("MFB: FLEXSPI module initialized!\r\n");
    mfb_printf("MFB: FLEXSPI Clk Frequency: %dHz.\r\n", flexspi_get_clock(EXAMPLE_FLEXSPI));

    /* Get JEDEC ID. */
    status = flexspi_nor_get_jedec_id(EXAMPLE_FLEXSPI, &jedecID);
    if (status != kStatus_Success)
    {
        mfb_printf("MFB: Get Flash Vendor ID failed.\r\n");
    }
    else
    {
        bool isValidVendorId = true;
        manufacturerID = jedecID & 0xFF;
        memoryTypeID = (jedecID >> 8) & 0xFF;
        capacityID = (jedecID >> 16) & 0xFF;
        uint32_t flashMemSizeInByte = mfb_decode_capacity_id(capacityID);

#if MFB_FLASH_SPEED_TEST_ENABLE
        microseconds_init();
        mfb_flash_speed_test();
#endif
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
                        case 0x80:
                            mfb_printf(" -- MX25(66)UMxxx45G Series.\r\n");
                            break;
                        case 0x81:
                            mfb_printf(" -- MX25UM51345G Series.\r\n");
                            break;
                        case 0x83:
                            mfb_printf(" -- MX25UM25345G Series.\r\n");
                            break;
                        case 0x84:
                            mfb_printf(" -- MX25UW51345G Series.\r\n");
                            break;
                        case 0x85:
                            mfb_printf(" -- MX25(66)LMxxx45G Series.\r\n");
                            break;
                        // Missing MX25LW51245G, MX66LW1G45G, MX66LW2G45G
                        // Missing MX25UW6445G, MX66UW12845G, MX25UW25645G, MX25UW51245G, MX66UW1G45G, MX66UW2G45G
                        // Missing MX25UW6345G, MX66LW12345G, MX66UW25345G      done                 , MX66UW2G345G
                        default:
                            mfb_printf(" -- Unsupported Series.\r\n");
                            break;
                    }
                    mfb_show_mem_size(capacityID);
#if MXIC_DEVICE_MX25UM51345
                    flexspi_clock_init(kFlexspiRootClkFreq_100MHz);
                    /* Update root clock */
                    deviceconfig.flexspiRootClk = 99000000;
                    deviceconfig.flashSize = flashMemSizeInByte / 0x400;
                    s_flashBusyStatusPol    = MXIC_FLASH_BUSY_STATUS_POL;
                    s_flashBusyStatusOffset = MXIC_FLASH_BUSY_STATUS_OFFSET;
                    s_flashEnableOctalCmd   = MXIC_FLASH_ENABLE_OCTAL_CMD;
                    /* Re-init FlexSPI using custom LUT */
                    flexspi_nor_flash_init(EXAMPLE_FLEXSPI, customLUT_MXIC, kFLEXSPI_ReadSampleClkExternalInputFromDqsPad);
                    /* Enter octal mode. */
                    status = flexspi_nor_enable_octal_mode(EXAMPLE_FLEXSPI);
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
                            mfb_printf(" -- IS25L(Q/P) Series.\r\n");
                            break;
                        case 0x60:
                            mfb_printf(" -- IS25L(P/E) Series.\r\n");
                            break;
                        case 0x70:
                            mfb_printf(" -- IS25W(P/J/E) Series.\r\n");
                            break;
                        default:
                            mfb_printf(" -- Unsupported Series.\r\n");
                            break;
                    }
                    mfb_show_mem_size(capacityID);
#if ISSI_DEVICE_IS25WP064A
                    flexspi_clock_init(kFlexspiRootClkFreq_100MHz);
                    /* Update root clock */
                    deviceconfig.flexspiRootClk = 100000000;
                    deviceconfig.flashSize = flashMemSizeInByte / 0x400;
                    s_flashBusyStatusPol    = ISSI_FLASH_BUSY_STATUS_POL;
                    s_flashBusyStatusOffset = ISSI_FLASH_BUSY_STATUS_OFFSET;
                    s_flashQuadEnableCfg    = ISSI_FLASH_QUAD_ENABLE;
                    /* Re-init FlexSPI using custom LUT */
                    flexspi_nor_flash_init(EXAMPLE_FLEXSPI, customLUT_ISSI, kFLEXSPI_ReadSampleClkLoopbackFromDqsPad);
                    /* Enter quad mode. */
                    status = flexspi_nor_enable_quad_mode(EXAMPLE_FLEXSPI);
#endif
                    break;
                }
#endif // ISSI_DEVICE_SERIES

            default:
                mfb_printf("\r\nMFB: Unsupported Manufacturer ID\r\n");
                isValidVendorId = false;
                break;
        }
        if (isValidVendorId)
        {
            if (status != kStatus_Success)
            {
                mfb_printf("MFB: Flash failed to Enter Octal/Quad mode.\r\n");
            }
            else
            {
                mfb_printf("MFB: Flash entered Octal/Quad mode.\r\n");
                mfb_printf("MFB: FLEXSPI Clk Frequency: %dHz.\r\n", flexspi_get_clock(EXAMPLE_FLEXSPI));
#if MFB_FLASH_SPEED_TEST_ENABLE
                mfb_flash_speed_test();
                microseconds_shutdown();
#endif
                mfb_printf("MFB: Jump to Application code at 0x%x.\r\n", EXAMPLE_FLEXSPI_AMBA_BASE + MFB_APP_IMAGE_OFFSET);
                mfb_printf("-------------------------------------\r\n");
                mfb_jump_to_application(EXAMPLE_FLEXSPI_AMBA_BASE + MFB_APP_IMAGE_OFFSET);
            }
        }
    }
    mfb_printf("-------------------------------------\r\n");
}
