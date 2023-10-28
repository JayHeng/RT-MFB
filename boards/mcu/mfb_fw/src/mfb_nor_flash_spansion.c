/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mfb_nor_flash_spansion.h"
#if SPANSION_DEVICE_SERIES
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/

#if SPANSION_DEVICE_S25FL064L
const uint32_t s_customLUT_SPANSION_Quad[CUSTOM_LUT_LENGTH] = {
#if !MFB_FLASH_QPI_MODE_ENABLE
    /* Fast read quad mode - SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0xEB, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_MODE8_SDR, kFLEXSPI_4PAD, 0x00, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_4PAD, 0x08),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 2] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR,  kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),
#else
    /* Fast read quad mode - QPI-SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_4PAD, 0xEB, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_MODE8_SDR, kFLEXSPI_4PAD, 0x00, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_4PAD, 0x08),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 2] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR,  kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),
#endif

    /* Read status register - 1 */
    // opcode 0x05/0x07 to read Status Registers (1/2)
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
    // QE bit in 8bit Configuration Register-1[1], there are three Configuration Registers (1/2/3)
    // opcode 0x01 to write Status Registers 1 and Configuration Registers (1&2&3)
    [4 * NOR_CMD_LUT_SEQ_IDX_ENABLEQE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x01, kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04),

    /* Read Any register */
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x65, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR,  kFLEXSPI_1PAD, 0x01, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* Enter QPI mode */
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTERQPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x38, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Read status register -QPI-SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_QPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_4PAD, 0x05, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_4PAD, 0x01),

    /* Write Enable -QPI-SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_QPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_4PAD, 0x06, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Erase Sector - QPI-SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_QPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_4PAD, 0x20, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x18),

    /* Page Program - QPI-SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_4PAD, 0x02, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),
};
#endif

#if SPANSION_DEVICE_S28HS512
const uint32_t s_customLUT_SPANSION_Octal[CUSTOM_LUT_LENGTH] = {
#if !MFB_FLASH_OPI_MODE_DISABLE
    /*  DDR OCTAL I/O FAST READ */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 0] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0xEE, kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0xEE),
    // Don't care the dummy value setting here, as it uses external DQS
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 0x06),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 2] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR,  kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),
#else
    /*  4-BYTE Fast READ (1S-1S-1S) */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 0] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x0B, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x20),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x09, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_1PAD, 0x04),
#endif

    /* Read status register -SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_1PAD, 0x01),

    /* Write Enable - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Set Dummy cycle */
    // Volatile Configuration Register address 800003h - Dummy cycle configuration
    //   - Default dummy cycle for OPI DDR is 20, max freq is 200MHz
    [4 * NOR_CMD_LUT_SEQ_IDX_SETDUMMY] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x71, kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x80),
    [4 * NOR_CMD_LUT_SEQ_IDX_SETDUMMY + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x00, kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x03),
    [4 * NOR_CMD_LUT_SEQ_IDX_SETDUMMY + 2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x01, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Erase Sector - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x21, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x20),

    /* 4-BYTE Page Program - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x20, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Dummy write, do nothing when AHB write command is triggered. */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* Enter OPI mode */
    // Volatile Configuration Register address 800006h - I/O mode
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTEROPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x71, kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x80),
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTEROPI + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x00, kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x06),
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTEROPI + 2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x01, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Read status register using Octal DDR read */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x05, kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x05),
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI + 1] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI + 2] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI + 3] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 0x06, kFLEXSPI_Command_READ_DDR,  kFLEXSPI_8PAD, 0x01),

    /* Write Enable - OPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x06, kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x06),

    /* Erase Sector - OPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_OPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x21, kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x21),
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_OPI + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* 4-BYTE Page Program - OPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x12, kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x12),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04),

    /* READ ANY REGISTER */
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG + 0] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x65, kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x65),
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG + 1] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 0x08),
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG + 2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR,  kFLEXSPI_8PAD, 0x01, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),
};
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

void mfb_flash_set_param_for_spansion(jedec_id_t *jedecID)
{
    mfb_printf(" -- Spansion Serial Flash.\r\n");
    mfb_printf("MFB: Flash Memory Type ID: 0x%x", jedecID->memoryTypeID);
    switch (jedecID->memoryTypeID)
    {
        /////////////////////////QuadSPI////////////////////////
        case 0x60:
            // S25FL-L QuadSPI
            g_flashPropertyInfo.flashHasQpiSupport = true;
            g_flashPropertyInfo.mixspiReadSampleClock = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
            g_flashPropertyInfo.flashQuadEnableCfg = SPANSION_25FL_L_FLASH_QUAD_ENABLE;
            g_flashPropertyInfo.flashQuadEnableBytes = 2;
            mfb_printf(" -- S25FL-L QuadSPI 3.3V Series.\r\n");
            break;
        ////////////////////////OctalSPI////////////////////////
        case 0x5A:
            g_flashPropertyInfo.flashIsOctal = true;
            mfb_printf(" -- S28HL OctalSPI 3.3V Series.\r\n");
            break;
        case 0x5B:
            g_flashPropertyInfo.flashIsOctal = true;
            mfb_printf(" -- S28HS OctalSPI 1.8V Series.\r\n");
            break;
        default:
            mfb_printf(" -- Unsupported Series.\r\n");
            break;
    }
    mfb_flash_show_mem_size(jedecID->capacityID, false);
#if SPANSION_DEVICE_QUAD
    if (!g_flashPropertyInfo.flashIsOctal)
    {
        g_flashPropertyInfo.mixspiPad                 = kFLEXSPI_4PAD;
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_100MHz;
        g_flashPropertyInfo.flashBusyStatusPol         = SPANSION_FLASH_BUSY_STATUS_POL;
        g_flashPropertyInfo.flashBusyStatusOffset      = SPANSION_FLASH_BUSY_STATUS_OFFSET;
        g_flashPropertyInfo.mixspiCustomLUTVendor     = s_customLUT_SPANSION_Quad;
    }
#endif
#if SPANSION_DEVICE_OCTAL
    if (g_flashPropertyInfo.flashIsOctal)
    {
        g_flashPropertyInfo.mixspiPad                 = kFLEXSPI_8PAD;
        g_flashPropertyInfo.flashBusyStatusPol         = SPANSION_FLASH_BUSY_STATUS_POL;
        g_flashPropertyInfo.flashBusyStatusOffset      = SPANSION_FLASH_BUSY_STATUS_OFFSET;
        g_flashPropertyInfo.flashEnableOctalCmd        = SPANSION_OCTAL_FLASH_ENABLE_DDR_CMD;
        g_flashPropertyInfo.mixspiCustomLUTVendor     = s_customLUT_SPANSION_Octal;
#if MFB_FLASH_OPI_MODE_DISABLE
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_30MHz;
        g_flashPropertyInfo.mixspiReadSampleClock     = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
#else
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_332MHz;
        g_flashPropertyInfo.mixspiReadSampleClock     = kFLEXSPI_ReadSampleClkExternalInputFromDqsPad;
        if (g_flashPropertyInfo.mixspiRootClkFreq == kMixspiRootClkFreq_400MHz)
#endif
        {
            g_flashPropertyInfo.flashDummyValue = SPANSION_OCTAL_FLASH_SET_DUMMY_CMD;
        }
    }
#endif
}

void mfb_flash_show_registers_for_spansion(bool isOctalFlash)
{
#if MFB_FLASH_REGS_READBACK_ENABLE
    flash_reg_access_t regAccess;
    if (!isOctalFlash)
    {
        regAccess.regNum = 1;
        regAccess.regAddr = 0x000000;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        flexspi_nor_read_register(EXAMPLE_FLEXSPI, &regAccess);
        mfb_printf("MFB: Flash Non-Volatile Status Register 1 (SR1NV): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x000002;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        flexspi_nor_read_register(EXAMPLE_FLEXSPI, &regAccess);
        mfb_printf("MFB: Flash Non-Volatile Configuration Register 1 (CR1NV): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x000003;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        flexspi_nor_read_register(EXAMPLE_FLEXSPI, &regAccess);
        mfb_printf("MFB: Flash Non-Volatile Configuration Register 2 (CR2NV): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x000004;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        flexspi_nor_read_register(EXAMPLE_FLEXSPI, &regAccess);
        mfb_printf("MFB: Flash Non-Volatile Configuration Register 3 (CR3NV): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x000030;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        flexspi_nor_read_register(EXAMPLE_FLEXSPI, &regAccess);
        mfb_printf("MFB: Flash Individual and Region Protection Register (IRP[7:0]): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x000031;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        flexspi_nor_read_register(EXAMPLE_FLEXSPI, &regAccess);
        mfb_printf("MFB: Flash Individual and Region Protection Register (IRP[15:8]): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x800000;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        flexspi_nor_read_register(EXAMPLE_FLEXSPI, &regAccess);
        mfb_printf("MFB: Flash Volatile Status Register 1 (SR1V): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x800001;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        flexspi_nor_read_register(EXAMPLE_FLEXSPI, &regAccess);
        mfb_printf("MFB: Flash Volatile Status Register 2 (SR2V): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x800002;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        flexspi_nor_read_register(EXAMPLE_FLEXSPI, &regAccess);
        mfb_printf("MFB: Flash Volatile Configuration Register 1 (CR1V): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x800003;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        flexspi_nor_read_register(EXAMPLE_FLEXSPI, &regAccess);
        mfb_printf("MFB: Flash Volatile Configuration Register 2 (CR2V): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x800004;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        flexspi_nor_read_register(EXAMPLE_FLEXSPI, &regAccess);
        mfb_printf("MFB: Flash Volatile Configuration Register 3 (CR3V): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x800040;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        flexspi_nor_read_register(EXAMPLE_FLEXSPI, &regAccess);
        mfb_printf("MFB: Flash Protection Register (PR): 0x%x\r\n", regAccess.regValue.B.reg1);
    }
    else
    {
        regAccess.regNum = 1;
        regAccess.regAddr = 0x0;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI;
        flexspi_nor_read_register(EXAMPLE_FLEXSPI, &regAccess);
        mfb_printf("MFB: Flash Status Register 1: 0x%x\r\n", regAccess.regValue.B.reg1);

        //regAccess.regAddr = 0x00800000;
        //regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        //flexspi_nor_read_register(EXAMPLE_FLEXSPI, &regAccess);
        //mfb_printf("MFB: Flash Status Register 1: 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x00800001;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        flexspi_nor_read_register(EXAMPLE_FLEXSPI, &regAccess);
        mfb_printf("MFB: Flash Status Register 2: 0x%x\r\n", regAccess.regValue.B.reg1);

        for (uint32_t idx = 2; idx <= 6; idx++)
        {
            regAccess.regAddr = 0x00800000 + idx;
            regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
            flexspi_nor_read_register(EXAMPLE_FLEXSPI, &regAccess);
            mfb_printf("MFB: Flash Configuration Register %d: 0x%x\r\n", idx - 1, regAccess.regValue.B.reg1);
        }
    }
#endif
}

#endif // SPANSION_DEVICE_SERIES

