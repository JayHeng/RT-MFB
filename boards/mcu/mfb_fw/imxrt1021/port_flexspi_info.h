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
#define EXAMPLE_FLEXSPI                 FLEXSPI
#define FLASH_SIZE                      0x2000 /* 64Mb/KByte */
#define EXAMPLE_FLEXSPI_AMBA_BASE       FlexSPI_AMBA_BASE
#define FLASH_PAGE_SIZE                 256
#define SECTOR_SIZE                     0x1000 /* 4K */
#define EXAMPLE_FLEXSPI_CLOCK           kCLOCK_FlexSpi
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
    mfb_printf("MFB: CPU Clk Frequency: %dHz.\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
#endif
}

static void flexspi_port_switch(FLEXSPI_Type *base, flexspi_port_t port, flexspi_pad_t pads)
{
}

static void flexspi_pin_init(FLEXSPI_Type *base, flexspi_port_t port, flexspi_pad_t pads)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);   
    if (base == FLEXSPI)
    {
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_FLEXSPI_A_DQS, 1U); 
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_06_FLEXSPI_A_DATA03, 1U); 
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_07_FLEXSPI_A_SCLK, 1U); 
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_08_FLEXSPI_A_DATA00, 1U); 
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_09_FLEXSPI_A_DATA02, 1U); 
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_10_FLEXSPI_A_DATA01, 1U); 
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_11_FLEXSPI_A_SS0_B, 1U); 
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_05_FLEXSPI_A_DQS, 0x10F1U); 
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_06_FLEXSPI_A_DATA03, 0x10F1U); 
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_07_FLEXSPI_A_SCLK, 0x10F1U); 
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_08_FLEXSPI_A_DATA00, 0x10F1U); 
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_09_FLEXSPI_A_DATA02, 0x10F1U); 
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_10_FLEXSPI_A_DATA01, 0x10F1U); 
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_11_FLEXSPI_A_SS0_B, 0x10F1U); 
    }
    else
    {
    }
}

static void flexspi_clock_init(FLEXSPI_Type *base, mixspi_root_clk_freq_t clkFreq)
{
    if (base == FLEXSPI)
    {
        const clock_usb_pll_config_t g_ccmConfigUsbPll = {.loopDivider = 0U};
        CLOCK_InitUsb1Pll(&g_ccmConfigUsbPll);
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 12-35.
        if (clkFreq == kMixspiRootClkFreq_30MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 30);   /* Set PLL3 PFD0 clock 288MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 7);   /* flexspi clock 36M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_50MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 22);   /* Set PLL3 PFD0 clock 392.7MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 7);   /* flexspi clock 49.1M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_60MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 18);   /* Set PLL3 PFD0 clock 480MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 7);   /* flexspi clock 60M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_80MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 18);   /* Set PLL3 PFD0 clock 480MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 5);   /* flexspi clock 80M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_100MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 13);   /* Set PLL3 PFD0 clock 664.6MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 6);   /* flexspi clock 94.9M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_120MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 18);   /* Set PLL3 PFD0 clock 480MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 3);   /* flexspi clock 120M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_133MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 33);   /* Set PLL3 PFD0 clock 261.8MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 1);   /* flexspi clock 130.9M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_166MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 26);   /* Set PLL3 PFD0 clock 332.3MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 1);   /* flexspi clock 166.1M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_200MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 22);   /* Set PLL3 PFD0 clock 392.7MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 1);   /* flexspi clock 196.3M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_240MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 18);   /* Set PLL3 PFD0 clock 480MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 1);   /* flexspi clock 240M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_266MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 33);   /* Set PLL3 PFD0 clock 261.8MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 0);   /* flexspi clock 261.8M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_332MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 26);   /* Set PLL3 PFD0 clock 332.3MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 0);   /* flexspi clock 332.3M. */
        }
        else if (clkFreq == kMixspiRootClkFreq_400MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 22);   /* Set PLL3 PFD0 clock 392.7MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 0);   /* flexspi clock 392.7M. */
        }
        else
        {
            mfb_printf("MFB: This FlexSPI clock freq is not set.\r\n");
        }
    }
    else
    {
    }
}

static uint32_t flexspi_get_clock(FLEXSPI_Type *base)
{
    if (base == FLEXSPI)
    {
        return CLOCK_GetClockRootFreq(kCLOCK_FlexspiClkRoot);
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
