/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_H_
#define _MFB_NOR_FLASH_H_

#include "mfb.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

// Supported NOR Flash protocol type
typedef enum _flash_protocol_type
{
    kFlashProtocolType_QSPI     = 0,
    kFlashProtocolType_XCCELA   = 2,
    kFlashProtocolType_HYPERBUS = 3,
    kFlashProtocolType_OctaBus  = 4,   // JESD251

    kFlashProtocolType_MAX      = 5,
} flash_protocol_type_t;

#define U32_VALUE_INVALID                 (0xdeadbeefu)
#define INVALID_JEDEC_ID_0                (0x00u)
#define INVALID_JEDEC_ID_1                (0xFFu)
   
/*! @brief JEDEC id structure. */
typedef struct _jedec_id
{
    uint8_t manufacturerID;
    uint8_t memoryTypeID;
    uint8_t capacityID;
    uint8_t reserved;
} jedec_id_t;

/*! @brief Infineon SEMPER HyperFlash id structure. */
typedef struct _infineon_samper_id
{
    uint16_t manufacturerID;
    uint16_t voltageType;
    uint16_t deviceDensity;
    uint16_t deviceIDLength;
    uint16_t familyID;
    uint16_t reserved;
} infineon_samper_id_t;

/*! @brief CFI device id structure. */
typedef struct _cfi_device_id
{
    uint8_t reserved0;
    uint8_t manufacturerID;
    uint8_t reserved1;
    uint8_t memoryTypeID;
    uint8_t reserved2;
    uint8_t capacityID;
    uint8_t reserved3;
    uint8_t voltageType;
} cfi_device_id_t;

// Supported Flash inst mode
typedef enum _flash_inst_mode
{
    kFlashInstMode_SPI    = 0,
    kFlashInstMode_QPI_1  = 1,
    kFlashInstMode_QPI_2  = 2,
    kFlashInstMode_OPI    = 3,
    kFlashInstMode_Hyper  = 4,

    kFlashInstMode_MAX    = 5,
} flash_inst_mode_t;

//!@brief SFDP related definitions
#define SFDP_SIGNATURE 0x50444653 /* ASCII: SFDP */
enum
{
    kSfdp_Version_Major_1_0 = 1,
    kSfdp_Version_Minor_0 = 0, // JESD216
    kSfdp_Version_Minor_A = 5, // JESD216A
    kSfdp_Version_Minor_B = 6, // JESD216B
    kSfdp_Version_Minor_C = 7, // JESD216C
    kSfdp_Version_Minor_D = 8, // JESD216D
    kSfdp_Version_Minor_E = 9, // JESD216E
    kSfdp_Version_Minor_F = 0xA, // JESD216F

    kSfdp_BasicProtocolTableSize_Rev0 = 36,
    kSfdp_BasicProtocolTableSize_RevA = 64,
    kSfdp_BasicProtocolTableSize_RevB = kSfdp_BasicProtocolTableSize_RevA,
    kSfdp_BasicProtocolTableSize_RevC = 80,
    kSfdp_BasicProtocolTableSize_RevD = 123,
};

typedef struct _sfdp_header
{
    uint32_t signature;
    uint8_t minor_rev;
    uint8_t major_rev;
    uint8_t param_hdr_num;
    uint8_t sfdp_access_protocol; // Defined in JESD216C, reserved for older version
} sfdp_header_t;

typedef struct _sfdp_table
{
    sfdp_header_t header;
    uint32_t tbd[kSfdp_BasicProtocolTableSize_RevD-2];
} sfdp_table_t;

// Flash property info for operation
typedef struct _flash_property_info
{
    mixspi_pad_t                mixspiPad;
    mixspi_root_clk_freq_t      mixspiRootClkFreq;
    mixspi_read_sample_clock_t  mixspiReadSampleClock;
    const uint32_t             *mixspiCustomLUTVendor;

    bool     flashIsOctal;
    bool     flashHasQpiSupport;
    uint32_t flashMemSizeInByte;
    uint8_t  flashBusyStatusPol;
    uint8_t  flashBusyStatusOffset;
    uint8_t  flashMixStatusMask;
    uint8_t  reserved;
    uint16_t flashQuadEnableCfg;
    uint8_t  flashQuadEnableBytes;
    uint8_t  flashEnableOctalCmd;
    uint32_t flashDummyValue;
    uint32_t flashDriveStrength;
    uint32_t flashUniqueCfg;
} flash_property_info_t;

// Flash cache config
typedef struct _flash_cache_config
{
    bool systemDcacheEnable;
    bool ipReadAhbPrefetch;
    bool ipReadAhbCachable;
} flash_cache_config_t;

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

// FlexSPI LUT seq defn (common)
#if !defined(NOR_CMD_LUT_SEQ_IDX_START)
#define NOR_CMD_LUT_SEQ_IDX_START           (0)
#endif
#define NOR_CMD_LUT_SEQ_IDX_READ            (NOR_CMD_LUT_SEQ_IDX_START+0)  // Used for HyperFlash
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR     (NOR_CMD_LUT_SEQ_IDX_START+1)  // Used for HyperFlash (x4)
#define NOR_CMD_LUT_SEQ_IDX_ENABLEQE        (NOR_CMD_LUT_SEQ_IDX_START+2)
#define NOR_CMD_LUT_SEQ_IDX_ENTERQPI        (NOR_CMD_LUT_SEQ_IDX_START+3)
#define NOR_CMD_LUT_SEQ_IDX_ENTEROPI        (NOR_CMD_LUT_SEQ_IDX_START+3)
#define NOR_CMD_LUT_SEQ_IDX_SETDUMMY        (NOR_CMD_LUT_SEQ_IDX_START+4)
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS      (NOR_CMD_LUT_SEQ_IDX_START+5)  // Used for HyperFlash (x2)
#define NOR_CMD_LUT_SEQ_IDX_SETDRIVE        (NOR_CMD_LUT_SEQ_IDX_START+6)
#define NOR_CMD_LUT_SEQ_IDX_UNIQUECFG       (NOR_CMD_LUT_SEQ_IDX_START+6)
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE     (NOR_CMD_LUT_SEQ_IDX_START+7)  // Used for HyperFlash (x2)
#define NOR_CMD_LUT_SEQ_IDX_READREG         (NOR_CMD_LUT_SEQ_IDX_START+8)
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM     (NOR_CMD_LUT_SEQ_IDX_START+10) // Used for HyperFlash (x2)
#define NOR_CMD_LUT_SEQ_IDX_READREG2        (NOR_CMD_LUT_SEQ_IDX_START+11)
// FlexSPI LUT seq defn (For HyperFlash 1bit spi mode)
#define NOR_CMD_LUT_SEQ_IDX_SWITCHHYPERBUS  (NOR_CMD_LUT_SEQ_IDX_START+2)
#define NOR_CMD_LUT_SEQ_IDX_READANYREG      (NOR_CMD_LUT_SEQ_IDX_START+4)
// FlexSPI LUT seq defn (1bit spi)
#define NOR_CMD_LUT_SEQ_IDX_ERASESECSFDP    (NOR_CMD_LUT_SEQ_IDX_START+6)
#define NOR_CMD_LUT_SEQ_IDX_WRITESECSFDP    (NOR_CMD_LUT_SEQ_IDX_START+8)
#define NOR_CMD_LUT_SEQ_IDX_READSFDP        (NOR_CMD_LUT_SEQ_IDX_START+11)
#define NOR_CMD_LUT_SEQ_IDX_READID          (NOR_CMD_LUT_SEQ_IDX_START+12)
#define NOR_CMD_LUT_SEQ_IDX_READID_QPI_1    (NOR_CMD_LUT_SEQ_IDX_START+13)
#define NOR_CMD_LUT_SEQ_IDX_READID_QPI_2    (NOR_CMD_LUT_SEQ_IDX_START+14)
#define NOR_CMD_LUT_SEQ_IDX_READID_OPI      (NOR_CMD_LUT_SEQ_IDX_START+15)
// FlexSPI LUT seq defn (quad lut)
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS_QPI  (NOR_CMD_LUT_SEQ_IDX_START+12)
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_QPI (NOR_CMD_LUT_SEQ_IDX_START+13)
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_QPI (NOR_CMD_LUT_SEQ_IDX_START+14)
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QPI (NOR_CMD_LUT_SEQ_IDX_START+15)

#define NOR_CMD_LUT_SEQ_IDX_READREG3        (NOR_CMD_LUT_SEQ_IDX_START+12)
#define NOR_CMD_LUT_SEQ_IDX_READREG4        (NOR_CMD_LUT_SEQ_IDX_START+13)
#define NOR_CMD_LUT_SEQ_IDX_READREG5        (NOR_CMD_LUT_SEQ_IDX_START+14)
#define NOR_CMD_LUT_SEQ_IDX_READREG6        (NOR_CMD_LUT_SEQ_IDX_START+15)
// FlexSPI LUT seq defn (octal lut)
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI  (NOR_CMD_LUT_SEQ_IDX_START+12)
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI (NOR_CMD_LUT_SEQ_IDX_START+13)
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_OPI (NOR_CMD_LUT_SEQ_IDX_START+14)
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI (NOR_CMD_LUT_SEQ_IDX_START+15)

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
#define NOR_CMD_LUT_SEQ_IDX_WRITE           (NOR_CMD_LUT_SEQ_IDX_START+9)   // Used for HyperFlash
#if defined(MIXSPI_LUT_SEQS)
#define CUSTOM_LUT_LENGTH                   (MIXSPI_LUT_SEQS * MIXSPI_LUT_SUB_SEQ_LEN)
#else
#define CUSTOM_LUT_LENGTH                   (16 * MIXSPI_LUT_SUB_SEQ_LEN)
#endif

////////////////////////////////////////////////////////////////////////////////
#define WINBOND_DEVICE_SERIES       (1)
#define WINBOND_DEVICE_VENDOR_ID    (0xEF)
#define WINBOND_DEVICE_QUAD         (1)
#define WINBOND_DEVICE_W25QxxxJW    (1)  // MIMXRT1180-EVK, MIMXRT1170-EVKB_Rev.A/B (W25Q128JW)
#define WINBOND_DEVICE_W25QxxxJV    (0)
#define WINBOND_DEVICE_W25QxxxCL    (0)  // Small package not supported on MIMXRT1180-MEM-BB&DS
#define WINBOND_DEVICE_W25QxxxNE    (0)
#define WINBOND_DEVICE_W25QxxxFW    (0)
#define WINBOND_DEVICE_W25QxxxEW    (0)
#define WINBOND_DEVICE_W25QxxxDV    (0)
#define WINBOND_DEVICE_W25QxxxNW    (0)  // MIMXRT1170-EVKB_Rev.Cx (W25Q512NW), MIMXRT1170-FSGPIO_Rev.A (W25Q512NW)
#define WINBOND_DEVICE_W25Q33PW     (0)  // RT1184-SIP
#define WINBOND_DEVICE_W25MxxxJW    (0)
#define WINBOND_DEVICE_W25HxxxJV    (0)
#define WINBOND_QUAD_FLASH_JEDEC_ID (0x003740EF)
#define WINBOND_DEVICE_OCTAL        (1)
#define WINBOND_DEVICE_W35T51NW     (1)
////////////////////////////////////////////////////////////////////////////////
#define MXIC_DEVICE_SERIES          (1)
#define MXIC_DEVICE_VENDOR_ID       (0xC2)
#define MXIC_DEVICE_QUAD            (1)
#define MXIC_DEVICE_MX25Vxx35F      (0)
#define MXIC_DEVICE_MX25Lxx33F      (0)
#define MXIC_DEVICE_MX25Uxx35F      (0)  // FRDM-K82F (MX25U3235F)
#define MXIC_DEVICE_MX25Lxxx33F     (0)
#define MXIC_DEVICE_MX25Uxxx32F     (0)
#define MXIC_DEVICE_MX25Lxxx45G     (0)
#define MXIC_DEVICE_MX25Uxxx45G     (1)  // RD-RW612-BGA (MX25U51245G)
#define MXIC_QUAD_FLASH_JEDEC_ID    (0x003725C2)
#define MXIC_DEVICE_OCTAL           (1)
#define MXIC_DEVICE_MX25UWxx345     (1)
#define MXIC_DEVICE_MX25UMxx345     (0)  // MIMXRT595-EVK (MX25UM51345), MIMXRT685-EVK (MX25UM51345)
#define MXIC_DEVICE_MX25LMxx245     (0)
#define MXIC_OCTAL_FLASH_JEDEC_ID   (0x003781C2)
////////////////////////////////////////////////////////////////////////////////
#define GIGADEVICE_DEVICE_SERIES    (1)
#define GIGADEVICE_DEVICE_VENDOR_ID (0xC8)
#define GIGADEVICE_DEVICE_QUAD      (1)
#define GIGADEVICE_DEVICE_GD25QxxxC (0)
#define GIGADEVICE_DEVICE_GD25BxxxE (0)
#define GIGADEVICE_DEVICE_GD25QxxxE (1)  // MIMXRT1040-EVK(Rework-GD25Q80E)
#define GIGADEVICE_DEVICE_GD25LQxxxC (0)
#define GIGADEVICE_DEVICE_GD25LExxxD (0)
#define GIGADEVICE_DEVICE_GD25LBxxxE (0)
#define GIGADEVICE_DEVICE_GD25LTxxxE (0)
#define GIGADEVICE_DEVICE_OCTAL     (1)
#define GIGADEVICE_DEVICE_GD25LX    (1)
#define GIGADEVICE_DEVICE_GD25X     (0)
////////////////////////////////////////////////////////////////////////////////
#define ISSI_DEVICE_SERIES          (1)  // ISSI is acquired by ±±¾©Îù³É in 2015, ±±¾©Îù³É is acquired Ingenic by in 2020
#define ISSI_DEVICE_VENDOR_ID       (0x9D)
#define ISSI_DEVICE_QUAD            (1)
#define ISSI_DEVICE_IS25LP064A      (0)  // MIMXRT1020-EVK (IS25LP064A)
#define ISSI_DEVICE_IS25WP064A      (0)  // MIMXRT1060-EVK (IS25WP064A)
#define ISSI_DEVICE_IS25LP064D      (1)
#define ISSI_DEVICE_IS25WP064D      (0)
#define ISSI_DEVICE_IS25WP128       (0)  // MIMXRT1170-EVK/PMIC (IS25WP128)
#define ISSI_DEVICE_IS25WJ128F      (0)
#define ISSI_DEVICE_IS25LP512M      (0)
#define ISSI_DEVICE_OCTAL           (1)
#define ISSI_DEVICE_IS25WX256       (0)
#define ISSI_DEVICE_IS25LX256       (1)
#define ISSI_DEVICE_HYPERBUS        (0)
#define ISSI_DEVICE_IS26KS256       (0)  // ISSI hyperflash seems to be EOL
////////////////////////////////////////////////////////////////////////////////
#define MICRON_DEVICE_SERIES        (1)
#define MICRON_DEVICE_VENDOR_ID     (0x20)
#define MICRON_DEVICE_VENDOR_ID2    (0x2C)
#define MICRON_DEVICE_QUAD          (1)
#define MICRON_DEVICE_MT25QU128     (0)
#define MICRON_DEVICE_MT25QL256     (1)  // MIMXRT1170-EVKB(Rework-MT25QL256)
#define MICRON_DEVICE_OCTAL         (1)
#define MICRON_DEVICE_MT35XU512_RW303 (0)
#define MICRON_DEVICE_MT35XU512_RW304 (1)// MIMXRT1180-EVK (MT35XU512ABA2G12-0AAT), MIMXRT1170-ADP_Rev.Ax (MT35XU512ABA2G12-0AAT)
#define MICRON_OCTAL_FLASH_JEDEC_ID (0x001a5B2C)
////////////////////////////////////////////////////////////////////////////////
#define ADESTO_DEVICE_SERIES        (1)  // Adesto is acquired by Dialog in 2020, Dialog is acquired by Renesas in 2021
#define ADESTO_DEVICE_VENDOR_ID     (0x1F)
#define ADESTO_DEVICE_VENDOR_ID2    (0x43)
#define DIALOG_DEVICE_VENDOR_ID     (0x1F)
#define RENESAS_DEVICE_VENDOR_ID    (0x1F)
#define ADESTO_DEVICE_QUAD          (1)
#define ADESTO_DEVICE_AT25SF128A    (1)  // MIMXRT1010-EVK (AT25SF128A), MIMXRT1015-EVK (AT25SF128A)
#define ADESTO_DEVICE_OCTAL         (0)
#define ADESTO_DEVICE_ATXP032       (0)  // Adesto octal flash is EOL
////////////////////////////////////////////////////////////////////////////////
#define SPANSION_DEVICE_SERIES      (1)  // Spansion is acquired by Cypress in 2014, Cypress is acquired by Infineon in 2019
#define SPANSION_DEVICE_VENDOR_ID   (0x01)
#define CYPRESS_DEVICE_VENDOR_ID    (0x01)
#define INFINEON_DEVICE_VENDOR_ID   (0x34)
#define SPANSION_DEVICE_QUAD        (1)
#define SPANSION_DEVICE_S25FL064L   (0)
#define SPANSION_DEVICE_S25HS512T   (1)
#define SPANSION_DEVICE_OCTAL       (1)
#define SPANSION_DEVICE_S28HS512T   (0)
#define SPANSION_DEVICE_S28HS256M   (1)
#define SPANSION_DEVICE_HYPERBUS    (1)
// This HyperFlash family support HYPERBUS™ interface only
#define SPANSION_DEVICE_S26KS512S   (0)  // MIMXRT1050-EVKB (S26KS512)
#define SPANSION_DEVICE_S26KL128S   (0)
// This SEMPER™ Flash family support  both the HYPERBUS™ interface only as well as legacy (x1) SPI.
//   The legacy (x1) SPI mode is the default protocol following Power-on-Reset (POR) but, flash devices can be
//   configured to boot-up in the HYPERBUS™ mode.
#define SPANSION_DEVICE_S26HL512T   (1)  

////////////////////////////////////////////////////////////////////////////////
#define PUYA_DEVICE_SERIES          (1)
#define PUYA_DEVICE_VENDOR_ID       (0x85)
#define PUYA_DEVICE_QUAD            (1)
#define PUYA_DEVICE_PY25Q128HA      (1)
////////////////////////////////////////////////////////////////////////////////
#define DOSILICON_DEVICE_SERIES     (1)
#define DOSILICON_DEVICE_VENDOR_ID  (0xE5)
#define DOSILICON_DEVICE_QUAD       (1)
#define DOSILICON_DEVICE_DS25M4AB   (1)
////////////////////////////////////////////////////////////////////////////////
#define XMC_DEVICE_SERIES           (1)  // Wuhan Xinxin Semiconductor Manufacturing Co., Ltd.
#define XMC_DEVICE_VENDOR_ID        (0x20)
#define XMC_DEVICE_QUAD             (1)
#define XMC_DEVICE_XM25QH64D        (1)
////////////////////////////////////////////////////////////////////////////////
#define BOYA_DEVICE_SERIES          (1)  // Boya Microelectronics
#define BOYA_DEVICE_VENDOR_ID       (0x68)
#define BOYA_DEVICE_QUAD            (1)
#define BOYA_DEVICE_BY25FQ64ES      (1)

#define FLASH_DEVICE_VENDOR_ID_LIST {WINBOND_DEVICE_VENDOR_ID,    \
                                     MXIC_DEVICE_VENDOR_ID,       \
                                     GIGADEVICE_DEVICE_VENDOR_ID, \
                                     ISSI_DEVICE_VENDOR_ID,       \
                                     MICRON_DEVICE_VENDOR_ID,     \
                                     MICRON_DEVICE_VENDOR_ID2,    \
                                     ADESTO_DEVICE_VENDOR_ID,     \
                                     ADESTO_DEVICE_VENDOR_ID2,    \
                                     SPANSION_DEVICE_VENDOR_ID,   \
                                     INFINEON_DEVICE_VENDOR_ID,   \
                                     PUYA_DEVICE_VENDOR_ID,       \
                                     DOSILICON_DEVICE_VENDOR_ID,  \
                                     BOYA_DEVICE_VENDOR_ID}

#if MFB_MIXSPI_MODULE == MFB_MIXSPI_MODULE_IS_FLEXSPI
#define MXIC_READ_STATUS_REG_DUMMY_DDR (0x08)
#elif MFB_MIXSPI_MODULE == MFB_MIXSPI_MODULE_IS_QUADSPI

#elif MFB_MIXSPI_MODULE == MFB_MIXSPI_MODULE_IS_XSPI
#define MXIC_READ_STATUS_REG_DUMMY_DDR (0x14)
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/

extern flash_property_info_t g_flashPropertyInfo;

extern const uint32_t g_mixspiRootClkFreqInMHz[];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void mixspi_device_config_init(void);
extern void mixspi_device_config_update_rootclock(uint32_t clkFreq);
extern void mixspi_device_config_update_flashsize(uint32_t flashSize);

extern status_t mixspi_nor_get_jedec_id(MIXSPI_Type *base, uint32_t *jedecId, flash_inst_mode_t flashInstMode);
extern status_t mixspi_nor_get_infineon_samper_id(MIXSPI_Type *base, infineon_samper_id_t *samperId);
extern status_t mixspi_nor_get_cfi_id(MIXSPI_Type *base, cfi_device_id_t *cfiDeviceId);
extern status_t mixspi_nor_get_jedec_sfdp(MIXSPI_Type *base, uint32_t addr, uint32_t *jedecSfdp, uint32_t sfdpSize);
extern status_t mixspi_nor_sfdp_sec_erase(MIXSPI_Type *base, uint32_t addr);
extern status_t mixspi_nor_sfdp_sec_program(MIXSPI_Type *base, uint32_t addr, const uint32_t *src, uint32_t length);
extern status_t mixspi_nor_set_dummy_cycle(MIXSPI_Type *base, uint8_t dummyCmd);
extern status_t mixspi_nor_set_drive_strength(MIXSPI_Type *base, uint8_t driveCmd);
extern status_t mixspi_nor_set_unique_cfg(MIXSPI_Type *base, uint8_t driveCmd);
extern status_t mixspi_nor_enable_quad_mode(MIXSPI_Type *base);
extern status_t mixspi_nor_enable_qpi_mode(MIXSPI_Type *base);
extern status_t mixspi_nor_enable_opi_mode(MIXSPI_Type *base);
extern status_t mixspi_nor_flash_erase_sector(MIXSPI_Type *base, uint32_t address, flash_inst_mode_t flashInstMode);
extern status_t mixspi_nor_flash_page_program(MIXSPI_Type *base, uint32_t address, const uint32_t *src, uint32_t length, flash_inst_mode_t flashInstMode);
extern void mixspi_nor_flash_init(MIXSPI_Type *base, const uint32_t *customLUT, mixspi_read_sample_clock_t rxSampleClock, flash_inst_mode_t flashInstMode);
extern status_t mixspi_nor_read_register(MIXSPI_Type *base, flash_reg_access_t *regAccess);
extern status_t mixspi_nor_write_register(MIXSPI_Type *base, flash_reg_access_t *regAccess);

extern uint32_t decode_mixspi_root_clk_defn(mixspi_root_clk_freq_t mixspiRootClkFreq);
extern mixspi_root_clk_freq_t get_current_mixspi_root_clk(uint32_t clkInHz);
extern uint32_t mfb_flash_decode_common_capacity_id(uint8_t capacityID);
extern uint32_t mfb_flash_decode_adesto_capacity_id(uint8_t capacityID);
extern void mfb_flash_show_mem_size(uint8_t capacityID, bool isAdesto);
extern void mfb_flash_show_registers(jedec_id_t *jedecID, bool isOctalFlash);
#if WINBOND_DEVICE_SERIES
extern void mfb_flash_set_param_for_winbond(jedec_id_t *jedecID);
extern void mfb_flash_show_registers_for_winbond(bool isOctalFlash);
#endif
#if MXIC_DEVICE_SERIES
extern void mfb_flash_set_param_for_mxic(jedec_id_t *jedecID);
extern void mfb_flash_show_registers_for_mxic(bool isOctalFlash);
#endif
#if GIGADEVICE_DEVICE_SERIES
extern void mfb_flash_set_param_for_gigadevice(jedec_id_t *jedecID);
extern void mfb_flash_show_registers_for_gigadevice(bool isOctalFlash);
#endif
#if ISSI_DEVICE_SERIES
extern void mfb_flash_set_param_for_issi(jedec_id_t *jedecID);
extern void mfb_flash_show_registers_for_issi(bool isOctalFlash);
#endif
#if MICRON_DEVICE_SERIES
extern void mfb_flash_set_param_for_micron(jedec_id_t *jedecID);
extern void mfb_flash_show_registers_for_micron(bool isOctalFlash);
#endif
#if ADESTO_DEVICE_SERIES
extern void mfb_flash_set_param_for_adesto(jedec_id_t *jedecID);
extern void mfb_flash_show_registers_for_adesto(bool isOctalFlash);
#endif
#if SPANSION_DEVICE_SERIES
extern void mfb_flash_set_param_for_spansion(jedec_id_t *jedecID);
extern void mfb_flash_show_registers_for_spansion(bool isOctalFlash);
extern bool mfb_hyperflash_switch_to_hyperbus_mode(void);
extern void mfb_hyperflash_set_param_for_spansion_for_spi_mode(infineon_samper_id_t *samperID);
extern void mfb_hyperflash_set_param_for_spansion_for_hyperbus_mode(void);
extern void mfb_hyperflash_show_info_for_spansion(cfi_device_id_t *cfiDeviceId);
#endif
#if PUYA_DEVICE_SERIES
extern void mfb_flash_set_param_for_puya(jedec_id_t *jedecID);
extern void mfb_flash_show_registers_for_puya(bool isOctalFlash);
#endif
#if DOSILICON_DEVICE_SERIES
extern void mfb_flash_set_param_for_dosilicon(jedec_id_t *jedecID);
extern void mfb_flash_show_registers_for_dosilicon(bool isOctalFlash);
#endif
#if XMC_DEVICE_SERIES
extern void mfb_flash_set_param_for_xmc(jedec_id_t *jedecID);
extern void mfb_flash_show_registers_for_xmc(bool isOctalFlash);
#endif
#if BOYA_DEVICE_SERIES
extern void mfb_flash_set_param_for_boya(jedec_id_t *jedecID);
extern void mfb_flash_show_registers_for_boya(bool isOctalFlash);
#endif

extern bool mfb_flash_is_valid_jedec_id(jedec_id_t *jedecID);
extern bool mfb_flash_is_valid_infineon_samper_id(infineon_samper_id_t *samperID);
extern bool mfb_flash_pattern_verify_test(bool showError);
extern bool mfb_flash_write_pattern_region(flash_inst_mode_t flashInstMode);
extern void mfb_flash_memcpy_perf_test(bool stressTestEnable);

#endif /* _MFB_NOR_FLASH_H_ */
