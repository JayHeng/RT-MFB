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



/*******************************************************************************
 * Code
 ******************************************************************************/
void mixspi_device_config_init(void)
{

}

void mixspi_device_config_update_rootclock(uint32_t clkFreq)
{

}

void mixspi_device_config_update_flashsize(uint32_t flashSize)
{

}

void mixspi_nor_disable_cache(xspi_cache_status_t *cacheStatus)
{

}

void mixspi_nor_enable_cache(xspi_cache_status_t cacheStatus)
{

}

status_t mixspi_nor_write_enable(XSPI_Type *base, uint32_t baseAddr, flash_inst_mode_t flashInstMode)
{
    return kStatus_Success;
}

status_t mixspi_nor_wait_bus_busy(XSPI_Type *base, flash_inst_mode_t flashInstMode)
{
    return kStatus_Success;
}

status_t mixspi_nor_enable_qpi_mode(XSPI_Type *base)
{
    return kStatus_Success;
}

static status_t mixspi_nor_write_register(XSPI_Type *base, flash_reg_access_t *regAccess)
{
    return kStatus_Success;
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
    return kStatus_Success;
}

status_t mixspi_nor_flash_erase_sector(XSPI_Type *base, uint32_t address, flash_inst_mode_t flashInstMode)
{
    return kStatus_Success;
}

status_t mixspi_nor_flash_page_program(XSPI_Type *base, uint32_t address, const uint32_t *src, uint32_t length, flash_inst_mode_t flashInstMode)
{
    return kStatus_Success;
}

#if defined(__ICCARM__)
#pragma optimize = none
#endif
status_t mixspi_nor_get_jedec_id(XSPI_Type *base, uint32_t *jedecId, flash_inst_mode_t flashInstMode)
{
    return kStatus_Success;
}

#if defined(__ICCARM__)
#pragma optimize = none
#endif
status_t mixspi_nor_get_jedec_sfdp(XSPI_Type *base, uint32_t addr, uint32_t *jedecSfdp, uint32_t sfdpSize)
{
    return kStatus_Success;
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

}
