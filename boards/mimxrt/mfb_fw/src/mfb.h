/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_H_
#define _MFB_H_


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MFB_LOG_INFO_ENABLE              (1)

#define MFB_FLASH_SPEED_TEST_ENABLE      (1)

#define MFB_APP_JUMP_ENABLE              (0)
#define MFB_APP_IMAGE_OFFSET             (0xC000)

#define MFB_FLASH_FAKE_JEDEC_ID_ENABLE   (0)

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void mfb_main(void);

int mfb_printf(const char *fmt_s, ...);

#endif /* _MFB_H_ */
