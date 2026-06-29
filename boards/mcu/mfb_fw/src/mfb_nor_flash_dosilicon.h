/*
 * Copyright 2018-2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_DOSILICON_H_
#define _MFB_NOR_FLASH_DOSILICON_H_

#include "mfb_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if DOSILICON_DEVICE_DS25M4AB
#define DOSILICON_FLASH_QUAD_ENABLE        0x02
#define DOSILICON_FLASH_BUSY_STATUS_POL    1
#define DOSILICON_FLASH_BUSY_STATUS_OFFSET 0

#define DOSILICON_QUAD_FLASH_DUMMY_CYCLES  0x06

/*
DS25M4AB JEDEC ID (9Fh): Manufacturer 0xE5, Device ID 0x4218 (Memory Type 0x42, Capacity 0x18, 128M-bit).
QE (Quad Enable) bit is bit 1 of Status Register-2 (S9), written by opcode 0x31.
opcode 0x05/0x35/0x15 read Status Register-1/2/3.
opcode 0x01/0x31/0x11 write Status Register-1/2/3.

Quad I/O Fast Read (EBh) requires 6 dummy cycles by default for SDR.

Latency Code Bits (LC1, LC0) - Volatile/Non-Volatile Writable
The Latency Code (LC) selects the mode and number of dummy cycles between the end of address and the start of read data output for DTR read command(EDh).
*/


//------------------------------------------------------------------------------
//   P[4:3]   |  dummy cycles  |Quad IO Fast Read(SPI) | Quad IO Fast Read(DTR) |
//------------------------------------------------------------------------------
//  2'b00     | 6(def for SDR) |        104MHz         |          66MHz         |
//  2'b01     |       4        |                       |          40MHz         |
//  2'b10     |       8        |                       |          66MHz         |
//  2'b11     |      10        |                       |          66MHz         |
//------------------------------------------------------------------------------

#elif DOSILICON_DEVICE_DS25Q64A
#define DOSILICON_FLASH_QUAD_ENABLE        0x02
#define DOSILICON_FLASH_BUSY_STATUS_POL    1
#define DOSILICON_FLASH_BUSY_STATUS_OFFSET 0

/*
The Fast Read Quad I/O instruction is also supported in QPI mode. When QPI mode is enabled, 
the number of dummy clocks is configured by the “Set Read Parameters (C0h)” instruction to 
accommodate a wide range of applications with different needs for either maximum Fast 
Read frequency or minimum data access latency. Depending on the Read Parameter Bits P[5:4] setting, 
the number of dummy clocks can be configured as either 6 or 8. The default number of dummy clocks 
upon power up or after a Reset instruction is 8. In QPI mode, the “Continuous Read Mode” bits M7-0 
are also considered as dummy clocks. In the default setting, the data output will follow the 
Continuous Read Mode bits immediately.
*/
#define DOSILICON_QUAD_FLASH_DUMMY_CYCLES  0x08

//------------------------------------------------------------------------------
//   P[5:4]   |  dummy cycles  |Quad IO Fast Read(SPI) | Quad IO Fast Read(QPI) |
//------------------------------------------------------------------------------
//  2'b00     |       6        |        133MHz         |         100MHz         |
//  2'b01     |       6        |        133MHz         |         100MHz         |
//  2'b10     |     8(def)     |        133MHz         |         133MHz         |
//  2'b11     |       8        |        133MHz         |         133MHz         |
//------------------------------------------------------------------------------

#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_DOSILICON_H_ */
