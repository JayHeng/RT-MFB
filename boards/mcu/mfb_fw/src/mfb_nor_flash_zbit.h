/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_ZBIT_H_
#define _MFB_NOR_FLASH_ZBIT_H_

#include "mfb_nor_flash.h"

#if ZBIT_DEVICE_ZB25VQ64D

#define ZBIT_FLASH_QUAD_ENABLE        0x02
#define ZBIT_FLASH_BUSY_STATUS_POL    1
#define ZBIT_FLASH_BUSY_STATUS_OFFSET 0
#define ZBIT_QUAD_FLASH_DUMMY_CYCLES  0x06
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

#endif /* _MFB_NOR_FLASH_ZBIT_H_ */
