/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDXLicense-Identifier: BSD-3-Clause
 */
#include "flash_config.h"
#include "board.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.flash_config"
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined(BOOT_HEADER_ENABLE) && (BOOT_HEADER_ENABLE == 1)
#if defined(__ARMCC_VERSION) || defined(__GNUC__)
__attribute__((section(".flash_conf"), used))
#elif defined(__ICCARM__)
#pragma location = ".flash_conf"
#endif

const flexspi_nor_config_t flash_config = {
    .memConfig =
        {
            .tag              = FLEXSPI_CFG_BLK_TAG,
            .version          = FLEXSPI_CFG_BLK_VERSION,
            .readSampleClkSrc = kFlexSPIReadSampleClk_LoopbackInternally,
            .csHoldTime       = 3u,
            .csSetupTime      = 3u,
            .sflashPadType    = kSerialFlash_1Pad,
            .serialClkFreq    = kFlexSpiSerialClk_30MHz,
            .sflashA1Size     = 8u * 1024u * 1024u,
            .lookupTable =
                {
                    // Read LUTs
                    FLEXSPI_LUT_SEQ(CMD_SDR,  FLEXSPI_1PAD, 0x03, RADDR_SDR, FLEXSPI_1PAD, 0x18),
                    FLEXSPI_LUT_SEQ(READ_SDR, FLEXSPI_1PAD, 0x04, STOP_EXE,  FLEXSPI_1PAD, 0x00),
                },
        },
    .pageSize      = 256u,
    .sectorSize    = 4u * 1024u,
    .blockSize     = 64u * 1024u,
    //.flashStateCtx = 0x07008200u,
};
#endif /* BOOT_HEADER_ENABLE */
