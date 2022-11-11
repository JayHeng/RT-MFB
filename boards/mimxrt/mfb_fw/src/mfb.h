/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_H_
#define _MFB_H_

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
// Whether to jump into app (appliable when MFB is used as FDCB)
#define MFB_APP_JUMP_ENABLE              (0)
// The app image offset in flash device
#define MFB_APP_IMAGE_OFFSET             (0xC000)

// Whether to enable QPI mode for Quad Flash
#define MFB_FLASH_QPI_MODE_ENABLE        (1)
// Whether to disable OPI mode for Octal Flash (test dummy cycle setting for Octal Flash)
#define MFB_FLASH_OPI_MODE_DISABLE       (0)

// Whether to show MFB log info via UART console (for development)
#define MFB_DEBUG_LOG_INFO_ENABLE        (1)
// The flash region size used for perf and pattern test
#define MFB_FLASH_ACCESS_REGION_START    (0x10000U)
#define MFB_FLASH_ACCESS_REGION_SIZE     (0x10000U)
// Whether to show flash to RAM memcpy perf result
#define MFB_FLASH_MEMCPY_PERF_ENABLE     (1)
// Whether to write pattern data into flash to do readback verify
#define MFB_FLASH_PATTERN_VERIFY_ENABLE  (1)

// Whether to use fake jedec id (appliable when there is no id in flash device)
#define MFB_FLASH_FAKE_JEDEC_ID_ENABLE   (0)
// Whether to show flash internal registers
#define MFB_FLASH_REGS_READBACK_ENABLE   (0)

// Supported Flexspi clock defn
typedef enum _flexspi_root_clk_freq
{
    kFlexspiRootClkFreq_30MHz  = 1,
    kFlexspiRootClkFreq_50MHz  = 2,
    kFlexspiRootClkFreq_60MHz  = 3,
    kFlexspiRootClkFreq_80MHz  = 4,
    kFlexspiRootClkFreq_100MHz = 5,
    kFlexspiRootClkFreq_120MHz = 6,
    kFlexspiRootClkFreq_133MHz = 7,
    kFlexspiRootClkFreq_166MHz = 8,
    kFlexspiRootClkFreq_200MHz = 9,
} flexspi_root_clk_freq_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if MFB_FLASH_MEMCPY_PERF_ENABLE | MFB_FLASH_PATTERN_VERIFY_ENABLE
extern uint32_t g_flashRwBuffer[];
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void mfb_main(void);

int mfb_printf(const char *fmt_s, ...);

#endif /* _MFB_H_ */
