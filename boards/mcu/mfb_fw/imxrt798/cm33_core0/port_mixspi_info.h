/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _PORT_MIXSPI_INFO_H_
#define _PORT_MIXSPI_INFO_H_

#include "mfb_config.h"
#include "mfb_define.h"

#include "fsl_cache.h"
#include "fsl_clock.h"
#include "fsl_iopctl.h"
#include "fsl_xspi.h"
#include "pin_mux.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MFB_MIXSPI_MODULE MFB_MIXSPI_MODULE_IS_XSPI

#define EXAMPLE_MIXSPI                  XSPI0
#define FLASH_SIZE                      0x10000 /* 64MB/KByte */
#define EXAMPLE_CACHE                   CACHE64_CTRL0
#define EXAMPLE_MIXSPI_AMBA_BASE        XSPI0_AMBA_BASE
#define EXAMPLE_FLASH_PAGE_SIZE         256
#define EXAMPLE_FLASH_SECTOR_SIZE       0x1000 /* 4K */
#define EXAMPLE_MIXSPI_CLOCK            kCLOCK_Xspi0
#define EXAMPLE_MIXSPI_PORT             kXSPI_TargetGroup0

#define CACHE_MAINTAIN           1
#define EXAMPLE_INVALIDATE_XSPI_CACHE do { \
  XCACHE_InvalidateCache(XCACHE1);      \
  XSPI_Cache64_InvalidateCache(CACHE64_CTRL0); \
  } while(0)

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
typedef struct _xspi_cache_status
{
    volatile bool codeCacheEnableFlag;
    volatile bool systemCacheEnableFlag;
} xspi_cache_status_t;
/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void cpu_show_clock_source(void)
{
#if MFB_DEBUG_LOG_INFO_ENABLE

#endif
}

static void mixspi_port_switch(XSPI_Type *base, mixspi_port_t port, xspi_pad_t pads)
{
}

static void mixspi_pin_init(XSPI_Type *base, mixspi_port_t port, xspi_pad_t pads)
{
    if (base == XSPI0)
    {
    }
    else if (base == XSPI1)
    {
    }
    else if (base == XSPI2)
    {
    }
    else
    {}
}

static void mixspi_clock_init(XSPI_Type *base, mixspi_root_clk_freq_t clkFreq)
{
    if (base == XSPI0)
    {
    }
    else if (base == XSPI1)
    {
    }
    else if (base == XSPI2)
    {
    }
    else
    {}
}

static uint32_t mixspi_get_clock(XSPI_Type *base)
{
    if (base == XSPI0)
    {
        return CLOCK_GetXspiClkFreq(0);
    }
    else if (base == XSPI1)
    {
        return CLOCK_GetXspiClkFreq(1);
    }
    else if (base == XSPI2)
    {
        return CLOCK_GetXspiClkFreq(2);
    }
    else
    {
        return 0;
    }
}

static void mixspi_show_clock_source(XSPI_Type *base)
{
#if MFB_DEBUG_LOG_INFO_ENABLE
    if (base == XSPI0)
    {
    }
    else if (base == XSPI1)
    {
    }
    else if (base == XSPI2)
    {
    }
    else
    {}
#endif
}

#define FREQ_1MHz (1000000U)
static void mixspi_sw_delay_us(uint64_t us)
{
    uint32_t ticks_per_us = CLOCK_GetFreq(kCLOCK_CoreSysClk) / FREQ_1MHz;
    while (us--)
    {
        // Measured on RTL testbench, the below loop needs 5 ticks
        register uint32_t ticks = ticks_per_us / 5;
        while (ticks--)
        {
            __NOP();
        }
    }
}

#endif /* _PORT_MIXSPI_INFO_H_ */
