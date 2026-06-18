/*
 * Copyright 2018-2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_FMSH_H_
#define _MFB_NOR_FLASH_FMSH_H_

#include "mfb_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if FMSH_DEVICE_FM25LQ64

#define FMSH_FLASH_QUAD_ENABLE        0x02
#define FMSH_FLASH_BUSY_STATUS_POL    1
#define FMSH_FLASH_BUSY_STATUS_OFFSET 0
#define FMSH_QUAD_FLASH_DUMMY_CYCLES  0x06
#endif

/*
FM25LQ64I3 (Shanghai Fudan Microelectronics, brand FM) is a 64M-bit (8M-byte) Serial Flash.
JEDEC ID (9Fh): Manufacturer 0xA1 (FMSH), Device ID 0x6017 (Memory Type 0x60, Capacity 0x17, 64M-bit).
QE (Quad Enable) bit is bit 1 of Status Register-2 (S9), written by opcode 0x31.
There are two Status Registers only:
opcode 0x05 reads Status Register-1 (S7-S0).
opcode 0x35 reads Status Register-2 (S15-S8).
opcode 0x01 writes Status Register-1 (and optionally Status Register-2).
opcode 0x31 writes Status Register-2.

Quad I/O Fast Read (EBh) requires 6 dummy cycles by default for SDR.
Max FAST_READ clock frequency: 133MHz.
Max DTR FASTREAD clock frequency: 104MHz.
Supply Voltage: 1.65V to 2.0V (1.8V class device).
*/


/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_FMSH_H_ */
