/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_WINBOND_H_
#define _MFB_NOR_FLASH_WINBOND_H_

#include "mfb_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if WINBOND_DEVICE_W25Q128JW
#define WINBOND_FLASH_QUAD_ENABLE        0x02
#define WINBOND_FLASH_BUSY_STATUS_POL    1
#define WINBOND_FLASH_BUSY_STATUS_OFFSET 0
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_WINBOND_H_ */
