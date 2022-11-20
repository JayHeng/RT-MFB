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

uint32_t mfb_decode_common_capacity_id(uint8_t capacityID)
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

uint32_t mfb_decode_adesto_capacity_id(uint8_t capacityID)
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
        flashMemSizeInKB = mfb_decode_adesto_capacity_id(capacityID)/ 0x400;
    }
    else
    {
        mfb_printf("MFB: Flash Capacity ID: 0x%x", capacityID);
        flashMemSizeInKB = mfb_decode_common_capacity_id(capacityID)/ 0x400;
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
    switch (jedecID->manufacturerID)
    {
#if WINBOND_DEVICE_SERIES
        // Winbond
        case WINBOND_DEVICE_VENDOR_ID:
            break;
#endif // WINBOND_DEVICE_SERIES

#if MXIC_DEVICE_SERIES
        // MXIC
        case MXIC_DEVICE_VENDOR_ID:
            break;
#endif // MXIC_DEVICE_SERIES

#if GIGADEVICE_DEVICE_SERIE
        // GigaDevice
        case GIGADEVICE_DEVICE_VENDOR_ID:
            break;
#endif // GIGADEVICE_DEVICE_SERIE

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
            break;
#endif // MICRON_DEVICE_SERIES

#if ADESTO_DEVICE_SERIE
        // Adesto
        case ADESTO_DEVICE_VENDOR_ID:
        case ADESTO_DEVICE_VENDOR_ID2:
            break;
#endif // ADESTO_DEVICE_SERIE

        default:
            break;
    }
#endif
}

