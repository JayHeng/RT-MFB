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
Read performance:
//------------------------------------------------------
//   DC[0]    |  dummy cycles  |Quad IO Fast Read(SPI) |
//------------------------------------------------------
//  1'b0      | 6(def for SDR) |        104MHz         |
//  1'b1      |       10       |        133MHz         |
//------------------------------------------------------
*/

#endif /* _MFB_NOR_FLASH_XTX_H_ */
