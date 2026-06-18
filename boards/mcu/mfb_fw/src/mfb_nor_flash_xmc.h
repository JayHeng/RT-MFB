/*
 * Copyright 2018-2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_XMC_H_
#define _MFB_NOR_FLASH_XMC_H_

#include "mfb_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if XMC_DEVICE_XM25QH64D
// QE bit is located in Status Register-2[1], written by opcode 0x31 (Write Status Register-2)
#define XMC_FLASH_QUAD_ENABLE        0x02
#define XMC_FLASH_BUSY_STATUS_POL    1
#define XMC_FLASH_BUSY_STATUS_OFFSET 0

// Default dummy cycles for Fast Read Quad I/O (EBh), 166MHz at 6 dummy cycles
#define XMC_QUAD_FLASH_DUMMY_CYCLES  0x06

/*
XM25QH64D: 64Mbit (8MByte), 2.3 - 3.6V, 4KB uniform sector, Quad/QPI.
JEDEC ID: Manufacturer 0x20 (XMC), Device ID 0x4017 (memoryType 0x40, capacity 0x17).

Read performance (STR mode):
//------------------------------------------------------
//   DC[1:0]  |  dummy cycles  |Quad IO Fast Read(SPI) |
//------------------------------------------------------
//  2'b00     | 6(def for SDR) |        166MHz         |
//  2'b01     |       4        |        108MHz         |
//  2'b10     |       8        |        166MHz         |
//  2'b11     |      10        |        166MHz         |
//------------------------------------------------------
*/
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_XMC_H_ */
