/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "microseconds.h"
#include "mfb_nor_flash.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

void mfb_flash_memcpy_perf_test()
{
#if MFB_FLASH_MEMCPY_PERF_ENABLE
    microseconds_shutdown();
    microseconds_init();

    uint64_t startTicks = microseconds_get_ticks();
    uint64_t totalSize = (8UL*1024*1024);
    uint32_t loopMax = totalSize / MFB_FLASH_ACCESS_REGION_SIZE;
    uint32_t unitSize = FLASH_PAGE_SIZE;
    uint32_t idxMax = MFB_FLASH_ACCESS_REGION_SIZE / unitSize;
    uint32_t srcAddr = 0;
#if MFB_FLASH_MEMCPY_STRESS_ENABLE
    while (1)
#endif
    {
        /* Read 8MB data from flash to test speed */
        for (uint32_t loop = 0; loop < loopMax; loop++)
        {
            /* Min NOR Flash size is 64KB */
            for (uint32_t idx = 0; idx < idxMax; idx++)
            {
                srcAddr = EXAMPLE_FLEXSPI_AMBA_BASE + MFB_FLASH_ACCESS_REGION_START + idx * unitSize;
                memcpy(g_flashRwBuffer, (uint8_t*)srcAddr, unitSize);
            }
        }
#if !MFB_FLASH_MEMCPY_STRESS_ENABLE
        uint64_t totalTicks = microseconds_get_ticks() - startTicks;
        uint32_t microSecs = microseconds_convert_to_microseconds(totalTicks);
        uint32_t kBps = (totalSize / 1024) * 1000000 / microSecs;
        mfb_printf("MFB: Flash to RAM memcpy speed: %dKB/s.\r\n", kBps);
#endif
    }

    microseconds_shutdown();
#endif
}
