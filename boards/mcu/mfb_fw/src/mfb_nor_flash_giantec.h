/*
 * Copyright 2018-2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_GIANTEC_H_
#define _MFB_NOR_FLASH_GIANTEC_H_

#include "mfb_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if GIANTEC_DEVICE_GT25Q64A
#define GIANTEC_FLASH_QUAD_ENABLE        0x02
#define GIANTEC_FLASH_BUSY_STATUS_POL    1
#define GIANTEC_FLASH_BUSY_STATUS_OFFSET 0
#define GIANTEC_QUAD_FLASH_DUMMY_CYCLES  0x06
#endif

/*
GT25Q64A-S JEDEC ID (9Fh): Manufacturer 0xC4, Device ID 0x6017.
QE (Quad Enable) bit is bit 1 of Status Register-2, written by opcode 0x31.
opcode 0x05/0x35/0x15 read Status Register-1/2/3.
opcode 0x01/0x31/0x11 write Status Register-1/2/3.

Quad I/O Fast Read (EBh) requires 6 dummy cycles (2 mode + 4 dummy) for SDR.
*/

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_GIANTEC_H_ */
