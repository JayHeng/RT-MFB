/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _PORT_MIXSPI_INFO_H_
#define _PORT_MIXSPI_INFO_H_

#include "mfb_config.h"
#include "mfb_define.h"

#include "fsl_clock.h"
#include "fsl_qspi.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MFB_MIXSPI_MODULE MFB_MIXSPI_MODULE_IS_QUADSPI

#define EXAMPLE_MIXSPI                  QuadSPI0
#define FLASH_SIZE                      0x00400000U
#define EXAMPLE_MIXSPI_AMBA_BASE        FSL_FEATURE_QSPI_AMBA_BASE
#define EXAMPLE_FLASH_PAGE_SIZE         256
#define EXAMPLE_FLASH_SECTOR_SIZE       0x1000 /* 4K */
#define EXAMPLE_MIXSPI_CLOCK            kCLOCK_McgPll0Clk
#define EXAMPLE_MIXSPI_PORT             0

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void cpu_show_clock_source(void)
{
#if MFB_DEBUG_LOG_INFO_ENABLE

#endif
}

static void mixspi_port_switch(QuadSPI_Type *base, uint32_t port, uint32_t pads)
{
}

static void mixspi_pin_init(QuadSPI_Type *base, uint32_t port, uint32_t pads)
{
    if (base == QuadSPI0)
    {

    }
    else
    {
    }
}

static void mixspi_clock_init(QuadSPI_Type *base, mixspi_root_clk_freq_t clkFreq)
{
    if (base == QuadSPI0)
    {

    }
    else
    {
    }
}

static uint32_t mixspi_get_clock(QuadSPI_Type *base)
{
    if (base == QuadSPI0)
    {
        return CLOCK_GetFreq(kCLOCK_McgPll0Clk);
    }
    else
    {
        return 0;
    }
}

static void mixspi_show_clock_source(QuadSPI_Type *base)
{
#if MFB_DEBUG_LOG_INFO_ENABLE

#endif
}

static void mixspi_sw_delay_us(uint64_t us)
{
    uint32_t ticks_per_us = CLOCK_GetFreq(kCLOCK_CoreSysClk) / 1000000;
    while (us--)
    {
        register uint32_t ticks = 1 + ticks_per_us / 4;
        while (ticks--)
        {
            __NOP();
        }
    }
}

#endif /* _PORT_MIXSPI_INFO_H_ */
