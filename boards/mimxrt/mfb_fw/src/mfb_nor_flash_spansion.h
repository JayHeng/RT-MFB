/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_SPANSION_H_
#define _MFB_NOR_FLASH_SPANSION_H_

#include "mfb_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if SPANSION_DEVICE_S25FL064L
#define SPANSION_25FL_L_FLASH_QUAD_ENABLE    0x0200
#define SPANSION_FLASH_BUSY_STATUS_POL    1
#define SPANSION_FLASH_BUSY_STATUS_OFFSET 0

//------------------------------------------------------
// CR3NV[3:0] |  dummy cycles  |Quad IO Fast Read(SPI) |
//            |                |Quad IO Fast Read(QPI) |
//------------------------------------------------------
//  4'b0001   |        1       |         35MHz         |
//  4'b0010   |        2       |         45MHz         |
//  4'b0011   |        3       |         55MHz         |
//  4'b0100   |        4       |         65MHz         |
//  4'b0101   |        5       |         75MHz         |
//  4'b0110   |        6       |         85MHz         |
//  4'b0111   |        7       |         95MHz         |
//  4'b1000   |   8(default)   |        108MHz         |
//   ...      |       ..       |        108MHz         |
//  4'b1111   |       15       |        108MHz         |
//------------------------------------------------------------------------------
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_SPANSION_H_ */
