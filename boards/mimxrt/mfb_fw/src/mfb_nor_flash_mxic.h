/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_MXIC_H_
#define _MFB_NOR_FLASH_MXIC_H_

#include "mfb_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MXIC_FLASH_BUSY_STATUS_POL    1
#define MXIC_FLASH_BUSY_STATUS_OFFSET 0

#if MXIC_DEVICE_MX25U25645G
#define MXIC_FLASH_QUAD_ENABLE        0x40
#endif

#if MXIC_DEVICE_MX25UM51345
// 0x00 - SPI (default)
// 0x01 - STR OPI enable
// 0x02 - DTR OPI enable
#define MXIC_OCTAL_FLASH_ENABLE_DDR_CMD   0x02

//------------------------------------------------------
//   DC[2:0]  |  dummy cycles  | OctalI/O SDR(1S-8S-8S)|
//            |                |  Octal DDR(8D-8D-8D)  |
//            |                |        BGA24          |
//------------------------------------------------------
//  3'b000    |  20(default)   |        200MHz         |
//  3'b001    |      18        |        200MHz         |
//  3'b010    |      16        |        173MHz         |
//  3'b011    |      14        |        155MHz         |
//  3'b100    |      12        |        139MHz         |
//  3'b101    |      10        |        121MHz         |
//  3'b110    |      08        |         86MHz         |
//  3'b111    |      06        |         70MHz         |
//------------------------------------------------------
#define MXIC_OCTAL_FLASH_SET_DUMMY_CMD    0x01   // 200MHz OPI DDR
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_MXIC_H_ */
