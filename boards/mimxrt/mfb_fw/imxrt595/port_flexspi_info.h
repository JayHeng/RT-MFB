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
                    mfb_printf("MFB: CPU Clk Source from MAINCLKSELA 2'b00 - LPOSC Clock %dHz.\r\n", CLOCK_GetLpOscFreq());
                    break;
                case CLKCTL0_MAINCLKSELA_SEL(1):
                   {
                       uint32_t freq = (CLK_FRO_CLK) / (2U << (((CLKCTL0->FRODIVSEL) & CLKCTL0_FRODIVSEL_SEL_MASK) >> CLKCTL0_FRODIVSEL_SEL_SHIFT));
                       mfb_printf("MFB: CPU Clk Source from MAINCLKSELA 2'b01 - FRODIVSEL Clock %dHz.\r\n", freq);
                       break;
                   }
                case CLKCTL0_MAINCLKSELA_SEL(2):
                    mfb_printf("MFB: CPU Clk Source from MAINCLKSELA 2'b10 - OSC Clock %dHz.\r\n", CLOCK_GetXtalInClkFreq());
                    break;
                case CLKCTL0_MAINCLKSELA_SEL(3):
                    mfb_printf("MFB: CPU Clk Source from MAINCLKSELA 2'b11 - FRO_DIV1 Clock %dHz.\r\n", CLK_FRO_CLK);
                    break;
                default:
                    break;
            }
            break;

        case CLKCTL0_MAINCLKSELB_SEL(1):
            mfb_printf("MFB: CPU Clk Source from MAINCLKSELB 2'b01 - Main System PLL Clock %dHz.\r\n", CLOCK_GetSysPfdFreq(kCLOCK_Pfd0) / ((CLKCTL0->MAINPLLCLKDIV & CLKCTL0_MAINPLLCLKDIV_DIV_MASK) + 1U));
            break;

        case CLKCTL0_MAINCLKSELB_SEL(2):
            mfb_printf("MFB: CPU Clk Source from MAINCLKSELB 2'b10 - RTC 32KHz Clock %dHz.\r\n", CLOCK_GetOsc32KFreq());
            break;

        default:
            mfb_printf("MFB: CPU Clk Source from MAINCLKSELB 2'b11 - Reserved.\r\n");
            break;
    }
    
    clkDiv = (CLKCTL0->SYSCPUAHBCLKDIV & CLKCTL0_SYSCPUAHBCLKDIV_DIV_MASK) >> CLKCTL0_SYSCPUAHBCLKDIV_DIV_SHIFT;
    mfb_printf("MFB: CPU Clk Source Divider: %d.\r\n", (clkDiv + 1U));
    mfb_printf("MFB: CPU Clk Frequency: %dHz.\r\n", CLOCK_GetFreq(kCLOCK_CoreSysClk));
#endif
}

static void flexspi_port_switch(FLEXSPI_Type *base, flexspi_port_t port, flexspi_pad_t pads)
{
}

static void flexspi_pin_init(FLEXSPI_Type *base, flexspi_port_t port, flexspi_pad_t pads)
{
    if (base == FLEXSPI0)
    {
        const uint32_t port1_pin18_config = (/* Pin is configured as FLEXSPI0_SCLK */
                                             IOPCTL_PIO_FUNC1 |
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
        /* PORT1 PIN18 (coords: T17) is configured as FLEXSPI0_SCLK */
        IOPCTL_PinMuxSet(IOPCTL, 1U, 18U, port1_pin18_config);

        const uint32_t port1_pin19_config = (/* Pin is configured as FLEXSPI0_SS0_N */
                                             IOPCTL_PIO_FUNC1 |
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
        /* PORT1 PIN19 (coords: U16) is configured as FLEXSPI0_SS0_N */
        IOPCTL_PinMuxSet(IOPCTL, 1U, 19U, port1_pin19_config);

        const uint32_t port1_pin20_config = (/* Pin is configured as FLEXSPI0_DATA0 */
                                             IOPCTL_PIO_FUNC1 |
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
        /* PORT1 PIN20 (coords: T15) is configured as FLEXSPI0_DATA0 */
        IOPCTL_PinMuxSet(IOPCTL, 1U, 20U, port1_pin20_config);

        const uint32_t port1_pin21_config = (/* Pin is configured as FLEXSPI0_DATA1 */
                                             IOPCTL_PIO_FUNC1 |
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
        /* PORT1 PIN21 (coords: T14) is configured as FLEXSPI0_DATA1 */
        IOPCTL_PinMuxSet(IOPCTL, 1U, 21U, port1_pin21_config);

        const uint32_t port1_pin22_config = (/* Pin is configured as FLEXSPI0_DATA2 */
                                             IOPCTL_PIO_FUNC1 |
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
        /* PORT1 PIN22 (coords: R13) is configured as FLEXSPI0_DATA2 */
        IOPCTL_PinMuxSet(IOPCTL, 1U, 22U, port1_pin22_config);

        const uint32_t port1_pin23_config = (/* Pin is configured as FLEXSPI0_DATA3 */
                                             IOPCTL_PIO_FUNC1 |
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
        /* PORT1 PIN23 (coords: R12) is configured as FLEXSPI0_DATA3 */
        IOPCTL_PinMuxSet(IOPCTL, 1U, 23U, port1_pin23_config);

        const uint32_t port1_pin24_config = (/* Pin is configured as FLEXSPI0_DATA4 */
                                             IOPCTL_PIO_FUNC1 |
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
        /* PORT1 PIN24 (coords: N12) is configured as FLEXSPI0_DATA4 */
        IOPCTL_PinMuxSet(IOPCTL, 1U, 24U, port1_pin24_config);

        const uint32_t port1_pin25_config = (/* Pin is configured as FLEXSPI0_DATA5 */
                                             IOPCTL_PIO_FUNC1 |
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
        /* PORT1 PIN25 (coords: R14) is configured as FLEXSPI0_DATA5 */
        IOPCTL_PinMuxSet(IOPCTL, 1U, 25U, port1_pin25_config);

        const uint32_t port1_pin26_config = (/* Pin is configured as FLEXSPI0_DATA6 */
                                             IOPCTL_PIO_FUNC1 |
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
        /* PORT1 PIN26 (coords: P14) is configured as FLEXSPI0_DATA6 */
        IOPCTL_PinMuxSet(IOPCTL, 1U, 26U, port1_pin26_config);

        const uint32_t port1_pin27_config = (/* Pin is configured as FLEXSPI0_DATA7 */
                                             IOPCTL_PIO_FUNC1 |
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
        /* PORT1 PIN27 (coords: P13) is configured as FLEXSPI0_DATA7 */
        IOPCTL_PinMuxSet(IOPCTL, 1U, 27U, port1_pin27_config);

        const uint32_t port1_pin28_config = (/* Pin is configured as FLEXSPI0_DQS */
                                             IOPCTL_PIO_FUNC1 |
                                             /* Enable pull-up / pull-down function */
                                             IOPCTL_PIO_PUPD_EN |
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
        /* PORT1 PIN28 (coords: U14) is configured as FLEXSPI0_DQS */
        IOPCTL_PinMuxSet(IOPCTL, 1U, 28U, port1_pin28_config);
    }
    else if (base == FLEXSPI1)
    {
        
    }
    else
    {
    }
}

static void flexspi_clock_init(FLEXSPI_Type *base, flexspi_root_clk_freq_t clkFreq)
{
    if (base == FLEXSPI0)
    {
        if (clkFreq == kFlexspiRootClkFreq_30MHz)
        {
            /* Move FLEXSPI clock source from main clock to FRO192M / 7 to avoid instruction/data fetch issue in XIP when
             * updating PLL and main clock.
             */
            BOARD_SetFlexspiClock(EXAMPLE_FLEXSPI, 3U, 7U);
        }
        else if (clkFreq == kFlexspiRootClkFreq_80MHz)
        {
            /* Set FlexSPI clock: source AUX0_PLL£¨396MHz£©, divide by 5 */
            BOARD_SetFlexspiClock(EXAMPLE_FLEXSPI, 2U, 5U);
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
        else if (clkFreq == kFlexspiRootClkFreq_166MHz)
        {
            /* Set FlexSPI clock: source AUX1_PLL£¨327MHz£©, divide by 2 */
            BOARD_SetFlexspiClock(EXAMPLE_FLEXSPI, 4U, 2U);
        }
        else if (clkFreq == kFlexspiRootClkFreq_200MHz)
        {
            /* Set FlexSPI clock: source AUX0_PLL, divide by 2 */
            BOARD_SetFlexspiClock(EXAMPLE_FLEXSPI, 2U, 2U);
        }
        else if (clkFreq == kFlexspiRootClkFreq_332MHz)
        {
            /* Set FlexSPI clock: source AUX1_PLL£¨327MHz£©, divide by 1 */
            BOARD_SetFlexspiClock(EXAMPLE_FLEXSPI, 4U, 1U);
        }
        else if (clkFreq == kFlexspiRootClkFreq_400MHz)
        {
            /* Set FlexSPI clock: source AUX0_PLL£¨396MHz£©, divide by 1 */
            BOARD_SetFlexspiClock(EXAMPLE_FLEXSPI, 2U, 1U);
        }
    }
    else if (base == FLEXSPI1)
    {
    }
    else
    {
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

static void flexspi_show_clock_source(FLEXSPI_Type *base)
{
#if MFB_DEBUG_LOG_INFO_ENABLE
    uint32_t index = 0;
    uint32_t clkSel;
    uint32_t clkDiv;
    if (base == FLEXSPI0)
    {
        index = 0;
        clkSel = CLKCTL0->FLEXSPI0FCLKSEL & CLKCTL0_FLEXSPI0FCLKSEL_SEL_MASK;
        clkDiv = CLKCTL0->FLEXSPI0FCLKDIV & CLKCTL0_FLEXSPI0FCLKDIV_DIV_MASK;
        switch (clkSel)
        {
            case CLKCTL0_FLEXSPI0FCLKSEL_SEL(0):
                mfb_printf("MFB: FLEXSPI0 Clk Source from 3'b000 - Main Clock %dHz.\r\n", CLOCK_GetMainClkFreq());
                break;

            case CLKCTL0_FLEXSPI0FCLKSEL_SEL(1):
               {
                   uint32_t MainPllClkFreq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd0) / ((CLKCTL0->MAINPLLCLKDIV & CLKCTL0_MAINPLLCLKDIV_DIV_MASK) + 1U);
                   mfb_printf("MFB: FLEXSPI0 Clk Source from 3'b001 - Main System PLL Clock %dHz.\r\n", MainPllClkFreq);
                   break;
               }

            case CLKCTL0_FLEXSPI0FCLKSEL_SEL(2):
               {
                   uint32_t Aux0PllClkFreq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd2) / ((CLKCTL0->AUX0PLLCLKDIV & CLKCTL0_AUX0PLLCLKDIV_DIV_MASK) + 1U);
                   mfb_printf("MFB: FLEXSPI0 Clk Source from 3'b010 - SYSPLL0 AUX0 PLL Clock %dHz.\r\n", Aux0PllClkFreq);
                   break;
               }

            case CLKCTL0_FLEXSPI0FCLKSEL_SEL(3):
                mfb_printf("MFB: FLEXSPI0 Clk Source from 3'b011 - FRO_DIV1 Clock %dHz.\r\n", CLK_FRO_CLK);
                break;

            case CLKCTL0_FLEXSPI0FCLKSEL_SEL(4):
               {
                   uint32_t Aux1PllClkFreq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd3) / ((CLKCTL0->AUX1PLLCLKDIV & CLKCTL0_AUX1PLLCLKDIV_DIV_MASK) + 1U);
                   mfb_printf("MFB: FLEXSPI0 Clk Source from 3'b100 - SYSPLL0 AUX1 PLL Clock %dHz.\r\n", Aux1PllClkFreq);
                   break;
               }

            default:
                break;
        }
    }
    else if (base == FLEXSPI1)
    {
        index = 1;
        clkSel = CLKCTL0->FLEXSPI1FCLKSEL & CLKCTL0_FLEXSPI1FCLKSEL_SEL_MASK;
        clkDiv = CLKCTL0->FLEXSPI1FCLKDIV & CLKCTL0_FLEXSPI1FCLKDIV_DIV_MASK;
        switch (clkSel)
        {
            case CLKCTL0_FLEXSPI1FCLKSEL_SEL(0):
                mfb_printf("MFB: FLEXSPI1 Clk Source from 3'b000 - Main Clock %dHz.\r\n", CLOCK_GetMainClkFreq());
                break;

            case CLKCTL0_FLEXSPI1FCLKSEL_SEL(1):
               {
                   uint32_t MainPllClkFreq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd0) / ((CLKCTL0->MAINPLLCLKDIV & CLKCTL0_MAINPLLCLKDIV_DIV_MASK) + 1U);
                   mfb_printf("MFB: FLEXSPI1 Clk Source from 3'b001 - Main System PLL Clock %dHz.\r\n", MainPllClkFreq);
                   break;
               }

            case CLKCTL0_FLEXSPI1FCLKSEL_SEL(2):
               {
                   uint32_t Aux0PllClkFreq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd2) / ((CLKCTL0->AUX0PLLCLKDIV & CLKCTL0_AUX0PLLCLKDIV_DIV_MASK) + 1U);
                   mfb_printf("MFB: FLEXSPI1 Clk Source from 3'b010 - SYSPLL0 AUX0 PLL Clock %dHz.\r\n", Aux0PllClkFreq);
                   break;
               }

            case CLKCTL0_FLEXSPI1FCLKSEL_SEL(3):
                mfb_printf("MFB: FLEXSPI1 Clk Source from 3'b011 - FRO_DIV1 Clock %dHz.\r\n", CLK_FRO_CLK);
                break;

            case CLKCTL0_FLEXSPI1FCLKSEL_SEL(4):
               {
                   uint32_t Aux1PllClkFreq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd3) / ((CLKCTL0->AUX1PLLCLKDIV & CLKCTL0_AUX1PLLCLKDIV_DIV_MASK) + 1U);
                   mfb_printf("MFB: FLEXSPI1 Clk Source from 3'b100 - SYSPLL0 AUX1 PLL Clock %dHz.\r\n", Aux1PllClkFreq);
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

#endif /* _PORT_FLEXSPI_INFO_H_ */
