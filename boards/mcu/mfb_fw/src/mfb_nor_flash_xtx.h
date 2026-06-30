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

#define XTX_QUAD_FLASH_SET_DUMMY_CMD 0x41
#if MFB_FLASH_USE_DEFAULT_DUMMY
#define XTX_QUAD_FLASH_DUMMY_CYCLES  0x06
#else
#define XTX_QUAD_FLASH_DUMMY_CYCLES  0x0A
#endif

#elif XTX_DEVICE_XTD25W64A
#define XTX_FLASH_QUAD_ENABLE        0x02
#define XTX_FLASH_BUSY_STATUS_POL    1
#define XTX_FLASH_BUSY_STATUS_OFFSET 0

/*
The Dummy Configuration Bits (DC1, DC0) select the mode and number of Dummy cycles 
between the end of the address and the start of read data output for command BBH, EBH, and EDH.
Dummy cycles provide additional latency that is needed to complete the initial read access 
of the flash array be-fore data can be returned to the host system. Some read commands 
require additional latency cycles as the SCLK fre-quency is increased. The following 
dummy cycle tables provide different dummy cycle settings that are configured.

//------------------------------------------------------
//   DC[0]    |  dummy cycles  |Quad IO Fast Read(0xEB)|
//------------------------------------------------------
//  1'b0      | 6(def for SDR) |        104MHz         |
//  1'b1      |       10       |        133MHz         |
//------------------------------------------------------

//------------------------------------------------------
//   DC[1]    |  dummy cycles  |Quad IO Fast Read(0xED)|
//------------------------------------------------------
//  1'b0      | 8(def for SDR) |        104MHz         |
//  1'b1      |       10       |        133MHz         |
//------------------------------------------------------
*/

#define XTX_QUAD_FLASH_SET_DUMMY_CMD 0x41
#if MFB_FLASH_USE_DEFAULT_DUMMY
#define XTX_QUAD_FLASH_DUMMY_CYCLES  0x06
#else
#define XTX_QUAD_FLASH_DUMMY_CYCLES  0x0A
#endif

#endif

#endif /* _MFB_NOR_FLASH_XTX_H_ */
