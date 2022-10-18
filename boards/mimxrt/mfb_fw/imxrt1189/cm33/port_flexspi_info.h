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
#include "mfb.h"
#include "mfb_nor_flash.h"

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
#if MFB_LOG_INFO_ENABLE
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

static void flexspi_clock_init(flexspi_root_clk_freq_t clkFreq)
{
    clock_root_config_t rootCfg = {0};

    // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 12-35.
    if (clkFreq == kFlexspiRootClkFreq_30MHz)
    {
        rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc24M;
        rootCfg.div = 1;
        CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
    }
    else if (clkFreq == kFlexspiRootClkFreq_100MHz)
    {
        /* Init System Pll2 (528MHz) pfd0. */
        // 528*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        // CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd0, 27);
        rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd0;
        rootCfg.div = 4;
        CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
    }
    else if (clkFreq == kFlexspiRootClkFreq_133MHz)
    {
        /* Init System Pll3 (480MHz) pfd0. */
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 13-35.
        // CLOCK_InitPfd(kCLOCK_PllSys3, kCLOCK_Pfd0, 22);
        rootCfg.mux = kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0;
        rootCfg.div = 3;
        CLOCK_SetRootClock(kCLOCK_Root_Flexspi1, &rootCfg);
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
#if MFB_LOG_INFO_ENABLE
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

#endif /* _PORT_FLEXSPI_INFO_H_ */
