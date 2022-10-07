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

#if ISSI_DEVICE_IS25WP064A
#define ISSI_FLASH_QUAD_ENABLE        0x40
#define ISSI_FLASH_BUSY_STATUS_POL    1
#define ISSI_FLASH_BUSY_STATUS_OFFSET 0
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_ISSI_H_ */
