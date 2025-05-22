/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mfb_nor_flash.h"
#include "fsl_debug_console.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/

#if !MFB_FLASH_FAKE_JEDEC_ID_ENABLE
/* All flash vendor id list */
static uint8_t s_flashVendorIDs[] = FLASH_DEVICE_VENDOR_ID_LIST;
#endif

#if MFB_FLASH_PROG_JEDEC_SFDP_ENABLE
static sfdp_table_t s_sfdp_table;
#endif

/* Flash Page buffer for r/w test */
#if MFB_FLASH_MEMCPY_PERF_ENABLE | MFB_FLASH_PATTERN_VERIFY_ENABLE
uint32_t g_flashRwBuffer[EXAMPLE_FLASH_PAGE_SIZE/4];
#endif

/* Main flash paramenter structure */
flash_property_info_t g_flashPropertyInfo;

/* Common FlexSPI config */

/* Common FlexSPI LUT */
const uint32_t s_customLUTCommonMode[CUSTOM_LUT_LENGTH] = {
    /*  Normal read */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 0] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x03, kMIXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] = 
        MIXSPI_LUT_SEQ(kMIXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0x04, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Read status register */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x05, kMIXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0x04),

    /* Write Enable */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x06, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0X00),

    /* Read ID */
    [4 * NOR_CMD_LUT_SEQ_IDX_READID] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x9F, kMIXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0x04),

    /* Read ID - QPI_1 */
    [4 * NOR_CMD_LUT_SEQ_IDX_READID_QPI_1] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_4PAD, 0xAF, kMIXSPI_Command_READ_SDR,  kMIXSPI_4PAD, 0x04),

    /* Read ID - QPI_2 */
    [4 * NOR_CMD_LUT_SEQ_IDX_READID_QPI_2] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_4PAD, 0x9F, kMIXSPI_Command_READ_SDR,  kMIXSPI_4PAD, 0x04),

    /* Read ID - OPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_READID_OPI] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_8PAD, 0x9F, kMIXSPI_Command_DUMMY_DDR, kMIXSPI_8PAD, 0x06),
    [4 * NOR_CMD_LUT_SEQ_IDX_READID_OPI + 1] = 
        MIXSPI_LUT_SEQ(kMIXSPI_Command_READ_DDR,  kMIXSPI_8PAD, 0x04, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Read SFDP */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSFDP] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x5A, kMIXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READSFDP + 1] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_DUMMY_SDR, kMIXSPI_1PAD, 0x08, kMIXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0xFF),

    /* Program Security registers or SFDP */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITESECSFDP] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x42, kMIXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITESECSFDP + 1] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0xFF, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Erase Security registers or SFDP */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECSFDP] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x44, kMIXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),

    /* Dummy write, do nothing when AHB write command is triggered. */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITE] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Erase Sector */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x20, kMIXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),

    /* Page Program - single mode */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x02, kMIXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x04, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),
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
    mfb_printf("\r\nMFB: Jump to Application code at 0x%x.\r\n", EXAMPLE_MIXSPI_AMBA_BASE + MFB_APP_IMAGE_OFFSET);
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

void mfb_mixspi_common_init(flash_inst_mode_t flashInstMode)
{
    switch (flashInstMode)
    {
        case kFlashInstMode_QPI_1:
        case kFlashInstMode_QPI_2:
            /* Init FlexSPI pinmux */
            mixspi_pin_init(EXAMPLE_MIXSPI,    EXAMPLE_MIXSPI_PORT, kMIXSPI_4PAD);
            /* Init FlexSPI using common LUT */ 
            mixspi_nor_flash_init(EXAMPLE_MIXSPI, s_customLUTCommonMode, kMIXSPI_SampClkLoopbackDummy0, flashInstMode);
            mfb_printf("MFB: FLEXSPI module is initialized to Quad-I/O for default QPI SDR mode.\r\n");
            break;

        case kFlashInstMode_OPI:
            /* Init FlexSPI pinmux */
            mixspi_pin_init(EXAMPLE_MIXSPI,    EXAMPLE_MIXSPI_PORT, kMIXSPI_8PAD);
            /* Init FlexSPI using common LUT */ 
            mixspi_nor_flash_init(EXAMPLE_MIXSPI, s_customLUTCommonMode, kMIXSPI_SampClkExtInputDqs, flashInstMode);
            mfb_printf("MFB: FLEXSPI module is initialized to Octal-I/O for default OPI DDR mode.\r\n");
            break;

        case kFlashInstMode_SPI:
        default:
            /* Init FlexSPI pinmux */
            mixspi_pin_init(EXAMPLE_MIXSPI,    EXAMPLE_MIXSPI_PORT, kMIXSPI_1PAD);
            /* Init FlexSPI using common LUT */ 
            mixspi_nor_flash_init(EXAMPLE_MIXSPI, s_customLUTCommonMode, kMIXSPI_SampClkLoopbackDummy0, flashInstMode);
            mfb_printf("MFB: FLEXSPI module is initialized to 1bit SPI SDR normal read mode.\r\n");
            break;
    }
}

void mfb_hyper_flash_test(void)
{
    status_t status = kStatus_Success;
    /* Adjust device parammenter */
    mixspi_device_config_init();
    mfb_hyperflash_set_param_for_spansion();
    g_flashPropertyInfo.flashMemSizeInByte = FLASH_SIZE * 0x400;

    /* Configure FlexSPI pinmux&clock as user prescriptive */
    mfb_printf("\r\nMFB: Set FlexSPI port to %d-bit pad.\r\n", 1u << (uint32_t)g_flashPropertyInfo.mixspiPad);
    mixspi_pin_init(EXAMPLE_MIXSPI, EXAMPLE_MIXSPI_PORT, g_flashPropertyInfo.mixspiPad);
    mfb_printf("MFB: Set FlexSPI root clock to %dMHz.\r\n", decode_mixspi_root_clk_defn(g_flashPropertyInfo.mixspiRootClkFreq));
    mixspi_clock_init(EXAMPLE_MIXSPI, g_flashPropertyInfo.mixspiRootClkFreq);
    /* Show FlexSPI clock source */
    mixspi_show_clock_source(EXAMPLE_MIXSPI);
    /* Update root clock and flash size */
    mixspi_device_config_update_rootclock(mixspi_get_clock(EXAMPLE_MIXSPI));
    mixspi_device_config_update_flashsize(g_flashPropertyInfo.flashMemSizeInByte / 0x400);
    /* Init FlexSPI using custom LUT */
    mixspi_nor_flash_init(EXAMPLE_MIXSPI, g_flashPropertyInfo.mixspiCustomLUTVendor, g_flashPropertyInfo.mixspiReadSampleClock, kFlashInstMode_Hyper);
    mfb_printf("MFB: FLEXSPI module is initialized to hyperbus read mode.\r\n");
    
    /* Get CFI device ID. */
    cfi_device_id_t cfiDeviceId;
    memset((void *)&cfiDeviceId, 0x00, sizeof(cfiDeviceId));
    status = mixspi_nor_get_cfi_id(EXAMPLE_MIXSPI, &cfiDeviceId);
    if (status != kStatus_Success)
    {
        mfb_printf("MFB: Get Flash CFI device ID failed");
    }
    else
    {
        mfb_hyperflash_show_info_for_spansion(&cfiDeviceId);
    }

    /* Don't show error info when it is 1st round, as flash may be blank this time */
    if (!mfb_flash_pattern_verify_test(false))
    {
         /* Try to write pattern into flash when error occurs in 1st round */
         if (!mfb_flash_write_pattern_region(kFlashInstMode_Hyper))
         {
             return;
         }
    }
    /* Do 2nd round patten verify test */
    mfb_flash_pattern_verify_test(true);
    /* Get perf test result */
    mfb_flash_memcpy_perf_test(true);

    /* Jump into user application */
    mfb_jump_to_application(EXAMPLE_MIXSPI_AMBA_BASE + MFB_APP_IMAGE_OFFSET);
}

static void mfb_show_final_result(bool isTrue)
{
    if (isTrue)
    {
        mfb_printf("\r\nMFB: PASS\r\n");
    }
    else
    {
        mfb_printf("\r\nMFB: FAIL\r\n");
    }
}

static bool mfb_validate_jedec(flash_inst_mode_t *sta_flashInstMode, jedec_id_t *jedecID)
{
    bool sta_isValidVendorId = false;
#if MFB_FLASH_FAKE_JEDEC_ID_ENABLE
    jedecID->manufacturerID = WINBOND_QUAD_FLASH_JEDEC_ID & 0xFF;
    jedecID->memoryTypeID = (WINBOND_QUAD_FLASH_JEDEC_ID >> 8) & 0xFF;
    jedecID->capacityID = (WINBOND_QUAD_FLASH_JEDEC_ID >> 16) & 0xFF;
    /* Can change this variable according to Flash default state */
    *sta_flashInstMode = kFlashInstMode_SPI;
    /* Init FlexSPI using common LUT */ 
    mfb_mixspi_common_init(*sta_flashInstMode);
    sta_isValidVendorId = true;
#else
    status_t status = kStatus_Success;
    while (*sta_flashInstMode < kFlashInstMode_MAX)
    {
        /* Init FlexSPI using common LUT */ 
        mfb_mixspi_common_init(*sta_flashInstMode);
        /* Read JEDEC id from flash */
        status = mixspi_nor_get_jedec_id(EXAMPLE_MIXSPI, (uint32_t *)jedecID, *sta_flashInstMode);
        if (status != kStatus_Success)
        {
            mfb_printf("MFB: Get Flash Vendor ID failed");
        }
        else
        {
            uint32_t idx;
            for (idx = 0; idx < sizeof(s_flashVendorIDs); idx++)
            {
                if (jedecID->manufacturerID == s_flashVendorIDs[idx])
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
                mfb_printf("MFB: Get Invalid Flash Vendor ID 0x%x", jedecID->manufacturerID);
            }
        }
        switch (*sta_flashInstMode)
        {
            case kFlashInstMode_OPI:
                mfb_printf(" under OPI DDR mode.\r\n");
                *sta_flashInstMode = kFlashInstMode_MAX;
                break;

            case kFlashInstMode_QPI_2:
                mfb_printf(" under QPI_2 SDR mode.\r\n");
                *sta_flashInstMode = kFlashInstMode_OPI;
                break;

            case kFlashInstMode_QPI_1:
                mfb_printf(" under QPI_1 SDR mode.\r\n");
                *sta_flashInstMode = kFlashInstMode_QPI_2;
                break;

            case kFlashInstMode_SPI:
            default:
                mfb_printf(" under Std/Ext SPI mode.\r\n");
                *sta_flashInstMode = kFlashInstMode_QPI_1;
                break;
        }
    }
    if (status == kStatus_Success)
#endif
    {
        /* Get real flash size according to jedec id result (it may not be appliable to some specifal adesto device) */
        g_flashPropertyInfo.flashMemSizeInByte = mfb_flash_decode_common_capacity_id(jedecID->capacityID);
        mfb_printf("MFB: Flash Manufacturer ID: 0x%x", jedecID->manufacturerID);
        /* Check Vendor ID. */
        sta_isValidVendorId = mfb_flash_is_valid_jedec_id(jedecID);
        if (sta_isValidVendorId && (*sta_flashInstMode == kFlashInstMode_SPI))
        {
            sfdp_header_t sfdp_header;
#if MFB_FLASH_PROG_JEDEC_SFDP_ENABLE
            status = mixspi_nor_get_jedec_sfdp(EXAMPLE_MIXSPI, 0, (uint32_t *)&s_sfdp_table, sizeof(sfdp_table_t));
            memcpy((uint8_t *)&sfdp_header, (uint8_t *)&s_sfdp_table, sizeof(sfdp_header));
#else
            status = mixspi_nor_get_jedec_sfdp(EXAMPLE_MIXSPI, 0, (uint32_t *)&sfdp_header, sizeof(sfdp_header));
#endif
            if (status == kStatus_Success)
            {
                if (sfdp_header.signature == SFDP_SIGNATURE)
                {
                    mfb_printf("MFB: Get Valid Flash SFDP.\r\n");
                    if (sfdp_header.major_rev == kSfdp_Version_Major_1_0)
                    {
                        mfb_printf("MFB: Flash SFDP Version is JESD216");
                        switch (sfdp_header.minor_rev)
                        {
                            case kSfdp_Version_Minor_C:
                                mfb_printf("C");
                                break;
                            case kSfdp_Version_Minor_B:
                                mfb_printf("B");
                                break;
                            case kSfdp_Version_Minor_A:
                                mfb_printf("A");
                                break;
                            case kSfdp_Version_Minor_0:
                                break;
                            default:
                                mfb_printf("x");
                                break;
                        }
                        mfb_printf(" - minor_rev = %d.\r\n", sfdp_header.minor_rev);
                    }
                }
                else
                {
                    mfb_printf("MFB: Get Invalid Flash SFDP, Signature = 0x%x.\r\n", sfdp_header.signature);
#if MFB_FLASH_PROG_JEDEC_SFDP_ENABLE
                    status = mixspi_nor_sfdp_sec_erase(EXAMPLE_MIXSPI, 0x000000);
                    mfb_printf("MFB: Erased Flash SFDP Region - ");
                    if (status == kStatus_Success)
                    {
                        mfb_printf(" Done.\r\n");
#if 0
                        memset((void *)&sfdp_header, 0xFF, sizeof(sfdp_header));
                        sfdp_header.signature = SFDP_SIGNATURE;
                        sfdp_header.major_rev = kSfdp_Version_Major_1_0;
                        sfdp_header.minor_rev = kSfdp_Version_Minor_A;
                        status = mixspi_nor_sfdp_sec_program(EXAMPLE_MIXSPI, 0x000000, (uint32_t *)&sfdp_header, sizeof(sfdp_header));
#else
                        {
                            #pragma section = "__sfdp_table"
                            uint32_t *sfdpStart = __section_begin("__sfdp_table");
                            status = mixspi_nor_sfdp_sec_program(EXAMPLE_MIXSPI, 0x000000, sfdpStart, sizeof(sfdp_table_t));
                        }
#endif
                        mfb_printf("MFB: Programmed Flash SFDP Region - ");
                        if (status == kStatus_Success)
                        {
                            mfb_printf(" Done.\r\n");
                        }
                        else
                        {
                            mfb_printf(" Failed.\r\n");
                        }
                    }
                    else
                    {
                        mfb_printf(" Failed.\r\n");
                    }
#endif
                }
            }
            else
            {
                mfb_printf("MFB: Get Flash SFDP failed\r\n");
            }
        }
    }
    return sta_isValidVendorId;
}

void mfb_main(void)
{
    status_t status = kStatus_Success;
    jedec_id_t jedecID;
    flash_inst_mode_t sta_flashInstMode = kFlashInstMode_SPI;

    mfb_printf("MFB: i.MXRT multi-flash boot solution.\r\n");
    mfb_printf("MFB: Get CPU root clock.\r\n");
    /* Show CPU clock source */
    cpu_show_clock_source();

#if MFB_FLASH_HYPER_FLASH_ENABLE
    mfb_hyper_flash_test();
    
    return;
#endif

    mfb_printf("\r\nMFB: Set FlexSPI port to 1-bit pad.\r\n");
    /* Switch FlexSPI port if needed */
    mixspi_port_switch(EXAMPLE_MIXSPI, EXAMPLE_MIXSPI_PORT, kMIXSPI_1PAD);
    mfb_printf("MFB: Set FlexSPI root clock to 30MHz.\r\n");
    /* Move FlexSPI clock to a stable clock source */ 
    mixspi_clock_init(EXAMPLE_MIXSPI, kMixspiRootClkFreq_30MHz);
    /* Update root clock */
    mixspi_device_config_update_rootclock(mixspi_get_clock(EXAMPLE_MIXSPI));
    /* Show FlexSPI clock source */
    mixspi_show_clock_source(EXAMPLE_MIXSPI);
    /* Set default paramenters */
    g_flashPropertyInfo.flashHasQpiSupport = false;
    g_flashPropertyInfo.flashIsOctal = false;
    g_flashPropertyInfo.mixspiPad = kMIXSPI_4PAD;
    g_flashPropertyInfo.mixspiRootClkFreq = kMixspiRootClkFreq_100MHz;
    g_flashPropertyInfo.mixspiReadSampleClock = kMIXSPI_SampClkLoopbackDqs;
    g_flashPropertyInfo.flashDummyValue = U32_VALUE_INVALID;
    g_flashPropertyInfo.flashDriveStrength = U32_VALUE_INVALID;
    g_flashPropertyInfo.flashQuadEnableBytes = 0;
    g_flashPropertyInfo.flashUniqueCfg = U32_VALUE_INVALID;
    /* Validate JEDEC ID and SFDP. */
    if (mfb_validate_jedec(&sta_flashInstMode, &jedecID))
    {
        /* Only run 1st perf and pattern verify when default flash state is Ext SPI mode */
        if (sta_flashInstMode == kFlashInstMode_SPI)
        {
            if (MFB_FLASH_ACCESS_REGION_START + MFB_FLASH_ACCESS_REGION_SIZE > (16*1024*1024UL))
            {
                mfb_printf("\r\nMFB: perf and pattern verify test is bypassed under Ext SPI mode as flash access region is larger than 16MB\r\n");
            }
            else
            {
                /* Do patten verify test under 1bit SPI mode */
                mfb_flash_pattern_verify_test(true);
                /* Get perf test result under 1bit SPI mode */
                mfb_flash_memcpy_perf_test(false);
            }
        }

        mfb_printf("\r\nMFB: Set FlexSPI port to %d-bit pad.\r\n", 1u << (uint32_t)g_flashPropertyInfo.mixspiPad);
        /* Configure FlexSPI pinmux as user prescriptive */
        mixspi_pin_init(EXAMPLE_MIXSPI, EXAMPLE_MIXSPI_PORT, g_flashPropertyInfo.mixspiPad);
        mixspi_device_config_update_flashsize(g_flashPropertyInfo.flashMemSizeInByte / 0x400);
        /* Re-init FlexSPI using custom LUT */
        mixspi_nor_flash_init(EXAMPLE_MIXSPI, g_flashPropertyInfo.mixspiCustomLUTVendor, g_flashPropertyInfo.mixspiReadSampleClock, sta_flashInstMode);
        mfb_printf("MFB: FLEXSPI module is initialized to multi-I/O fast read mode.\r\n");
        /* Write dummy cycle value into flash if needed */
        if (g_flashPropertyInfo.flashDummyValue != U32_VALUE_INVALID)
        {
            mixspi_nor_set_dummy_cycle(EXAMPLE_MIXSPI, (uint8_t)(g_flashPropertyInfo.flashDummyValue & 0xFF));
            mfb_printf("MFB: Flash register (with dummy cycle) is set to 0x%x.\r\n", g_flashPropertyInfo.flashDummyValue);
        }
        /* Write drive strength value into flash if needed */
        if (g_flashPropertyInfo.flashDriveStrength != U32_VALUE_INVALID)
        {
            mixspi_nor_set_drive_strength(EXAMPLE_MIXSPI, (uint8_t)(g_flashPropertyInfo.flashDriveStrength & 0xFF));
            mfb_printf("MFB: Flash register (with drive strength) is set to 0x%x.\r\n", g_flashPropertyInfo.flashDriveStrength);
        }
        /* Write unique config value into flash if needed */
        if (g_flashPropertyInfo.flashUniqueCfg != U32_VALUE_INVALID)
        {
            mixspi_nor_set_unique_cfg(EXAMPLE_MIXSPI, (uint8_t)(g_flashPropertyInfo.flashUniqueCfg & 0xFF));
            mfb_printf("MFB: Flash register (for unique cfg) is set to 0x%x.\r\n", g_flashPropertyInfo.flashUniqueCfg);
        }
        if (!g_flashPropertyInfo.flashIsOctal)
        {
            if (sta_flashInstMode == kFlashInstMode_SPI)
            {
#if MFB_FLASH_QPI_MODE_ENABLE
                if (g_flashPropertyInfo.flashHasQpiSupport)
                {
                    /* Enter QPI SDR mode. */
                    status = mixspi_nor_enable_qpi_mode(EXAMPLE_MIXSPI);
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
                    if (g_flashPropertyInfo.flashQuadEnableBytes)
                    {
                        status = mixspi_nor_enable_quad_mode(EXAMPLE_MIXSPI);
                        if (status != kStatus_Success)
                        {
                            mfb_printf("MFB: Flash failed to Enter Quad I/O SDR mode.\r\n");
                        }
                        else
                        {
                            mfb_printf("MFB: Flash entered Quad I/O SDR mode.\r\n");
                        }
                    }
                    else
                    {
                        mfb_printf("MFB: Flash remained in default Quad Enable mode.\r\n");
                    }
                    /* Read internal registers of Flash */
                    mfb_flash_show_registers(&jedecID, false);
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
                status = mixspi_nor_enable_opi_mode(EXAMPLE_MIXSPI);
                if (status != kStatus_Success)
                {
                    mfb_printf("MFB: Flash failed to Enter OPI DDR mode.\r\n");
                }
                else
                {
                    sta_flashInstMode = kFlashInstMode_OPI;
                    mfb_printf("MFB: Flash entered OPI DDR mode.\r\n");
                    /* Read internal regiters of Flash */
                    mfb_flash_show_registers(&jedecID, true);
                }
#else
                mfb_printf("MFB: Flash ran in Octal I/O SPI mode.\r\n");
#endif
            }
            else
            {
#if !MFB_FLASH_OPI_MODE_DISABLE
                mfb_printf("MFB: Flash remained in default OPI DDR mode.\r\n");
                /* Read internal regiters of Flash */
                mfb_flash_show_registers(&jedecID, true);
#else
#warning "Do not support loopback dqs option when flash default state in OPI DDR"
#endif
            }
        }
        if (status == kStatus_Success)
        {
            /* Do patten verify test under Multi I/O fast read mode */
            uint32_t round = 1;
            while (round <= 2)
            {
                //bool showError = (round == 2);
                /* Don't show error info when it is 1st round, as flash may be blank this time */
                if (!mfb_flash_pattern_verify_test(true))
                {
                     /* Try to write pattern into flash when error occurs in 1st round */
                     if (round == 1)
                     {
                         if (!mfb_flash_write_pattern_region(sta_flashInstMode))
                         {
                             mfb_show_final_result(false);
                             return;
                         }
                     }
                     else
                     {
                         mfb_show_final_result(false);
                         return;
                     }
                }
                /* Increase speed for 2nd round */
                if (round != 2)
                {
                    /* Get perf test result under Multi I/O fast read mode and pre-set speed*/
                    mfb_flash_memcpy_perf_test(false);

                    mfb_printf("\r\nMFB: Set FlexSPI root clock to %dMHz.\r\n", decode_mixspi_root_clk_defn(g_flashPropertyInfo.mixspiRootClkFreq));
                    /* Configure FlexSPI clock as user prescriptive */ 
                    mixspi_clock_init(EXAMPLE_MIXSPI, g_flashPropertyInfo.mixspiRootClkFreq);
                    /* Update root clock */
                    mixspi_device_config_update_rootclock(mixspi_get_clock(EXAMPLE_MIXSPI));
                    /* Show FlexSPI clock source */
                    mixspi_show_clock_source(EXAMPLE_MIXSPI);
                    /* Re-init FlexSPI using custom LUT */
                    mixspi_nor_flash_init(EXAMPLE_MIXSPI, g_flashPropertyInfo.mixspiCustomLUTVendor, g_flashPropertyInfo.mixspiReadSampleClock, sta_flashInstMode);
                    mfb_printf("MFB: FLEXSPI module is initialized to multi-I/O fast read mode.\r\n");

                    round = 2;
                }
                else
                {
                    break;
                }
            }
            /* Get perf test result under Multi I/O fast read mode and user-set speed */
            mfb_flash_memcpy_perf_test(true);
            mfb_show_final_result(true);
            /* Jump into user application */
            mfb_jump_to_application(EXAMPLE_MIXSPI_AMBA_BASE + MFB_APP_IMAGE_OFFSET);
        }
    }
}
