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

#if GIGADEVICE_DEVICE_GD25QxxxC
#define GIGADEVICE_FLASH_QUAD_ENABLE        0x02
#define GIGADEVICE_QUAD_FLASH_DUMMY_CYCLES  0x06

// Q64C
//------------------------------------------------------
//    NTR     |  dummy cycles  |Quad IO Fast Read(SPI) | 
//------------------------------------------------------
//    N/A     | 6(def for SPI) |        120MHz         |
//------------------------------------------------------

// Q127C
//------------------------------------------------------
//    NTR     |  dummy cycles  |Quad IO Fast Read(SPI) | 
//------------------------------------------------------
//    N/A     | 6(def for SPI) |        104MHz         |
//------------------------------------------------------

#elif GIGADEVICE_DEVICE_GD25QxxxE 
#define GIGADEVICE_FLASH_QUAD_ENABLE        0x1200
#define GIGADEVICE_QUAD_FLASH_DUMMY_CYCLES  0x0A

//-----------------------------------------------------
//   SR2[4]   |  dummy cycles  |Quad IO Fast Read(SPI) |
//-----------------------------------------------------
//   1'b0     |    6(def)     |        104MHz         |
//   1'b1     |     10        |        133MHz         |
//------------------------------------------------------

#elif GIGADEVICE_DEVICE_GD25BxxxE 
#define GIGADEVICE_FLASH_QUAD_ENABLE        0x02

#define GIGADEVICE_QUAD_FLASH_SET_DUMMY_CMD 0x21
#define GIGADEVICE_QUAD_FLASH_DUMMY_CYCLES  0x0A

//-----------------------------------------------------
//   SR3[0]   |  dummy cycles  |Quad IO Fast Read(SPI) |
//-----------------------------------------------------
//   1'b0     |    6(def)     |        104MHz         |
//   1'b1     |     10        |        133MHz         |
//------------------------------------------------------

#elif GIGADEVICE_DEVICE_GD25LExxxD | GIGADEVICE_DEVICE_GD25LQxxxC
#define GIGADEVICE_FLASH_QUAD_ENABLE        0x0200

#define GIGADEVICE_QUAD_FLASH_DUMMY_CYCLES  0x06

// In Standard SPI mode, the ¡°Set Read Parameters (C0h)¡± instruction is not accepted. The dummy clocks
//  for various Fast Read instructions in Standard/Dual/Quad SPI mode are fixed

// In QPI mode, to accommodate a wide range of applications with different needs for either maximum read
//  frequency or minimum data access latency, ¡°Set Read Parameters (C0h)¡± instruction can be used to
//  configure the number of dummy clocks for ¡°Fast Read (0Bh)¡±, ¡°Fast Read Quad I/O (EBh)¡± & ¡°Burst Read
//  with Wrap (0Ch)¡± instructions

//#define GIGADEVICE_QUAD_FLASH_SET_DUMMY_CMD 0x20
//#define GIGADEVICE_QUAD_FLASH_DUMMY_CYCLES  0x08

//------------------------------------------------------
//   P[5:4]   |  dummy cycles  |Quad IO Fast Read(SPI) |
//------------------------------------------------------
//  2'b00     |      4(def)    |         80MHz         |
//  2'b01     |       6        |        108MHz         |
//  2'b10     |       8        |        120MHz         |
//  2'b11     |       8        |        120MHz         |
//------------------------------------------------------

#elif GIGADEVICE_DEVICE_GD25LBxxxE
#define GIGADEVICE_FLASH_QUAD_ENABLE        0x0000

#define GIGADEVICE_QUAD_FLASH_SET_DUMMY_CMD 0x0A
#define GIGADEVICE_QUAD_FLASH_DUMMY_CYCLES  0x0A

//-------------------------------------------------------
//   DC[7:0]  |  dummy cycles  | Quad IO Fast Read(SPI) |
//            |                |      TFBGA24           |
//-------------------------------------------------------
//   0x00     |  16(default)   |        133MHz          |
//   0x01     |      01        |         N/A            |
//   0x02     |      02        |         N/A            |
//   0x03     |      03        |         20MHz          |
//   0x04     |      04        |         40MHz          |
//   0x05     |      05        |         60MHz          |
//   0x06     |      06        |         84MHz          |
//   0x07     |      07        |         84MHz          |
//   0x08     |      08        |        104MHz          |
//   0x09     |      09        |        104MHz          |
//   0x0a     |      10        |        133MHz          |
//   0x0b     |      11        |        133MHz          |
//   ...      |      ..        |        133MHz          |
//   0x1E     |      30        |        133MHz          |
//   0x1F     |  16(default)   |        133MHz          |
//-------------------------------------------------------

#elif GIGADEVICE_DEVICE_GD25LTxxxE
#define GIGADEVICE_FLASH_QUAD_ENABLE        0x0000

#define GIGADEVICE_QUAD_FLASH_SET_DUMMY_CMD 0x0E
#define GIGADEVICE_QUAD_FLASH_DUMMY_CYCLES  0x0E

//-------------------------------------------------------
//   DC[7:0]  |  dummy cycles  | Quad IO Fast Read(SPI) |
//            |                |      TFBGA24           |
//-------------------------------------------------------
//   0x00     |  16(default)   |        200MHz          |
//   0x01     |      01        |         N/A            |
//   0x02     |      02        |         N/A            |
//   0x03     |      03        |         20MHz          |
//   0x04     |      04        |         40MHz          |
//   0x05     |      05        |         60MHz          |
//   0x06     |      06        |         84MHz          |
//   0x07     |      07        |         84MHz          |
//   0x08     |      08        |        104MHz          |
//   0x09     |      09        |        104MHz          |
//   0x0a     |      10        |        133MHz          |
//   0x0b     |      11        |        133MHz          |
//   0x0c     |      12        |        152MHz          |
//   0x0d     |      13        |        152MHz          |
//   0x0e     |      14        |        166MHz          |
//   0x0f     |      15        |        166MHz          |
//   0x10     |      16        |        166MHz          |
//   0x11     |      17        |        166MHz          |
//   0x12     |      18        |        166MHz          |
//   0x13     |      19        |        166MHz          |
//   0x14     |      20        |        166MHz          |
//   ...      |      ..        |        166MHz          |
//   0x1E     |      30        |        166MHz          |
//   0x1F     |  16(default)   |        200MHz          |
//-------------------------------------------------------

#endif

#if GIGADEVICE_DEVICE_GD25LX | GIGADEVICE_DEVICE_GD25X
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
#define GIGADEVICE_OCTAL_FLASH_SET_DUMMY_CMD     0x00
#define GIGADEVICE_OCTAL_FLASH_DUMMY_CYCLES      0x10   // 200MHz SPI SDR
#else
#define GIGADEVICE_OCTAL_FLASH_SET_DUMMY_CMD     0x10
#define GIGADEVICE_OCTAL_FLASH_DUMMY_CYCLES      0x10   // 200MHz OPI DDR
#endif
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_GIGADEVICE_H_ */
