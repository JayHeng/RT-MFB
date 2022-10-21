/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_ISSI_H_
#define _MFB_NOR_FLASH_ISSI_H_

#include "mfb_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define ISSI_FLASH_BUSY_STATUS_POL    1
#define ISSI_FLASH_BUSY_STATUS_OFFSET 0

#if ISSI_DEVICE_IS25WP064A
#define ISSI_FLASH_QUAD_ENABLE        0x40
#endif

#if ISSI_DEVICE_IS25WX256
#define ISSI_FLASH_ENABLE_OCTAL_CMD     0xe7
#define ISSI_FLASH_SET_DUMMY_CMD        0x14   // 200MHz OPI DDR
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_ISSI_H_ */
