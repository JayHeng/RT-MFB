/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_ISSI_H_
#define _MFB_NOR_FLASH_ISSI_H_

#include "mfb_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define ISSI_FLASH_BUSY_STATUS_POL    1
#define ISSI_FLASH_BUSY_STATUS_OFFSET 0

#if ISSI_DEVICE_IS25WP064A
#define ISSI_FLASH_QUAD_ENABLE        0x40
#endif

#if ISSI_DEVICE_IS25WX256
// 0xFF - Extended SPI (Default)
// 0xDF - Extended SPI without DQS
// 0xE7 - Octal DDR
// 0xC7 - Octal DDR without DQS
#define ISSI_OCTAL_FLASH_ENABLE_DDR_CMD     0xE7

//----------------------------------------------------
//   DC[7:0]  |  dummy cycles  |   Octal I/O DDR     |
//            |                |        BGA24        |
//----------------------------------------------------
//   0x00     |  31(default)   |        200MHz       |
//   0x01     |      01        |         N/A         |
//   0x02     |      02        |         N/A         |
//   0x03     |      03        |         16MHz       |
//   0x04     |      04        |         33MHz       |
//   0x05     |      05        |         50MHz       |
//   0x06     |      06        |         66MHz       |
//   0x07     |      07        |         83MHz       |
//   0x08     |      08        |         95MHz       |
//   0x09     |      09        |        105MHz       |
//   0x0a     |      10        |        114MHz       |
//   0x0b     |      11        |        124MHz       |
//   0x0c     |      12        |        133MHz       |
//   0x0d     |      13        |        143MHz       |
//   0x0e     |      14        |        152MHz       |
//   0x0f     |      15        |        162MHz       |
//   0x10     |      16        |        171MHz       |
//   0x11     |      17        |        181MHz       |
//   0x12     |      18        |        191MHz       |
//   0x13     |      19        |        200MHz       |
//   0x14     |      20        |        200MHz       |
//   ...      |      ..        |        200MHz       |
//   0x1E     |      30        |        200MHz       |
//   0x1F     |      31        |        200MHz       |
//----------------------------------------------------
#define ISSI_OCTAL_FLASH_SET_DUMMY_CMD     0x14   // 200MHz OPI DDR
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_ISSI_H_ */
