/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_DEFINE_H_
#define _MFB_DEFINE_H_

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MFB_MIXSPI_MODULE_IS_FLEXSPI 0U
#define MFB_MIXSPI_MODULE_IS_QUADSPI 1U

// MFB cfg block tag
#define MFB_CFG_BLK_TAG                  (0x4246U)   // Ascii "FB" Big Endian
// MFB cfg block version
#define MFB_CFG_BLK_VERSION              (0x0100U)   // V1.0
// MFB cfg block struct
typedef struct _mfb_cfg
{
    uint16_t tag;
    uint16_t version;
    union
    {
        struct
        {
            uint32_t enMultiIO : 1;      // 0 - Single I/O,   1 - Quad/Octal I/O
            uint32_t enDtr : 1;          // 0 - SDR,          1 - DTR
            uint32_t en4ByteAddr : 1;    // 0 - 3Byte Addr,   1 - 4Byte Addr
            uint32_t enQpiOpi : 1;       // 0 - Std/Ext SPI,  1 - QPI/OPI
            uint32_t enPerfXip : 1;      // 0 - Normal Read,  1 - Performance enhance mode (Continuous mode)
            uint32_t reserved0 : 3;
            uint32_t freq : 4;           // 0 - Highest, 1 - 30MHz,  2 - 50MHz,  3 - 60MHz,  
                                         // 4 - 80MHz,   5 - 100MHz, 6 - 120MHz, 7 - 133MHz,
                                         // 8 - 166MHz,  9 - 200MHz
            uint32_t reserved1 : 20;
        } B;
        uint32_t U;
    } cfgValue;
} mfb_cfg_t;
// Whether to get cfg block from app
#define MFB_APP_CFG_ENABLE               (0)
// The app image offset in flash device
#define MFB_APP_CFG_OFFSET               (0x20)

// Supported Mixspi clock defn
typedef enum _mixspi_root_clk_freq
{
    kMixspiRootClkFreq_30MHz  = 1,
    kMixspiRootClkFreq_50MHz  = 2,
    kMixspiRootClkFreq_60MHz  = 3,
    kMixspiRootClkFreq_80MHz  = 4,
    kMixspiRootClkFreq_100MHz = 5,
    kMixspiRootClkFreq_120MHz = 6,
    kMixspiRootClkFreq_133MHz = 7,
    kMixspiRootClkFreq_166MHz = 8,
    kMixspiRootClkFreq_200MHz = 9,
    kMixspiRootClkFreq_240MHz = 10,
    kMixspiRootClkFreq_266MHz = 11,
    kMixspiRootClkFreq_332MHz = 12,
    kMixspiRootClkFreq_400MHz = 13,
} mixspi_root_clk_freq_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

int mfb_printf(const char *fmt_s, ...);

#endif /* _MFB_DEFINE_H_ */
