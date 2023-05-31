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

extern flexspi_device_config_t g_deviceconfig;

/*******************************************************************************
 * Code
 ******************************************************************************/
void flexspi_nor_disable_cache(flexspi_cache_status_t *cacheStatus)
{
#if (defined __CORTEX_M) && (__CORTEX_M == 7U)
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    /* Disable D cache. */
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        SCB_DisableDCache();
        cacheStatus->DCacheEnableFlag = true;
    }
#endif /* __DCACHE_PRESENT */

#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    /* Disable I cache. */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
        cacheStatus->ICacheEnableFlag = true;
    }
#endif /* __ICACHE_PRESENT */

#elif (defined FSL_FEATURE_SOC_LMEM_COUNT) && (FSL_FEATURE_SOC_LMEM_COUNT != 0U)
    /* Disable code bus cache and system bus cache */
    if (LMEM_PCCCR_ENCACHE_MASK == (LMEM_PCCCR_ENCACHE_MASK & LMEM->PCCCR))
    {
        L1CACHE_DisableCodeCache();
        cacheStatus->codeCacheEnableFlag = true;
    }
    if (LMEM_PSCCR_ENCACHE_MASK == (LMEM_PSCCR_ENCACHE_MASK & LMEM->PSCCR))
    {
        L1CACHE_DisableSystemCache();
        cacheStatus->systemCacheEnableFlag = true;
    }

#elif (defined FSL_FEATURE_SOC_CACHE64_CTRL_COUNT) && (FSL_FEATURE_SOC_CACHE64_CTRL_COUNT != 0U)
    /* Disable cache */
    CACHE64_DisableCache(EXAMPLE_CACHE);
    cacheStatus->CacheEnableFlag = true;
#endif
}

void flexspi_nor_enable_cache(flexspi_cache_status_t cacheStatus)
{
#if (defined __CORTEX_M) && (__CORTEX_M == 7U)
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    if (cacheStatus.DCacheEnableFlag)
    {
        /* Enable D cache. */
        SCB_EnableDCache();
    }
#endif /* __DCACHE_PRESENT */

#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    if (cacheStatus.ICacheEnableFlag)
    {
        /* Enable I cache. */
        SCB_EnableICache();
    }
#endif /* __ICACHE_PRESENT */

#elif (defined FSL_FEATURE_SOC_LMEM_COUNT) && (FSL_FEATURE_SOC_LMEM_COUNT != 0U)
    if (cacheStatus.codeCacheEnableFlag)
    {
        /* Enable code cache. */
        L1CACHE_EnableCodeCache();
    }

    if (cacheStatus.systemCacheEnableFlag)
    {
        /* Enable system cache. */
        L1CACHE_EnableSystemCache();
    }
#elif (defined FSL_FEATURE_SOC_CACHE64_CTRL_COUNT) && (FSL_FEATURE_SOC_CACHE64_CTRL_COUNT != 0U)
    if (cacheStatus.CacheEnableFlag)
    {
        /* Enable cache. */
        CACHE64_EnableCache(EXAMPLE_CACHE);
    }
#endif
}

status_t flexspi_nor_write_enable(FLEXSPI_Type *base, uint32_t baseAddr, flash_inst_mode_t flashInstMode)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write enable */
    flashXfer.deviceAddress = baseAddr;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
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
        default:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE;
            break;
    }

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

status_t flexspi_nor_wait_bus_busy(FLEXSPI_Type *base, flash_inst_mode_t flashInstMode)
{
    /* Wait status ready. */
    bool isBusy;
    uint32_t readValue;
    status_t status;
    flexspi_transfer_t flashXfer;

    flashXfer.deviceAddress = 0;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    switch (flashInstMode)
    {
        case kFlashInstMode_QPI_1:
        case kFlashInstMode_QPI_2:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_READSTATUS_QPI;
            break;

        case kFlashInstMode_OPI:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI;
            break;

        case kFlashInstMode_SPI:
        default:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_READSTATUS;
            break;
    }
    flashXfer.data     = &readValue;
    flashXfer.dataSize = 1;

    do
    {
        status = FLEXSPI_TransferBlocking(base, &flashXfer);

        if (status != kStatus_Success)
        {
            return status;
        }
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

    } while (isBusy);

    return status;
}

status_t flexspi_nor_enable_qpi_mode(FLEXSPI_Type *base)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write enable */
    flashXfer.deviceAddress = 0;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_ENTERQPI;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

static status_t flexspi_nor_write_register(FLEXSPI_Type *base, flash_reg_access_t *regAccess)
{
    flexspi_transfer_t flashXfer;
    status_t status;
    
    if (!regAccess->regNum)
    {
        return kStatus_Success;
    }

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    flexspi_cache_status_t cacheStatus;
    flexspi_nor_disable_cache(&cacheStatus);
#endif

    uint32_t writeValue = regAccess->regValue.U;

    /* Write enable */
    status = flexspi_nor_write_enable(base, 0, kFlashInstMode_SPI);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress = 0;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = regAccess->regSeqIdx;
    flashXfer.data          = &writeValue;
    flashXfer.dataSize      = regAccess->regNum;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    if ((regAccess->regSeqIdx == NOR_CMD_LUT_SEQ_IDX_SETDUMMY) || \
        (regAccess->regSeqIdx == NOR_CMD_LUT_SEQ_IDX_ENABLEQE))
    {
        status = flexspi_nor_wait_bus_busy(base, kFlashInstMode_SPI);
        //flexspi_sw_delay_us(100UL);
    }
    else if (regAccess->regSeqIdx == NOR_CMD_LUT_SEQ_IDX_ENTEROPI)
    {
        status = flexspi_nor_wait_bus_busy(base, kFlashInstMode_OPI);
        //flexspi_sw_delay_us(100UL);
    }

    /* Do software reset. */
    FLEXSPI_SoftwareReset(base);

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    flexspi_nor_enable_cache(cacheStatus);
#endif

    return status;
}

status_t flexspi_nor_set_dummy_cycle(FLEXSPI_Type *base, uint8_t dummyCmd)
{
    flash_reg_access_t regAccess;
    regAccess.regNum = 1;
    regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_SETDUMMY;
    regAccess.regValue.U = dummyCmd;
    return flexspi_nor_write_register(base, &regAccess);
}

status_t flexspi_nor_enable_quad_mode(FLEXSPI_Type *base)
{
    flash_reg_access_t regAccess;
    regAccess.regNum = g_flashPropertyInfo.flashQuadEnableBytes;
    regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_ENABLEQE;
    regAccess.regValue.U = g_flashPropertyInfo.flashQuadEnableCfg;
    return flexspi_nor_write_register(base, &regAccess);
}

status_t flexspi_nor_enable_opi_mode(FLEXSPI_Type *base)
{
    flash_reg_access_t regAccess;
    regAccess.regNum = 1;
    regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_ENTEROPI;
    regAccess.regValue.U = g_flashPropertyInfo.flashEnableOctalCmd;
    return flexspi_nor_write_register(base, &regAccess);
}

status_t flexspi_nor_read_register(FLEXSPI_Type *base, flash_reg_access_t *regAccess)
{
    uint32_t regVal = 0;
    flexspi_transfer_t flashXfer;
    flashXfer.deviceAddress = regAccess->regAddr;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = regAccess->regSeqIdx;
    flashXfer.data          = &regVal;
    flashXfer.dataSize      = regAccess->regNum;

    status_t status = FLEXSPI_TransferBlocking(base, &flashXfer);

    /* Do software reset or clear AHB buffer directly. */
#if defined(FSL_FEATURE_SOC_OTFAD_COUNT) && defined(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK) && \
    defined(FLEXSPI_AHBCR_CLRAHBTXBUF_MASK)
    base->AHBCR |= FLEXSPI_AHBCR_CLRAHBRXBUF_MASK | FLEXSPI_AHBCR_CLRAHBTXBUF_MASK;
    base->AHBCR &= ~(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK | FLEXSPI_AHBCR_CLRAHBTXBUF_MASK);
#else
    FLEXSPI_SoftwareReset(base);
#endif
    
    regAccess->regValue.U = regVal;

    return status;
}

status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address, flash_inst_mode_t flashInstMode)
{
    status_t status;
    flexspi_transfer_t flashXfer;

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    flexspi_cache_status_t cacheStatus;
    flexspi_nor_disable_cache(&cacheStatus);
#endif

    /* Write enable */
    status = flexspi_nor_write_enable(base, address, flashInstMode);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress = address;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
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
        default:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR;
            break;
    }

    status                  = FLEXSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base, flashInstMode);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(base);

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    flexspi_nor_enable_cache(cacheStatus);
#endif

    return status;
}

status_t flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t address, const uint32_t *src, uint32_t length, flash_inst_mode_t flashInstMode)
{
    status_t status;
    flexspi_transfer_t flashXfer;

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    flexspi_cache_status_t cacheStatus;
    flexspi_nor_disable_cache(&cacheStatus);
#endif

    /* Write enable */
    status = flexspi_nor_write_enable(base, address, flashInstMode);

    if (status != kStatus_Success)
    {
        return status;
    }

    /* Prepare page program command */
    flashXfer.deviceAddress = address;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
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
        default:
            flashXfer.seqIndex  = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM;
            break;
    }
    flashXfer.data          = (uint32_t *)src;
    flashXfer.dataSize      = length;
    status                  = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base, flashInstMode);

    /* Do software reset or clear AHB buffer directly. */
#if defined(FSL_FEATURE_SOC_OTFAD_COUNT) && defined(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK) && \
    defined(FLEXSPI_AHBCR_CLRAHBTXBUF_MASK)
    base->AHBCR |= FLEXSPI_AHBCR_CLRAHBRXBUF_MASK | FLEXSPI_AHBCR_CLRAHBTXBUF_MASK;
    base->AHBCR &= ~(FLEXSPI_AHBCR_CLRAHBRXBUF_MASK | FLEXSPI_AHBCR_CLRAHBTXBUF_MASK);
#else
    FLEXSPI_SoftwareReset(base);
#endif

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    flexspi_nor_enable_cache(cacheStatus);
#endif

    return status;
}

#if defined(__ICCARM__)
#pragma optimize = none
#endif
status_t flexspi_nor_get_jedec_id(FLEXSPI_Type *base, uint32_t *jedecId, flash_inst_mode_t flashInstMode)
{
    uint32_t temp = 0;
    flexspi_transfer_t flashXfer;
    flashXfer.deviceAddress = 0;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
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
    flashXfer.data          = &temp;
    flashXfer.dataSize      = 3;

    status_t status = FLEXSPI_TransferBlocking(base, &flashXfer);

    *jedecId = temp;

    return status;
}

void flexspi_nor_flash_init(FLEXSPI_Type *base, const uint32_t *customLUT, flexspi_read_sample_clock_t rxSampleClock)
{
    flexspi_config_t config;

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    flexspi_cache_status_t cacheStatus;
    flexspi_nor_disable_cache(&cacheStatus);
#endif

    /*Get FLEXSPI default settings and configure the flexspi. */
    FLEXSPI_GetDefaultConfig(&config);

    /*Set AHB buffer size for reading data through AHB bus. */
    config.ahbConfig.enableAHBPrefetch = true;
    config.rxSampleClock               = rxSampleClock;
#if !(defined(FSL_FEATURE_FLEXSPI_HAS_NO_MCR0_COMBINATIONEN) && FSL_FEATURE_FLEXSPI_HAS_NO_MCR0_COMBINATIONEN)
    flexspi_port_t port = FLASH_PORT;
    if ((port == kFLEXSPI_PortA1) || (port == kFLEXSPI_PortA2))
    {
        config.enableCombination = true;
    }
    else
    {
        config.enableCombination = false;
    }
#endif
    config.ahbConfig.enableAHBBufferable = true;
    config.ahbConfig.enableAHBCachable   = true;
    FLEXSPI_Init(base, &config);

    /* Configure flash settings according to serial flash feature. */
    FLEXSPI_SetFlashConfig(base, &g_deviceconfig, FLASH_PORT);

    /* Update LUT table into a specific mode, such as octal SDR mode or octal DDR mode based on application's
     * requirement. */
    FLEXSPI_UpdateLUT(base, 0, customLUT, CUSTOM_LUT_LENGTH);

    /* Do software reset. */
    FLEXSPI_SoftwareReset(base);

#if defined(EXAMPLE_INVALIDATE_FLEXSPI_CACHE)
    EXAMPLE_INVALIDATE_FLEXSPI_CACHE();
#endif

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    flexspi_nor_enable_cache(cacheStatus);
#endif
}
