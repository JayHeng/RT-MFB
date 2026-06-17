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
Latency Code Bits (LC1, LC0) – Volatile/Non-Volatile Writable
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
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_DOSILICON_H_ */
