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

#if MICRON_DEVICE_MT25QL256
#define MICRON_FLASH_QUAD_ENABLE        0x00
#define MICRON_FLASH_BUSY_STATUS_POL    1
#define MICRON_FLASH_BUSY_STATUS_OFFSET 0
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_MICRON_H_ */
