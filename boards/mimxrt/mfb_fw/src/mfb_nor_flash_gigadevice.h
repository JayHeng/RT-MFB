/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_GIGADEVICE_H_
#define _MFB_NOR_FLASH_GIGADEVICE_H_

#include "mfb_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define GIGADEVICE_FLASH_BUSY_STATUS_POL    1
#define GIGADEVICE_FLASH_BUSY_STATUS_OFFSET 0

#if GIGADEVICE_DEVICE_GD25LE128
#define GIGADEVICE_25LE_FLASH_QUAD_ENABLE        0x0200

//------------------------------------------------------------------------------
//   P[5:4]   |  dummy cycles  |Quad IO Fast Read(SPI) | Quad IO Fast Read(QPI) |
//------------------------------------------------------------------------------
//  2'b00     | 4(def for QPI) |        120MHz         |          80MHz         |
//  2'b01     | 6(def for SPI) |        120MHz         |         108MHz         |
//  2'b10     |       8        |        120MHz         |         120MHz         |
//  2'b11     |       8        |        120MHz         |         120MHz         |
//------------------------------------------------------------------------------
#endif

#if GIGADEVICE_DEVICE_GD25LT256
#define GIGADEVICE_25LT_FLASH_QUAD_ENABLE        0x0000
#endif

#if GIGADEVICE_DEVICE_GD25LX256
// 0xFF - SPI with DQS (Default)
// 0xDF - SPI W/O DQS
// 0xE7 - Octal DTR with DQS
// 0xC7 - Octal DTR W/O DQS
// 0xB7 - Octal with DQS
// 0x97 - Octal W/O DQS
#define GIGADEVICE_OCTAL_FLASH_ENABLE_DDR_CMD     0xE7

//------------------------------------------------------------------------------
//   DC[7:0]  |  dummy cycles  |Octal I/O DDR(1S-8D-8D) |Octal I/O SDR(1S-8S-8S)|
//            |                |  Octal DDR(8S-8D-8D)   |                       |
//            |                |      TFBGA24           |                       |
//-------------------------------------------------------------------------------
//   0x00     |  16(default)   |        200MHz          |         166MHz        |
//   0x01     |      01        |         N/A            |          N/A          |
//   0x02     |      02        |         N/A            |          N/A          |
//   0x03     |      03        |         20MHz          |          20MHz        |
//   0x04     |      04        |         40MHz          |          40MHz        |
//   0x05     |      05        |         60MHz          |          60MHz        |
//   0x06     |      06        |         84MHz          |          84MHz        |
//   0x07     |      07        |         84MHz          |          84MHz        |
//   0x08     |      08        |        104MHz          |         104MHz        |
//   0x09     |      09        |        104MHz          |         104MHz        |
//   0x0a     |      10        |        133MHz          |         133MHz        |
//   0x0b     |      11        |        133MHz          |         133MHz        |
//   0x0c     |      12        |        152MHz          |         152MHz        |
//   0x0d     |      13        |        152MHz          |         152MHz        |
//   0x0e     |      14        |        166MHz          |         166MHz        |
//   0x0f     |      15        |        166MHz          |         166MHz        |
//   0x10     |      16        |        200MHz          |         166MHz        |
//   0x11     |      17        |        200MHz          |         166MHz        |
//   0x12     |      18        |        200MHz          |         166MHz        |
//   0x13     |      19        |        200MHz          |         166MHz        |
//   0x14     |      20        |        200MHz          |         166MHz        |
//   ...      |      ..        |        200MHz          |         166MHz        |
//   0x1E     |      30        |        200MHz          |         166MHz        |
//   0x1F     |  16(default)   |        200MHz          |         166MHz        |
//-------------------------------------------------------------------------------
#if MFB_FLASH_OPI_MODE_DISABLE
#define GIGADEVICE_OCTAL_FLASH_SET_DUMMY_CMD     0x04
#else
#define GIGADEVICE_OCTAL_FLASH_SET_DUMMY_CMD     0x10   // 200MHz OPI DDR
#endif
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_GIGADEVICE_H_ */
