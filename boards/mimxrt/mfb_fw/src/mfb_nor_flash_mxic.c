/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mfb_nor_flash_mxic.h"
#if MXIC_DEVICE_SERIES
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if MXIC_DEVICE_MX25L6433F | MXIC_DEVICE_MX25U6432F
const uint32_t s_customLUT_MXIC_Quad[CUSTOM_LUT_LENGTH] = {
#if !MFB_FLASH_QPI_MODE_ENABLE
    /* Fast read quad mode - SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0xEB, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_MODE8_SDR, kFLEXSPI_4PAD, 0x00, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_4PAD, 0x04),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 2] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR,  kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),
#else
    /* Fast read quad mode - QPI-SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_4PAD, 0xEB, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_MODE8_SDR, kFLEXSPI_4PAD, 0x00, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_4PAD, 0x04),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 2] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR,  kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),
#endif

    /* Read status register -SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_1PAD, 0x01),

    /* Write Enable -SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Erase Sector -SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x20, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),

    /* Page Program - single mode */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x02, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Enable Quad mode */
    // QE bit in 8bit Status Register[6], there is only one Status Register
    [4 * NOR_CMD_LUT_SEQ_IDX_ENABLEQE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x01, kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x01),

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* Enter QPI mode */
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTERQPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x35, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

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

#if MXIC_DEVICE_MX25UM51345
const uint32_t s_customLUT_MXIC_Octal[CUSTOM_LUT_LENGTH] = {
    /* OPI DDR read */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 0] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0xEE, kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x11),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 0x08),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR,  kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Read status register -SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR,  kFLEXSPI_1PAD, 0x01),

    /* Write Enable - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Set Dummy cycle */
    // Configuration Register 2 address 00000300h - Dummy cycle configuration
    //   - Default dummy cycle for OPI DDR is 20, max freq is 200MHz(BGA24)/133MHz(SOP16)
    //   - To support freq 200MHz, min dummy cycle is 18
    [4 * NOR_CMD_LUT_SEQ_IDX_SETDUMMY] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x72, kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_SETDUMMY + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x00, kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x03),
    [4 * NOR_CMD_LUT_SEQ_IDX_SETDUMMY + 2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x00, kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x01),

    /* Erase Sector - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x21, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x20),

    /* 4-BYTE Page Program - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x12, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x20),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Dummy write, do nothing when AHB write command is triggered. */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* Enter OPI mode */
    // Configuration Register 2 address 00000000h - I/O mode
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTEROPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x72, kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTEROPI + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x00, kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTEROPI + 2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kFLEXSPI_1PAD, 0x00, kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x01), 

    /* Read status register using Octal DDR read */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x05, kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0xFA),
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI + 1] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_8PAD, 0x08),
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI + 2] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR,  kFLEXSPI_8PAD, 0x01, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* Write Enable - OPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x06, kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0xF9),

    /* Erase Sector - OPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_OPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x21, kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0xDE),
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_OPI + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_STOP,      kFLEXSPI_1PAD, 0x00),

    /* 4-BYTE Page Program - OPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0x12, kFLEXSPI_Command_DDR,       kFLEXSPI_8PAD, 0xED),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI + 1] = 
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04),
};
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

void mfb_flash_set_param_for_mxic(jedec_id_t *jedecID)
{
    mfb_printf(" -- MXIC Serial Flash.\r\n");
    mfb_printf("MFB: Flash Memory Type ID: 0x%x", jedecID->memoryTypeID);
    switch (jedecID->memoryTypeID)
    {
        /////////////////////////QuadSPI////////////////////////
        case 0x20:
            mfb_printf(" -- MX25L/MX66L/MX25V QuadSPI 3.3V Series.\r\n");
            break;
        case 0x25:
            mfb_printf(" -- MX25U/MX66U QuadSPI 1.8V Series.\r\n");
            g_flashPropertyInfo.flashHasQpiSupport = true;
            break;
        case 0x28:
            mfb_printf(" -- MX25R QuadSPI 1.8-3.3V Series.\r\n");
            break;
        case 0x75:
            mfb_printf(" -- MX77L QuadSPI 3.3V Series.\r\n");
            break;
        ////////////////////////OctalSPI////////////////////////
        case 0x80:
            g_flashPropertyInfo.flashIsOctal = true;
            mfb_printf(" -- MX25UM/MX66UM OctalSPI 1.8V Series.\r\n");
            break;
        case 0x81:
            g_flashPropertyInfo.flashIsOctal = true;
            mfb_printf(" -- MX25UM51345G OctalSPI 1.8V Series.\r\n");
            break;
        case 0x83:
            g_flashPropertyInfo.flashIsOctal = true;
            mfb_printf(" -- MX25UM25345G OctalSPI 1.8V Series.\r\n");
            break;
        case 0x84:
            g_flashPropertyInfo.flashIsOctal = true;
            mfb_printf(" -- MX25UW51345G OctalSPI 1.8V Series.\r\n");
            break;
        case 0x85:
            g_flashPropertyInfo.flashIsOctal = true;
            mfb_printf(" -- MX25LM/MX66LM OctalSPI 3.3V Series.\r\n");
            break;
        // Missing MX25LW51245G, MX66LW1G45G, MX66LW2G45G
        // Missing MX25UW6445G, MX66UW12845G, MX25UW25645G, MX25UW51245G, MX66UW1G45G, MX66UW2G45G
        // Missing MX25UW6345G, MX66LW12345G, MX66UW25345G      done                 , MX66UW2G345G
        default:
            mfb_printf(" -- Unsupported Series.\r\n");
            break;
    }
    mfb_flash_show_mem_size(jedecID->capacityID, false);
#if MXIC_DEVICE_QUAD
    if (!g_flashPropertyInfo.flashIsOctal)
    {
        g_flashPropertyInfo.flexspiPad                 = kFLEXSPI_4PAD;
        g_flashPropertyInfo.flexspiRootClkFreq         = kFlexspiRootClkFreq_80MHz;
        g_flashPropertyInfo.flexspiReadSampleClock     = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
        g_flashPropertyInfo.flashBusyStatusPol         = MXIC_FLASH_BUSY_STATUS_POL;
        g_flashPropertyInfo.flashBusyStatusOffset      = MXIC_FLASH_BUSY_STATUS_OFFSET;
        g_flashPropertyInfo.flashQuadEnableCfg         = MXIC_FLASH_QUAD_ENABLE;
        g_flashPropertyInfo.flashQuadEnableBytes       = 1;
        g_flashPropertyInfo.flexspiCustomLUTVendor     = s_customLUT_MXIC_Quad;
    }
#endif
#if MXIC_DEVICE_OCTAL
    if (g_flashPropertyInfo.flashIsOctal)
    {
        g_flashPropertyInfo.flexspiPad                 = kFLEXSPI_8PAD;
        g_flashPropertyInfo.flexspiRootClkFreq         = kFlexspiRootClkFreq_166MHz;
        g_flashPropertyInfo.flexspiReadSampleClock     = kFLEXSPI_ReadSampleClkExternalInputFromDqsPad;
        g_flashPropertyInfo.flashBusyStatusPol         = MXIC_FLASH_BUSY_STATUS_POL;
        g_flashPropertyInfo.flashBusyStatusOffset      = MXIC_FLASH_BUSY_STATUS_OFFSET;
        g_flashPropertyInfo.flashEnableOctalCmd        = MXIC_OCTAL_FLASH_ENABLE_DDR_CMD;
        g_flashPropertyInfo.flexspiCustomLUTVendor     = s_customLUT_MXIC_Octal;
        if (g_flashPropertyInfo.flexspiRootClkFreq == kFlexspiRootClkFreq_200MHz)
        {
            g_flashPropertyInfo.flashDummyValue = MXIC_OCTAL_FLASH_SET_DUMMY_CMD;
        }
    }
#endif
}
#endif // MXIC_DEVICE_SERIES

