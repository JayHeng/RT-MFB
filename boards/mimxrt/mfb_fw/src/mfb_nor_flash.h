/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_H_
#define _MFB_NOR_FLASH_H_


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DUMMY_VALUE_INVALID               (0xdeadbeefu)

// Flash property info for operation
typedef struct _flash_property_info
{
    uint8_t flashBusyStatusPol;
    uint8_t flashBusyStatusOffset;
    uint16_t flashQuadEnableCfg;
    uint8_t flashEnableOctalCmd;
    uint8_t reserved[3];
} flash_property_info_t;

// Flash status/cfg register r/w access helper
typedef struct _flash_reg_access
{
    uint8_t regNum;
    uint8_t regSeqIdx;
    uint8_t reserved[2];
    uint32_t regAddr;
    union
    {
        struct
        {
            uint32_t reg1 : 8;
            uint32_t reg2 : 8;
            uint32_t reg3 : 8;
            uint32_t reg4 : 8;
        } B;
        uint32_t U;
    } regValue;
} flash_reg_access_t;

// Supported Flexspi clock defn
typedef enum _flexspi_root_clk_freq
{
    kFlexspiRootClkFreq_30MHz  = 1,
    kFlexspiRootClkFreq_50MHz  = 2,
    kFlexspiRootClkFreq_60MHz  = 3,
    kFlexspiRootClkFreq_80MHz  = 4,
    kFlexspiRootClkFreq_100MHz = 5,
    kFlexspiRootClkFreq_120MHz = 6,
    kFlexspiRootClkFreq_133MHz = 7,
    kFlexspiRootClkFreq_166MHz = 8,
    kFlexspiRootClkFreq_200MHz = 9,
} flexspi_root_clk_freq_t;

// FlexSPI LUT seq defn
#define NOR_CMD_LUT_SEQ_IDX_READ            0
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS      1
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI  2
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE     3
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI 4
#define NOR_CMD_LUT_SEQ_IDX_READID          5

#define NOR_CMD_LUT_SEQ_IDX_ENABLEQE        6
#define NOR_CMD_LUT_SEQ_IDX_ENTERQPI        7
#define NOR_CMD_LUT_SEQ_IDX_ENTEROPI        8
#define NOR_CMD_LUT_SEQ_IDX_SETDUMMY        10

#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR     11
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM     12
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_OPI 13
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI 14
    
#define NOR_CMD_LUT_SEQ_IDX_READREG         15
#define NOR_CMD_LUT_SEQ_IDX_READREG2        5

/* NOTE: Workaround for debugger.
   Must define AHB write FlexSPI sequence index to 9 to avoid debugger issue.
   Debugger can attach to the CM33 core only when ROM executes to certain place.
   At that point, AHB write FlexSPI sequence index is set to 9, but in LUT, the
   command is not filled by ROM. If the debugger sets software breakpoint at flash
   after reset/attachment, FlexSPI AHB write command will be triggered. It may
   cause AHB bus hang if the command in LUT sequence index 9 is any read opeartion.
   So we need to ensure at any time, the FlexSPI LUT sequence 9 for the flash must
   be set to STOP command to avoid unexpected debugger behaivor.
 */
#define NOR_CMD_LUT_SEQ_IDX_WRITE           9
#define CUSTOM_LUT_LENGTH                   64

////////////////////////////////////////////////////////////////////////////////
#define MXIC_DEVICE_SERIES          (1)
#define MXIC_DEVICE_VENDOR_ID       (0xC2)
#define MXIC_DEVICE_QUAD            (0)
#define MXIC_DEVICE_MX25L12865      (0)
#define MXIC_DEVICE_OCTAL           (1)
#define MXIC_DEVICE_MX25UM51345     (1)  // MIMXRT595-EVK
////////////////////////////////////////////////////////////////////////////////
#define ISSI_DEVICE_SERIES          (1)
#define ISSI_DEVICE_VENDOR_ID       (0x9D)
#define ISSI_DEVICE_QUAD            (1)
#define ISSI_DEVICE_IS25LP064A      (0)  // MIMXRT1020-EVK
#define ISSI_DEVICE_IS25WP064A      (1)  // MIMXRT1060-EVK, MIMXRT1170-EVK/PMIC
#define ISSI_DEVICE_OCTAL           (1)
#define ISSI_DEVICE_IS25WX256       (1)  // MIMXRT1180-MEM-BB&DS
////////////////////////////////////////////////////////////////////////////////
#define WINBOND_DEVICE_SERIES       (1)
#define WINBOND_DEVICE_VENDOR_ID    (0xEF)
#define WINBOND_DEVICE_QUAD         (1)
#define WINBOND_DEVICE_W25Q128JW    (1)  // MIMXRT1180-EVK
////////////////////////////////////////////////////////////////////////////////
#define MICRON_DEVICE_SERIES        (1)
#define MICRON_DEVICE_VENDOR_ID     (0x2C)
#define MICRON_DEVICE_QUAD          (1)
#define MICRON_DEVICE_MT25QL256     (1)  // MIMXRT1170-EVKB(Rework)
#define MICRON_DEVICE_OCTAL         (1)
#define MICRON_DEVICE_MT35XU512     (1)  // MIMXRT1180-EVK
////////////////////////////////////////////////////////////////////////////////
#define GIGADEVICE_DEVICE_SERIE     (1)
#define GIGADEVICE_DEVICE_VENDOR_ID (0xC8)
#define GIGADEVICE_DEVICE_Quad      (1)
#define GIGADEVICE_DEVICE_GD25LE128 (1)  // MIMXRT1180-MEM-BB&DS
#define GIGADEVICE_DEVICE_OCTAL     (1)
#define GIGADEVICE_DEVICE_GD25LX256 (1)  // MIMXRT1180-MEM-BB&DS
////////////////////////////////////////////////////////////////////////////////
#define ADESTO_DEVICE_SERIE         (1)  // Adesto is acquired by Dialog in 2020, Dialog is acquired by Renesas in 2021
#define ADESTO_DEVICE_VENDOR_ID     (0x1F)
#define DIALOG_DEVICE_VENDOR_ID     (0x1F)
#define RENESAS_DEVICE_VENDOR_ID    (0x1F)
#define ADESTO_DEVICE_QUAD          (1)
#define ADESTO_DEVICE_AT25SF128A    (1)  // MIMXRT1010-EVK, MIMXRT1015-EVK
////////////////////////////////////////////////////////////////////////////////
#define SPANSION_DEVICE_SERIE       (0)  // Spansion is acquired by Cypress in 2014, Cypress is acquired by Infineon in 2019
#define SPANSION_DEVICE_VENDOR_ID   (0x01)
#define CYPRESS_DEVICE_VENDOR_ID    (0x01)
#define INFINEON_DEVICE_VENDOR_ID   (0x34)
#define SPANSION_DEVICE_QUAD        (0)
#define SPANSION_DEVICE_S25FL128    (0)
#define SPANSION_DEVICE_Octal       (0)
#define SPANSION_DEVICE_S28HS512    (0)
//#define SPANSION_DEVICE_S26KS512    (0)  // MIMXRT1050-EVKB

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_H_ */
