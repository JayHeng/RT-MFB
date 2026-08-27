/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mfb_nor_flash_boya.h"
#if BOYA_DEVICE_SERIES
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/

#if BOYA_DEVICE_QUAD
const uint32_t s_customLUT_BOYA_Quad[CUSTOM_LUT_LENGTH] = {
    /* Fast read quad mode - SDR */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_READ] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0xEB, kMIXSPI_Command_RADDR_SDR, kMIXSPI_4PAD, 0x18),
    // If the "Continuous Read Mode" bits M5-4 = (1,0), then the next Fast Read Quad I/O instruction (after
    //   /CS is raised and then lowered) does not require the EBh instruction code
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_READ + 1] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_MODE8_SDR, kMIXSPI_4PAD, 0x00, kMIXSPI_Command_DUMMY_SDR, kMIXSPI_4PAD, BOYA_QUAD_FLASH_DUMMY_CYCLES - 2),
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_READ + 2] = 
        MIXSPI_LUT_SEQ(kMIXSPI_Command_READ_SDR,  kMIXSPI_4PAD, 0x04, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Read status register-1[7:0] */
    // opcode 0x05/0x35/0x15 to read Status Registers (1/2/3)
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x05, kMIXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0x01),

    /* Write Enable - SPI */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x06, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Erase Sector - SPI */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x20, kMIXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),

    /* Page Program - single mode */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x02, kMIXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x04, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Enable Quad mode */
    // QE bit in Status Register-2[1], there are three Status Registers (1/2/3)
    // opcode 0x31 to write Status Register-2
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_ENABLEQE] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x31, kMIXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x01),

    /* Set Dummy cycle */
    // DC bit in Status Register-3[4], there are three Status Registers (1/2/3)
    // opcode 0x11 to write Status Register-3
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_SETDUMMY] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x11, kMIXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x01),

    /* Read status register-2[15:8] */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_READREG] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x35, kMIXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0x01),

    /* Read status register-3[23:16] */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_READREG2] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x15, kMIXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0x01),
};
#endif

#if BOYA_DEVICE_OCTAL
const uint32_t s_customLUT_BOYA_Octal[CUSTOM_LUT_LENGTH] = {
#if !MFB_FLASH_OPI_MODE_DISABLE
    /*  DTR OCTAL I/O FAST READ (4-Byte address) */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_READ + 0] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_8PAD, 0xFD, kMIXSPI_Command_RADDR_DDR, kMIXSPI_8PAD, 0x20),
    // Don't care the dummy value setting here, as it uses external DQS
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_READ + 1] = 
        MIXSPI_LUT_SEQ(kMIXSPI_Command_DUMMY_DDR, kMIXSPI_8PAD, 0x06, kMIXSPI_Command_READ_DDR,  kMIXSPI_8PAD, 0x04),
#else
    /*  4-BYTE OCTAL I/O FAST READ (STR) */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_READ + 0] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0xCC, kMIXSPI_Command_RADDR_SDR, kMIXSPI_8PAD, 0x20),
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_READ + 1] = 
        MIXSPI_LUT_SEQ(kMIXSPI_Command_DUMMY_SDR, kMIXSPI_8PAD, 0x08, kMIXSPI_Command_READ_SDR,  kMIXSPI_8PAD, 0x04),
#endif

    /* Read status register - SPI */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x05, kMIXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0x01),

    /* Write Enable - SPI */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x06, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Set Dummy cycle */
    // Volatile Configuration Register address 000001h - Dummy cycle configuration
    //   - Default dummy cycle for OPI DTR is 16, max freq is 166MHz (with DQS)
    //   - To support freq 200MHz, min dummy cycle is 18 (0x12)
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_SETDUMMY] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x81, kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00),
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_SETDUMMY + 1] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00, kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x01),
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_SETDUMMY + 2] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x01, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Set driver strength */
    // Volatile Configuration Register address 000003h - Driver strength configuration
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_SETDRIVE] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x81, kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00),
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_SETDRIVE + 1] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00, kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x03),
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_SETDRIVE + 2] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x01, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Erase Sector - SPI (4-Byte address) */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x21, kMIXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x20),

    /* 4-BYTE Page Program - SPI */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x12, kMIXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x20),
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x04, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Dummy write, do nothing when AHB write command is triggered. */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_WRITE] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* Enter OPI mode */
    // Volatile Configuration Register address 000000h - I/O mode
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_ENTEROPI] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x81, kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00),
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_ENTEROPI + 1] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00, kMIXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00),
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_ENTEROPI + 2] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x01, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Read status register using Octal DTR read */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_8PAD, 0x05, kMIXSPI_Command_DUMMY_DDR, kMIXSPI_8PAD, 0x10),
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI + 1] = 
        MIXSPI_LUT_SEQ(kMIXSPI_Command_READ_DDR,  kMIXSPI_8PAD, 0x01, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Write Enable - OPI */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_8PAD, 0x06, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Erase Sector - OPI (4-Byte address) */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_OPI] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_8PAD, 0x21, kMIXSPI_Command_RADDR_DDR, kMIXSPI_8PAD, 0x20),

    /* 4-BYTE Page Program - OPI */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_8PAD, 0x12, kMIXSPI_Command_RADDR_DDR, kMIXSPI_8PAD, 0x20),
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI + 1] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_WRITE_DDR, kMIXSPI_8PAD, 0x04, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* READ VOLATILE CONFIGURATION REGISTER */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_READREG] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_8PAD, 0x85, kMIXSPI_Command_RADDR_DDR, kMIXSPI_8PAD, 0x20),
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_READREG + 1] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_DUMMY_DDR, kMIXSPI_8PAD, 0x10, kMIXSPI_Command_READ_DDR,  kMIXSPI_8PAD, 0x01),

    /* READ FLAG STATUS REGISTER */
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_READREG2] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_SDR,       kMIXSPI_8PAD, 0x70, kMIXSPI_Command_DUMMY_DDR, kMIXSPI_8PAD, 0x10),
    [MIXSPI_LUT_SUB_SEQ_LEN * NOR_CMD_LUT_SEQ_IDX_READREG2 + 1] =
        MIXSPI_LUT_SEQ(kMIXSPI_Command_READ_DDR,  kMIXSPI_8PAD, 0x01, kMIXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),
};
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

void mfb_flash_set_param_for_boya(jedec_id_t *jedecID)
{
    mfb_printf(" -- Boya Serial Flash.\r\n");
    mfb_printf("MFB: Flash Memory Type ID: 0x%x", jedecID->memoryTypeID);
    switch (jedecID->memoryTypeID)
    {
        /////////////////////////QuadSPI////////////////////////
        case 0x40:
            mfb_printf(" -- BY25FQ QuadSPI 3.3V Series.\r\n");
            g_flashPropertyInfo.mixspiRootClkFreq = kMixspiRootClkFreq_100MHz;
            break;
        ////////////////////////OctalSPI////////////////////////
        case 0x60:
        case 0x68:
            g_flashPropertyInfo.flashIsOctal = true;
            mfb_printf(" -- BY25XxxxEL OctalSPI 1.8V Series.\r\n");
            break;
        default:
            mfb_printf(" -- Unsupported Series.\r\n");
            break;
    }
    mfb_flash_show_mem_size(jedecID->capacityID, false);
#if BOYA_DEVICE_QUAD
    if (!g_flashPropertyInfo.flashIsOctal)
    {
        g_flashPropertyInfo.mixspiPad                 = kMIXSPI_4PAD;
        g_flashPropertyInfo.mixspiReadSampleClock     = kMIXSPI_SampClkLoopbackDqs;
        g_flashPropertyInfo.flashBusyStatusPol        = BOYA_FLASH_BUSY_STATUS_POL;
        g_flashPropertyInfo.flashBusyStatusOffset     = BOYA_FLASH_BUSY_STATUS_OFFSET;
        g_flashPropertyInfo.flashQuadEnableCfg        = BOYA_FLASH_QUAD_ENABLE;
        g_flashPropertyInfo.flashQuadEnableBytes      = 1;
        g_flashPropertyInfo.mixspiCustomLUTVendor     = s_customLUT_BOYA_Quad;
#if !MFB_FLASH_USE_DEFAULT_DUMMY
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_133MHz;
        g_flashPropertyInfo.flashDummyValue           = BOYA_QUAD_FLASH_SET_DUMMY_CMD;
#endif
    }
#endif
#if BOYA_DEVICE_OCTAL
    if (g_flashPropertyInfo.flashIsOctal)
    {
        g_flashPropertyInfo.mixspiPad                 = kMIXSPI_8PAD;
        g_flashPropertyInfo.flashBusyStatusPol        = BOYA_FLASH_BUSY_STATUS_POL;
        g_flashPropertyInfo.flashBusyStatusOffset     = BOYA_FLASH_BUSY_STATUS_OFFSET;
        g_flashPropertyInfo.flashEnableOctalCmd       = BOYA_OCTAL_FLASH_ENABLE_DDR_CMD;
        g_flashPropertyInfo.mixspiCustomLUTVendor     = s_customLUT_BOYA_Octal;
#if MFB_FLASH_OPI_MODE_DISABLE
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_30MHz;
        g_flashPropertyInfo.mixspiReadSampleClock     = kMIXSPI_SampClkLoopbackDqs;
#else
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_200MHz;
        g_flashPropertyInfo.mixspiReadSampleClock     = kMIXSPI_SampClkExtInputDqs;
#endif
        g_flashPropertyInfo.flashDummyValue           = BOYA_OCTAL_FLASH_SET_DUMMY_CMD;
    }
#endif
}

void mfb_flash_show_registers_for_boya(bool isOctalFlash)
{
#if MFB_FLASH_REGS_READBACK_ENABLE
    flash_reg_access_t regAccess;
    if (!isOctalFlash)
    {
        regAccess.regNum = 1;
        regAccess.regAddr = 0x0;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READSTATUS;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Status Register [7:0]: 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Status Register [15:8]: 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG2;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Status Register [23:16]: 0x%x\r\n", regAccess.regValue.B.reg1);
    }
    else
    {
        regAccess.regNum = 1;
        regAccess.regAddr = 0x0;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Status Register: 0x%x\r\n", regAccess.regValue.B.reg1);

        for (uint32_t idx = 0; idx <= 0x07; idx++)
        {
            regAccess.regAddr = 0x00000000 + idx;
            regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
            mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
            mfb_printf("MFB: Flash Volatile Configuration Register (0x%x): 0x%x\r\n", regAccess.regAddr, regAccess.regValue.B.reg1);
        }

        regAccess.regAddr = 0x00000000;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG2;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Flag Status Register: 0x%x\r\n", regAccess.regValue.B.reg1);
    }
#endif
}

#endif // BOYA_DEVICE_SERIES
