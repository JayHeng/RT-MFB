/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_PUYA_H_
#define _MFB_NOR_FLASH_PUYA_H_

#include "mfb_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if PUYA_DEVICE_PY25Q128HA
#define PUYA_FLASH_QUAD_ENABLE        0x02
#define PUYA_FLASH_BUSY_STATUS_POL    1
#define PUYA_FLASH_BUSY_STATUS_OFFSET 0

#define PUYA_QUAD_FLASH_DUMMY_CYCLES  0x06

//------------------------------------------------------
//    NTR     |  dummy cycles  |Quad IO Fast Read(SPI) | 
//------------------------------------------------------
//    N/A     | 6(def for SPI) |        133MHz         |
//------------------------------------------------------
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_PUYA_H_ */
