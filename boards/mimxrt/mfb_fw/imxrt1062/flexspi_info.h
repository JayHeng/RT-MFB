/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FLEXSPI_INFO_H_
#define _FLEXSPI_INFO_H_

#include "fsl_clock.h"
#include "nor_flash.h"

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

static void flexspi_clock_init(flexspi_root_clk_freq_t clkFreq)
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
}

static uint32_t flexspi_get_clock(FLEXSPI_Type *base)
{
    if (base == FLEXSPI)
    {
        return CLOCK_GetClockRootFreq(kCLOCK_FlexspiClkRoot);
    }
    else if (base == FLEXSPI2)
    {
        return CLOCK_GetClockRootFreq(kCLOCK_Flexspi2ClkRoot);
    }
    else
    {
        return 0;
    }
}

#endif /* _FLEXSPI_INFO_H_ */
