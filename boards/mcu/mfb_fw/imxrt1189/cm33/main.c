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

void bsp_print_chipid()
{
    // For A0/B0, Below register is not accurate due to system bug
    uint32_t chipid0 = ANADIG_MISC->MISC_DIFPROG;
    PRINTF("Silicon Version From REG = 0x%x\r\n", chipid0);
    // For C0, There is workaround to store info in Fuse
    uint32_t chipid1 = OCOTP_FSB->OTP_SHADOW_PARTA[21];
    chipid1 = chipid1 >> 9;
    PRINTF("Silicon Version From Fuse = 0x%x\r\n", chipid1);
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

    mfb_printf("\r\nMFB: Target i.MXRT1180.\r\n");
    bsp_print_chipid();
    mfb_printf("\r\n-------------------------------------\r\n");
    mfb_main();
    mfb_printf("-------------------------------------\r\n");

    while (1)
    {
    }
}
