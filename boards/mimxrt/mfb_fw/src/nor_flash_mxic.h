/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NOR_FLASH_MXIC_H_
#define _NOR_FLASH_MXIC_H_

#include "nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if MXIC_DEVICE_MX25UM51345
#define MXIC_FLASH_BUSY_STATUS_POL    1
#define MXIC_FLASH_BUSY_STATUS_OFFSET 0
#define MXIC_FLASH_ENABLE_OCTAL_CMD   0x02
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _NOR_FLASH_MXIC_H_ */
