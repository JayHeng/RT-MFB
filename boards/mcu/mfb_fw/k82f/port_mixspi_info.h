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
#include "fsl_port.h"
#include "pin_mux.h"
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
        /* Port E Clock Gate Control: Clock enabled */
        CLOCK_EnableClock(kCLOCK_PortE);

        /* PORTE5 (pin 8) is configured as QSPI0A_SS0_B */
        PORT_SetPinMux(PORTE, 5U, kPORT_MuxAlt5);

        /* PORTE1 (pin 2) is configured as QSPI0A_SCLK */
        PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt5);

        /* PORTE2 (pin 3) is configured as QSPI0A_DATA0 */
        PORT_SetPinMux(PORTE, 2U, kPORT_MuxAlt5);

        /* PORTE4 (pin 7) is configured as QSPI0A_DATA1 */
        PORT_SetPinMux(PORTE, 4U, kPORT_MuxAlt5);

        /* PORTE3 (pin 4) is configured as QSPI0A_DATA2 */
        PORT_SetPinMux(PORTE, 3U, kPORT_MuxAlt5);

        /* PORTE0 (pin 1) is configured as QSPI0A_DATA3 */
        PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt5);



        /* PORTE11 (pin 14) is configured as QSPI0B_SS0_B */
        PORT_SetPinMux(PORTE, 11U, kPORT_MuxAlt5);

        /* PORTE7 (pin 10) is configured as QSPI0B_SCLK */
        PORT_SetPinMux(PORTE, 7U, kPORT_MuxAlt5);

        /* PORTE8 (pin 11) is configured as QSPI0B_DATA0 */
        PORT_SetPinMux(PORTE, 8U, kPORT_MuxAlt5);

        /* PORTE10 (pin 13) is configured as QSPI0B_DATA1 */
        PORT_SetPinMux(PORTE, 10U, kPORT_MuxAlt5);

        /* PORTE9 (pin 12) is configured as QSPI0B_DATA2 */
        PORT_SetPinMux(PORTE, 9U, kPORT_MuxAlt5);

        /* PORTE6 (pin 9) is configured as QSPI0B_DATA3 */
        PORT_SetPinMux(PORTE, 6U, kPORT_MuxAlt5);
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
