/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mfb_nor_flash_adesto.h"
#if ADESTO_DEVICE_SERIES
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/

#if ADESTO_DEVICE_AT25SF128A
const uint32_t s_customLUT_ADESTO_Quad[CUSTOM_LUT_LENGTH] = {
    /* Fast read quad mode - SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0xEB, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_MODE8_SDR, kFLEXSPI_4PAD, 0x00, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_4PAD, 0x04),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 2] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR,  kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Read status register - 1 */
    // opcode 0x05/0x35/0x15 to read Status Registers (1/2/3)
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_1PAD, 0x01),

    /* Write Enable - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Erase Sector - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x20, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),

    /* Page Program - single mode */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x02, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Enable Quad mode */
    // QE bit in 8bit Status Register-2[1], there are three Status Registers (1/2/3)
    // opcode 0x01/0x31/0x11 to write Status Registers (1/2/3)
    [4 * NOR_CMD_LUT_SEQ_IDX_ENABLEQE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x31, kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x01),

    /* Read status register */
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_1PAD, 0x03),
};
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

void mfb_flash_set_param_for_adesto(jedec_id_t *jedecID)
{
    mfb_printf(" -- Adesto Serial Flash.\r\n");
    if (jedecID->memoryTypeID != 0x42)
    {
        jedecID->capacityID = jedecID->memoryTypeID & 0x1F;
        jedecID->memoryTypeID = (jedecID->memoryTypeID & 0xE0) >> 5;
        mfb_printf("MFB: Flash Family Code: 0x%x", jedecID->memoryTypeID);
    }
    else
    {
        mfb_printf("MFB: Flash Memory Type ID: 0x%x", jedecID->memoryTypeID);
    }
    switch (jedecID->memoryTypeID)
    {
        /////////////////////////QuadSPI////////////////////////
        case 0x00:
            mfb_printf(" -- AT25EU QuadSPI 1.8-3.3V Series.\r\n");
            break;
        case 0x01:
            mfb_printf(" -- AT25DQ QuadSPI 2.5V Series.\r\n");
            break;
        case 0x02:
            mfb_printf(" -- AT25FF/AT25XE/AT25XV QuadSPI 1.8-3.3V Series.\r\n");
            break;
        case 0x04:
            mfb_printf(" -- AT25SF/AT25QF QuadSPI 3.3V Series.\r\n");
            break;
        // Only this type is same as other vendors
        case 0x42:
            mfb_printf(" -- AT25SL/AT25QL QuadSPI 1.8V Series.\r\n");
            break;
        ////////////////////////OctalSPI////////////////////////
        case 0x05:
            g_flashPropertyInfo.flashIsOctal = true;
            mfb_printf(" -- ATXP OctalSPI 1.8V Series.\r\n");
            break;
        default:
            mfb_printf(" -- Unsupported Series.\r\n");
            break;
    }
    if (jedecID->memoryTypeID != 0x42)
    {
        mfb_flash_show_mem_size(jedecID->capacityID, true);
        g_flashPropertyInfo.flashMemSizeInByte = mfb_flash_decode_adesto_capacity_id(jedecID->capacityID);
    }
    else
    {
        mfb_flash_show_mem_size(jedecID->capacityID, false);
        g_flashPropertyInfo.flashMemSizeInByte = mfb_flash_decode_common_capacity_id(jedecID->capacityID);
    }
#if ADESTO_DEVICE_QUAD
    if (!g_flashPropertyInfo.flashIsOctal)
    {
        g_flashPropertyInfo.mixspiPad                 = kFLEXSPI_4PAD;
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_133MHz;
        g_flashPropertyInfo.mixspiReadSampleClock     = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
        g_flashPropertyInfo.flashBusyStatusPol         = ADESTO_FLASH_BUSY_STATUS_POL;
        g_flashPropertyInfo.flashBusyStatusOffset      = ADESTO_FLASH_BUSY_STATUS_OFFSET;
        g_flashPropertyInfo.flashQuadEnableCfg         = ADESTO_FLASH_QUAD_ENABLE;
        g_flashPropertyInfo.flashQuadEnableBytes       = 1;
        g_flashPropertyInfo.mixspiCustomLUTVendor     = s_customLUT_ADESTO_Quad;
    }
#endif
}

void mfb_flash_show_registers_for_adesto(bool isOctalFlash)
{
#if MFB_FLASH_REGS_READBACK_ENABLE
    flash_reg_access_t regAccess;
    if (!isOctalFlash)
    {
        regAccess.regNum = 3;
        regAccess.regAddr = 0x0;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Status Register [7:0]: 0x%x\r\n", regAccess.regValue.B.reg1);
        mfb_printf("MFB: Flash Status Register [15:8]: 0x%x\r\n", regAccess.regValue.B.reg2);
        mfb_printf("MFB: Flash Status Register [23:16]: 0x%x\r\n", regAccess.regValue.B.reg3);
    }
#endif
}

#endif // ADESTO_DEVICE_SERIES

