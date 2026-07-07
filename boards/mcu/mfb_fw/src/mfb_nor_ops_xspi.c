/*
 * Copyright 2019-2021 NXP
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

static xspi_device_ddr_config_t s_flashDDrConfig = 
{
    .ddrDataAlignedClk = kXSPI_DDRDataAlignedWith2xInternalRefClk,
    .enableDdr = true,
    .enableByteSwapInOctalMode = false,
};

static xspi_device_config_t s_deviceconfig =
{
    .xspiRootClk = 27400000,
    .enableCknPad = false, 
    .deviceInterface = kXSPI_StrandardExtendedSPI,
    .interfaceSettings.strandardExtendedSPISettings.pageSize = EXAMPLE_FLASH_PAGE_SIZE,
    .CSHoldTime = 3,
    .CSSetupTime = 3,
    .sampleClkConfig.sampleClkSource = kXSPI_SampleClkFromDqsPadLoopback,
    .sampleClkConfig.enableDQSLatency = false,
    .sampleClkConfig.dllConfig.dllMode = kXSPI_AutoUpdateMode,
    .sampleClkConfig.dllConfig.useRefValue = true,
    .sampleClkConfig.dllConfig.enableCdl8 = true,
    .ptrDeviceDdrConfig = &s_flashDDrConfig,
    .addrMode = kXSPI_DeviceByteAddressable,
    .columnAddrWidth = 0U,
    .enableCASInterleaving = false,
    .deviceSize[0] = 0x4000, /* 128Mb/KByte */
    .deviceSize[1] = 0x4000, /* 128Mb/KByte */
    .ptrDeviceRegInfo = NULL,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
void mixspi_device_config_init(void)
{

}

void mixspi_device_config_update_rootclock(uint32_t clkFreq)
{
    s_deviceconfig.xspiRootClk = clkFreq;
}

void mixspi_device_config_update_flashsize(uint32_t flashSize)
{
    s_deviceconfig.deviceSize[0] = flashSize;
    s_deviceconfig.deviceSize[1] = flashSize;
}

void mixspi_nor_disable_cache(xspi_cache_status_t *cacheStatus)
{

}

void mixspi_nor_enable_cache(xspi_cache_status_t cacheStatus)
{

}

status_t mixspi_nor_write_enable(XSPI_Type *base, uint32_t baseAddr, flash_inst_mode_t flashInstMode)
{
    xspi_transfer_t flashXfer;
    status_t status;

    /* Write enable */
    flashXfer.deviceAddress   = EXAMPLE_MIXSPI_AMBA_BASE + baseAddr;
    flashXfer.cmdType         = kXSPI_Command;
    flashXfer.targetGroup     = EXAMPLE_MIXSPI_PORT;
    flashXfer.data            = NULL;
    flashXfer.dataSize        = 0UL;
    flashXfer.lockArbitration = false;
    switch (flashInstMode)
    {
        case kFlashInstMode_QPI_1:
        case kFlashInstMode_QPI_2:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_QPI;
            break;

        case kFlashInstMode_OPI:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI;
            break;

        case kFlashInstMode_SPI:
        case kFlashInstMode_Hyper:
        default:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE;
            break;
    }

    status = XSPI_TransferBlocking(base, &flashXfer);

    return status;
}

status_t mixspi_nor_wait_bus_busy(XSPI_Type *base, flash_inst_mode_t flashInstMode)
{
    /* Wait status ready. */
    bool isBusy;
    uint32_t readValue;
    status_t status;
    xspi_transfer_t flashXfer;

    flashXfer.deviceAddress = EXAMPLE_MIXSPI_AMBA_BASE;
    flashXfer.cmdType       = kXSPI_Read;
    flashXfer.data          = &readValue;
    flashXfer.dataSize      = 1;
    flashXfer.targetGroup   = EXAMPLE_MIXSPI_PORT;
    flashXfer.lockArbitration = false;
    switch (flashInstMode)
    {
        case kFlashInstMode_QPI_1:
        case kFlashInstMode_QPI_2:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_READSTATUS_QPI;
            break;

        case kFlashInstMode_OPI:
            flashXfer.dataSize  = 2;
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI;
            break;

        case kFlashInstMode_SPI:
        case kFlashInstMode_Hyper:
        default:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_READSTATUS;
            break;
    }

    do
    {
        status = XSPI_TransferBlocking(base, &flashXfer);

        if (status != kStatus_Success)
        {
            return status;
        }
        if (flashInstMode == kFlashInstMode_Hyper)
        {
            if (readValue & (1U << (g_flashPropertyInfo.flashBusyStatusOffset + 8)))
            {
                isBusy = false;
            }
            else
            {
                isBusy = true;
            }
            if (readValue & ((uint16_t)g_flashPropertyInfo.flashMixStatusMask << 8))
            {
                status = kStatus_Fail;
                break;
            }
        }
        else
        {
            if (g_flashPropertyInfo.flashBusyStatusPol)
            {
                if (readValue & (1U << g_flashPropertyInfo.flashBusyStatusOffset))
                {
                    isBusy = true;
                }
                else
                {
                    isBusy = false;
                }
            }
            else
            {
                if (readValue & (1U << g_flashPropertyInfo.flashBusyStatusOffset))
                {
                    isBusy = false;
                }
                else
                {
                    isBusy = true;
                }
            }
        }

    } while (isBusy);

    return status;
}

status_t mixspi_nor_enable_qpi_mode(XSPI_Type *base)
{
    xspi_transfer_t flashXfer;
    status_t status;

    flashXfer.deviceAddress   = EXAMPLE_MIXSPI_AMBA_BASE;
    flashXfer.cmdType         = kXSPI_Command;
    flashXfer.seqIndex        = NOR_CMD_LUT_SEQ_IDX_ENTERQPI;
    flashXfer.targetGroup     = EXAMPLE_MIXSPI_PORT;
    flashXfer.data            = NULL;
    flashXfer.dataSize        = 0UL;
    flashXfer.lockArbitration = false;

    status = XSPI_TransferBlocking(base, &flashXfer);

    return status;
}

status_t mixspi_nor_write_register(XSPI_Type *base, flash_reg_access_t *regAccess)
{
    xspi_transfer_t flashXfer;
    status_t status;
    
    if (!regAccess->regNum)
    {
        return kStatus_Success;
    }

    uint32_t writeValue = regAccess->regValue.U;

    /* Write enable */
    status = mixspi_nor_write_enable(base, 0, kFlashInstMode_SPI);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress   = EXAMPLE_MIXSPI_AMBA_BASE;
    flashXfer.cmdType         = kXSPI_Write;
    flashXfer.seqIndex        = regAccess->regSeqIdx;
    flashXfer.targetGroup     = EXAMPLE_MIXSPI_PORT;
    flashXfer.data            = &writeValue;
    flashXfer.dataSize        = regAccess->regNum;
    flashXfer.lockArbitration = false;

    status = XSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    if ((regAccess->regSeqIdx == NOR_CMD_LUT_SEQ_IDX_SETDUMMY) || \
        (regAccess->regSeqIdx == NOR_CMD_LUT_SEQ_IDX_ENABLEQE))
    {
        status = mixspi_nor_wait_bus_busy(base, kFlashInstMode_SPI);
        mixspi_sw_delay_us(100UL);
    }
    else if (regAccess->regSeqIdx == NOR_CMD_LUT_SEQ_IDX_ENTEROPI)
    {
        status = mixspi_nor_wait_bus_busy(base, kFlashInstMode_OPI);
        mixspi_sw_delay_us(100UL);
    }
    else if (regAccess->regSeqIdx == NOR_CMD_LUT_SEQ_IDX_UNIQUECFG)
    {
        // For Infineon MirrorBit device, typical delay time for CFR3N setting is 45ms
        mixspi_sw_delay_us(100000UL);
    }

    return status;
}

status_t mixspi_nor_set_dummy_cycle(XSPI_Type *base, uint8_t dummyCmd)
{
    flash_reg_access_t regAccess;
    regAccess.regNum = 1;
    regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_SETDUMMY;
    regAccess.regValue.U = dummyCmd;
    return mixspi_nor_write_register(base, &regAccess);
}

status_t mixspi_nor_set_drive_strength(XSPI_Type *base, uint8_t driveCmd)
{
    flash_reg_access_t regAccess;
    regAccess.regNum = 1;
    regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_SETDRIVE;
    regAccess.regValue.U = driveCmd;
    return mixspi_nor_write_register(base, &regAccess);
}

status_t mixspi_nor_set_unique_cfg(XSPI_Type *base, uint8_t driveCmd)
{
    flash_reg_access_t regAccess;
    regAccess.regNum = 1;
    regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_UNIQUECFG;
    regAccess.regValue.U = driveCmd;
    return mixspi_nor_write_register(base, &regAccess);
}

status_t mixspi_nor_enable_quad_mode(XSPI_Type *base)
{
    flash_reg_access_t regAccess;
    regAccess.regNum = g_flashPropertyInfo.flashQuadEnableBytes;
    regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_ENABLEQE;
    regAccess.regValue.U = g_flashPropertyInfo.flashQuadEnableCfg;
    return mixspi_nor_write_register(base, &regAccess);
}

status_t mixspi_nor_enable_opi_mode(XSPI_Type *base)
{
    flash_reg_access_t regAccess;
    regAccess.regNum = 1;
    regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_ENTEROPI;
    regAccess.regValue.U = g_flashPropertyInfo.flashEnableOctalCmd;
    return mixspi_nor_write_register(base, &regAccess);
}

status_t mixspi_nor_read_register(XSPI_Type *base, flash_reg_access_t *regAccess)
{
    uint32_t regVal = 0;
    xspi_transfer_t flashXfer;
    flashXfer.deviceAddress   = EXAMPLE_MIXSPI_AMBA_BASE;
    flashXfer.cmdType         = kXSPI_Read;
    flashXfer.seqIndex        = regAccess->regSeqIdx;
    flashXfer.targetGroup     = EXAMPLE_MIXSPI_PORT;
    flashXfer.data            = &regVal;
    flashXfer.dataSize        = regAccess->regNum;
    flashXfer.lockArbitration = false;

    status_t status = XSPI_TransferBlocking(base, &flashXfer);
    
    regAccess->regValue.U = regVal;

    return status;
}

status_t mixspi_nor_flash_erase_sector(XSPI_Type *base, uint32_t address, flash_inst_mode_t flashInstMode)
{
    status_t status;
    xspi_transfer_t flashXfer;

    /* Write enable */
    status = mixspi_nor_write_enable(base, address, flashInstMode);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress   = EXAMPLE_MIXSPI_AMBA_BASE + address;
    flashXfer.cmdType         = kXSPI_Command;
    flashXfer.targetGroup     = EXAMPLE_MIXSPI_PORT;
    flashXfer.lockArbitration = false;
    flashXfer.dataSize        = 0UL;
    flashXfer.data            = NULL;
    switch (flashInstMode)
    {
        case kFlashInstMode_QPI_1:
        case kFlashInstMode_QPI_2:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_QPI;
            break;

        case kFlashInstMode_OPI:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_OPI;
            break;

        case kFlashInstMode_SPI:
        case kFlashInstMode_Hyper:
        default:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR;
            break;
    }

    status = XSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = mixspi_nor_wait_bus_busy(base, flashInstMode);

    return status;
}

status_t mixspi_nor_flash_page_program(XSPI_Type *base, uint32_t address, const uint32_t *src, uint32_t length, flash_inst_mode_t flashInstMode)
{
    status_t status;
    xspi_transfer_t flashXfer;

    /* To make sure external flash be in idle status, added wait for busy before program data for
        an external flash without RWW(read while write) attribute.*/
    status = mixspi_nor_wait_bus_busy(base, flashInstMode);
    if (kStatus_Success != status)
    {
        return status;
    }

    /* Write enable. */
    status = mixspi_nor_write_enable(base, address, flashInstMode);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Prepare page program command */
    flashXfer.deviceAddress   = EXAMPLE_MIXSPI_AMBA_BASE + address;
    flashXfer.cmdType         = kXSPI_Write;
    flashXfer.targetGroup     = EXAMPLE_MIXSPI_PORT;
    flashXfer.data            = (uint32_t *)src;
    flashXfer.dataSize        = length;
    flashXfer.lockArbitration = false;
    switch (flashInstMode)
    {
        case kFlashInstMode_QPI_1:
        case kFlashInstMode_QPI_2:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QPI;
            break;

        case kFlashInstMode_OPI:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI;
            break;

        case kFlashInstMode_SPI:
        case kFlashInstMode_Hyper:
        default:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM;
            break;
    }

    status = XSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = mixspi_nor_wait_bus_busy(base, flashInstMode);

    return status;
}

#if defined(__ICCARM__)
#pragma optimize = none
#endif
status_t mixspi_nor_get_jedec_id(XSPI_Type *base, uint32_t *jedecId, flash_inst_mode_t flashInstMode)
{
    uint32_t temp;
    xspi_transfer_t flashXfer;
    flashXfer.deviceAddress = EXAMPLE_MIXSPI_AMBA_BASE;
    flashXfer.cmdType       = kXSPI_Read;
    flashXfer.targetGroup   = EXAMPLE_MIXSPI_PORT;
    flashXfer.data            = &temp;
    flashXfer.dataSize        = 3;
    flashXfer.lockArbitration = false;
    switch (flashInstMode)
    {
        case kFlashInstMode_QPI_1:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_READID_QPI_1;
            break;

        case kFlashInstMode_QPI_2:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_READID_QPI_2;
            break;

        case kFlashInstMode_OPI:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_READID_OPI;
            break;

        case kFlashInstMode_SPI:
        default:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_READID;
            break;
    }

    status_t status = XSPI_TransferBlocking(base, &flashXfer);

    *jedecId = temp;

    return status;
}

#if defined(__ICCARM__)
#pragma optimize = none
#endif
status_t mixspi_nor_get_infineon_samper_id(XSPI_Type *base, infineon_samper_id_t *samperId)
{
    xspi_transfer_t flashXfer;
    flashXfer.deviceAddress = EXAMPLE_MIXSPI_AMBA_BASE;
    flashXfer.cmdType       = kXSPI_Read;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_READID;
    flashXfer.targetGroup   = EXAMPLE_MIXSPI_PORT;
    flashXfer.data            = (uint32_t *)samperId;;
    flashXfer.dataSize        = sizeof(infineon_samper_id_t);
    flashXfer.lockArbitration = false;

    status_t status = XSPI_TransferBlocking(base, &flashXfer);

    return status;
}

#if defined(__ICCARM__)
#pragma optimize = none
#endif
status_t mixspi_nor_get_jedec_sfdp(XSPI_Type *base, uint32_t addr, uint32_t *jedecSfdp, uint32_t sfdpSize)
{
    xspi_transfer_t flashXfer;
    flashXfer.deviceAddress = EXAMPLE_MIXSPI_AMBA_BASE + addr;
    flashXfer.cmdType       = kXSPI_Read;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_READSFDP;
    flashXfer.targetGroup   = EXAMPLE_MIXSPI_PORT;
    flashXfer.data            = jedecSfdp;
    flashXfer.dataSize        = sfdpSize;
    flashXfer.lockArbitration = false;

    status_t status = XSPI_TransferBlocking(base, &flashXfer);

    return status;
}

status_t mixspi_nor_sfdp_sec_erase(XSPI_Type *base, uint32_t addr)
{
    return kStatus_Success;
}

status_t mixspi_nor_sfdp_sec_program(XSPI_Type *base, uint32_t addr, const uint32_t *src, uint32_t length)
{
    return kStatus_Success;
}

static status_t mixspi_nor_read_cfi(XSPI_Type *base, uint32_t addr, uint32_t *buffer, uint32_t bytes)
{
    return kStatus_Success;
}

static status_t mixspi_nor_write_cfi(XSPI_Type *base, uint32_t addr, uint32_t *buffer, uint32_t bytes)
{
    return kStatus_Success;
}

#if defined(__ICCARM__)
#pragma optimize = none
#endif
status_t mixspi_nor_get_cfi_id(XSPI_Type *base, cfi_device_id_t *cfiDeviceId)
{
    return kStatus_Success;
}

void mixspi_nor_flash_init(XSPI_Type *base, const uint32_t *customLUT, xspi_sample_clk_source_t rxSampleClock, flash_inst_mode_t flashInstMode)
{
    xspi_config_t config;
    xspi_ahb_access_config_t xspiAhbAccessConfig;
    xspi_ip_access_config_t xspiIpAccessConfig;

    /* To store custom's LUT table in local. */
    uint32_t tempLUT[CUSTOM_LUT_LENGTH] = {0x00U};

    /* Copy LUT information from flash region into RAM region, because LUT update maybe corrupt read sequence(LUT[0])
     * and load wrong LUT table from FLASH region. */
    memcpy(tempLUT, customLUT, sizeof(tempLUT));

    config.ptrAhbAccessConfig = &xspiAhbAccessConfig;
    config.ptrIpAccessConfig  = &xspiIpAccessConfig;
    XSPI_GetDefaultConfig(&config);

#if (defined(FSL_FEATURE_XSPI_HAS_END_CFG) && FSL_FEATURE_XSPI_HAS_END_CFG)
    config.byteOrder = kXSPI_64BitLE;
#endif
    xspiAhbAccessConfig.ahbErrorPayload.highPayload = 0x5A5A5A5AUL;
    xspiAhbAccessConfig.ahbErrorPayload.lowPayload  = 0x5A5A5A5AUL;
    xspiAhbAccessConfig.ptrAhbWriteConfig           = NULL;
    xspiAhbAccessConfig.ARDSeqIndex                 = NOR_CMD_LUT_SEQ_IDX_READ;
    xspiAhbAccessConfig.enableAHBBufferWriteFlush   = true;
    xspiAhbAccessConfig.enableAHBPrefetch           = true;

#if defined(ENABLE_SFP_CONFIG) && (ENABLE_SFP_CONFIG)
    extern xspi_sfp_mdad_config_t *pSfpMdadConfig;
    extern xspi_sfp_frad_config_t *pSfpFradConfig;
    config.ptrIpAccessConfig->ptrSfpFradConfig = pSfpFradConfig;
    config.ptrIpAccessConfig->ptrSfpMdadConfig = pSfpMdadConfig;
#else
    config.ptrIpAccessConfig->ptrSfpFradConfig = NULL; /* This demo does not demonstrate SFP feature.*/
    config.ptrIpAccessConfig->ptrSfpMdadConfig = NULL;
#endif
    config.ptrIpAccessConfig->ipAccessTimeoutValue           = 0xFFFFFFFFUL;
    config.ptrIpAccessConfig->sfpArbitrationLockTimeoutValue = 0xFFFFFFUL;
    XSPI_Init(base, &config);

    s_deviceconfig.sampleClkConfig.sampleClkSource = rxSampleClock;

    /* Configure flash settings according to serial flash feature. */
    XSPI_SetDeviceConfig(base, &s_deviceconfig);

    /*update LUT*/
    XSPI_UpdateLUT(base, 0, tempLUT, CUSTOM_LUT_LENGTH);
}
