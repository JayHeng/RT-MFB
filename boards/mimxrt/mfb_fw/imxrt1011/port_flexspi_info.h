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
#include "mfb_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_FLEXSPI                 FLEXSPI
#define FLASH_SIZE                      0x4000 /* 128Mb/KByte */
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
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_06_FLEXSPI_A_SS0_B, 1U); 
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_07_FLEXSPI_A_DATA1, 1U); 
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_08_FLEXSPI_A_DATA2, 1U); 
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_09_FLEXSPI_A_DATA0, 1U); 
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_10_FLEXSPI_A_SCLK, 1U); 
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_11_FLEXSPI_A_DATA3, 1U); 
        IOMUXC_SetPinMux(IOMUXC_GPIO_SD_12_FLEXSPI_A_DQS, 1U); 
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_06_FLEXSPI_A_SS0_B, 0x10E1U); 
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_07_FLEXSPI_A_DATA1, 0x10E1U); 
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_08_FLEXSPI_A_DATA2, 0x10E1U); 
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_09_FLEXSPI_A_DATA0, 0x10E1U); 
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_10_FLEXSPI_A_SCLK, 0x10E1U); 
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_11_FLEXSPI_A_DATA3, 0x10E1U); 
        IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_12_FLEXSPI_A_DQS, 0x10E1U);
    }
    else
    {
    }
}

static void flexspi_clock_init(FLEXSPI_Type *base, flexspi_root_clk_freq_t clkFreq)
{
    if (base == FLEXSPI)
    {
        const clock_usb_pll_config_t g_ccmConfigUsbPll = {.loopDivider = 0U};
        CLOCK_InitUsb1Pll(&g_ccmConfigUsbPll);
        // 480*18/PFDx_FRAC where PFDx_FRAC is in the range 12-35.
        if (clkFreq == kFlexspiRootClkFreq_30MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 30);   /* Set PLL3 PFD0 clock 288MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 7);   /* flexspi clock 36M. */
        }
        else if (clkFreq == kFlexspiRootClkFreq_80MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 22);   /* Set PLL3 PFD0 clock 392.7MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 4);   /* flexspi clock 78.5M. */
        }
        else if (clkFreq == kFlexspiRootClkFreq_100MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 13);   /* Set PLL3 PFD0 clock 664.6MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 6);   /* flexspi clock 94.9M. */
        }
        else if (clkFreq == kFlexspiRootClkFreq_133MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 33);   /* Set PLL3 PFD0 clock 261.8MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 1);   /* flexspi clock 130.9M. */
        }
        else if (clkFreq == kFlexspiRootClkFreq_166MHz)
        {
            CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 26);   /* Set PLL3 PFD0 clock 332.3MHZ. */
            CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
            CLOCK_SetDiv(kCLOCK_FlexspiDiv, 1);   /* flexspi clock 1366.1M. */
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
    mfb_printf("MFB: FLEXSPI Clk Frequency: %dHz.\r\n", flexspi_get_clock(EXAMPLE_FLEXSPI));
#endif
}

#endif /* _PORT_FLEXSPI_INFO_H_ */
