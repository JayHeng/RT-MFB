/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_WINBOND_H_
#define _MFB_NOR_FLASH_WINBOND_H_

#include "mfb_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define WINBOND_FLASH_BUSY_STATUS_POL    1
#define WINBOND_FLASH_BUSY_STATUS_OFFSET 0

#define WINBOND_FLASH_DRVIE_STRENGTH_100P  0x00
#define WINBOND_FLASH_DRVIE_STRENGTH_075P  0x20
#define WINBOND_FLASH_DRVIE_STRENGTH_050P  0x40
#define WINBOND_FLASH_DRVIE_STRENGTH_025P  0x60

#if WINBOND_DEVICE_W25QxxxCL
#endif
   
////////////////////////////////////////////////////////////////////////////////

#if WINBOND_DEVICE_W25QxxxJW | \
    WINBOND_DEVICE_W25QxxxJV | \
    WINBOND_DEVICE_W25QxxxFW | \
    WINBOND_DEVICE_W25QxxxNE | \
    WINBOND_DEVICE_W25QxxxEW | \
    WINBOND_DEVICE_W25QxxxDV | \
    WINBOND_DEVICE_W25MxxxJW | \
    WINBOND_DEVICE_W25HxxxJV
#define WINBOND_FLASH_QUAD_ENABLE        0x02

// In Standard SPI mode, the ¡°Set Read Parameters (C0h)¡± instruction is not accepted. The dummy clocks
//  for various Fast Read instructions in Standard/Dual/Quad SPI mode are fixed

// In QPI mode, to accommodate a wide range of applications with different needs for either maximum read
//  frequency or minimum data access latency, ¡°Set Read Parameters (C0h)¡± instruction can be used to
//  configure the number of dummy clocks for ¡°Fast Read (0Bh)¡±, ¡°Fast Read Quad I/O (EBh)¡± & ¡°Burst Read
//  with Wrap (0Ch)¡± instructions
#define WINBOND_QUAD_FLASH_DUMMY_CYCLES  0x06

//------------------------------------------------------------------------------
//   P[5:4]   |  dummy cycles  |Quad IO Fast Read(SPI) | Quad IO Fast Read(QPI) |
//------------------------------------------------------------------------------
//  2'b00     | 2(def for QPI) |        104MHz         |          26MHz         |
//  2'b01     |       4        |        104MHz         |          80MHz         |
//  2'b10     | 6(def for SPI) |        104MHz         |         104MHz         |
//  2'b11     |       8        |        104MHz         |         104MHz         |
//------------------------------------------------------------------------------

#elif WINBOND_DEVICE_W25QxxxNW
#define WINBOND_FLASH_QUAD_ENABLE        0x02

// ¡°Set Read Parameters (C0h)¡± instruction is used to accommodate a wide range of applications with different needs
//    for either maximum read frequency or minimum data access latency. This is accomplished by setting the number 
//    of dummy clocks and wrap length configurations for set of selected instructions. Set Read Parameters (C0h) 
//    instruction writes to the Read Parameter Register (P[7:0]).

// In SPI mode, SPI Set Read Parameters (C0h)" instruction writes to ¡®Dummy Clocks¡¯ P[6:4] bits only, while it 
//    will ignore ¡®Wrap Length¡¯ P[1:0] bits input as they are don¡¯t care in SPI mode.
#define WINBOND_QUAD_FLASH_SET_DUMMY_CMD 0x30
#if MFB_FLASH_USE_DEFAULT_DUMMY
#define WINBOND_QUAD_FLASH_DUMMY_CYCLES  0x06
#else
#define WINBOND_QUAD_FLASH_DUMMY_CYCLES  0x08
#endif

//--------------------------------------------------
//   P[6:4]   |  dummy cycles  | MAXIMUM READ FREQ |
//                               Required address alignment and start address for Reads: LSB A[1:0]=00b
//--------------------------------------------------
//  3'b000    | 6(def for SPI) |        104MHz     |
//  3'b001    |       6        |        104MHz     |
//  3'b010    |       6        |        104MHz     |
//  3'b011    |       8        |        133MHz     | 
//  3'b100    |       10       |        133MHz     | 
//  3'b101    |       12       |        133MHz     |
//  3'b110    |       14       |        133MHz     |
//  3'b111    |       16       |        133MHz     |
//--------------------------------------------------
#endif

////////////////////////////////////////////////////////////////////////////////
#define WINBOND_X8_FLASH_BUS_PROTOCOL (kFlashProtocolType_XCCELA)

#if WINBOND_DEVICE_W35T51NW
// 0xFF - Extended SPI (Default)
// 0xDF - Extended SPI without DQS
// 0xE7 - Octal DDR
// 0xC7 - Octal DDR without DQS
#define WINBOND_OCTAL_FLASH_ENABLE_DDR_CMD     0xE7

// Random Start Address
//------------------------------------------------------------------------------
//   DC[7:0]  |  dummy cycles  |Octal I/O DDR(1S-8D-8D) |Octal I/O SDR(1S-8S-8S)|
//            |                |  Octal DDR(8S-8D-8D)   |                       |
//            |                |        BGA24           |                       |
//-------------------------------------------------------------------------------
//   0x00     |  16(default)   |        143MHz          |         143MHz        |
//   0x08     |      08        |         66MHz          |          66MHz        |
//   0x10     |      16        |        143MHz          |         143MHz        |
//   0x16     |      22        |        200MHz          |         166MHz        |
//   ...      |      ..        |        200MHz          |         166MHz        |
//   0x1E     |      30        |        200MHz          |         166MHz        |
//   0x1F     |  16(default)   |        143MHz          |         143MHz        |
//-------------------------------------------------------------------------------
// 4-Byte Aligned Start Address (A[1:0]=00b)
//------------------------------------------------------------------------------
//   DC[7:0]  |  dummy cycles  |Octal I/O DDR(1S-8D-8D) |Octal I/O SDR(1S-8S-8S)|
//            |                |  Octal DDR(8S-8D-8D)   |                       |
//            |                |        BGA24           |                       |
//-------------------------------------------------------------------------------
//   0x00     |  16(default)   |        166MHz          |         166MHz        |
//   0x08     |      08        |         50MHz          |          50MHz        |
//   0x10     |      16        |        166MHz          |         166MHz        |
//   0x16     |      22        |        200MHz          |         166MHz        |
//   ...      |      ..        |        200MHz          |         166MHz        |
//   0x1E     |      30        |        200MHz          |         166MHz        |
//   0x1F     |  16(default)   |        166MHz          |         166MHz        |
//-------------------------------------------------------------------------------
// 32-Byte Aligned Start Address (A[4:0]=00000b)
//------------------------------------------------------------------------------
//   DC[7:0]  |  dummy cycles  |Octal I/O DDR(1S-8D-8D) |Octal I/O SDR(1S-8S-8S)|
//            |                |  Octal DDR(8S-8D-8D)   |                       |
//            |                |        BGA24           |                       |
//-------------------------------------------------------------------------------
//   0x00     |  16(default)   |        200MHz          |         200MHz        |
//   0x08     |      08        |        120MHz          |         120MHz        |
//   0x10     |      16        |        200MHz          |         200MHz        |
//   ...      |      ..        |        200MHz          |         200MHz        |
//   0x1E     |      30        |        200MHz          |         200MHz        |
//   0x1F     |  16(default)   |        200MHz          |         200MHz        |
//-------------------------------------------------------------------------------
#if MFB_FLASH_OPI_MODE_DISABLE
#define WINBOND_OCTAL_FLASH_SET_DUMMY_CMD     0x00
#define WINBOND_OCTAL_FLASH_DUMMY_CYCLES      0x10   // 143MHz SPI SDR
#else
#define WINBOND_OCTAL_FLASH_SET_DUMMY_CMD     0x16
#define WINBOND_OCTAL_FLASH_DUMMY_CYCLES      0x16   // 200MHz OPI DDR
#endif
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_WINBOND_H_ */
