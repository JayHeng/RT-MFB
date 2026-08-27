/*
 * Copyright 2018-2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_BOYA_H_
#define _MFB_NOR_FLASH_BOYA_H_

#include "mfb_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if BOYA_DEVICE_BY25FQ64ES
// QE bit is located in Status Register-2[1], written by opcode 0x31 (Write Status Register-2)
#define BOYA_FLASH_QUAD_ENABLE        0x02
#define BOYA_FLASH_BUSY_STATUS_POL    1
#define BOYA_FLASH_BUSY_STATUS_OFFSET 0

/*
BYT Semiconductor (Boya) BY25FQ64ES: 64Mbit (8MByte), 2.7 - 3.6V, 4KB uniform sector, Quad/QPI.
JEDEC ID (9Fh): Manufacturer 0x68 (Boya), Device ID 0x4017 (memoryType 0x40, capacity 0x17).

Read performance:
//------------------------------------------------------
//   DC[4]    |  dummy cycles  |Quad IO Fast Read(SPI) |
//------------------------------------------------------
//  1'b0      | 6(def for SDR) |        104MHz         |
//  1'b1      |       10       |     104/133R MHz      |
//------------------------------------------------------
*/

#define BOYA_QUAD_FLASH_SET_DUMMY_CMD 0x30
#if MFB_FLASH_USE_DEFAULT_DUMMY
#define BOYA_QUAD_FLASH_DUMMY_CYCLES  0x06
#else
#define BOYA_QUAD_FLASH_DUMMY_CYCLES  0x0A
#endif

#endif

////////////////////////////////////////////////////////////////////////////////
#define BOYA_FLASH_BUSY_STATUS_POL    1
#define BOYA_FLASH_BUSY_STATUS_OFFSET 0

#define BOYA_FLASH_DRVIE_STRENGTH_50Ohm  0xFF
#define BOYA_FLASH_DRVIE_STRENGTH_35Ohm  0xFE
#define BOYA_FLASH_DRVIE_STRENGTH_25Ohm  0xFD
#define BOYA_FLASH_DRVIE_STRENGTH_18Ohm  0xFC

#if BOYA_DEVICE_BY25X512EL
/*
BYT Semiconductor (Boya) BY25X512EL: 512Mbit (64MByte) OctalSPI, 1.65 - 2.0V, 4KB uniform sector.
JEDEC ID (9Fh): Manufacturer 0x68 (Boya), memoryType 0x1A, capacityID 0xFF.
Micron/Winbond-compatible register model:
  - I/O mode is selected by Nonvolatile/Volatile Configuration Register Byte<0>.
  - Dummy cycles are set by Configuration Register Byte<1>.
  - Volatile Configuration Register: write 0x81, read 0x85.
  - Flag Status Register: read 0x70.

Configuration Register Byte<0> - I/O mode:
  0xFF - SPI with DQS (Default)
  0xDF - SPI without DQS
  0xE7 - Octal DTR with DQS
  0xC7 - Octal DTR without DQS
  0xB7 - Octal STR with DQS
  0x97 - Octal STR without DQS
*/
#define BOYA_OCTAL_FLASH_ENABLE_DDR_CMD     0xE7

// Configuration Register Byte<1> - Dummy cycle configuration
//   0x04 - 4 dummy, 0x06 - 6 dummy, 0x08 - 8 dummy, 0x0A~0x1E - 10~30 dummy (Default 0x10 = 16)
// Only even number of dummy can be configured.
//------------------------------------------------------------------------------
//   DC[7:0]  |  dummy cycles  |Octal I/O DTR(with DQS)|Octal I/O STR(with DQS)|
//------------------------------------------------------------------------------
//   0x08     |      08        |         84MHz         |          84MHz        |
//   0x0A     |      10        |        104MHz         |         104MHz        |
//   0x0C     |      12        |        133MHz         |         133MHz        |
//   0x0E     |      14        |        150MHz         |         150MHz        |
//   0x10     |  16(default)   |        166MHz         |         166MHz        |
// (T= -40?~125?, VCC=1.65~2.0V)
//  >0x10     |      XX        |        166MHz         |         166MHz        |
// (T= -40?~85?, VCC=1.65~2.0V)
// (T= -40?~105?, VCC=1.65~2.0V)
//   0x12     |      18        |        188MHz         |         188MHz        |
//   0x14     |      20        |        200MHz         |         200MHz        |
//   0x16     |      22        |        200MHz         |         200MHz        |
//------------------------------------------------------------------------------
#if MFB_FLASH_OPI_MODE_DISABLE
#define BOYA_OCTAL_FLASH_SET_DUMMY_CMD     0x10
#define BOYA_OCTAL_FLASH_DUMMY_CYCLES      0x10   // 166MHz SPI SDR
#else
#define BOYA_OCTAL_FLASH_SET_DUMMY_CMD     0x14
#define BOYA_OCTAL_FLASH_DUMMY_CYCLES      0x14   // 200MHz OPI DTR
#endif
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_BOYA_H_ */
