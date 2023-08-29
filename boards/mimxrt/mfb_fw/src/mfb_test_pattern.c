/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

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

#if MFB_FLASH_PATTERN_VERIFY_ENABLE
static bool mfb_flash_handle_one_pattern_page(uint32_t pageAddr, bool isDataGen, bool showError)
{
    uint32_t patternWord = MFB_FLASH_PATTERN_STATIC_WORD;
    if (isDataGen)
    {
        for (uint32_t idx = 0; idx < FLASH_PAGE_SIZE / sizeof(uint32_t); idx++)
        {
            if (patternWord)
            {
                g_flashRwBuffer[idx] = patternWord;
            }
            else
            {
                g_flashRwBuffer[idx] = pageAddr + idx * sizeof(uint32_t);
            }
        }
    }
    else
    {
        uint32_t srcAddr = EXAMPLE_FLEXSPI_AMBA_BASE + pageAddr;
        memcpy(g_flashRwBuffer, (uint8_t*)srcAddr, FLASH_PAGE_SIZE);
        for (uint32_t idx = 0; idx < FLASH_PAGE_SIZE / sizeof(uint32_t); idx++)
        {
            uint32_t compareWord;
            if (patternWord)
            {
                compareWord = patternWord;
            }
            else
            {
                compareWord = pageAddr + idx * sizeof(uint32_t);
            }
            if (g_flashRwBuffer[idx] != compareWord)
            {
                if (showError)
                {
                    mfb_printf("MFB: Pattern verify failure at flash address 0x%x, error value is 0x%x.\r\n", pageAddr + idx * sizeof(uint32_t), g_flashRwBuffer[idx]);
                }
                return false;
            }
        }
    }
    
    return true;
}
#endif

bool mfb_flash_write_pattern_region(flash_inst_mode_t flashInstMode)
{
    bool result = true;

#if MFB_FLASH_PATTERN_VERIFY_ENABLE
    mfb_printf("MFB: Write pattern data into Flash region 0x%x - 0x%x.\r\n", MFB_FLASH_ACCESS_REGION_START, MFB_FLASH_ACCESS_REGION_START + MFB_FLASH_ACCESS_REGION_SIZE - 1);
    uint32_t sectorMax = MFB_FLASH_ACCESS_REGION_SIZE / SECTOR_SIZE;
    uint32_t pagesPerSector = SECTOR_SIZE / FLASH_PAGE_SIZE;
    for (uint32_t sectorId = 0; sectorId < sectorMax; sectorId++)
    {
        uint32_t sectorAddr = MFB_FLASH_ACCESS_REGION_START + sectorId * SECTOR_SIZE;
        status_t status = flexspi_nor_flash_erase_sector(EXAMPLE_FLEXSPI, sectorAddr, flashInstMode);
        if (status != kStatus_Success)
        {
            mfb_printf("MFB: Erase flash sector failure at address 0x%x!\r\n", sectorAddr);
            return false;
        }
        for (uint32_t pageId = 0; pageId < pagesPerSector; pageId++)
        {
            uint32_t pageAddr = sectorAddr + pageId * FLASH_PAGE_SIZE;
            mfb_flash_handle_one_pattern_page(pageAddr, true, false);
            status = flexspi_nor_flash_page_program(EXAMPLE_FLEXSPI, pageAddr, (const uint32_t *)g_flashRwBuffer, FLASH_PAGE_SIZE, flashInstMode);
            if (status != kStatus_Success)
            {
                mfb_printf("MFB: Program flash page failure at address 0x%x!\r\n", pageAddr);
                return false;
            }
        }
    }
#endif
    
    return result;
}

bool mfb_flash_pattern_verify_test(bool showError)
{
    bool result = true;

    // For QuadSPI/OctalSPI Flash
    //  1. It is 1st time verify (before QE/QPI/OPI enablment), do nothing when failure
    //  2. It is 2nd time verify (after QE/QPI/OPI enablment), write pattern when failure (ERASE/PROGRAM seq in vendor LUT)
    //  3. It is 3rd time verify (after QE/QPI/OPI enablment), just verify even failure (ERASE/PROGRAM seq in vendor LUT)
#if MFB_FLASH_PATTERN_VERIFY_ENABLE
#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    DCACHE_InvalidateByRange(EXAMPLE_FLEXSPI_AMBA_BASE + MFB_FLASH_ACCESS_REGION_START, MFB_FLASH_ACCESS_REGION_SIZE);
#endif
    for (uint32_t idx = 0; idx < MFB_FLASH_ACCESS_REGION_SIZE / FLASH_PAGE_SIZE; idx++)
    {
        if (!mfb_flash_handle_one_pattern_page(MFB_FLASH_ACCESS_REGION_START + idx * FLASH_PAGE_SIZE, false, showError))
        {
            result = false;
            break;
        }
    }

    if (result)
    {
        mfb_printf("MFB: Pass Flash Pattern data readback verification.\r\n");
    }
    else
    {
        //mfb_printf("MFB: Failed to verify Flash Pattern data via readback.\r\n");
    }
#endif
    
    return result;
}
