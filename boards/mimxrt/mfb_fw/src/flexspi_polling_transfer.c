/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mfb_cfg.h"
#include "microseconds.h"
#include "flexspi_info.h"
#include "nor_flash.h"
#if MXIC_DEVICE_SERIES
#include "nor_flash_mxic.h"
#endif
#if ISSI_DEVICE_SERIES
#include "nor_flash_issi.h"
#endif
#include "fsl_flexspi.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "fsl_power.h"
#include "fsl_reset.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern const uint32_t customLUT_ISSI[CUSTOM_LUT_LENGTH];
extern const uint32_t customLUT_MXIC[CUSTOM_LUT_LENGTH];

extern status_t flexspi_nor_get_vendor_id(FLEXSPI_Type *base, uint8_t *vendorId);
extern status_t flexspi_nor_enable_octal_mode(FLEXSPI_Type *base);
extern status_t flexspi_nor_enable_quad_mode(FLEXSPI_Type *base);
extern void flexspi_nor_flash_init(FLEXSPI_Type *base, const uint32_t *customLUT, flexspi_read_sample_clock_t rxSampleClock);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_nor_read_buffer[256];

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

#if MFP_FLASH_SPEED_TEST_ENABLE
void mfb_flash_speed_test(void)
{
    uint64_t startTicks = microseconds_get_ticks();
    /* Read 8MB data from flash to test speed */
    for (uint32_t loop = 0; loop < 16 * 8; loop++)
    {
        /* Min NOR Flash size is 64KB */
        for (uint32_t idx = 0; idx < 256; idx++)
        {
            memcpy(s_nor_read_buffer, (uint8_t*)(FlexSPI0_AMBA_BASE + idx * sizeof(s_nor_read_buffer)), sizeof(s_nor_read_buffer));
        }
    }
    uint64_t totalTicks = microseconds_get_ticks() - startTicks;
    uint32_t microSecs = microseconds_convert_to_microseconds(totalTicks);
    PRINTF("Flash to RAM memcpy speed: %fMB/s.\r\n", (8.0 * 1000000)/microSecs);
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

void mfb_main(void)
{
    status_t status;
    uint8_t vendorID = 0;

#if !defined(FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE)
    POWER_DisablePD(kPDRUNCFG_APD_FLEXSPI0_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_FLEXSPI0_SRAM);
    POWER_ApplyPD();
#endif
    /* Move FlexSPI clock to a stable clock source */ 
    BOARD_FlexspiClockSafeConfig();
    /* Init FlexSPI using common LUT */ 
    flexspi_nor_flash_init(EXAMPLE_FLEXSPI, customLUTCommonMode, kFLEXSPI_ReadSampleClkLoopbackInternally);
    PRINTF("FLEXSPI module initialized!\r\n");
    PRINTF("FLEXSPI Clk Frequency: %dHz.\r\n", CLOCK_GetFlexspiClkFreq(0));

    /* Get vendor ID. */
    status = flexspi_nor_get_vendor_id(EXAMPLE_FLEXSPI, &vendorID);
    if (status != kStatus_Success)
    {
        PRINTF("Get Flash Vendor ID failed.\r\n");
        return;
    }
    else
    {
        PRINTF("Flash Vendor ID: 0x%x.\r\n", vendorID);
    }

#if MFP_FLASH_SPEED_TEST_ENABLE
    microseconds_init();
    mfb_flash_speed_test();
#endif

    switch (vendorID)
    {
#if MXIC_DEVICE_SERIES
        // MXIC
        case 0xc2:
            {
#if MXIC_DEVICE_MX25UM51345
                /* Set FlexSPI clock: source AUX0_PLL, divide by 4 */
                BOARD_SetFlexspiClock(FLEXSPI0, 2U, 4U);
                /* Update root clock */
                deviceconfig.flexspiRootClk = 99000000;
                deviceconfig.flashSize = 0x10000; /* 512Mb/KByte */
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
#if ISSI_DEVICE_IS25WP064A
                /* Set FlexSPI clock: source AUX0_PLL, divide by 3 */
                BOARD_SetFlexspiClock(FLEXSPI0, 2U, 3U);
                /* Update root clock */
                deviceconfig.flexspiRootClk = 132000000;
                deviceconfig.flashSize = 0x2000; /* 64Mb/KByte */
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
            PRINTF("Unsupported Vendor ID\r\n");
            return;
    }
    
    if (status != kStatus_Success)
    {
        PRINTF("Flash failed to Enter Octal/Quad mode.\r\n");
    }
    else
    {
        PRINTF("Flash entered Octal/Quad mode.\r\n");
        PRINTF("FLEXSPI Clk Frequency: %dHz.\r\n", CLOCK_GetFlexspiClkFreq(0));
#if MFP_FLASH_SPEED_TEST_ENABLE
        mfb_flash_speed_test();
        microseconds_shutdown();
#endif
        PRINTF("Jump to Application code at 0x%x.\r\n", FlexSPI0_AMBA_BASE + MFP_APP_IMAGE_OFFSET);
        mfb_jump_to_application(FlexSPI0_AMBA_BASE + MFP_APP_IMAGE_OFFSET);
    }
}