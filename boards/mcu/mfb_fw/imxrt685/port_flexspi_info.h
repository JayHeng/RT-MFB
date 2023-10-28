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
#include "fsl_clock.h"
#include "fsl_cache.h"
#include "fsl_iopctl.h"
#include "fsl_flexspi.h"
#include "pin_mux.h"
#include "board.h"
#include "mfb.h"
/*${header:end}*/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_MIXSPI                  FLEXSPI
#define EXAMPLE_CACHE                   CACHE64
#define EXAMPLE_MIXSPI_AMBA_BASE        FlexSPI_AMBA_BASE
#define EXAMPLE_FLASH_PAGE_SIZE         256
#define EXAMPLE_FLASH_SECTOR_SIZE       0x1000 /* 4K */
#define EXAMPLE_MIXSPI_CLOCK            kCLOCK_FlexSpi
#define EXAMPLE_MIXSPI_PORT             kFLEXSPI_PortB1

#define CACHE_MAINTAIN           1
#define EXAMPLE_INVALIDATE_FLEXSPI_CACHE()                                                                          \
    do                                                                                                              \
    {                                                                                                               \
        CACHE64->CCR |= CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK | CACHE64_CTRL_CCR_GO_MASK; \
        while (CACHE64->CCR & CACHE64_CTRL_CCR_GO_MASK)                                                       \
        {                                                                                                           \
        }                                                                                                           \
        CACHE64->CCR &= ~(CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK);                         \
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

static void cpu_show_clock_source(void)
{
#if MFB_DEBUG_LOG_INFO_ENABLE
    // Refer to CLOCK_GetMainClkFreq() in fsl_clock.c
    uint32_t mainClkSelB = (CLKCTL0->MAINCLKSELB) & CLKCTL0_MAINCLKSELB_SEL_MASK;
    uint32_t mainClkSelA = (CLKCTL0->MAINCLKSELA) & CLKCTL0_MAINCLKSELA_SEL_MASK;
    uint32_t clkDiv = 0;

    switch (mainClkSelB)
    {
        case CLKCTL0_MAINCLKSELB_SEL(0):
            switch (mainClkSelA)
            {
                case CLKCTL0_MAINCLKSELA_SEL(0):
                    mfb_printf("MFB: CPU Clk Source from MAINCLKSELA 2'b00 - FFRO clock (48/60m_irc) div4 Clock %dHz.\r\n", CLOCK_GetFFroFreq()/4);
                    break;
                case CLKCTL0_MAINCLKSELA_SEL(1):
                    mfb_printf("MFB: CPU Clk Source from MAINCLKSELA 2'b01 - OSC clock %dHz.\r\n", CLOCK_GetXtalInClkFreq());
                    break;
                case CLKCTL0_MAINCLKSELA_SEL(2):
                    mfb_printf("MFB: CPU Clk Source from MAINCLKSELA 2'b10 - LPOSC Clock %dHz.\r\n", CLOCK_GetLpOscFreq());
                    break;
                case CLKCTL0_MAINCLKSELA_SEL(3):
                    mfb_printf("MFB: CPU Clk Source from MAINCLKSELA 2'b11 - FFRO Clock %dHz.\r\n", CLOCK_GetFFroFreq());
                    break;
                default:
                    break;
            }
            break;

        case CLKCTL0_MAINCLKSELB_SEL(1):
            mfb_printf("MFB: CPU Clk Source from MAINCLKSELB 2'b01 - SFRO Clock %dHz.\r\n", CLOCK_GetSFroFreq());
            break;

        case CLKCTL0_MAINCLKSELB_SEL(2):
            mfb_printf("MFB: CPU Clk Source from MAINCLKSELB 2'b10 - Main System PLL Clock %dHz.\r\n", CLOCK_GetSysPllFreq());
            break;

        case CLKCTL0_MAINCLKSELB_SEL(3):
        default:
            mfb_printf("MFB: CPU Clk Source from MAINCLKSELB 2'b11 - RTC 32KHz Clock %dHz.\r\n", CLOCK_GetOsc32KFreq());
            break;
    }
    
    clkDiv = (CLKCTL0->SYSCPUAHBCLKDIV & CLKCTL0_SYSCPUAHBCLKDIV_DIV_MASK) >> CLKCTL0_SYSCPUAHBCLKDIV_DIV_SHIFT;
    mfb_printf("MFB: CPU Clk Source Divider: %d.\r\n", (clkDiv + 1U));
    mfb_printf("MFB: CPU Clk Frequency: %dHz.\r\n", CLOCK_GetFreq(kCLOCK_CoreSysClk));
#endif
}

static void mixspi_port_switch(FLEXSPI_Type *base, flexspi_port_t port, flexspi_pad_t pads)
{
}

static void mixspi_pin_init(FLEXSPI_Type *base, flexspi_port_t port, flexspi_pad_t pads)
{
    if (base == FLEXSPI)
    {
        const uint32_t port1_pin11_config = (/* Pin is configured as FLEXSPI0B_DATA0 */
                                             IOPCTL_PIO_FUNC6 |
                                             /* Disable pull-up / pull-down function */
                                             IOPCTL_PIO_PUPD_DI |
                                             /* Enable pull-down function */
                                             IOPCTL_PIO_PULLDOWN_EN |
                                             /* Enables input buffer function */
                                             IOPCTL_PIO_INBUF_EN |
                                             /* Normal mode */
                                             IOPCTL_PIO_SLEW_RATE_NORMAL |
                                             /* Full drive enable */
                                             IOPCTL_PIO_FULLDRIVE_EN |
                                             /* Analog mux is disabled */
                                             IOPCTL_PIO_ANAMUX_DI |
                                             /* Pseudo Output Drain is disabled */
                                             IOPCTL_PIO_PSEDRAIN_DI |
                                             /* Input function is not inverted */
                                             IOPCTL_PIO_INV_DI);
        /* PORT1 PIN11 (coords: L2) is configured as FLEXSPI0B_DATA0 */
        IOPCTL_PinMuxSet(IOPCTL, 1U, 11U, port1_pin11_config);

        const uint32_t port1_pin12_config = (/* Pin is configured as FLEXSPI0B_DATA1 */
                                             IOPCTL_PIO_FUNC6 |
                                             /* Disable pull-up / pull-down function */
                                             IOPCTL_PIO_PUPD_DI |
                                             /* Enable pull-down function */
                                             IOPCTL_PIO_PULLDOWN_EN |
                                             /* Enables input buffer function */
                                             IOPCTL_PIO_INBUF_EN |
                                             /* Normal mode */
                                             IOPCTL_PIO_SLEW_RATE_NORMAL |
                                             /* Full drive enable */
                                             IOPCTL_PIO_FULLDRIVE_EN |
                                             /* Analog mux is disabled */
                                             IOPCTL_PIO_ANAMUX_DI |
                                             /* Pseudo Output Drain is disabled */
                                             IOPCTL_PIO_PSEDRAIN_DI |
                                             /* Input function is not inverted */
                                             IOPCTL_PIO_INV_DI);
        /* PORT1 PIN12 (coords: M2) is configured as FLEXSPI0B_DATA1 */
        IOPCTL_PinMuxSet(IOPCTL, 1U, 12U, port1_pin12_config);

        const uint32_t port1_pin13_config = (/* Pin is configured as FLEXSPI0B_DATA2 */
                                             IOPCTL_PIO_FUNC6 |
                                             /* Disable pull-up / pull-down function */
                                             IOPCTL_PIO_PUPD_DI |
                                             /* Enable pull-down function */
                                             IOPCTL_PIO_PULLDOWN_EN |
                                             /* Enables input buffer function */
                                             IOPCTL_PIO_INBUF_EN |
                                             /* Normal mode */
                                             IOPCTL_PIO_SLEW_RATE_NORMAL |
                                             /* Full drive enable */
                                             IOPCTL_PIO_FULLDRIVE_EN |
                                             /* Analog mux is disabled */
                                             IOPCTL_PIO_ANAMUX_DI |
                                             /* Pseudo Output Drain is disabled */
                                             IOPCTL_PIO_PSEDRAIN_DI |
                                             /* Input function is not inverted */
                                             IOPCTL_PIO_INV_DI);
        /* PORT1 PIN13 (coords: N1) is configured as FLEXSPI0B_DATA2 */
        IOPCTL_PinMuxSet(IOPCTL, 1U, 13U, port1_pin13_config);

        const uint32_t port1_pin14_config = (/* Pin is configured as FLEXSPI0B_DATA3 */
                                             IOPCTL_PIO_FUNC6 |
                                             /* Disable pull-up / pull-down function */
                                             IOPCTL_PIO_PUPD_DI |
                                             /* Enable pull-down function */
                                             IOPCTL_PIO_PULLDOWN_EN |
                                             /* Enables input buffer function */
                                             IOPCTL_PIO_INBUF_EN |
                                             /* Normal mode */
                                             IOPCTL_PIO_SLEW_RATE_NORMAL |
                                             /* Full drive enable */
                                             IOPCTL_PIO_FULLDRIVE_EN |
                                             /* Analog mux is disabled */
                                             IOPCTL_PIO_ANAMUX_DI |
                                             /* Pseudo Output Drain is disabled */
                                             IOPCTL_PIO_PSEDRAIN_DI |
                                             /* Input function is not inverted */
                                             IOPCTL_PIO_INV_DI);
        /* PORT1 PIN14 (coords: N2) is configured as FLEXSPI0B_DATA3 */
        IOPCTL_PinMuxSet(IOPCTL, 1U, 14U, port1_pin14_config);

        const uint32_t port1_pin29_config = (/* Pin is configured as FLEXSPI0B_SCLK */
                                             IOPCTL_PIO_FUNC5 |
                                             /* Disable pull-up / pull-down function */
                                             IOPCTL_PIO_PUPD_DI |
                                             /* Enable pull-down function */
                                             IOPCTL_PIO_PULLDOWN_EN |
                                             /* Enables input buffer function */
                                             IOPCTL_PIO_INBUF_EN |
                                             /* Normal mode */
                                             IOPCTL_PIO_SLEW_RATE_NORMAL |
                                             /* Full drive enable */
                                             IOPCTL_PIO_FULLDRIVE_EN |
                                             /* Analog mux is disabled */
                                             IOPCTL_PIO_ANAMUX_DI |
                                             /* Pseudo Output Drain is disabled */
                                             IOPCTL_PIO_PSEDRAIN_DI |
                                             /* Input function is not inverted */
                                             IOPCTL_PIO_INV_DI);
        /* PORT1 PIN29 (coords: U3) is configured as FLEXSPI0B_SCLK */
        IOPCTL_PinMuxSet(IOPCTL, 1U, 29U, port1_pin29_config);

        const uint32_t port2_pin12_config = (/* Pin is configured as PIO2_12 */
                                             IOPCTL_PIO_FUNC0 |
                                             /* Disable pull-up / pull-down function */
                                             IOPCTL_PIO_PUPD_DI |
                                             /* Enable pull-down function */
                                             IOPCTL_PIO_PULLDOWN_EN |
                                             /* Disable input buffer function */
                                             IOPCTL_PIO_INBUF_DI |
                                             /* Normal mode */
                                             IOPCTL_PIO_SLEW_RATE_NORMAL |
                                             /* Normal drive */
                                             IOPCTL_PIO_FULLDRIVE_DI |
                                             /* Analog mux is disabled */
                                             IOPCTL_PIO_ANAMUX_DI |
                                             /* Pseudo Output Drain is disabled */
                                             IOPCTL_PIO_PSEDRAIN_DI |
                                             /* Input function is not inverted */
                                             IOPCTL_PIO_INV_DI);
        /* PORT2 PIN12 (coords: T3) is configured as PIO2_12 */
        IOPCTL_PinMuxSet(IOPCTL, 2U, 12U, port2_pin12_config);

        const uint32_t port2_pin17_config = (/* Pin is configured as FLEXSPI0B_DATA4 */
                                             IOPCTL_PIO_FUNC6 |
                                             /* Disable pull-up / pull-down function */
                                             IOPCTL_PIO_PUPD_DI |
                                             /* Enable pull-down function */
                                             IOPCTL_PIO_PULLDOWN_EN |
                                             /* Enables input buffer function */
                                             IOPCTL_PIO_INBUF_EN |
                                             /* Normal mode */
                                             IOPCTL_PIO_SLEW_RATE_NORMAL |
                                             /* Full drive enable */
                                             IOPCTL_PIO_FULLDRIVE_EN |
                                             /* Analog mux is disabled */
                                             IOPCTL_PIO_ANAMUX_DI |
                                             /* Pseudo Output Drain is disabled */
                                             IOPCTL_PIO_PSEDRAIN_DI |
                                             /* Input function is not inverted */
                                             IOPCTL_PIO_INV_DI);
        /* PORT2 PIN17 (coords: U1) is configured as FLEXSPI0B_DATA4 */
        IOPCTL_PinMuxSet(IOPCTL, 2U, 17U, port2_pin17_config);

        const uint32_t port2_pin18_config = (/* Pin is configured as FLEXSPI0B_DATA5 */
                                             IOPCTL_PIO_FUNC6 |
                                             /* Disable pull-up / pull-down function */
                                             IOPCTL_PIO_PUPD_DI |
                                             /* Enable pull-down function */
                                             IOPCTL_PIO_PULLDOWN_EN |
                                             /* Enables input buffer function */
                                             IOPCTL_PIO_INBUF_EN |
                                             /* Normal mode */
                                             IOPCTL_PIO_SLEW_RATE_NORMAL |
                                             /* Full drive enable */
                                             IOPCTL_PIO_FULLDRIVE_EN |
                                             /* Analog mux is disabled */
                                             IOPCTL_PIO_ANAMUX_DI |
                                             /* Pseudo Output Drain is disabled */
                                             IOPCTL_PIO_PSEDRAIN_DI |
                                             /* Input function is not inverted */
                                             IOPCTL_PIO_INV_DI);
        /* PORT2 PIN18 (coords: R2) is configured as FLEXSPI0B_DATA5 */
        IOPCTL_PinMuxSet(IOPCTL, 2U, 18U, port2_pin18_config);

        const uint32_t port2_pin19_config = (/* Pin is configured as FLEXSPI0B_SS0_N */
                                             IOPCTL_PIO_FUNC6 |
                                             /* Disable pull-up / pull-down function */
                                             IOPCTL_PIO_PUPD_DI |
                                             /* Enable pull-down function */
                                             IOPCTL_PIO_PULLDOWN_EN |
                                             /* Enables input buffer function */
                                             IOPCTL_PIO_INBUF_EN |
                                             /* Normal mode */
                                             IOPCTL_PIO_SLEW_RATE_NORMAL |
                                             /* Full drive enable */
                                             IOPCTL_PIO_FULLDRIVE_EN |
                                             /* Analog mux is disabled */
                                             IOPCTL_PIO_ANAMUX_DI |
                                             /* Pseudo Output Drain is disabled */
                                             IOPCTL_PIO_PSEDRAIN_DI |
                                             /* Input function is not inverted */
                                             IOPCTL_PIO_INV_DI);
        /* PORT2 PIN19 (coords: T2) is configured as FLEXSPI0B_SS0_N */
        IOPCTL_PinMuxSet(IOPCTL, 2U, 19U, port2_pin19_config);

        const uint32_t port2_pin22_config = (/* Pin is configured as FLEXSPI0B_DATA6 */
                                             IOPCTL_PIO_FUNC6 |
                                             /* Disable pull-up / pull-down function */
                                             IOPCTL_PIO_PUPD_DI |
                                             /* Enable pull-down function */
                                             IOPCTL_PIO_PULLDOWN_EN |
                                             /* Enables input buffer function */
                                             IOPCTL_PIO_INBUF_EN |
                                             /* Normal mode */
                                             IOPCTL_PIO_SLEW_RATE_NORMAL |
                                             /* Full drive enable */
                                             IOPCTL_PIO_FULLDRIVE_EN |
                                             /* Analog mux is disabled */
                                             IOPCTL_PIO_ANAMUX_DI |
                                             /* Pseudo Output Drain is disabled */
                                             IOPCTL_PIO_PSEDRAIN_DI |
                                             /* Input function is not inverted */
                                             IOPCTL_PIO_INV_DI);
        /* PORT2 PIN22 (coords: P3) is configured as FLEXSPI0B_DATA6 */
        IOPCTL_PinMuxSet(IOPCTL, 2U, 22U, port2_pin22_config);

        const uint32_t port2_pin23_config = (/* Pin is configured as FLEXSPI0B_DATA7 */
                                             IOPCTL_PIO_FUNC6 |
                                             /* Disable pull-up / pull-down function */
                                             IOPCTL_PIO_PUPD_DI |
                                             /* Enable pull-down function */
                                             IOPCTL_PIO_PULLDOWN_EN |
                                             /* Enables input buffer function */
                                             IOPCTL_PIO_INBUF_EN |
                                             /* Normal mode */
                                             IOPCTL_PIO_SLEW_RATE_NORMAL |
                                             /* Full drive enable */
                                             IOPCTL_PIO_FULLDRIVE_EN |
                                             /* Analog mux is disabled */
                                             IOPCTL_PIO_ANAMUX_DI |
                                             /* Pseudo Output Drain is disabled */
                                             IOPCTL_PIO_PSEDRAIN_DI |
                                             /* Input function is not inverted */
                                             IOPCTL_PIO_INV_DI);
        /* PORT2 PIN23 (coords: P5) is configured as FLEXSPI0B_DATA7 */
        IOPCTL_PinMuxSet(IOPCTL, 2U, 23U, port2_pin23_config);
    }
    else
    {
    }
}

static void mixspi_clock_init(FLEXSPI_Type *base, mixspi_root_clk_freq_t clkFreq)
{
    if (base == FLEXSPI)
    {
        if (clkFreq == kMixspiRootClkFreq_30MHz)
        {
            /* Move FLEXSPI clock source from main clock to FFRO to avoid instruction/data fetch issue in XIP when
             * updating PLL and main clock.
             */
            BOARD_SetFlexspiClock(3U, 1U);
        }
        else if (clkFreq == kMixspiRootClkFreq_50MHz)
        {
            /* Set FlexSPI clock: source AUX0_PLL£¨396MHz£©, divide by 8 */
            BOARD_SetFlexspiClock(2U, 8U);
        }
        else if (clkFreq == kMixspiRootClkFreq_60MHz)
        {
            /* Set FlexSPI clock: source AUX0_PLL£¨396MHz£©, divide by 7 */
            BOARD_SetFlexspiClock(2U, 7U);
        }
        else if (clkFreq == kMixspiRootClkFreq_80MHz)
        {
            /* Set FlexSPI clock: source AUX0_PLL£¨396MHz£©, divide by 5 */
            BOARD_SetFlexspiClock(2U, 5U);
        }
        else if (clkFreq == kMixspiRootClkFreq_100MHz)
        {
            /* Set FlexSPI clock: source AUX0_PLL£¨396MHz£©, divide by 4 */
            BOARD_SetFlexspiClock(2U, 4U);
        }
        else if (clkFreq == kMixspiRootClkFreq_120MHz)
        {
            /* Set FlexSPI clock: source AUX1_PLL£¨327MHz£©, divide by 3 */
            BOARD_SetFlexspiClock(4U, 3U);
        }
        else if (clkFreq == kMixspiRootClkFreq_133MHz)
        {
            /* Set FlexSPI clock: source AUX0_PLL£¨396MHz£©, divide by 3 */
            BOARD_SetFlexspiClock(2U, 3U);
        }
        else if (clkFreq == kMixspiRootClkFreq_166MHz)
        {
            /* Set FlexSPI clock: source AUX1_PLL£¨327MHz£©, divide by 2 */
            BOARD_SetFlexspiClock(4U, 2U);
        }
        else if (clkFreq == kMixspiRootClkFreq_200MHz)
        {
            /* Set FlexSPI clock: source AUX0_PLL£¨396MHz£©, divide by 2 */
            BOARD_SetFlexspiClock(2U, 2U);
        }
        else if (clkFreq == kMixspiRootClkFreq_332MHz)
        {
            /* Set FlexSPI clock: source AUX1_PLL£¨327MHz£©, divide by 1 */
            BOARD_SetFlexspiClock(4U, 1U);
        }
        else if (clkFreq == kMixspiRootClkFreq_400MHz)
        {
            /* Set FlexSPI clock: source AUX0_PLL£¨396MHz£©, divide by 1 */
            BOARD_SetFlexspiClock(2U, 1U);
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

static uint32_t mixspi_get_clock(FLEXSPI_Type *base)
{
    if (base == FLEXSPI)
    {
        return CLOCK_GetFlexspiClkFreq();
    }
    else
    {
        return 0;
    }
}

static void mixspi_show_clock_source(FLEXSPI_Type *base)
{
#if MFB_DEBUG_LOG_INFO_ENABLE
    uint32_t index = 0;
    uint32_t clkSel;
    uint32_t clkDiv;
    if (base == FLEXSPI)
    {
        index = 0;
        clkSel = CLKCTL0->FLEXSPIFCLKSEL & CLKCTL0_FLEXSPIFCLKSEL_SEL_MASK;
        clkDiv = CLKCTL0->FLEXSPIFCLKDIV & CLKCTL0_FLEXSPIFCLKDIV_DIV_MASK;
        switch (clkSel)
        {
            case CLKCTL0_FLEXSPIFCLKSEL_SEL(0):
                mfb_printf("MFB: FLEXSPI0 Clk Source from 3'b000 - Main Clock %dHz.\r\n", CLOCK_GetMainClkFreq());
                break;

            case CLKCTL0_FLEXSPIFCLKSEL_SEL(1):
               {
                   uint32_t MainPllClkFreq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd0) / ((CLKCTL0->MAINPLLCLKDIV & CLKCTL0_MAINPLLCLKDIV_DIV_MASK) + 1U);
                   mfb_printf("MFB: FLEXSPI0 Clk Source from 3'b001 - Main System PLL Clock %dHz.\r\n", MainPllClkFreq);
                   break;
               }

            case CLKCTL0_FLEXSPIFCLKSEL_SEL(2):
               {
                   uint32_t Aux0PllClkFreq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd2) / ((CLKCTL0->AUX0PLLCLKDIV & CLKCTL0_AUX0PLLCLKDIV_DIV_MASK) + 1U);
                   mfb_printf("MFB: FLEXSPI0 Clk Source from 3'b010 - SYSPLL0 AUX0 PLL Clock %dHz.\r\n", Aux0PllClkFreq);
                   break;
               }

            case CLKCTL0_FLEXSPIFCLKSEL_SEL(3):
                mfb_printf("MFB: FLEXSPI0 Clk Source from 3'b011 - FFRO Clock %dHz.\r\n", CLOCK_GetFFroFreq());
                break;

            case CLKCTL0_FLEXSPIFCLKSEL_SEL(4):
               {
                   uint32_t Aux1PllClkFreq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd3) / ((CLKCTL0->AUX1PLLCLKDIV & CLKCTL0_AUX1PLLCLKDIV_DIV_MASK) + 1U);
                   mfb_printf("MFB: FLEXSPI0 Clk Source from 3'b100 - SYSPLL0 AUX1 PLL Clock %dHz.\r\n", Aux1PllClkFreq);
                   break;
               }

            default:
                break;
        }
    }
    else
    {}
    mfb_printf("MFB: FLEXSPI%d Clk Source Divider: %d.\r\n", index, (clkDiv + 1U));
    mfb_printf("MFB: FLEXSPI%d Clk Frequency: %dHz.\r\n", index, flexspi_get_clock(EXAMPLE_FLEXSPI));
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

#endif /* _PORT_FLEXSPI_INFO_H_ */
