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
#include "fsl_power.h"
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
    mfb_printf("MFB: CPU0-CM33 Clk Frequency: %dHz.\r\n", CLOCK_GetFreq(kCLOCK_CoreSysClk));
#endif
}

static void mixspi_port_switch(XSPI_Type *base, xspi_target_group_t port, xspi_pad_t pads)
{
}

static void mixspi_pin_init(XSPI_Type *base, xspi_target_group_t port, xspi_pad_t pads)
{
    if (base == XSPI0)
    {
        /* Reset IOPCTL2 module */
        RESET_ClearPeripheralReset(kIOPCTL2_RST_SHIFT_RSTn);

        const uint32_t port6_pin1_config = (/* Pin is configured as XSPI0_SCK_A */
                                            IOPCTL_PIO_FUNC1 |
                                            /* Disable pull-up / pull-down function */
                                            IOPCTL_PIO_PUPD_DI |
                                            /* Enable pull-down function */
                                            IOPCTL_PIO_PULLDOWN_EN |
                                            /* Enables input buffer function */
                                            IOPCTL_PIO_INBUF_EN |
                                            /* Pseudo Output Drain is disabled */
                                            IOPCTL_PIO_PSEDRAIN_DI |
                                            /* Input function is not inverted */
                                            IOPCTL_PIO_INV_DI |
                                            /* Selects transmitter current drive 100ohm */
                                            IOPCTL_PIO_DRIVE_100OHM);
        /* PORT6 PIN1 (coords: D17) is configured as XSPI0_SCK_A */
        IOPCTL_PinMuxSet(6U, 1U, port6_pin1_config);

        const uint32_t port6_pin2_config = (/* Pin is configured as XSPI0_PCS_A_0 */
                                            IOPCTL_PIO_FUNC1 |
                                            /* Disable pull-up / pull-down function */
                                            IOPCTL_PIO_PUPD_DI |
                                            /* Enable pull-down function */
                                            IOPCTL_PIO_PULLDOWN_EN |
                                            /* Enables input buffer function */
                                            IOPCTL_PIO_INBUF_EN |
                                            /* Pseudo Output Drain is disabled */
                                            IOPCTL_PIO_PSEDRAIN_DI |
                                            /* Input function is not inverted */
                                            IOPCTL_PIO_INV_DI |
                                            /* Selects transmitter current drive 100ohm */
                                            IOPCTL_PIO_DRIVE_100OHM);
        /* PORT6 PIN2 (coords: G11) is configured as XSPI0_PCS_A_0 */
        IOPCTL_PinMuxSet(6U, 2U, port6_pin2_config);

        const uint32_t port6_pin7_config = (/* Pin is configured as XSPI0_DQS_A_0 */
                                            IOPCTL_PIO_FUNC1 |
                                            /* Disable pull-up / pull-down function */
                                            IOPCTL_PIO_PUPD_DI |
                                            /* Enable pull-down function */
                                            IOPCTL_PIO_PULLDOWN_EN |
                                            /* Enables input buffer function */
                                            IOPCTL_PIO_INBUF_EN |
                                            /* Pseudo Output Drain is disabled */
                                            IOPCTL_PIO_PSEDRAIN_DI |
                                            /* Input function is not inverted */
                                            IOPCTL_PIO_INV_DI |
                                            /* Selects transmitter current drive 100ohm */
                                            IOPCTL_PIO_DRIVE_100OHM);
        /* PORT6 PIN7 (coords: D15) is configured as XSPI0_DQS_A_0 */
        IOPCTL_PinMuxSet(6U, 7U, port6_pin7_config);

        const uint32_t port6_pin3_config = (/* Pin is configured as XSPI0_DATA_A0 */
                                            IOPCTL_PIO_FUNC1 |
                                            /* Disable pull-up / pull-down function */
                                            IOPCTL_PIO_PUPD_DI |
                                            /* Enable pull-down function */
                                            IOPCTL_PIO_PULLDOWN_EN |
                                            /* Enables input buffer function */
                                            IOPCTL_PIO_INBUF_EN |
                                            /* Pseudo Output Drain is disabled */
                                            IOPCTL_PIO_PSEDRAIN_DI |
                                            /* Input function is not inverted */
                                            IOPCTL_PIO_INV_DI |
                                            /* Selects transmitter current drive 100ohm */
                                            IOPCTL_PIO_DRIVE_100OHM);
        /* PORT6 PIN3 (coords: F13) is configured as XSPI0_DATA_A0 */
        IOPCTL_PinMuxSet(6U, 3U, port6_pin3_config);

        const uint32_t port6_pin4_config = (/* Pin is configured as XSPI0_DATA_A1 */
                                            IOPCTL_PIO_FUNC1 |
                                            /* Disable pull-up / pull-down function */
                                            IOPCTL_PIO_PUPD_DI |
                                            /* Enable pull-down function */
                                            IOPCTL_PIO_PULLDOWN_EN |
                                            /* Enables input buffer function */
                                            IOPCTL_PIO_INBUF_EN |
                                            /* Pseudo Output Drain is disabled */
                                            IOPCTL_PIO_PSEDRAIN_DI |
                                            /* Input function is not inverted */
                                            IOPCTL_PIO_INV_DI |
                                            /* Selects transmitter current drive 100ohm */
                                            IOPCTL_PIO_DRIVE_100OHM);
        /* PORT6 PIN4 (coords: D14) is configured as XSPI0_DATA_A1 */
        IOPCTL_PinMuxSet(6U, 4U, port6_pin4_config);

        const uint32_t port6_pin5_config = (/* Pin is configured as XSPI0_DATA_A2 */
                                            IOPCTL_PIO_FUNC1 |
                                            /* Disable pull-up / pull-down function */
                                            IOPCTL_PIO_PUPD_DI |
                                            /* Enable pull-down function */
                                            IOPCTL_PIO_PULLDOWN_EN |
                                            /* Enables input buffer function */
                                            IOPCTL_PIO_INBUF_EN |
                                            /* Pseudo Output Drain is disabled */
                                            IOPCTL_PIO_PSEDRAIN_DI |
                                            /* Input function is not inverted */
                                            IOPCTL_PIO_INV_DI |
                                            /* Selects transmitter current drive 100ohm */
                                            IOPCTL_PIO_DRIVE_100OHM);
        /* PORT6 PIN5 (coords: C15) is configured as XSPI0_DATA_A2 */
        IOPCTL_PinMuxSet(6U, 5U, port6_pin5_config);

        const uint32_t port6_pin6_config = (/* Pin is configured as XSPI0_DATA_A3 */
                                            IOPCTL_PIO_FUNC1 |
                                            /* Disable pull-up / pull-down function */
                                            IOPCTL_PIO_PUPD_DI |
                                            /* Enable pull-down function */
                                            IOPCTL_PIO_PULLDOWN_EN |
                                            /* Enables input buffer function */
                                            IOPCTL_PIO_INBUF_EN |
                                            /* Pseudo Output Drain is disabled */
                                            IOPCTL_PIO_PSEDRAIN_DI |
                                            /* Input function is not inverted */
                                            IOPCTL_PIO_INV_DI |
                                            /* Selects transmitter current drive 100ohm */
                                            IOPCTL_PIO_DRIVE_100OHM);
        /* PORT6 PIN6 (coords: E13) is configured as XSPI0_DATA_A3 */
        IOPCTL_PinMuxSet(6U, 6U, port6_pin6_config);

        const uint32_t port6_pin8_config = (/* Pin is configured as XSPI0_DATA_A4 */
                                            IOPCTL_PIO_FUNC1 |
                                            /* Disable pull-up / pull-down function */
                                            IOPCTL_PIO_PUPD_DI |
                                            /* Enable pull-down function */
                                            IOPCTL_PIO_PULLDOWN_EN |
                                            /* Enables input buffer function */
                                            IOPCTL_PIO_INBUF_EN |
                                            /* Pseudo Output Drain is disabled */
                                            IOPCTL_PIO_PSEDRAIN_DI |
                                            /* Input function is not inverted */
                                            IOPCTL_PIO_INV_DI |
                                            /* Selects transmitter current drive 100ohm */
                                            IOPCTL_PIO_DRIVE_100OHM);
        /* PORT6 PIN8 (coords: G13) is configured as XSPI0_DATA_A4 */
        IOPCTL_PinMuxSet(6U, 8U, port6_pin8_config);

        const uint32_t port6_pin9_config = (/* Pin is configured as XSPI0_DATA_A5 */
                                            IOPCTL_PIO_FUNC1 |
                                            /* Disable pull-up / pull-down function */
                                            IOPCTL_PIO_PUPD_DI |
                                            /* Enable pull-down function */
                                            IOPCTL_PIO_PULLDOWN_EN |
                                            /* Enables input buffer function */
                                            IOPCTL_PIO_INBUF_EN |
                                            /* Pseudo Output Drain is disabled */
                                            IOPCTL_PIO_PSEDRAIN_DI |
                                            /* Input function is not inverted */
                                            IOPCTL_PIO_INV_DI |
                                            /* Selects transmitter current drive 100ohm */
                                            IOPCTL_PIO_DRIVE_100OHM);
        /* PORT6 PIN9 (coords: F15) is configured as XSPI0_DATA_A5 */
        IOPCTL_PinMuxSet(6U, 9U, port6_pin9_config);

        const uint32_t port6_pin10_config = (/* Pin is configured as XSPI0_DATA_A6 */
                                             IOPCTL_PIO_FUNC1 |
                                             /* Disable pull-up / pull-down function */
                                             IOPCTL_PIO_PUPD_DI |
                                             /* Enable pull-down function */
                                             IOPCTL_PIO_PULLDOWN_EN |
                                             /* Enables input buffer function */
                                             IOPCTL_PIO_INBUF_EN |
                                             /* Pseudo Output Drain is disabled */
                                             IOPCTL_PIO_PSEDRAIN_DI |
                                             /* Input function is not inverted */
                                             IOPCTL_PIO_INV_DI |
                                             /* Selects transmitter current drive 100ohm */
                                             IOPCTL_PIO_DRIVE_100OHM);
        /* PORT6 PIN10 (coords: E14) is configured as XSPI0_DATA_A6 */
        IOPCTL_PinMuxSet(6U, 10U, port6_pin10_config);

        const uint32_t port6_pin11_config = (/* Pin is configured as XSPI0_DATA_A7 */
                                             IOPCTL_PIO_FUNC1 |
                                             /* Disable pull-up / pull-down function */
                                             IOPCTL_PIO_PUPD_DI |
                                             /* Enable pull-down function */
                                             IOPCTL_PIO_PULLDOWN_EN |
                                             /* Enables input buffer function */
                                             IOPCTL_PIO_INBUF_EN |
                                             /* Pseudo Output Drain is disabled */
                                             IOPCTL_PIO_PSEDRAIN_DI |
                                             /* Input function is not inverted */
                                             IOPCTL_PIO_INV_DI |
                                             /* Selects transmitter current drive 100ohm */
                                             IOPCTL_PIO_DRIVE_100OHM);
        /* PORT6 PIN11 (coords: F14) is configured as XSPI0_DATA_A7 */
        IOPCTL_PinMuxSet(6U, 11U, port6_pin11_config);
        
        mfb_printf("MFB: XSPI0 pins are - P6[11:01].\r\n");
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
        ///////////////////////////////////////////////////////////////////
        // default settings in clock_config.c
        // baseclk_comn  - 192MHz
        // audio_pll_vco - 532.48 MHz
        // fro0_max      - 250MHz
        // main_pll_pfd1 - 396MHz
        ///////////////////////////////////////////////////////////////////
        if (clkFreq == kMixspiRootClkFreq_30MHz)
        {
            CLOCK_AttachClk(kCOMMON_BASE_to_XSPI0);
            CLOCK_SetClkDiv(kCLOCK_DivXspi0Clk, 7u);
        }
        else if (clkFreq == kMixspiRootClkFreq_50MHz)
        {
            CLOCK_AttachClk(kMAIN_PLL_PFD1_to_XSPI0);
            CLOCK_SetClkDiv(kCLOCK_DivXspi0Clk, 8u);
        }
        else if (clkFreq == kMixspiRootClkFreq_60MHz)
        {

        }
        else if (clkFreq == kMixspiRootClkFreq_80MHz)
        {
            CLOCK_AttachClk(kMAIN_PLL_PFD1_to_XSPI0);
            CLOCK_SetClkDiv(kCLOCK_DivXspi0Clk, 5u);
        }
        else if (clkFreq == kMixspiRootClkFreq_100MHz)
        {
            CLOCK_AttachClk(kMAIN_PLL_PFD1_to_XSPI0);
            CLOCK_SetClkDiv(kCLOCK_DivXspi0Clk, 4u);
        }
        else if (clkFreq == kMixspiRootClkFreq_120MHz)
        {

        }
        else if (clkFreq == kMixspiRootClkFreq_133MHz)
        {
            CLOCK_AttachClk(kMAIN_PLL_PFD1_to_XSPI0);
            CLOCK_SetClkDiv(kCLOCK_DivXspi0Clk, 3u);
        }
        else if (clkFreq == kMixspiRootClkFreq_166MHz)
        {

        }
        else if (clkFreq == kMixspiRootClkFreq_200MHz)
        {
            CLOCK_AttachClk(kMAIN_PLL_PFD1_to_XSPI0);
            CLOCK_SetClkDiv(kCLOCK_DivXspi0Clk, 2u);
        }
        else if (clkFreq == kMixspiRootClkFreq_332MHz)
        {

        }
        else if (clkFreq == kMixspiRootClkFreq_400MHz)
        {
            CLOCK_AttachClk(kMAIN_PLL_PFD1_to_XSPI0);
            CLOCK_SetClkDiv(kCLOCK_DivXspi0Clk, 1u);
        }
        else
        {
            mfb_printf("MFB: This FlexSPI clock freq is not set.\r\n");
        }
        POWER_DisablePD(kPDRUNCFG_APD_XSPI0);
        POWER_DisablePD(kPDRUNCFG_PPD_XSPI0);
        POWER_ApplyPD();
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
    uint32_t index = 0;
    if (base == XSPI0)
    {
        index = 0;
    }
    else if (base == XSPI1)
    {
        index = 1;
    }
    else if (base == XSPI2)
    {
        index = 2;
    }
    else
    {}
    mfb_printf("MFB: XSPI%d Clk Frequency: %dHz.\r\n", index, mixspi_get_clock(EXAMPLE_MIXSPI));
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
