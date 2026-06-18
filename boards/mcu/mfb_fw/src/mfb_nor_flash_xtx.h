/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_XTX_H_
#define _MFB_NOR_FLASH_XTX_H_

#include "mfb_nor_flash.h"

#if XTX_DEVICE_XT25F64F
#define XTX_FLASH_QUAD_ENABLE        0x02
#define XTX_FLASH_BUSY_STATUS_POL    1
#define XTX_FLASH_BUSY_STATUS_OFFSET 0
#define XTX_QUAD_FLASH_DUMMY_CYCLES  0x06
#endif

/*
XT25F64F JEDEC ID (9Fh): Manufacturer 0x0B (XTX), Device ID 0x4017 (Memory Type 0x40, Capacity 0x17, 64M-bit).
QE (Quad Enable) bit is bit 1 of Status Register-2 (S9), written by opcode 0x31.
opcode 0x05/0x35/0x15 read Status Register-1/2/3.
opcode 0x01/0x31/0x11 write Status Register-1/2/3.

Quad I/O Fast Read (EBh) requires 6 dummy cycles by default for SDR.
The DC (Dummy Configuration) bit in Status Register-3 selects the number of dummy
cycles between the end of address and the start of read data output.

Read performance:
//------------------------------------------------------
//   DC[0]    |  dummy cycles  |Quad IO Fast Read(SPI) |
//------------------------------------------------------
//  1'b0      | 6(def for SDR) |        104MHz         |
//  1'b1      |       10       |        133MHz         |
//------------------------------------------------------
*/


#endif /* _MFB_NOR_FLASH_XTX_H_ */
