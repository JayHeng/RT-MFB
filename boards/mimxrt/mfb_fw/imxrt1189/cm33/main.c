/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_iomuxc.h"
#include "fsl_rgpio.h"
#include "fsl_flexspi.h"
#include "mfb.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static void board_flexspi_port_switch(FLEXSPI_Type *base, flexspi_port_t port)
{
    rgpio_pin_config_t do_config = {kRGPIO_DigitalOutput, 0};
    if (base == FLEXSPI1)
    {
        IOMUXC_SetPinMux(IOMUXC_GPIO_AD_23_GPIO4_IO23, 0U);
        RGPIO_PinInit(RGPIO4, 23, &do_config);
        switch (port)
        {
            case kFLEXSPI_PortA1:
            case kFLEXSPI_PortA2:
                RGPIO_PinWrite(RGPIO4, 23, 0);
                break;
            case kFLEXSPI_PortB1:
            case kFLEXSPI_PortB2:
                RGPIO_PinWrite(RGPIO4, 23, 1);
                break;
            default:
                break;
        }
    }
    else if (base == FLEXSPI2)
    {
        
    }
    else
    {
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    board_flexspi_port_switch(FLEXSPI1, kFLEXSPI_PortA1);

    mfb_printf("\r\nMFB: Target i.MXRT1180.\r\n");
    mfb_main();

    while (1)
    {
    }
}
