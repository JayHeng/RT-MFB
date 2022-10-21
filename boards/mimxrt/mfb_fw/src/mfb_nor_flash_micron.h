/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_MICRON_H_
#define _MFB_NOR_FLASH_MICRON_H_

#include "mfb_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MICRON_FLASH_BUSY_STATUS_POL    1
#define MICRON_FLASH_BUSY_STATUS_OFFSET 0

#if MICRON_DEVICE_MT25QL256
#define MICRON_FLASH_QUAD_ENABLE        0x00
#endif

#if MICRON_DEVICE_MT35XU512
#define MICRON_OCTAL_FLASH_JEDEC_ID     (0x001a5B20)
#define MICRON_FLASH_ENABLE_OCTAL_CMD   0xe7
#define MICRON_FLASH_SET_DUMMY_CMD      0x14   // 200MHz OPI DDR
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_MICRON_H_ */
