/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_MICRON_H_
#define _MFB_NOR_FLASH_MICRON_H_

#include "mfb_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MICRON_FLASH_BUSY_STATUS_POL    1
#define MICRON_FLASH_BUSY_STATUS_OFFSET 0

#if MICRON_DEVICE_MT25QU128
#define MICRON_FLASH_QUAD_ENABLE        0x00

//------------------------------------------------------
//   DC[7:4]  |  dummy cycles  |   Quad IO Fast Read   |
//            |                |Quad IO Fast Read(QPI) |
//------------------------------------------------------
//    0x0     |   10(default)  |        125MHz         |
//    0x1     |       1        |         39MHz         |
//    0x2     |       2        |         48MHz         |
//    0x3     |       3        |         58MHz         |
//    0x4     |       4        |         69MHz         |
//    0x5     |       5        |         78MHz         |
//    0x6     |       6        |         86MHz         |
//    0x7     |       7        |         97MHz         |
//    0x8     |       8        |        106MHz         |
//    0x9     |       9        |        115MHz         |
//    0xa     |      10        |        125MHz         |
//    0xb     |      11        |        134MHz         |
//    0xc     |      12        |        143MHz         |
//    0xd     |      13        |        156MHz         |
//    0xe     |      14        |        166MHz         |
//    0xf     |      10        |        125MHz         |
//------------------------------------------------------

#endif

#if MICRON_DEVICE_MT35XU512_RW303 | MICRON_DEVICE_MT35XU512_RW304
#define MICRON_OCTAL_FLASH_JEDEC_ID     (0x001a5B2C)
// 0xFF - Extended SPI (Default)
// 0xDF - Extended SPI without DQS
// 0xE7 - Octal DDR
// 0xC7 - Octal DDR without DQS
#define MICRON_OCTAL_FLASH_ENABLE_DDR_CMD     0xE7

//------------------------------------------------------------------------------
//   DC[7:0]  |  dummy cycles  |Octal I/O DDR(1S-8D-8D) |Octal I/O SDR(1S-8S-8S)|
//            |                |  Octal DDR(8S-8D-8D)   |                       |
//            |                |        BGA24           |                       |
//-------------------------------------------------------------------------------
//   0x00     |  31(default)   |        200MHz          |         166MHz        |
//   0x01     |      01        |         N/A            |          N/A          |
//   0x02     |      02        |         N/A            |          N/A          |
//   0x03     |      03        |         16MHz          |          16MHz        |
//   0x04     |      04        |         33MHz          |          33MHz        |
//   0x05     |      05        |         50MHz          |          50MHz        |
//   0x06     |      06        |         66MHz          |          66MHz        |
//   0x07     |      07        |         83MHz          |          83MHz        |
//   0x08     |      08        |         95MHz          |          95MHz        |
//   0x09     |      09        |        105MHz          |         105MHz        |
//   0x0a     |      10        |        114MHz          |         114MHz        |
//   0x0b     |      11        |        124MHz          |         124MHz        |
//   0x0c     |      12        |        133MHz          |         133MHz        |
//   0x0d     |      13        |        143MHz          |         143MHz        |
//   0x0e     |      14        |        152MHz          |         152MHz        |
//   0x0f     |      15        |        162MHz          |         162MHz        |
//   0x10     |      16        |        171MHz          |         166MHz        |
//   0x11     |      17        |        181MHz          |         166MHz        |
//   0x12     |      18        |        191MHz          |         166MHz        |
//   0x13     |      19        |        200MHz          |         166MHz        |
//   0x14     |      20        |        200MHz          |         166MHz        |
//   ...      |      ..        |        200MHz          |         166MHz        |
//   0x1E     |      30        |        200MHz          |         166MHz        |
//   0x1F     |      31        |        200MHz          |         166MHz        |
//-------------------------------------------------------------------------------
#if MFB_FLASH_FORCE_LOOPBACK_DQS
#define MICRON_OCTAL_FLASH_SET_DUMMY_CMD     0x06
#else
#define MICRON_OCTAL_FLASH_SET_DUMMY_CMD     0x14   // 200MHz OPI DDR
#endif
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_MICRON_H_ */
