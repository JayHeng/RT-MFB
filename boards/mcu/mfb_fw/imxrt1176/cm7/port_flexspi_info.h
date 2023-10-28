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
#include "fsl_flexspi.h"
#include "mfb.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_FLEXSPI                 FLEXSPI1
#define FLASH_SIZE                      0x8000 /* 32MB/KByte */
#define EXAMPLE_FLEXSPI_AMBA_BASE       FlexSPI1_AMBA_BASE
#define FLASH_PAGE_SIZE                 256
#define SECTOR_SIZE                     0x1000 /* 4K */
#define EXAMPLE_FLEXSPI_CLOCK           kCLOCK_Flexspi1
#define FLASH_PORT                      kFLEXSPI_PortA1

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
    volatile bool DCacheEnableFlag;
    volatile bool ICacheEnableFlag;
} flexspi_cache_status_t;
#endif
/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void cpu_show_clock_source(void)
{
#if MFB_DEBUG_LOG_INFO_ENABLE
    uint32_t clkSel = CLOCK_GetRootClockMux(kCLOCK_Root_M7);
    switch (clkSel)
    {
        case kCLOCK_M7_ClockRoot_MuxOscRc48MDiv2:
            mfb_printf("MFB: CPU-CM7 Clk Source from 3'b000 - OSC RC48M Div2 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc48MDiv2));
            break;

        case kCLOCK_M7_ClockRoot_MuxOsc24MOut:
            mfb_printf("MFB: CPU-CM7 Clk Source from 3'b001 - OSC RC24M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_Osc24MOut));
            break;

        case kCLOCK_M7_ClockRoot_MuxOscRc400M:
            mfb_printf("MFB: CPU-CM7 Clk Source from 3'b010 - OSC RC400M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc400M));
            break;

        case kCLOCK_M7_ClockRoot_MuxOscRc16M:
            mfb_printf("MFB: CPU-CM7 Clk Source from 3'b011 - OSC RC16M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc16M));
            break;

        case kCLOCK_M7_ClockRoot_MuxArmPllOut:
            mfb_printf("MFB: CPU-CM7 Clk Source from 3'b100 - ARM PLL clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_ArmPllOut));
            break;

        case kCLOCK_M7_ClockRoot_MuxSysPll1Out:
            mfb_printf("MFB: CPU-CM7 Clk Source from 3'b101 - Sys PLL1 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll1Out));
            break;

        case kCLOCK_M7_ClockRoot_MuxSysPll3Out:
            mfb_printf("MFB: CPU-CM7 Clk Source from 3'b110 - Sys PLL3 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll3Out));
            break;

        case kCLOCK_M7_ClockRoot_MuxVideoPllOut:
            mfb_printf("MFB: CPU-CM7 Clk Source from 3'b111 - Video PLL clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_VideoPllOut));
            break;

        default:
            break;
    }

    uint32_t clkDiv = CLOCK_GetRootClockDiv(kCLOCK_Root_M7);
    mfb_printf("MFB: CPU-CM7 Clk Source Divider: %d.\r\n", clkDiv);
    mfb_printf("MFB: CPU-CM7 Clk Frequency: %dHz.\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
#endif
}

static void flexspi_port_switch(FLEXSPI_Type *base, flexspi_port_t port, flexspi_pad_t pads)
{
}

static void flexspi_pin_init(FLEXSPI_Type *base, flexspi_port_t port, flexspi_pad_t pads)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);           /* LPCG on: LPCG is ON. */
    if (base == FLEXSPI1)
    {
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_00_FLEXSPI1_B_DATA03, 1U);
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_01_FLEXSPI1_B_DATA02, 1U);
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_02_FLEXSPI1_B_DATA01, 1U);
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_03_FLEXSPI1_B_DATA00, 1U);

        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_05_FLEXSPI1_A_DQS,    1U);
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_06_FLEXSPI1_A_SS0_B,  1U);
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_07_FLEXSPI1_A_SCLK,   1U);
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_08_FLEXSPI1_A_DATA00, 1U);
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_09_FLEXSPI1_A_DATA01, 1U);
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_10_FLEXSPI1_A_DATA02, 1U);
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B2_11_FLEXSPI1_A_DATA03, 1U);

        // 0x0A - Normal drive 
        // 0x08 - High drive
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_00_FLEXSPI1_B_DATA03,  0x08U);
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_01_FLEXSPI1_B_DATA02,  0x08U);
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_02_FLEXSPI1_B_DATA01,  0x08U);
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_03_FLEXSPI1_B_DATA00,  0x08U);

        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_05_FLEXSPI1_A_DQS,     0x08U);
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_06_FLEXSPI1_A_SS0_B,   0x0AU);
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_07_FLEXSPI1_A_SCLK,    0x08U);
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_08_FLEXSPI1_A_DATA00,  0x08U);
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_09_FLEXSPI1_A_DATA01,  0x08U);
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_10_FLEXSPI1_A_DATA02,  0x08U);
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B2_11_FLEXSPI1_A_DATA03,  0x08U);
    }
    else if (base == FLEXSPI2)
    {
        
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
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 12-35.
        if (clkFreq == kMixspiRootClkFreq_30MHz)
        {
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc48MDiv2;
            rootCfg.div = 1;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_50MHz)
        {
            /* Init System Pll2 (528MHz) pfd2. */
            // 528*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            // CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd2, 24);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd2;
            rootCfg.div = 8;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_60MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 18);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 8;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_80MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 18);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 6;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_100MHz)
        {
            /* Init System Pll2 (528MHz) pfd2. */
            // 528*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            // CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd2, 24);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd2;
            rootCfg.div = 4;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_120MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 18);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 4;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_133MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            //CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 13);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 5;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_166MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            //CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 13);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 4;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_200MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 22);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 2;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_240MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 18);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 2;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_266MHz)
        {
            /* Init System Pll2 (528MHz) pfd2. */
            // 528*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd2, 18);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd2;
            rootCfg.div = 2;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_332MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            //CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 13);
            rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
            rootCfg.div = 2;
            CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
        }
        else if (clkFreq == kMixspiRootClkFreq_400MHz)
        {
            /* Init System Pll3 (480MHz) pfd0. */
            // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
            CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 22);
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
            case kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc48MDiv2:
                mfb_printf("MFB: FLEXSPI1 Clk Source from 3'b000 - OSC RC48M Div2 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc48MDiv2));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxOsc24MOut:
                mfb_printf("MFB: FLEXSPI1 Clk Source from 3'b001 - OSC RC24M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_Osc24MOut));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc400M:
                mfb_printf("MFB: FLEXSPI1 Clk Source from 3'b010 - OSC RC400M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc400M));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc16M:
                mfb_printf("MFB: FLEXSPI1 Clk Source from 3'b011 - OSC RC16M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc16M));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0:
                mfb_printf("MFB: FLEXSPI1 Clk Source from 3'b100 - Sys PLL3 PFD0 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll3Pfd0));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Out:
                mfb_printf("MFB: FLEXSPI1 Clk Source from 3'b101 - Sys PLL2 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll2));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd2:
                mfb_printf("MFB: FLEXSPI1 Clk Source from 3'b110 - Sys PLL2 PFD2 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll2Pfd2));
                break;

            case kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Out:
                mfb_printf("MFB: FLEXSPI1 Clk Source from 3'b111 - Sys PLL3 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll3));
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
            case kCLOCK_FLEXSPI2_ClockRoot_MuxOscRc48MDiv2:
                mfb_printf("MFB: FLEXSPI2 Clk Source from 3'b000 - OSC RC48M Div2 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc48MDiv2));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxOsc24MOut:
                mfb_printf("MFB: FLEXSPI2 Clk Source from 3'b001 - OSC RC24M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_Osc24MOut));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxOscRc400M:
                mfb_printf("MFB: FLEXSPI2 Clk Source from 3'b010 - OSC RC400M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc400M));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxOscRc16M:
                mfb_printf("MFB: FLEXSPI2 Clk Source from 3'b011 - OSC RC16M clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_OscRc16M));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Pfd0:
                mfb_printf("MFB: FLEXSPI2 Clk Source from 3'b100 - Sys PLL3 PFD0 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll3Pfd0));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll2Out:
                mfb_printf("MFB: FLEXSPI2 Clk Source from 3'b101 - Sys PLL2 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll2));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll2Pfd2:
                mfb_printf("MFB: FLEXSPI2 Clk Source from 3'b110 - Sys PLL2 PFD2 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll2Pfd2));
                break;

            case kCLOCK_FLEXSPI2_ClockRoot_MuxSysPll3Out:
                mfb_printf("MFB: FLEXSPI2 Clk Source from 3'b111 - Sys PLL3 clock %dHz.\r\n", CLOCK_GetFreq(kCLOCK_SysPll3));
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
