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
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_BOYA_H_ */
