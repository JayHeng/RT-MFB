/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mfb_nor_flash_gigadevice.h"
#if GIGADEVICE_DEVICE_SERIES
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if GIGADEVICE_DEVICE_QUAD
const uint32_t s_customLUT_GIGADEVICE_Quad[CUSTOM_LUT_LENGTH] = {
#if !MFB_FLASH_QPI_MODE_ENABLE
    /* Fast read quad mode - SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0xEB, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_4PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_MODE8_SDR, kMIXSPI_4PAD, 0x00, kFLEXSPI_Command_DUMMY_SDR, kMIXSPI_4PAD, GIGADEVICE_QUAD_FLASH_DUMMY_CYCLES - 2),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 2] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR,  kMIXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),
#else
    /* Fast read quad mode - QPI-SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_4PAD, 0xEB, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_4PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_MODE8_SDR, kMIXSPI_4PAD, 0x00, kFLEXSPI_Command_DUMMY_SDR, kMIXSPI_4PAD, GIGADEVICE_QUAD_FLASH_DUMMY_CYCLES - 2),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 2] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR,  kMIXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),
#endif

    /* Read status register - 1 */
    // opcode 0x05/0x35 to read Status Registers (1/2)
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0x01),

    /* Write Enable - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Erase Sector - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x20, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),

    /* Page Program - single mode */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x02, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Enable Quad mode */
#if GIGADEVICE_DEVICE_GD25QxxxC | GIGADEVICE_DEVICE_GD25BxxxE
    // QE bit in 8bit Status Register-2[1], there are two Status Registers (1/2)
    // opcode 0x31 to write Status Register 2
    [4 * NOR_CMD_LUT_SEQ_IDX_ENABLEQE] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x31, kFLEXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x01),
#elif GIGADEVICE_DEVICE_GD25QxxxE | GIGADEVICE_DEVICE_GD25LExxxD | GIGADEVICE_DEVICE_GD25LQxxxC
    // QE bit in 8bit Status Register-2[1], there are two Status Registers (1/2)
    // opcode 0x01 to write Status Registers (1&2)
    [4 * NOR_CMD_LUT_SEQ_IDX_ENABLEQE] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x01, kFLEXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x02),
#endif

#if GIGADEVICE_DEVICE_GD25BxxxE
    /* Set Dummy cycle */
    [4 * NOR_CMD_LUT_SEQ_IDX_SETDUMMY] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x11, kFLEXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x01),
#elif GIGADEVICE_DEVICE_GD25LExxxD | GIGADEVICE_DEVICE_GD25LQxxxC
    /* Set Dummy cycle */
    [4 * NOR_CMD_LUT_SEQ_IDX_SETDUMMY] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0xC0, kFLEXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x01),
#elif GIGADEVICE_DEVICE_GD25LTxxxE | GIGADEVICE_DEVICE_GD25LBxxxE
    // Volatile Configuration Register address 000001h - Dummy cycle configuration
    //   - To support freq 166MHz, min dummy cycle is 14
    [4 * NOR_CMD_LUT_SEQ_IDX_SETDUMMY] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x81, kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_SETDUMMY + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00, kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x01),
    [4 * NOR_CMD_LUT_SEQ_IDX_SETDUMMY + 2] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x01, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),
#endif

    /* Read status register - 2 */
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x35, kFLEXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0x01),

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* Enter QPI mode */
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTERQPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x38, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Read status register -QPI-SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_QPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_4PAD, 0x05, kFLEXSPI_Command_READ_SDR,  kMIXSPI_4PAD, 0x01),

    /* Write Enable -QPI-SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_QPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_4PAD, 0x06, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Erase Sector - QPI-SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_QPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_4PAD, 0x20, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_4PAD, 0x18),

    /* Page Program - QPI-SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_4PAD, 0x02, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_4PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kMIXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),
};
#endif

#if GIGADEVICE_DEVICE_OCTAL
const uint32_t s_customLUT_GIGADEVICE_Octal[CUSTOM_LUT_LENGTH] = {
#if !MFB_FLASH_OPI_MODE_DISABLE
    /*  DDR OCTAL I/O FAST READ */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 0] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_8PAD, 0xFD, kFLEXSPI_Command_RADDR_DDR, kMIXSPI_8PAD, 0x20),
    // Don't care the dummy value setting here, as it uses external DQS
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DUMMY_DDR, kMIXSPI_8PAD, 0x06, kFLEXSPI_Command_READ_DDR,  kMIXSPI_8PAD, 0x04),
#else
    /*  4-BYTE OCTAL I/O FAST READ */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 0] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0xCC, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_8PAD, 0x20),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DUMMY_SDR, kMIXSPI_8PAD, GIGADEVICE_OCTAL_FLASH_DUMMY_CYCLES, kFLEXSPI_Command_READ_SDR,  kMIXSPI_8PAD, 0x04),
#endif

    /* Read status register -SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0x01),

    /* Write Enable - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Set Dummy cycle */
    // Volatile Configuration Register address 000001h - Dummy cycle configuration
    //   - Default dummy cycle for OPI DDR is 16, max freq is 200MHz
    //   - To support freq 200MHz, min dummy cycle is 16
    [4 * NOR_CMD_LUT_SEQ_IDX_SETDUMMY] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x81, kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_SETDUMMY + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00, kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x01),
    [4 * NOR_CMD_LUT_SEQ_IDX_SETDUMMY + 2] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x01, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Erase Sector - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x21, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x20),

    /* 4-BYTE Page Program - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x12, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x20),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /*  Dummy write, do nothing when AHB write command is triggered. */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITE] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* Enter OPI mode */
    // Volatile Configuration Register address 000000h - I/O mode
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTEROPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x81, kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTEROPI + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00, kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTEROPI + 2] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x01, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /*  Read status register using Octal DDR read */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_8PAD, 0x05, kFLEXSPI_Command_DUMMY_DDR, kMIXSPI_8PAD, 0x10),
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI + 1] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR,  kMIXSPI_8PAD, 0x01, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Write Enable - OPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_8PAD, 0x06, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Erase Sector - OPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_OPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_8PAD, 0x21, kFLEXSPI_Command_RADDR_DDR, kMIXSPI_8PAD, 0x20),

    /* 4-BYTE Page Program - OPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_8PAD, 0x12, kFLEXSPI_Command_RADDR_DDR, kMIXSPI_8PAD, 0x20),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_DDR, kMIXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* READ VOLATILE CONFIGURATION REGISTER */
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_8PAD, 0x85, kFLEXSPI_Command_RADDR_DDR, kMIXSPI_8PAD, 0x20),
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DUMMY_DDR, kMIXSPI_8PAD, 0x10, kFLEXSPI_Command_READ_DDR,  kMIXSPI_8PAD, 0x01),

    /* READ FLAG STATUS REGISTER */
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG2] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_8PAD, 0x70, kFLEXSPI_Command_DUMMY_DDR, kMIXSPI_8PAD, 0x10),
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG2 + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR,  kMIXSPI_8PAD, 0x01, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Read Extended Addr Register */
    /*
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG2] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_8PAD, 0xC8, kFLEXSPI_Command_DUMMY_DDR, kMIXSPI_8PAD, 0x10),
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG2 + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR,  kMIXSPI_8PAD, 0x01, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),
    */
};
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

void mfb_flash_set_param_for_gigadevice(jedec_id_t *jedecID)
{
    mfb_printf(" -- GigaDevice Serial Flash.\r\n");
    mfb_printf("MFB: Flash Memory Type ID: 0x%x", jedecID->memoryTypeID);
    switch (jedecID->memoryTypeID)
    {
        /////////////////////////QuadSPI////////////////////////
        case 0x40:
            // GD25D DualSPI
            mfb_printf(" -- GD25Q/GD25B/GD25S QuadSPI 3.3V Series.\r\n");
            g_flashPropertyInfo.flashHasQpiSupport = false;
            break;
        case 0x42:
            mfb_printf(" -- GD25VQ/GD25VE QuadSPI 2.5V Series.\r\n");
            break;
        case 0x47:
            mfb_printf(" -- GD55B QuadSPI 3.3V Series.\r\n");
            break;
        case 0x60:
            // GD25LD DualSPI
            g_flashPropertyInfo.flashHasQpiSupport = true;
            mfb_printf(" -- GD25LE/GD25LQ QuadSPI 1.8V Series.\r\n");
            break;
        case 0x63:
            mfb_printf(" -- GD25LF/GD55LF QuadSPI 1.8V Series.\r\n");
            break;
        case 0x65:
            // GD25WD DualSPI
            mfb_printf(" -- GD25W/GD55W QuadSPI 1.8-3.3V Series.\r\n");
            break;
        case 0x66:
            g_flashPropertyInfo.flashHasQpiSupport = true;
            mfb_printf(" -- GD25LT/GD55LT QuadSPI 1.8V Series.\r\n");
            break;
        case 0x67:
            mfb_printf(" -- GD25LB/GD55LB QuadSPI 1.8V Series.\r\n");
            break;
        ////////////////////////OctalSPI////////////////////////
        case 0x48:
            g_flashPropertyInfo.flashIsOctal = true;
            mfb_printf(" -- GD25X/GD55X OctalSPI 3.3V Series.\r\n");
            break;
        case 0x68:
            g_flashPropertyInfo.flashIsOctal = true;
            mfb_printf(" -- GD25LX/GD55LX OctalSPI 1.8V Series.\r\n");
            break;
        // Missing GD25F, GD25LR, GD25T, GD25R
        default:
            mfb_printf(" -- Unsupported Series.\r\n");
            break;
    }
    mfb_flash_show_mem_size(jedecID->capacityID, false);
#if GIGADEVICE_DEVICE_QUAD
    if (!g_flashPropertyInfo.flashIsOctal)
    {
        g_flashPropertyInfo.mixspiPad                 = kMIXSPI_4PAD;

        g_flashPropertyInfo.flashBusyStatusPol        = GIGADEVICE_FLASH_BUSY_STATUS_POL;
        g_flashPropertyInfo.flashBusyStatusOffset     = GIGADEVICE_FLASH_BUSY_STATUS_OFFSET;
        g_flashPropertyInfo.mixspiCustomLUTVendor     = s_customLUT_GIGADEVICE_Quad;
#if GIGADEVICE_DEVICE_GD25QxxxC
        g_flashPropertyInfo.mixspiReadSampleClock     = kMIXSPI_SampClkLoopbackDqs;
        g_flashPropertyInfo.flashQuadEnableCfg        = GIGADEVICE_FLASH_QUAD_ENABLE;
        g_flashPropertyInfo.flashQuadEnableBytes      = 1;
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_120MHz;
#elif GIGADEVICE_DEVICE_GD25QxxxE
        g_flashPropertyInfo.mixspiReadSampleClock     = kMIXSPI_SampClkLoopbackDqs;
        g_flashPropertyInfo.flashQuadEnableCfg        = GIGADEVICE_FLASH_QUAD_ENABLE;
        g_flashPropertyInfo.flashQuadEnableBytes      = 2;
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_133MHz;
#elif GIGADEVICE_DEVICE_GD25BxxxE
        g_flashPropertyInfo.mixspiReadSampleClock     = kMIXSPI_SampClkLoopbackDqs;
        g_flashPropertyInfo.flashQuadEnableCfg        = GIGADEVICE_FLASH_QUAD_ENABLE;
        g_flashPropertyInfo.flashQuadEnableBytes      = 1;
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_100MHz;
#if !MFB_FLASH_USE_DEFAULT_DUMMY
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_133MHz;
        g_flashPropertyInfo.flashDummyValue           = GIGADEVICE_QUAD_FLASH_SET_DUMMY_CMD;
#endif
#elif GIGADEVICE_DEVICE_GD25LExxxD | GIGADEVICE_DEVICE_GD25LQxxxC
        g_flashPropertyInfo.mixspiReadSampleClock     = kMIXSPI_SampClkLoopbackDqs;
        g_flashPropertyInfo.flashQuadEnableCfg        = GIGADEVICE_FLASH_QUAD_ENABLE;
        g_flashPropertyInfo.flashQuadEnableBytes      = 2;
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_120MHz;
#elif GIGADEVICE_DEVICE_GD25LBxxxE
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_133MHz;
        g_flashPropertyInfo.mixspiReadSampleClock     = kMIXSPI_SampClkLoopbackDqs;
#if !MFB_FLASH_USE_DEFAULT_DUMMY
        g_flashPropertyInfo.flashDummyValue           = GIGADEVICE_QUAD_FLASH_SET_DUMMY_CMD;
#endif
#elif GIGADEVICE_DEVICE_GD25LTxxxE
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_166MHz;
        g_flashPropertyInfo.mixspiReadSampleClock     = kMIXSPI_SampClkExtInputDqs;
#if !MFB_FLASH_USE_DEFAULT_DUMMY
        g_flashPropertyInfo.flashDummyValue           = GIGADEVICE_QUAD_FLASH_SET_DUMMY_CMD;
#endif
        //g_flashPropertyInfo.flashQuadEnableCfg        = GIGADEVICE_FLASH_QUAD_ENABLE;
        /* No need to enable quad mode for this device. */
#endif

    }
#endif
#if GIGADEVICE_DEVICE_OCTAL
    if (g_flashPropertyInfo.flashIsOctal)
    {
        g_flashPropertyInfo.mixspiPad                 = kMIXSPI_8PAD;
        g_flashPropertyInfo.flashBusyStatusPol        = GIGADEVICE_FLASH_BUSY_STATUS_POL;
        g_flashPropertyInfo.flashBusyStatusOffset     = GIGADEVICE_FLASH_BUSY_STATUS_OFFSET;
        g_flashPropertyInfo.flashEnableOctalCmd       = GIGADEVICE_OCTAL_FLASH_ENABLE_DDR_CMD;
        g_flashPropertyInfo.mixspiCustomLUTVendor     = s_customLUT_GIGADEVICE_Octal;
#if MFB_FLASH_OPI_MODE_DISABLE
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_30MHz;
        g_flashPropertyInfo.mixspiReadSampleClock     = kMIXSPI_SampClkLoopbackDqs;
#else
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_332MHz;
        g_flashPropertyInfo.mixspiReadSampleClock     = kMIXSPI_SampClkExtInputDqs;
#endif
        g_flashPropertyInfo.flashDummyValue           = GIGADEVICE_OCTAL_FLASH_SET_DUMMY_CMD;
    }
#endif
}

void mfb_flash_show_registers_for_gigadevice(bool isOctalFlash)
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

#endif // GIGADEVICE_DEVICE_SERIES
