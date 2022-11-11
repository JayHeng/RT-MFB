/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mfb_nor_flash.h"
#if WINBOND_DEVICE_SERIES
#include "mfb_nor_flash_winbond.h"
#endif
#if MXIC_DEVICE_SERIES
#include "mfb_nor_flash_mxic.h"
#endif
#if GIGADEVICE_DEVICE_SERIE
#include "mfb_nor_flash_gigadevice.h"
#endif
#if ISSI_DEVICE_SERIES
#include "mfb_nor_flash_issi.h"
#endif
#if MICRON_DEVICE_SERIES
#include "mfb_nor_flash_micron.h"
#endif
#if ADESTO_DEVICE_SERIE
#include "mfb_nor_flash_adesto.h"
#endif
#include "fsl_debug_console.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern const uint32_t s_customLUT_WINBOND_Quad[CUSTOM_LUT_LENGTH];
extern const uint32_t s_customLUT_MXIC_Quad[CUSTOM_LUT_LENGTH];
extern const uint32_t s_customLUT_MXIC_Octal[CUSTOM_LUT_LENGTH];
extern const uint32_t s_customLUT_GIGADEVICE_Quad[CUSTOM_LUT_LENGTH];
extern const uint32_t s_customLUT_GIGADEVICE_Octal[CUSTOM_LUT_LENGTH];
extern const uint32_t s_customLUT_ISSI_Quad[CUSTOM_LUT_LENGTH];
extern const uint32_t s_customLUT_ISSI_Octal[CUSTOM_LUT_LENGTH];
extern const uint32_t s_customLUT_MICRON_Quad[CUSTOM_LUT_LENGTH];
extern const uint32_t s_customLUT_MICRON_Octal[CUSTOM_LUT_LENGTH];
extern const uint32_t s_customLUT_ADESTO_Quad[CUSTOM_LUT_LENGTH];

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint8_t s_flashVendorIDs[] = FLASH_DEVICE_VENDOR_ID_LIST;

#if MFB_FLASH_MEMCPY_PERF_ENABLE | MFB_FLASH_PATTERN_VERIFY_ENABLE
uint32_t g_flashRwBuffer[FLASH_PAGE_SIZE/4];
#endif

flash_property_info_t g_flashPropertyInfo;

flexspi_device_config_t g_deviceconfig = {
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

const uint32_t s_customLUTCommonMode[CUSTOM_LUT_LENGTH] = {
    /*  Normal read */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 0] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x03, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR,  kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Read status register */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_1PAD, 0x04),

    /* Write Enable */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0X00),

    /* Read ID */
    [4 * NOR_CMD_LUT_SEQ_IDX_READID] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x9F, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_1PAD, 0x04),

    /* Read ID - QPI_1 */
    [4 * NOR_CMD_LUT_SEQ_IDX_READID_QPI_1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_4PAD, 0xAF, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_4PAD, 0x04),

    /* Read ID - QPI_2 */
    [4 * NOR_CMD_LUT_SEQ_IDX_READID_QPI_2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_4PAD, 0x9F, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_4PAD, 0x04),

    /* Read ID - OPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_READID_OPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_8PAD, 0x9F, kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 0x06),
    [4 * NOR_CMD_LUT_SEQ_IDX_READID_OPI + 1] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR,  kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Dummy write, do nothing when AHB write command is triggered. */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Erase Sector */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x20, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),

    /* Page Program - single mode */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x02, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),
};

/*******************************************************************************
 * Code
 ******************************************************************************/

int mfb_printf(const char *fmt_s, ...)
{
#if MFB_DEBUG_LOG_INFO_ENABLE
    PRINTF(fmt_s);
#endif
    
    return 0;
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

void mfb_flexspi_common_init(flash_inst_mode_t flashInstMode)
{
    switch (flashInstMode)
    {
        case kFlashInstMode_QPI_1:
        case kFlashInstMode_QPI_2:
            /* Init FlexSPI pinmux */
            flexspi_pin_init(EXAMPLE_FLEXSPI,    FLASH_PORT, kFLEXSPI_4PAD);
            /* Init FlexSPI using common LUT */ 
            flexspi_nor_flash_init(EXAMPLE_FLEXSPI, s_customLUTCommonMode, kFLEXSPI_ReadSampleClkLoopbackInternally);
            mfb_printf("MFB: FLEXSPI module is initialized to Quad-I/O for default QPI SDR mode.\r\n");
            break;

        case kFlashInstMode_OPI:
            /* Init FlexSPI pinmux */
            flexspi_pin_init(EXAMPLE_FLEXSPI,    FLASH_PORT, kFLEXSPI_8PAD);
            /* Init FlexSPI using common LUT */ 
            flexspi_nor_flash_init(EXAMPLE_FLEXSPI, s_customLUTCommonMode, kFLEXSPI_ReadSampleClkExternalInputFromDqsPad);
            mfb_printf("MFB: FLEXSPI module is initialized to Octal-I/O for default OPI DDR mode.\r\n");
            break;

        case kFlashInstMode_SPI:
        default:
            /* Init FlexSPI pinmux */
            flexspi_pin_init(EXAMPLE_FLEXSPI,    FLASH_PORT, kFLEXSPI_1PAD);
            /* Init FlexSPI using common LUT */ 
            flexspi_nor_flash_init(EXAMPLE_FLEXSPI, s_customLUTCommonMode, kFLEXSPI_ReadSampleClkLoopbackInternally);
            mfb_printf("\r\nMFB: FLEXSPI module is initialized to 1bit SPI SDR normal read mode.\r\n");
            break;
    }
}

void mfb_main(void)
{
    status_t status = kStatus_Success;
    jedec_id_t jedecID;
    flash_inst_mode_t sta_flashInstMode = kFlashInstMode_SPI;

    mfb_printf("MFB: i.MXRT multi-flash boot solution.\r\n");
    /* Show CPU clock source */
    cpu_show_clock_source();
    /* Switch FlexSPI port if needed */
    flexspi_port_switch(EXAMPLE_FLEXSPI, FLASH_PORT, kFLEXSPI_1PAD);
    /* Move FlexSPI clock to a stable clock source */ 
    flexspi_clock_init(EXAMPLE_FLEXSPI, kFlexspiRootClkFreq_30MHz);
    /* Show FlexSPI clock source */
    flexspi_show_clock_source(EXAMPLE_FLEXSPI);

    /* Get JEDEC ID. */
#if MFB_FLASH_FAKE_JEDEC_ID_ENABLE
    jedecID.manufacturerID = MXIC_QUAD_FLASH_JEDEC_ID & 0xFF;
    jedecID.memoryTypeID = (MXIC_QUAD_FLASH_JEDEC_ID >> 8) & 0xFF;
    jedecID.capacityID = (MXIC_QUAD_FLASH_JEDEC_ID >> 16) & 0xFF;
#else
    while (sta_flashInstMode < kFlashInstMode_MAX)
    {
        /* Init FlexSPI using common LUT */ 
        mfb_flexspi_common_init(sta_flashInstMode);
        /* Read JEDEC id from flash */
        status = flexspi_nor_get_jedec_id(EXAMPLE_FLEXSPI, (uint32_t *)&jedecID, sta_flashInstMode);
        if (status != kStatus_Success)
        {
            mfb_printf("MFB: Get Flash Vendor ID failed");
        }
        else
        {
            uint32_t idx;
            for (idx = 0; idx < sizeof(s_flashVendorIDs); idx++)
            {
                if (jedecID.manufacturerID == s_flashVendorIDs[idx])
                {
                    break;
                }
            }
            if (idx != sizeof(s_flashVendorIDs))
            {
                mfb_printf("MFB: Get Valid Flash Vendor ID.\r\n");
                break;
            }
            else
            {
                mfb_printf("MFB: Get Invalid Flash Vendor ID 0x%x", jedecID.manufacturerID);
            }
        }
        switch (sta_flashInstMode)
        {
            case kFlashInstMode_OPI:
                mfb_printf(" under OPI DDR mode.\r\n");
                sta_flashInstMode = kFlashInstMode_MAX;
                break;

            case kFlashInstMode_QPI_2:
                mfb_printf(" under QPI_2 SDR mode.\r\n");
                sta_flashInstMode = kFlashInstMode_OPI;
                break;

            case kFlashInstMode_QPI_1:
                mfb_printf(" under QPI_1 SDR mode.\r\n");
                sta_flashInstMode = kFlashInstMode_QPI_2;
                break;

            case kFlashInstMode_SPI:
            default:
                mfb_printf(" under Std/Ext SPI mode.\r\n");
                sta_flashInstMode = kFlashInstMode_QPI_1;
                break;
        }
    }
    if (status == kStatus_Success)
#endif
    {
        bool sta_hasQpiSupport = false;
        bool sta_isOctalFlash = false;
        bool sta_isValidVendorId = true;
        /* Set default paramenters */
        flexspi_pad_t cfg_pad = kFLEXSPI_4PAD;
        flexspi_root_clk_freq_t cfg_rootClkFreq = kFlexspiRootClkFreq_100MHz;
        flexspi_read_sample_clock_t cfg_readSampleClock = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
        const uint32_t *cfg_customLUTVendor;
        uint32_t cfg_dummyValue = DUMMY_VALUE_INVALID;
        /* Get real flash size according to jedec id result (it may not be appliable to some specifal adesto device) */
        uint32_t cfg_flashMemSizeInByte = mfb_decode_common_capacity_id(jedecID.capacityID);
        /* Only run 1st perf and pattern verify when default flash state is Ext SPI mode */
        if (sta_flashInstMode == kFlashInstMode_SPI)
        {
            /* Do patten verify test under 1bit SPI mode */
            mfb_flash_pattern_verify_test(false);
            /* Get perf test result under 1bit SPI mode */
            mfb_flash_memcpy_perf_test();
        }
        mfb_printf("MFB: Flash Manufacturer ID: 0x%x", jedecID.manufacturerID);
        /* Check Vendor ID. */
        switch (jedecID.manufacturerID)
        {
#if WINBOND_DEVICE_SERIES
            // Winbond
            case WINBOND_DEVICE_VENDOR_ID:
                {
                    mfb_printf(" -- Winbond Serial Flash.\r\n");
                    mfb_printf("MFB: Flash Memory Type ID: 0x%x", jedecID.memoryTypeID);
                    switch (jedecID.memoryTypeID)
                    {
                        /////////////////////////QuadSPI////////////////////////
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
                        ////////////////////////OctalSPI////////////////////////
                        // Missing W25H, W25M, W25R
                        // Missing xxxJL, xxxDW, xxxRV
                        default:
                            mfb_printf(" -- Unsupported Series.\r\n");
                            break;
                    }
                    mfb_flash_show_mem_size(jedecID.capacityID, false);
#if WINBOND_DEVICE_QUAD
                    if (!sta_isOctalFlash)
                    {
                        cfg_pad                 = kFLEXSPI_4PAD;
                        cfg_rootClkFreq         = kFlexspiRootClkFreq_100MHz;
                        cfg_readSampleClock     = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
                        g_flashPropertyInfo.flashBusyStatusPol    = WINBOND_FLASH_BUSY_STATUS_POL;
                        g_flashPropertyInfo.flashBusyStatusOffset = WINBOND_FLASH_BUSY_STATUS_OFFSET;
                        g_flashPropertyInfo.flashQuadEnableCfg    = WINBOND_FLASH_QUAD_ENABLE;
                        g_flashPropertyInfo.flashQuadEnableBytes  = 1;
                        cfg_customLUTVendor     = s_customLUT_WINBOND_Quad;
                    }
#endif
                    break;
                }
#endif // WINBOND_DEVICE_SERIES

#if MXIC_DEVICE_SERIES
            // MXIC
            case MXIC_DEVICE_VENDOR_ID:
                {
                    mfb_printf(" -- MXIC Serial Flash.\r\n");
                    mfb_printf("MFB: Flash Memory Type ID: 0x%x", jedecID.memoryTypeID);
                    switch (jedecID.memoryTypeID)
                    {
                        /////////////////////////QuadSPI////////////////////////
                        case 0x20:
                            mfb_printf(" -- MX25L/MX66L/MX25V QuadSPI 3.3V Series.\r\n");
                            break;
                        case 0x25:
                            mfb_printf(" -- MX25U/MX66U QuadSPI 1.8V Series.\r\n");
                            sta_hasQpiSupport = true;
                            break;
                        case 0x28:
                            mfb_printf(" -- MX25R QuadSPI 1.8-3.3V Series.\r\n");
                            break;
                        case 0x75:
                            mfb_printf(" -- MX77L QuadSPI 3.3V Series.\r\n");
                            break;
                        ////////////////////////OctalSPI////////////////////////
                        case 0x80:
                            sta_isOctalFlash = true;
                            mfb_printf(" -- MX25UM/MX66UM OctalSPI 1.8V Series.\r\n");
                            break;
                        case 0x81:
                            sta_isOctalFlash = true;
                            mfb_printf(" -- MX25UM51345G OctalSPI 1.8V Series.\r\n");
                            break;
                        case 0x83:
                            sta_isOctalFlash = true;
                            mfb_printf(" -- MX25UM25345G OctalSPI 1.8V Series.\r\n");
                            break;
                        case 0x84:
                            sta_isOctalFlash = true;
                            mfb_printf(" -- MX25UW51345G OctalSPI 1.8V Series.\r\n");
                            break;
                        case 0x85:
                            sta_isOctalFlash = true;
                            mfb_printf(" -- MX25LM/MX66LM OctalSPI 3.3V Series.\r\n");
                            break;
                        // Missing MX25LW51245G, MX66LW1G45G, MX66LW2G45G
                        // Missing MX25UW6445G, MX66UW12845G, MX25UW25645G, MX25UW51245G, MX66UW1G45G, MX66UW2G45G
                        // Missing MX25UW6345G, MX66LW12345G, MX66UW25345G      done                 , MX66UW2G345G
                        default:
                            mfb_printf(" -- Unsupported Series.\r\n");
                            break;
                    }
                    mfb_flash_show_mem_size(jedecID.capacityID, false);
#if MXIC_DEVICE_QUAD
                    if (!sta_isOctalFlash)
                    {
                        cfg_pad                 = kFLEXSPI_4PAD;
                        cfg_rootClkFreq         = kFlexspiRootClkFreq_80MHz;
                        cfg_readSampleClock     = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
                        g_flashPropertyInfo.flashBusyStatusPol    = MXIC_FLASH_BUSY_STATUS_POL;
                        g_flashPropertyInfo.flashBusyStatusOffset = MXIC_FLASH_BUSY_STATUS_OFFSET;
                        g_flashPropertyInfo.flashQuadEnableCfg    = MXIC_FLASH_QUAD_ENABLE;
                        g_flashPropertyInfo.flashQuadEnableBytes  = 1;
                        cfg_customLUTVendor     = s_customLUT_MXIC_Quad;
                    }
#endif
#if MXIC_DEVICE_OCTAL
                    if (sta_isOctalFlash)
                    {
                        cfg_pad                 = kFLEXSPI_8PAD;
                        cfg_rootClkFreq         = kFlexspiRootClkFreq_166MHz;
                        cfg_readSampleClock     = kFLEXSPI_ReadSampleClkExternalInputFromDqsPad;
                        g_flashPropertyInfo.flashBusyStatusPol    = MXIC_FLASH_BUSY_STATUS_POL;
                        g_flashPropertyInfo.flashBusyStatusOffset = MXIC_FLASH_BUSY_STATUS_OFFSET;
                        g_flashPropertyInfo.flashEnableOctalCmd   = MXIC_OCTAL_FLASH_ENABLE_DDR_CMD;
                        cfg_customLUTVendor     = s_customLUT_MXIC_Octal;
                        if (cfg_rootClkFreq == kFlexspiRootClkFreq_200MHz)
                        {
                            cfg_dummyValue = MXIC_OCTAL_FLASH_SET_DUMMY_CMD;
                        }
                    }
#endif
                    break;
                }
#endif // MXIC_DEVICE_SERIES

#if GIGADEVICE_DEVICE_SERIE
            // GigaDevice
            case GIGADEVICE_DEVICE_VENDOR_ID:
                {
                    mfb_printf(" -- GigaDevice Serial Flash.\r\n");
                    mfb_printf("MFB: Flash Memory Type ID: 0x%x", jedecID.memoryTypeID);
                    switch (jedecID.memoryTypeID)
                    {
                        /////////////////////////QuadSPI////////////////////////
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
                        ////////////////////////OctalSPI////////////////////////
                        case 0x68:
                            sta_isOctalFlash = true;
                            mfb_printf(" -- GD25LX/GD55LX OctalSPI 1.8V Series.\r\n");
                            break;
                        // Missing GD25F, GD25LR, GD25T, GD25R
                        // Missing GD25X,
                        default:
                            mfb_printf(" -- Unsupported Series.\r\n");
                            break;
                    }
                    mfb_flash_show_mem_size(jedecID.capacityID, false);
#if GIGADEVICE_DEVICE_Quad
                    if (!sta_isOctalFlash)
                    {
                        cfg_pad                 = kFLEXSPI_4PAD;
                        cfg_rootClkFreq         = kFlexspiRootClkFreq_120MHz;
                        cfg_readSampleClock     = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
                        g_flashPropertyInfo.flashBusyStatusPol    = GIGADEVICE_FLASH_BUSY_STATUS_POL;
                        g_flashPropertyInfo.flashBusyStatusOffset = GIGADEVICE_FLASH_BUSY_STATUS_OFFSET;
                        g_flashPropertyInfo.flashQuadEnableCfg    = GIGADEVICE_FLASH_QUAD_ENABLE;
                        g_flashPropertyInfo.flashQuadEnableBytes  = 2;
                        cfg_customLUTVendor     = s_customLUT_GIGADEVICE_Quad;
                    }
#endif
#if GIGADEVICE_DEVICE_OCTAL
                    if (sta_isOctalFlash)
                    {
                        cfg_pad                 = kFLEXSPI_8PAD;
                        g_flashPropertyInfo.flashBusyStatusPol    = GIGADEVICE_FLASH_BUSY_STATUS_POL;
                        g_flashPropertyInfo.flashBusyStatusOffset = GIGADEVICE_FLASH_BUSY_STATUS_OFFSET;
                        g_flashPropertyInfo.flashEnableOctalCmd   = GIGADEVICE_OCTAL_FLASH_ENABLE_DDR_CMD;
                        cfg_customLUTVendor     = s_customLUT_GIGADEVICE_Octal;
#if MFB_FLASH_OPI_MODE_DISABLE
                        cfg_rootClkFreq         = kFlexspiRootClkFreq_30MHz;
                        cfg_readSampleClock     = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
#else
                        cfg_rootClkFreq         = kFlexspiRootClkFreq_166MHz;
                        cfg_readSampleClock     = kFLEXSPI_ReadSampleClkExternalInputFromDqsPad;
                        if (cfg_rootClkFreq == kFlexspiRootClkFreq_200MHz)
#endif
                        {
                            cfg_dummyValue = GIGADEVICE_OCTAL_FLASH_SET_DUMMY_CMD;
                        }
                    }
#endif
                    break;
                }
#endif // GIGADEVICE_DEVICE_SERIE

#if ISSI_DEVICE_SERIES
            // ISSI
            case ISSI_DEVICE_VENDOR_ID:
                {
                    mfb_printf(" -- ISSI Serial Flash.\r\n");
                    mfb_printf("MFB: Flash Memory Type ID: 0x%x", jedecID.memoryTypeID);
                    switch (jedecID.memoryTypeID)
                    {
                        /////////////////////////QuadSPI////////////////////////
                        case 0x40:
                            mfb_printf(" -- IS25LQ/IS25LP QuadSPI 3.3V Series.\r\n");
                            break;
                        case 0x60:
                            mfb_printf(" -- IS25LP/IS25LE QuadSPI 3.3V Series.\r\n");
                            break;
                        case 0x70:
                            mfb_printf(" -- IS25WP/IS25WJ/IS25WE QuadSPI 1.8V Series.\r\n");
                            break;
                        ////////////////////////OctalSPI////////////////////////
                        case 0x5A:
                            sta_isOctalFlash = true;
                            mfb_printf(" -- IS25LX OctalSPI 3.3V Series.\r\n");
                            break;
                        case 0x5B:
                            sta_isOctalFlash = true;
                            mfb_printf(" -- IS25WX OctalSPI 1.8V Series.\r\n");
                            break;
                        default:
                            mfb_printf(" -- Unsupported Series.\r\n");
                            break;
                    }
                    mfb_flash_show_mem_size(jedecID.capacityID, false);
#if ISSI_DEVICE_QUAD
                    if (!sta_isOctalFlash)
                    {
                        cfg_pad                 = kFLEXSPI_4PAD;
                        cfg_rootClkFreq         = kFlexspiRootClkFreq_80MHz;
                        cfg_readSampleClock     = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
                        g_flashPropertyInfo.flashBusyStatusPol    = ISSI_FLASH_BUSY_STATUS_POL;
                        g_flashPropertyInfo.flashBusyStatusOffset = ISSI_FLASH_BUSY_STATUS_OFFSET;
                        g_flashPropertyInfo.flashQuadEnableCfg    = ISSI_FLASH_QUAD_ENABLE;
                        g_flashPropertyInfo.flashQuadEnableBytes  = 1;
                        cfg_customLUTVendor     = s_customLUT_ISSI_Quad;
                    }
#endif
#if ISSI_DEVICE_OCTAL
                    if (sta_isOctalFlash)
                    {
                        cfg_pad                 = kFLEXSPI_8PAD;
                        g_flashPropertyInfo.flashBusyStatusPol    = ISSI_FLASH_BUSY_STATUS_POL;
                        g_flashPropertyInfo.flashBusyStatusOffset = ISSI_FLASH_BUSY_STATUS_OFFSET;
                        g_flashPropertyInfo.flashEnableOctalCmd   = ISSI_OCTAL_FLASH_ENABLE_DDR_CMD;
                        cfg_customLUTVendor     = s_customLUT_ISSI_Octal;
#if MFB_FLASH_OPI_MODE_DISABLE
                        cfg_rootClkFreq         = kFlexspiRootClkFreq_30MHz;
                        cfg_readSampleClock     = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
#else
                        cfg_rootClkFreq         = kFlexspiRootClkFreq_166MHz;
                        cfg_readSampleClock     = kFLEXSPI_ReadSampleClkExternalInputFromDqsPad;
                        if (cfg_rootClkFreq == kFlexspiRootClkFreq_200MHz)
#endif
                        {
                            cfg_dummyValue = ISSI_OCTAL_FLASH_SET_DUMMY_CMD;
                        }
                    }
#endif
                    break;
                }
#endif // ISSI_DEVICE_SERIES

#if MICRON_DEVICE_SERIES
            // Micron
            case MICRON_DEVICE_VENDOR_ID:
            case MICRON_DEVICE_VENDOR_ID2:
                {
                    mfb_printf(" -- Micron Serial Flash.\r\n");
                    mfb_printf("MFB: Flash Memory Type ID: 0x%x", jedecID.memoryTypeID);
                    switch (jedecID.memoryTypeID)
                    {
                        /////////////////////////QuadSPI////////////////////////
                        case 0xBA:
                            mfb_printf(" -- MT25QL QuadSPI 3.3V Series.\r\n");
                            break;
                        case 0xBB:
                            mfb_printf(" -- MT25QU QuadSPI 1.8V Series.\r\n");
                            break;
                        ////////////////////////OctalSPI////////////////////////
                        case 0x5A:
                            sta_isOctalFlash = true;
                            mfb_printf(" -- MT35XL OctalSPI 3.3V Series.\r\n");
                            break;
                        case 0x5B:
                            sta_isOctalFlash = true;
                            mfb_printf(" -- MT35XU OctalSPI 1.8V Series.\r\n");
                            break;
                        default:
                            mfb_printf(" -- Unsupported Series.\r\n");
                            break;
                    }
                    mfb_flash_show_mem_size(jedecID.capacityID, false);
#if MICRON_DEVICE_QUAD
                    if (!sta_isOctalFlash)
                    {
                        cfg_pad                 = kFLEXSPI_4PAD;
                        cfg_rootClkFreq         = kFlexspiRootClkFreq_120MHz;
                        cfg_readSampleClock     = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
                        g_flashPropertyInfo.flashBusyStatusPol    = MICRON_FLASH_BUSY_STATUS_POL;
                        g_flashPropertyInfo.flashBusyStatusOffset = MICRON_FLASH_BUSY_STATUS_OFFSET;
                        //g_flashPropertyInfo.flashQuadEnableCfg    = MICRON_FLASH_QUAD_ENABLE;
                        cfg_customLUTVendor     = s_customLUT_MICRON_Quad;
                        /* No need to enable quad mode for micron device. */
                    }
#endif
#if MICRON_DEVICE_OCTAL
                    if (sta_isOctalFlash)
                    {
                        cfg_pad                 = kFLEXSPI_8PAD;
                        g_flashPropertyInfo.flashBusyStatusPol    = MICRON_FLASH_BUSY_STATUS_POL;
                        g_flashPropertyInfo.flashBusyStatusOffset = MICRON_FLASH_BUSY_STATUS_OFFSET;
                        g_flashPropertyInfo.flashEnableOctalCmd   = MICRON_OCTAL_FLASH_ENABLE_DDR_CMD;
                        cfg_customLUTVendor     = s_customLUT_MICRON_Octal;
#if MFB_FLASH_OPI_MODE_DISABLE
                        cfg_rootClkFreq         = kFlexspiRootClkFreq_30MHz;
                        cfg_readSampleClock     = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
#else
                        cfg_rootClkFreq         = kFlexspiRootClkFreq_166MHz;
                        cfg_readSampleClock     = kFLEXSPI_ReadSampleClkExternalInputFromDqsPad;
                        if (cfg_rootClkFreq == kFlexspiRootClkFreq_200MHz)
#endif
                        {
                            cfg_dummyValue = MICRON_OCTAL_FLASH_SET_DUMMY_CMD;
                        }
                    }
#endif
                    break;
                }
#endif // MICRON_DEVICE_SERIES

#if ADESTO_DEVICE_SERIE
            // Adesto
            case ADESTO_DEVICE_VENDOR_ID:
            case ADESTO_DEVICE_VENDOR_ID2:
                {
                    mfb_printf(" -- Adesto Serial Flash.\r\n");
                    if (jedecID.memoryTypeID != 0x42)
                    {
                        jedecID.capacityID = jedecID.memoryTypeID & 0x1F;
                        jedecID.memoryTypeID = (jedecID.memoryTypeID & 0xE0) >> 5;
                        mfb_printf("MFB: Flash Family Code: 0x%x", jedecID.memoryTypeID);
                    }
                    else
                    {
                        mfb_printf("MFB: Flash Memory Type ID: 0x%x", jedecID.memoryTypeID);
                    }
                    switch (jedecID.memoryTypeID)
                    {
                        /////////////////////////QuadSPI////////////////////////
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
                        ////////////////////////OctalSPI////////////////////////
                        case 0x05:
                            sta_isOctalFlash = true;
                            mfb_printf(" -- ATXP OctalSPI 1.8V Series.\r\n");
                            break;
                        default:
                            mfb_printf(" -- Unsupported Series.\r\n");
                            break;
                    }
                    if (jedecID.memoryTypeID != 0x42)
                    {
                        mfb_flash_show_mem_size(jedecID.capacityID, true);
                        cfg_flashMemSizeInByte = mfb_decode_adesto_capacity_id(jedecID.capacityID);
                    }
                    else
                    {
                        mfb_flash_show_mem_size(jedecID.capacityID, false);
                        cfg_flashMemSizeInByte = mfb_decode_common_capacity_id(jedecID.capacityID);
                    }
#if ADESTO_DEVICE_QUAD
                    if (!sta_isOctalFlash)
                    {
                        cfg_pad                 = kFLEXSPI_4PAD;
                        cfg_rootClkFreq         = kFlexspiRootClkFreq_133MHz;
                        cfg_readSampleClock     = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
                        g_flashPropertyInfo.flashBusyStatusPol    = ADESTO_FLASH_BUSY_STATUS_POL;
                        g_flashPropertyInfo.flashBusyStatusOffset = ADESTO_FLASH_BUSY_STATUS_OFFSET;
                        g_flashPropertyInfo.flashQuadEnableCfg    = ADESTO_FLASH_QUAD_ENABLE;
                        g_flashPropertyInfo.flashQuadEnableBytes  = 1;
                        cfg_customLUTVendor     = s_customLUT_ADESTO_Quad;
                    }
#endif
                    break;
                }
#endif // ADESTO_DEVICE_SERIE

            default:
                mfb_printf("\r\nMFB: Unsupported Manufacturer ID\r\n");
                sta_isValidVendorId = false;
                break;
        }
        if (sta_isValidVendorId)
        {
            /* Configure FlexSPI pinmux as user prescriptive */
            flexspi_pin_init(EXAMPLE_FLEXSPI, FLASH_PORT, cfg_pad);
            /* Update root clock */
            //s_deviceconfig.flexspiRootClk = flexspi_get_clock(EXAMPLE_FLEXSPI);
            g_deviceconfig.flashSize = cfg_flashMemSizeInByte / 0x400;
            /* Re-init FlexSPI using custom LUT */
            flexspi_nor_flash_init(EXAMPLE_FLEXSPI, cfg_customLUTVendor, cfg_readSampleClock);
            mfb_printf("\r\nMFB: FLEXSPI module is initialized to multi-I/O fast read mode.\r\n");
            /* Write dummy cycle value into flash if needed */
            if (cfg_dummyValue != DUMMY_VALUE_INVALID)
            {
                flexspi_nor_set_dummy_cycle(EXAMPLE_FLEXSPI, (uint8_t)(cfg_dummyValue & 0xFF));
                mfb_printf("MFB: Flash dummy cycle is set to %d.\r\n", cfg_dummyValue);
            }
            if (!sta_isOctalFlash)
            {
                if (sta_flashInstMode == kFlashInstMode_SPI)
                {
#if MFB_FLASH_QPI_MODE_ENABLE
                    if (sta_hasQpiSupport)
                    {
                        /* Enter QPI SDR mode. */
                        status = flexspi_nor_enable_qpi_mode(EXAMPLE_FLEXSPI);
                        if (status != kStatus_Success)
                        {
                            mfb_printf("MFB: Flash failed to Enter QPI SDR mode.\r\n");
                        }
                        else
                        {
                            sta_flashInstMode = kFlashInstMode_QPI_1;
                            mfb_printf("MFB: Flash entered QPI SDR mode.\r\n");
                        }
                    }
                    else
#endif
                    {
                        /* Enable quad mode. */
                        status = flexspi_nor_enable_quad_mode(EXAMPLE_FLEXSPI);
                        if (status != kStatus_Success)
                        {
                            mfb_printf("MFB: Flash failed to Enter Quad I/O SDR mode.\r\n");
                        }
                        else
                        {
                            mfb_printf("MFB: Flash entered Quad I/O SDR mode.\r\n");
                        }
                    }
                }
                else
                {
                    mfb_printf("MFB: Flash remained in default QPI SDR mode.\r\n");
                }
            }
            else
            {
                /* Only When defult flash is Ext SPI mode, Enter OPI DDR mode then */
                if (sta_flashInstMode != kFlashInstMode_OPI)
                {
                    /* Enter OPI DDR mode. */
#if !MFB_FLASH_OPI_MODE_DISABLE
                    status = flexspi_nor_enable_opi_mode(EXAMPLE_FLEXSPI);
                    if (status != kStatus_Success)
                    {
                        mfb_printf("MFB: Flash failed to Enter OPI DDR mode.\r\n");
                    }
                    else
                    {
                        sta_flashInstMode = kFlashInstMode_OPI;
                        mfb_printf("MFB: Flash entered OPI DDR mode.\r\n");
                    }
#else
                    mfb_printf("MFB: Flash ran in Octal I/O SPI mode.\r\n");
#endif
                }
                else
                {
#if !MFB_FLASH_OPI_MODE_DISABLE
                    mfb_printf("MFB: Flash remained in default OPI DDR mode.\r\n");
#else
#warning "Do not support loopback dqs option when flash default state in OPI DDR"
#endif
                }
            }
            if (status == kStatus_Success)
            {
                mfb_flash_show_registers();
                /* Do patten verify test under Multi I/O fast read mode */
                uint32_t round = 1;
                while (round < 2)
                {
                    bool showError = (round == 2);
                    /* Don't show error info when it is 1st round, as flash may be blank this time */
                    if (!mfb_flash_pattern_verify_test(showError))
                    {
                         /* Try to write pattern into flash when error occurs in 1st round */
                         if (round == 1)
                         {
                             if (!mfb_flash_write_pattern_region(sta_flashInstMode))
                             {
                                 return;
                             }
                         }
                         else
                         {
                             return;
                         }
                    }
                    /* Increase speed for 2nd round */
                    if (round != 2)
                    {
                        /* Configure FlexSPI clock as user prescriptive */ 
                        flexspi_clock_init(EXAMPLE_FLEXSPI, cfg_rootClkFreq);
                        /* Show FlexSPI clock source */
                        flexspi_show_clock_source(EXAMPLE_FLEXSPI);
                        round = 2;
                    }
                }
                /* Get perf test result under Multi I/O fast read mode */
                mfb_flash_memcpy_perf_test();
                /* Jump into user application */
                mfb_jump_to_application(EXAMPLE_FLEXSPI_AMBA_BASE + MFB_APP_IMAGE_OFFSET);
            }
        }
    }
}
