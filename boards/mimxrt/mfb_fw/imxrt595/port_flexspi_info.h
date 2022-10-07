/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _PORT_FLEXSPI_INFO_H_
#define _PORT_FLEXSPI_INFO_H_

/*${header:start}*/
#include "fsl_cache.h"
#include "board.h"
#include "mfb_nor_flash.h"
/*${header:end}*/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_FLEXSPI                 FLEXSPI0
#define EXAMPLE_CACHE                   CACHE64_CTRL0
#define EXAMPLE_FLEXSPI_AMBA_BASE       FlexSPI0_AMBA_BASE
#define FLASH_PAGE_SIZE                 256
#define SECTOR_SIZE                     0x1000 /* 4K */
#define EXAMPLE_FLEXSPI_CLOCK           kCLOCK_FlexSpi0
#define FLASH_PORT                      kFLEXSPI_PortA1

#define CACHE_MAINTAIN           1
#define EXAMPLE_INVALIDATE_FLEXSPI_CACHE()                                                                          \
    do                                                                                                              \
    {                                                                                                               \
        CACHE64_CTRL0->CCR |= CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK | CACHE64_CTRL_CCR_GO_MASK; \
        while (CACHE64_CTRL0->CCR & CACHE64_CTRL_CCR_GO_MASK)                                                       \
        {                                                                                                           \
        }                                                                                                           \
        CACHE64_CTRL0->CCR &= ~(CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK);                         \
    } while (0)

/*${macro:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
typedef struct _flexspi_cache_status
{
    volatile bool CacheEnableFlag;
} flexspi_cache_status_t;
/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void flexspi_clock_init(flexspi_root_clk_freq_t clkFreq)
{
    if (clkFreq == kFlexspiRootClkFreq_30MHz)
    {
        /* Move FLEXSPI clock source from main clock to FRO192M / 7 to avoid instruction/data fetch issue in XIP when
         * updating PLL and main clock.
         */
        BOARD_SetFlexspiClock(EXAMPLE_FLEXSPI, 3U, 7U);
    }
    else if (clkFreq == kFlexspiRootClkFreq_100MHz)
    {
        /* Set FlexSPI clock: source AUX0_PLL, divide by 4 */
        BOARD_SetFlexspiClock(EXAMPLE_FLEXSPI, 2U, 4U);
    }
    else if (clkFreq == kFlexspiRootClkFreq_133MHz)
    {
        /* Set FlexSPI clock: source AUX0_PLL, divide by 3 */
        BOARD_SetFlexspiClock(EXAMPLE_FLEXSPI, 2U, 3U);
    }
}

static uint32_t flexspi_get_clock(FLEXSPI_Type *base)
{
    if (base == FLEXSPI0)
    {
        return CLOCK_GetFlexspiClkFreq(0);
    }
    else if (base == FLEXSPI1)
    {
        return CLOCK_GetFlexspiClkFreq(1);
    }
    else
    {
        return 0;
    }
}

#endif /* _PORT_FLEXSPI_INFO_H_ */
