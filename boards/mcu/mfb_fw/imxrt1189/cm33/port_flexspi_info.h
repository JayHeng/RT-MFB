/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _PORT_FLEXSPI_INFO_H_
#define _PORT_FLEXSPI_INFO_H_

#include "fsl_clock.h"
#include "fsl_iomuxc.h"
#include "fsl_cache.h"
#include "fsl_rgpio.h"
#include "fsl_flexspi.h"
#include "mfb.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_FLEXSPI                 FLEXSPI1
#define FLASH_SIZE                      0x4000 /* 16Mb/KByte */
#define EXAMPLE_FLEXSPI_AMBA_BASE       FlexSPI1_AMBA_BASE
#define FLASH_PAGE_SIZE                 256
#define SECTOR_SIZE                     0x1000 /* 4K */
#define EXAMPLE_FLEXSPI_CLOCK           kCLOCK_Flexspi1
#define FLASH_PORT                      kFLEXSPI_PortA1

//#define BOARD_IS_RT1180_EVK_PORTA_QUAD      (0)
//#define BOARD_IS_RT1180_EVK_PORTA_OCTAL     (0)
#define BOARD_IS_RT1180_EVK_PORTB_QUAD      (0)
#define BOARD_IS_RT1180_EVK_PORTB_OCTAL     (0)  // To Flash memory daughter card

#define BOARD_IS_RT1180_MEM_DC_MUX1         (1)
#define BOARD_IS_RT1180_MEM_DC_MUX2         (0)
#define BOARD_IS_RT1180_MEM_DC_MUX3         (0)

/*
 * If cache is enabled, this example should maintain the cache to make sure
 * CPU core accesses the memory, not cache only.
 */
#define CACHE_MAINTAIN 1

/*${macro:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
#if (defined CACHE_MAINTAIN) && (CACHE_MAINTAIN == 1)
typedef struct _flexspi_cache_status
{
    volatile bool codeCacheEnableFlag;
    volatile bool systemCacheEnableFlag;
} flexspi_cache_status_t;
#endif
/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void cpu_show_clock_source(void)
{
#if MFB_DEBUG_LOG_INFO_ENABLE
    uint32_t clkSel = CLOCK_GetRootClockMux(kCLOCK_Root_M33);
    switch (clkSel)
    {
        case kCLOCK_M33_ClockRoot_MuxOscRc24M:
            mfb_printf("MFB: CPU-CM33 Clk Source from 2'b00 - OSC RC24M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_Osc24M));
            break;

        case kCLOCK_M33_ClockRoot_MuxOscRc400M:
            mfb_printf("MFB: CPU-CM33 Clk Source from 2'b01 - OSC RC400M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc400M));
            break;

        case kCLOCK_M33_ClockRoot_MuxSysPll3Out:
            mfb_printf("MFB: CPU-CM33 Clk Source from 2'b10 - Sys PLL3 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll3Out));
            break;

        case kCLOCK_M33_ClockRoot_MuxArmPllOut:
            mfb_printf("MFB: CPU-CM33 Clk Source from 2'b11 - ARM PLL clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_ArmPllOut));
            break;

        default:
            break;
    }

    uint32_t clkDiv = CLOCK_GetRootClockDiv(kCLOCK_Root_M33);
    mfb_printf("MFB: CPU-CM33 Clk Source Divider: %d.\r\n", clkDiv);
    mfb_printf("MFB: CPU-CM33 Clk Frequency: %dHz.\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
#endif
}

static void flexspi_port_switch(FLEXSPI_Type *base, flexspi_port_t port, flexspi_pad_t pads)
{
    rgpio_pin_config_t do_config = {kRGPIO_DigitalOutput, 0};
    if (base == FLEXSPI1)
    {
#if BOARD_IS_RT1180_MEM_DC_MUX1 | BOARD_IS_RT1180_MEM_DC_MUX2
        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_23_GPIO4_IO23, 0U);
        RGPIO_PinInit(RGPIO4, 23, &do_config);
#endif
#if BOARD_IS_RT1180_EVK_PORTB_QUAD | BOARD_IS_RT1180_EVK_PORTB_OCTAL
        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_04_GPIO4_IO04, 0U);
        RGPIO_PinInit(RGPIO4, 4, &do_config);
#endif
        switch (port)
        {
            case kFLEXSPI_PortA1:
            case kFLEXSPI_PortA2:
                RGPIO_PinWrite(RGPIO4, 23, 0);
                break;
            case kFLEXSPI_PortB1:
            case kFLEXSPI_PortB2:
#if BOARD_IS_RT1180_MEM_DC_MUX1
                RGPIO_PinWrite(RGPIO4, 23, 0);
#elif BOARD_IS_RT1180_MEM_DC_MUX2
                RGPIO_PinWrite(RGPIO4, 23, 1);
#endif
#if BOARD_IS_RT1180_EVK_PORTB_QUAD
                RGPIO_PinWrite(RGPIO4, 4, 0);
#elif BOARD_IS_RT1180_EVK_PORTB_OCTAL
                RGPIO_PinWrite(RGPIO4, 4, 1);
#endif
                break;
            default:
                break;
        }
    }
    else if (base == FLEXSPI2)
    {
#if BOARD_IS_RT1180_MEM_DC_MUX1 | BOARD_IS_RT1180_MEM_DC_MUX2 | BOARD_IS_RT1180_MEM_DC_MUX3
        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_22_GPIO4_IO22, 0U);
        RGPIO_PinInit(RGPIO4, 22, &do_config);
#endif
        switch (port)
        {
            case kFLEXSPI_PortA1:
            case kFLEXSPI_PortA2:
#if BOARD_IS_RT1180_MEM_DC_MUX1
                RGPIO_PinWrite(RGPIO4, 22, 0);
#elif BOARD_IS_RT1180_MEM_DC_MUX2
                RGPIO_PinWrite(RGPIO4, 22, 1);
#endif
                break;
            case kFLEXSPI_PortB1:
            case kFLEXSPI_PortB2:
#if BOARD_IS_RT1180_MEM_DC_MUX1
                RGPIO_PinWrite(RGPIO4, 22, 0);
#elif BOARD_IS_RT1180_MEM_DC_MUX2 | BOARD_IS_RT1180_MEM_DC_MUX3
                RGPIO_PinWrite(RGPIO4, 22, 1);
#endif
                break;
            default:
                break;
        }
    }
    else
    {
    }
}

static void flexspi_pin_init(FLEXSPI_Type *base, flexspi_port_t port, flexspi_pad_t pads)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc1);          /* Turn on LPCG: LPCG is ON. */
    CLOCK_EnableClock(kCLOCK_Iomuxc2);          /* Turn on LPCG: LPCG is ON. */
    if (base == FLEXSPI1)
    {
        switch (port)
        {
            case kFLEXSPI_PortA1:
            case kFLEXSPI_PortA2:
                IOMUXC_SetPinMux(IOMUXC_GPIO_B2_07_FLEXSPI1_BUS2BIT_A_DQS,    1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B2_08_FLEXSPI1_BUS2BIT_A_SCLK,   1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B2_09_FLEXSPI1_BUS2BIT_A_SS0_B,  1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B2_10_FLEXSPI1_BUS2BIT_A_DATA00, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B2_11_FLEXSPI1_BUS2BIT_A_DATA01, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B2_12_FLEXSPI1_BUS2BIT_A_DATA02, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B2_13_FLEXSPI1_BUS2BIT_A_DATA03, 1U);

                IOMUXC_SetPinMux(IOMUXC_GPIO_B2_03_FLEXSPI1_BUS2BIT_A_DATA04, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B2_04_FLEXSPI1_BUS2BIT_A_DATA05, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B2_05_FLEXSPI1_BUS2BIT_A_DATA06, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B2_06_FLEXSPI1_BUS2BIT_A_DATA07, 1U);
                break;
            case kFLEXSPI_PortB1:
            case kFLEXSPI_PortB2:
#if BOARD_IS_RT1180_MEM_DC_MUX1 | BOARD_IS_RT1180_EVK_PORTB_QUAD | BOARD_IS_RT1180_EVK_PORTB_OCTAL
                IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_05_FLEXSPI1_BUS2BIT_B_DQS,    1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_06_FLEXSPI1_BUS2BIT_B_SS0_B,  1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_07_FLEXSPI1_BUS2BIT_B_SCLK,   1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_08_FLEXSPI1_BUS2BIT_B_DATA00, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_09_FLEXSPI1_BUS2BIT_B_DATA01, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_10_FLEXSPI1_BUS2BIT_B_DATA02, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_11_FLEXSPI1_BUS2BIT_B_DATA03, 1U);
                
                IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_00_FLEXSPI1_BUS2BIT_B_DATA04, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_01_FLEXSPI1_BUS2BIT_B_DATA05, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_02_FLEXSPI1_BUS2BIT_B_DATA06, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_03_FLEXSPI1_BUS2BIT_B_DATA07, 1U);
#elif BOARD_IS_RT1180_MEM_DC_MUX2
                IOMUXC_SetPinMux(IOMUXC_GPIO_B1_03_FLEXSPI1_BUS2BIT_B_DQS,    1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B1_04_FLEXSPI1_BUS2BIT_B_SS0_B,  1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B1_05_FLEXSPI1_BUS2BIT_B_SCLK,   1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B1_13_FLEXSPI1_BUS2BIT_B_DATA00, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_FLEXSPI1_BUS2BIT_B_DATA01, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B1_11_FLEXSPI1_BUS2BIT_B_DATA02, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B1_10_FLEXSPI1_BUS2BIT_B_DATA03, 1U);
                
                IOMUXC_SetPinMux(IOMUXC_GPIO_B1_09_FLEXSPI1_BUS2BIT_B_DATA04, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B1_08_FLEXSPI1_BUS2BIT_B_DATA05, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B1_07_FLEXSPI1_BUS2BIT_B_DATA06, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_B1_06_FLEXSPI1_BUS2BIT_B_DATA07, 1U);
#endif
                break;
            default:
                break;
        }

    }
    else if (base == FLEXSPI2)
    {
        switch (port)
        {
            case kFLEXSPI_PortA1:
            case kFLEXSPI_PortA2:
            case kFLEXSPI_PortB1:
            case kFLEXSPI_PortB2:
#if BOARD_IS_RT1180_MEM_DC_MUX1
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_35_FLEXSPI2_BUS2BIT_A_DATA00, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_36_FLEXSPI2_BUS2BIT_A_DATA01, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_37_FLEXSPI2_BUS2BIT_A_DATA02, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_38_FLEXSPI2_BUS2BIT_A_DATA03, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_39_FLEXSPI2_BUS2BIT_A_SS0_B,  1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_40_FLEXSPI2_BUS2BIT_A_DQS,    1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_41_FLEXSPI2_BUS2BIT_A_SCLK,   1U);

                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_28_FLEXSPI2_BUS2BIT_B_SS0_B,  1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_29_FLEXSPI2_BUS2BIT_B_DQS,    1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_30_FLEXSPI2_BUS2BIT_B_DATA03, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_31_FLEXSPI2_BUS2BIT_B_DATA02, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_32_FLEXSPI2_BUS2BIT_B_DATA01, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_33_FLEXSPI2_BUS2BIT_B_DATA00, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_34_FLEXSPI2_BUS2BIT_B_SCLK,   1U);
#elif BOARD_IS_RT1180_MEM_DC_MUX2
                IOMUXC_SetPinMux(IOMUXC_GPIO_AON_21_FLEXSPI2_BUS2BIT_A_DQS,       1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_AON_22_FLEXSPI2_BUS2BIT_A_SS0_B,     1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_AON_23_FLEXSPI2_BUS2BIT_A_SCLK,      1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_AON_24_FLEXSPI2_BUS2BIT_A_DATA00,    1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_AON_25_FLEXSPI2_BUS2BIT_A_DATA01,    1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_AON_26_FLEXSPI2_BUS2BIT_A_DATA02,    1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_AON_27_FLEXSPI2_BUS2BIT_A_DATA03,    1U);

#if BOARD_IS_RT1180_MEM_DC_MUX3
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_22_FLEXSPI2_BUS2BIT_B_DATA03, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_23_FLEXSPI2_BUS2BIT_B_DATA02, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_24_FLEXSPI2_BUS2BIT_B_DATA01, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_25_FLEXSPI2_BUS2BIT_B_DATA00, 1U);
#else
                IOMUXC_SetPinMux(IOMUXC_GPIO_AON_15_FLEXSPI2_BUS2BIT_B_DATA03, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_AON_16_FLEXSPI2_BUS2BIT_B_DATA02, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_AON_17_FLEXSPI2_BUS2BIT_B_DATA01, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_AON_18_FLEXSPI2_BUS2BIT_B_DATA00, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_AON_19_FLEXSPI2_BUS2BIT_B_SCLK,   1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_AON_20_FLEXSPI2_BUS2BIT_B_DQS,    1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_AON_21_FLEXSPI2_BUS2BIT_B_SS0_B,  1U);
#endif
#elif BOARD_IS_RT1180_MEM_DC_MUX3
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_28_FLEXSPI2_BUS2BIT_B_SS0_B,  1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_AON_19_FLEXSPI2_BUS2BIT_B_SCLK,   1U);

                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_21_FLEXSPI2_BUS2BIT_B_DQS,    1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_22_FLEXSPI2_BUS2BIT_B_DATA03, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_23_FLEXSPI2_BUS2BIT_B_DATA02, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_24_FLEXSPI2_BUS2BIT_B_DATA01, 1U);
                IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_B1_25_FLEXSPI2_BUS2BIT_B_DATA00, 1U);
#endif
                break;
            default:
                break;
        }
    }
    else
    {
    }
}

static void flexspi_clock_init(FLEXSPI_Type *base, mixspi_root_clk_freq_t clkFreq)
{
    clock_root_config_t rootCfg = {0};
    if (base == FLEXSPI1)
    {
        ///////////////////////////////////////////////////////////////////
        // default settings in clock_config.c

        /* Init System Pll2 pfd0. */
        // 528*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        //CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd0, 29);  -- 327MHz

        /* Init System Pll3 pfd0. */
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        //CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 22);  -- 392MHz
        ///////////////////////////////////////////////////////////////////

        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 12-35.
        if (clkFreq == kMixspiRootClkFreq_30MHz)
        {
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc24M;
            rootCfg.div = 1;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_50MHz)
        {
            // System Pll3 pfd0 (392MHz) / 8
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 8;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_60MHz)
        {
            // System Pll3 pfd0 (392MHz) / 7
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 7;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_80MHz)
        {
            // System Pll3 pfd0 (392MHz) / 5
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 5;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_100MHz)
        {
            // System Pll3 pfd0 (392MHz) / 4
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 4;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_120MHz)
        {
            // System Pll2 pfd0 (327MHz) / 3
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd0;
            rootCfg.div = 3;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_133MHz)
        {
            // System Pll3 pfd0 (392MHz) / 3
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 3;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_166MHz)
        {
            // System Pll2 pfd0 (327MHz) / 2
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd0;
            rootCfg.div = 2;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_200MHz)
        {
            // System Pll3 pfd0 (392MHz) / 2
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 2;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_332MHz)
        {
            // System Pll2 pfd0 (327MHz) / 1
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd0;
            rootCfg.div = 1;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_400MHz)
        {
            // System Pll3 pfd0 (392MHz) / 1
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 1;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else
        {
            mfb_printf("MFB: This FlexSPI clock freq is not set.\r\n");
        }
        uint32_t flexspiClk = CLOCK_GetRootClockFreq(kCLOCK_Root_Flexspi1);
        if (flexspiClk > 166000000U)
        {
            __NOP();
        }
    }
    else if (base == FLEXSPI2)
    {
        ///////////////////////////////////////////////////////////////////
        // default settings in clock_config.c

        /* Init System Pll2 pfd1. */
        // 528*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        //CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd1, 29);  -- 327MHz

        /* Init System Pll3 pfd2. */
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        //CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd2, 22);  -- 392MHz
        ///////////////////////////////////////////////////////////////////

        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 12-35.
        if (clkFreq == kMixspiRootClkFreq_30MHz)
        {
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxOscRc24M;
            rootCfg.div = 1;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_50MHz)
        {
            // System Pll3 pfd2 (392MHz) / 8
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2;
            rootCfg.div = 8;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_60MHz)
        {
            // System Pll3 pfd2 (392MHz) / 7
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2;
            rootCfg.div = 7;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_80MHz)
        {
            // System Pll3 pfd2 (392MHz) / 5
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2;
            rootCfg.div = 5;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_100MHz)
        {
            // System Pll3 pfd2 (392MHz) / 4
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2;
            rootCfg.div = 4;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_120MHz)
        {
            // System Pll2 pfd1 (327MHz) / 3
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll2Pfd1;
            rootCfg.div = 3;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_133MHz)
        {
            // System Pll3 pfd2 (392MHz) / 3
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2;
            rootCfg.div = 3;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_166MHz)
        {
            // System Pll2 pfd1 (327MHz) / 2
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll2Pfd1;
            rootCfg.div = 2;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_200MHz)
        {
            // System Pll3 pfd2 (392MHz) / 2
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2;
            rootCfg.div = 2;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_332MHz)
        {
            // System Pll2 pfd1 (327MHz) / 1
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll2Pfd1;
            rootCfg.div = 1;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_400MHz)
        {
            // System Pll3 pfd2 (392MHz) / 1
            rootCfg.mux = kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2;
            rootCfg.div = 1;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi2, &rootCfg);
        }
        else
        {
            mfb_printf("MFB: This FlexSPI clock freq is not set.\r\n");
        }
        uint32_t flexspiClk = CLOCK_GetRootClockFreq(kCLOCK_Root_Flexspi2);
        if (flexspiClk > 166000000U)
        {
            __NOP();
        }
    }
    else
    {
    }
}

static uint32_t flexspi_get_clock(FLEXSPI_Type *base)
{
    if (base == FLEXSPI1)
    {
        return CLOCK_GetRootClockFreq(kCLOCK_Root_Flexspi1);
    }
    else if (base == FLEXSPI2)
    {
        return CLOCK_GetRootClockFreq(kCLOCK_Root_Flexspi2);
    }
    else
    {
        return 0;
    }
}

static void flexspi_show_clock_source(FLEXSPI_Type *base)
{
#if MFB_DEBUG_LOG_INFO_ENABLE
    uint32_t index = 0;
    clock_root_t root;
    uint32_t clkSel;
    if (base == FLEXSPI1)
    {
        root = kCLOCK_Root_Flexspi1;
        clkSel = CLOCK_GetRootClockMux(root);
        switch (clkSel)
        {
            case kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc24M:
                mfb_printf("MFB: FLEXSPI1 Clk Source from 2'b00 - OSC RC24M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_Osc24M));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc400M:
                mfb_printf("MFB: FLEXSPI1 Clk Source from 2'b01 - OSC RC400M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc400M));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0:
                mfb_printf("MFB: FLEXSPI1 Clk Source from 2'b10 - Sys PLL3 PFD0 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll3Pfd0));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd0:
                mfb_printf("MFB: FLEXSPI1 Clk Source from 2'b11 - Sys PLL2 PFD0 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll2Pfd0));
                break;

            default:
                break;
        }
    }
    else if (base == FLEXSPI2)
    {
        root = kCLOCK_Root_Flexspi2;
        clkSel = CLOCK_GetRootClockMux(root);
        switch (clkSel)
        {
            case kCLOCK_FLEXSPI2_ClockRoot_MuxOscRc24M:
                mfb_printf("MFB: FLEXSPI2 Clk Source from 2'b00 - OSC RC24M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_Osc24M));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxOscRc400M:
                mfb_printf("MFB: FLEXSPI2 Clk Source from 2'b01 - OSC RC400M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc400M));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd2:
                mfb_printf("MFB: FLEXSPI2 Clk Source from 2'b10 - Sys PLL3 PFD2 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll3Pfd2));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll2Pfd1:
                mfb_printf("MFB: FLEXSPI2 Clk Source from 2'b11 - Sys PLL2 PFD1 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll2Pfd1));
                break;

            default:
                break;
        }
    }
    else
    {
    }
    index = (root - kCLOCK_Root_Flexspi1) + 1;
    uint32_t clkDiv = CLOCK_GetRootClockDiv(root);
    mfb_printf("MFB: FLEXSPI%d Clk Source Divider: %d.\r\n", index, clkDiv);
    mfb_printf("MFB: FLEXSPI%d Clk Frequency: %dHz.\r\n", index, flexspi_get_clock(EXAMPLE_FLEXSPI));
#endif
}

static void flexspi_sw_delay_us(uint64_t us)
{
    uint32_t ticks_per_us = CLOCK_GetFreq(kCLOCK_CpuClk) / 1000000;
    while (us--)
    {
        register uint32_t ticks = 1 + ticks_per_us / 4;
        while (ticks--)
        {
            __NOP();
        }
    }
}

#endif /* _PORT_FLEXSPI_INFO_H_ */
