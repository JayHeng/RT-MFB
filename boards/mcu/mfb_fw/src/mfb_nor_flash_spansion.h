/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_NOR_FLASH_SPANSION_H_
#define _MFB_NOR_FLASH_SPANSION_H_

#include "mfb_nor_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SPANSION_FLASH_BUSY_STATUS_POL    1
#define SPANSION_FLASH_BUSY_STATUS_OFFSET 0

#if SPANSION_DEVICE_S25FL064L
#define SPANSION_FLASH_QUAD_ENABLE           0x0200

#define SPANSION_QUAD_FLASH_DUMMY_CYCLES     0x08

#define SPANSION_QUAD_FLASH_UNIQUE_CFG       0x00

//------------------------------------------------------
// CR3NV[3:0] |  dummy cycles  |Quad IO Fast Read(SPI) |
//            |                |Quad IO Fast Read(QPI) |
//------------------------------------------------------
//  4'b0001   |        1       |         35MHz         |
//  4'b0010   |        2       |         45MHz         |
//  4'b0011   |        3       |         55MHz         |
//  4'b0100   |        4       |         65MHz         |
//  4'b0101   |        5       |         75MHz         |
//  4'b0110   |        6       |         85MHz         |
//  4'b0111   |        7       |         95MHz         |
//  4'b1000   |   8(default)   |        108MHz         |
//   ...      |       ..       |        108MHz         |
//  4'b1111   |       15       |        108MHz         |
//------------------------------------------------------
#elif SPANSION_DEVICE_S25FL256S
// No unique/hybrid sector selection register applies to legacy FL-S QuadSPI.
#define SPANSION_QUAD_FLASH_UNIQUE_CFG       0x00

//------------------------------------------------------------------------------
// CR1[7:6] |  mode  |  dummy  | Quad I/O Fast Read (EBh/ECh) SDR
//------------------------------------------------------------------------------
//   00b    |   2    |    4    |         80MHz
//   01b    |   2    |    4    |         90MHz
//   10b    |   2    |    5    |         104MHz
//   11b    |   2    |    1    |         50MHz
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// CR1[7:6] |  mode  |  dummy  | Quad I/O Fast Read (EBh/ECh) DDR
//------------------------------------------------------------------------------
//   00b    |   1    |    6    |         66MHz
//   01b    |   1    |    7    |         66MHz
//   10b    |   1    |    8    |         66MHz
//   11b    |   1    |    3    |         50MHz
//------------------------------------------------------------------------------

// LC, QE bit in Configuration Register 1 (CR1[7:6], CR1[1]), written via WRR (01h) as the
//   second data byte (SR1 first, then CR1). So the 16-bit WRR payload for QE is 0x8200/0200.
#define SPANSION_QUAD_FLASH_SET_DUMMY_CMD 0x8200
#if MFB_FLASH_USE_DEFAULT_DUMMY
#define SPANSION_QUAD_FLASH_DUMMY_CYCLES  0x04
#define SPANSION_FLASH_QUAD_ENABLE           0x0200
#else
#define SPANSION_QUAD_FLASH_DUMMY_CYCLES  0x05
#define SPANSION_FLASH_QUAD_ENABLE           0x8200
#endif

#elif SPANSION_DEVICE_S25HS512T
#define SPANSION_FLASH_QUAD_ENABLE           0x0200

#define SPANSION_QUAD_FLASH_DUMMY_CYCLES     0x08

//------------------------------------------------------
// CR2NV[3:0] |  dummy cycles  |Quad IO Fast Read(SPI) |
//            |                |Quad IO Fast Read(QPI) |
//------------------------------------------------------
//  4'b0000   |        0       |         43MHz         |
//  4'b0001   |        1       |         56MHz         |
//  4'b0010   |        2       |         68MHz         |
//  4'b0011   |        3       |         81MHz         |
//  4'b0100   |        4       |         93MHz         |
//  4'b0101   |        5       |        106MHz         |
//  4'b0110   |        6       |        118MHz         |
//  4'b0111   |        7       |        131MHz         |
//  4'b1000   |   8(default)   |        143MHz         |
//   ...      |       ..       |        156MHz         |
//  4'b1111   |       15       |        156MHz         |
//------------------------------------------------------

// CFR3NV[3] - Uniform or Hybrid Sector Architecture selection
#define SPANSION_QUAD_FLASH_UNIQUE_CFG       0x08
#endif

////////////////////////////////////////////////////////////////////////////////
#define SPANSION_X8_FLASH_BUS_PROTOCOL (kFlashProtocolType_OctaBus)

#if SPANSION_DEVICE_S28HS512T | SPANSION_DEVICE_S28HS256M
// CFR5N/V
// bit0 - SPI/OPI enable
// bit1 - SDR/DDR enable
// bit6 - reserved 1
#define SPANSION_OCTAL_FLASH_ENABLE_DDR_CMD     0x43

//------------------------------------------------------------------------------
//CFR2N/V[3:0]|  dummy cycles  |  Octal SDR(8S-8S-8S)   |Octal SPI SDR(1S-1S-8S)|
//            |                |  Octal DDR(8D-8D-8D)   |                       |
//            |                |        BGA24           |                       |
//-------------------------------------------------------------------------------
//    0x0     |      5/0       |        50/42MHz        |          50MHz        |
//  0x8(def)  |     20/8       |      200/178MHz        |         156MHz        |
//    0x9     |     22/9       |      200/192MHz        |         166MHz        |
//    0xf     |     28/15      |         200MHz         |         166MHz        |
//-------------------------------------------------------------------------------
// bit7 - 3B/4B addr
#if MFB_FLASH_OPI_MODE_DISABLE
#define SPANSION_OCTAL_FLASH_SET_DUMMY_CMD     0x89
#define SPANSION_OCTAL_FLASH_DUMMY_CYCLES      0x09   // 166MHz SPI SDR
#else
#define SPANSION_OCTAL_FLASH_SET_DUMMY_CMD     0x08
#define SPANSION_OCTAL_FLASH_DUMMY_CYCLES      0x14   // 200MHz OPI DDR
#endif
#endif


#if SPANSION_DEVICE_S26KS512S || SPANSION_DEVICE_S26KL128S || SPANSION_DEVICE_S26HL512T
#define SPANSION_HYPERFLASH_BUSY_STATUS_OFFSET 7
#define SPANSION_HYPERFLASH_MIX_STATUS_MASK    0x32

#if SPANSION_DEVICE_S26KS512S || SPANSION_DEVICE_S26KL128S
#define SPANSION_HYPERFLASH_ID_ADDR_MID  (0x0000)  // Manufacturer ID = 0x0001, Device ID = 0x007E
#define SPANSION_HYPERFLASH_ID_ADDR_QRY  (0x0010)
#define SPANSION_HYPERFLASH_ID_ADDR_DID0 (0x000E)  // Device ID Density & Voltage Type
#elif SPANSION_DEVICE_S26HL512T
#define SPANSION_HYPERFLASH_ID_ADDR_MID  (0x0800)  // Manufacturer ID = 0x0034, Interface Voltage Type
#define SPANSION_HYPERFLASH_ID_ADDR_DID0 (0x0802)  // Device ID Density
#define SPANSION_HYPERFLASH_ID_ADDR_DID1 (0x0804)  // Device ID Family = 0x0090
#endif

#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_NOR_FLASH_SPANSION_H_ */
