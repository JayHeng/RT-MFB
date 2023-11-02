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

uint32_t mfb_flash_decode_common_capacity_id(uint8_t capacityID)
{
    uint32_t memSizeInBytes = 0;
    //| ISSI QuadSPI       |  MXIC OctalSPI     |  Micron QuadSPI    |
    //| ISSI OctalSPI      |  MXIC QuadSPI U    |GigaDevice QuadSPI Q|
    //| MXIC QuadSPI R/L/V |                    |Spansion QuadSPI FL-L|
    //| Winbond QuadSPI    |                    |                    |
    //| Winbond OctalSPI   |                    |                    |
    //| Micron OctalSPI    |                    |                    |
    //| GigaDevice QuadSPI |                    |                    |
    //| GigaDevice OctalSPI|                    |                    |
    //|Adesto QuadSPI SL/QL|                    |                    |
    //|---------------------------------------------------------------
    //| 09h - 256Kb        |                    |                    |
    //| 10h - 512Kb        |                    |                    |
    //| 11h - 1Mb          |                    |                    |
    //| 12h - 2Mb          |                    |                    |
    //| 13h - 4Mb          |                    |                    |
    //| 14h - 8Mb          |                    |                    |
    //| 15h - 16Mb         |                    |                    |
    //| 16h - 32Mb         |                    |                    |
    //| 17h - 64Mb         |  37h - 64Mb        |   17h - 64Mb       |
    //| 18h - 128Mb        |  38h - 128Mb       |   18h - 128Mb      |
    //| 19h - 256Mb        |  39h - 256Mb       |   19h - 256Mb      |
    //| 1ah - 512Mb        |  3ah - 512Mb       |   20h - 512Mb      |
    //| 1bh - 1Gb          |  3bh - 1Gb         |   21h - 1Gb        |
    //| 1ch - 2Gb          |  3ch - 2Gb         |   22h - 2Gb        |
    if (capacityID <= 0x09)
    {
        capacityID += 6;
    }
    else if (capacityID <= 0x1c)
    {
        // Do Nothing
    }
    else if (capacityID <= 0x22)
    {
        capacityID -= 6;
    }
    else
    {
        capacityID &= 0x1F;
    }
    memSizeInBytes = 1 << capacityID;
    return memSizeInBytes;
}

uint32_t mfb_flash_decode_adesto_capacity_id(uint8_t capacityID)
{
    uint32_t memSizeInBytes = 0;
    //|Adesto QuadSPI      | Adesto QuadSPI EU  |
    //|------------------------------------------
    //|                    | 10h - 1Mb          |
    //| 03h - 2Mb          | 11h - 2Mb          |
    //| 04h - 4Mb          | 14h - 4Mb          |
    //| 05h - 8Mb          |                    |
    //| 06h - 16Mb         |                    |
    //| 07h - 32Mb         |                    |
    //| 08h - 64Mb         |                    |
    //| 09h - 128Mb        |                    |
    if (capacityID <= 0x09)
    {
        capacityID += 15;
    }
    else if (capacityID == 0x10)
    {
        capacityID = 0x11;
    }
    else if (capacityID == 0x11)
    {
        capacityID = 0x12;
    }
    else if (capacityID == 0x14)
    {
        capacityID = 0x13;
    }
    memSizeInBytes = 1 << capacityID;
    return memSizeInBytes;
}

void mfb_flash_show_mem_size(uint8_t capacityID, bool isAdesto)
{
#if MFB_DEBUG_LOG_INFO_ENABLE
    uint32_t flashMemSizeInKB;
    if (isAdesto)
    {
        mfb_printf("MFB: Flash Density Code: 0x%x", capacityID);
        flashMemSizeInKB = mfb_flash_decode_adesto_capacity_id(capacityID)/ 0x400;
    }
    else
    {
        mfb_printf("MFB: Flash Capacity ID: 0x%x", capacityID);
        flashMemSizeInKB = mfb_flash_decode_common_capacity_id(capacityID)/ 0x400;
    }
    if (flashMemSizeInKB <= 0x400)
    {
        mfb_printf(" -- %dKB.\r\n", flashMemSizeInKB);
    }
    else
    {
        mfb_printf(" -- %dMB.\r\n", flashMemSizeInKB / 0x400);
    }
#endif
}

void mfb_flash_show_registers(jedec_id_t *jedecID, bool isOctalFlash)
{
#if MFB_FLASH_REGS_READBACK_ENABLE
    mfb_printf("MFB: Read internal registers from Flash.\r\n");
    switch (jedecID->manufacturerID)
    {
#if WINBOND_DEVICE_SERIES
        // Winbond
        case WINBOND_DEVICE_VENDOR_ID:
            mfb_flash_show_registers_for_winbond(isOctalFlash);
            break;
#endif // WINBOND_DEVICE_SERIES

#if MXIC_DEVICE_SERIES
        // MXIC
        case MXIC_DEVICE_VENDOR_ID:
            mfb_flash_show_registers_for_mxic(isOctalFlash);
            break;
#endif // MXIC_DEVICE_SERIES

#if GIGADEVICE_DEVICE_SERIES
        // GigaDevice
        case GIGADEVICE_DEVICE_VENDOR_ID:
            mfb_flash_show_registers_for_gigadevice(isOctalFlash);
            break;
#endif // GIGADEVICE_DEVICE_SERIES

#if ISSI_DEVICE_SERIES
        // ISSI
        case ISSI_DEVICE_VENDOR_ID:
            mfb_flash_show_registers_for_issi(isOctalFlash);
            break;
#endif // ISSI_DEVICE_SERIES

#if MICRON_DEVICE_SERIES
        // Micron
        case MICRON_DEVICE_VENDOR_ID:
        case MICRON_DEVICE_VENDOR_ID2:
            mfb_flash_show_registers_for_micron(isOctalFlash);
            break;
#endif // MICRON_DEVICE_SERIES

#if ADESTO_DEVICE_SERIES
        // Adesto
        case ADESTO_DEVICE_VENDOR_ID:
        case ADESTO_DEVICE_VENDOR_ID2:
            mfb_flash_show_registers_for_adesto(isOctalFlash);
            break;
#endif // ADESTO_DEVICE_SERIES

#if SPANSION_DEVICE_SERIES
        // Spansion
        case SPANSION_DEVICE_VENDOR_ID:
        case INFINEON_DEVICE_VENDOR_ID:
            mfb_flash_show_registers_for_spansion(isOctalFlash);
            break;
#endif // SPANSION_DEVICE_SERIES

        default:
            break;
    }
#endif
}

bool mfb_flash_is_valid_jedec_id(jedec_id_t *jedecID)
{
    bool isValidVendorId = true;
    /* Check Vendor ID. */
    switch (jedecID->manufacturerID)
    {
#if WINBOND_DEVICE_SERIES
        // Winbond
        case WINBOND_DEVICE_VENDOR_ID:
            mfb_flash_set_param_for_winbond(jedecID);
            break;
#endif // WINBOND_DEVICE_SERIES

#if MXIC_DEVICE_SERIES
        // MXIC
        case MXIC_DEVICE_VENDOR_ID:
            mfb_flash_set_param_for_mxic(jedecID);
            break;
#endif // MXIC_DEVICE_SERIES

#if GIGADEVICE_DEVICE_SERIES
        // GigaDevice
        case GIGADEVICE_DEVICE_VENDOR_ID:
            mfb_flash_set_param_for_gigadevice(jedecID);
            break;
#endif // GIGADEVICE_DEVICE_SERIES

#if ISSI_DEVICE_SERIES
        // ISSI
        case ISSI_DEVICE_VENDOR_ID:
            mfb_flash_set_param_for_issi(jedecID);
            break;
#endif // ISSI_DEVICE_SERIES

#if MICRON_DEVICE_SERIES
        // Micron
        case MICRON_DEVICE_VENDOR_ID:
        case MICRON_DEVICE_VENDOR_ID2:
            mfb_flash_set_param_for_micron(jedecID);
            break;
#endif // MICRON_DEVICE_SERIES

#if ADESTO_DEVICE_SERIES
        // Adesto
        case ADESTO_DEVICE_VENDOR_ID:
        case ADESTO_DEVICE_VENDOR_ID2:
            mfb_flash_set_param_for_adesto(jedecID);
            break;
#endif // ADESTO_DEVICE_SERIES

#if SPANSION_DEVICE_SERIES
        // Spansion
        case SPANSION_DEVICE_VENDOR_ID:
        case INFINEON_DEVICE_VENDOR_ID:
            mfb_flash_set_param_for_spansion(jedecID);
            break;
#endif // SPANSION_DEVICE_SERIES

        default:
            mfb_printf("\r\nMFB: Unsupported Manufacturer ID\r\n");
            isValidVendorId = false;
            break;
    }
    
    return isValidVendorId;
}

#if MFB_FLASH_PATTERN_VERIFY_ENABLE
static bool mfb_flash_handle_one_pattern_page(uint32_t pageAddr, bool isDataGen, bool showError)
{
    uint32_t patternWord = MFB_FLASH_PATTERN_STATIC_WORD;
    if (isDataGen)
    {
        for (uint32_t idx = 0; idx < EXAMPLE_FLASH_PAGE_SIZE / sizeof(uint32_t); idx++)
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
        uint32_t srcAddr = EXAMPLE_MIXSPI_AMBA_BASE + pageAddr;
        memcpy(g_flashRwBuffer, (uint8_t*)srcAddr, EXAMPLE_FLASH_PAGE_SIZE);
        for (uint32_t idx = 0; idx < EXAMPLE_FLASH_PAGE_SIZE / sizeof(uint32_t); idx++)
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
    uint32_t sectorMax = MFB_FLASH_ACCESS_REGION_SIZE / EXAMPLE_FLASH_SECTOR_SIZE;
    uint32_t pagesPerSector = EXAMPLE_FLASH_SECTOR_SIZE / EXAMPLE_FLASH_PAGE_SIZE;
    for (uint32_t sectorId = 0; sectorId < sectorMax; sectorId++)
    {
        uint32_t sectorAddr = MFB_FLASH_ACCESS_REGION_START + sectorId * EXAMPLE_FLASH_SECTOR_SIZE;
        status_t status = mixspi_nor_flash_erase_sector(EXAMPLE_MIXSPI, sectorAddr, flashInstMode);
        if (status != kStatus_Success)
        {
            mfb_printf("MFB: Erase flash sector failure at address 0x%x!\r\n", sectorAddr);
            return false;
        }
        for (uint32_t pageId = 0; pageId < pagesPerSector; pageId++)
        {
            uint32_t pageAddr = sectorAddr + pageId * EXAMPLE_FLASH_PAGE_SIZE;
            mfb_flash_handle_one_pattern_page(pageAddr, true, false);
            status = mixspi_nor_flash_page_program(EXAMPLE_MIXSPI, pageAddr, (const uint32_t *)g_flashRwBuffer, EXAMPLE_FLASH_PAGE_SIZE, flashInstMode);
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
    DCACHE_InvalidateByRange(EXAMPLE_MIXSPI_AMBA_BASE + MFB_FLASH_ACCESS_REGION_START, MFB_FLASH_ACCESS_REGION_SIZE);
#endif
    for (uint32_t idx = 0; idx < MFB_FLASH_ACCESS_REGION_SIZE / EXAMPLE_FLASH_PAGE_SIZE; idx++)
    {
        if (!mfb_flash_handle_one_pattern_page(MFB_FLASH_ACCESS_REGION_START + idx * EXAMPLE_FLASH_PAGE_SIZE, false, showError))
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

void mfb_flash_memcpy_perf_test(bool stressTestEnable)
{
#if MFB_FLASH_MEMCPY_PERF_ENABLE
    microseconds_shutdown();
    microseconds_init();

    uint64_t startTicks = microseconds_get_ticks();
    uint64_t totalSize = (8UL*1024*1024);
    uint32_t loopMax = totalSize / MFB_FLASH_ACCESS_REGION_SIZE;
    uint32_t unitSize = EXAMPLE_FLASH_PAGE_SIZE;
    uint32_t idxMax = MFB_FLASH_ACCESS_REGION_SIZE / unitSize;
    uint32_t srcAddr = 0;
#if MFB_FLASH_MEMCPY_STRESS_ENABLE
    if (!stressTestEnable)
    {
        microseconds_shutdown();
        return;
    }
    mfb_printf("MFB: Starting memcpy stress test...\r\n");
    while (1)
#endif
    {
        /* Read 8MB data from flash to test speed */
        for (uint32_t loop = 0; loop < loopMax; loop++)
        {
            /* Min NOR Flash size is 64KB */
            for (uint32_t idx = 0; idx < idxMax; idx++)
            {
                srcAddr = EXAMPLE_MIXSPI_AMBA_BASE + MFB_FLASH_ACCESS_REGION_START + idx * unitSize;
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

