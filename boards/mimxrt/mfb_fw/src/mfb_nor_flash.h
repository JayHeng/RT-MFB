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

#define NOR_CMD_LUT_SEQ_IDX_READ            0
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS      1
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE     2
#define NOR_CMD_LUT_SEQ_IDX_READID          3

#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI 4
#define NOR_CMD_LUT_SEQ_IDX_ENTEROPI        5
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI  6

#define NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG  7
#define NOR_CMD_LUT_SEQ_IDX_ENTERQPI        8
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
#define NOR_CMD_LUT_SEQ_IDX_WRITE          9
#define CUSTOM_LUT_LENGTH        60

////////////////////////////////////////////////////////////////////////////////
#define MXIC_DEVICE_SERIES         (1)
#define MXIC_DEVICE_MX25UM51345    (1)
////////////////////////////////////////////////////////////////////////////////
#define ISSI_DEVICE_SERIES         (1)
#define ISSI_DEVICE_IS25WP064A     (1)
////////////////////////////////////////////////////////////////////////////////
#define WINBOND_DEVICE_SERIES      (1)
#define WINBOND_DEVICE_W25Q64JV    (1)
////////////////////////////////////////////////////////////////////////////////
#define MICRON_DEVICE_SERIES       (1)
#define MICRON_DEVICE_MT25QL128    (1)
////////////////////////////////////////////////////////////////////////////////
#define GIGADEVICE_DEVICE_SERIE    (1)
#define GIGADEVICE_DEVICE_GD25Q64C (1)

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_H_ */
