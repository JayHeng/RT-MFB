/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mfb_nor_flash_spansion.h"
#if SPANSION_DEVICE_SERIES
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/

#if SPANSION_DEVICE_QUAD
const uint32_t s_customLUT_SPANSION_Quad[CUSTOM_LUT_LENGTH] = {
    // For Infineon device, dummy cycles in DS doesn't include mode seq
#if !MFB_FLASH_QPI_MODE_ENABLE
    /* Fast read quad mode - SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0xEB, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_4PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_MODE8_SDR, kMIXSPI_4PAD, 0x00, kFLEXSPI_Command_DUMMY_SDR, kMIXSPI_4PAD, SPANSION_QUAD_FLASH_DUMMY_CYCLES),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 2] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR,  kMIXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),
#else
    /* Fast read quad mode - QPI-SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_4PAD, 0xEB, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_4PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_MODE8_SDR, kMIXSPI_4PAD, 0x00, kFLEXSPI_Command_DUMMY_SDR, kMIXSPI_4PAD, SPANSION_QUAD_FLASH_DUMMY_CYCLES),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 2] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR,  kMIXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),
#endif

    /* Read status register - 1 */
    // opcode 0x05/0x07 to read Status Registers (1/2)
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0x01),

    /* Write Enable - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

#if SPANSION_DEVICE_S25FL064L
    /* Erase 4KB Sector - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x20, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),
#elif SPANSION_DEVICE_S25HS512T
    /* Erase 256KB Sector - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0xD8, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),
#endif

    /* Page Program - single mode */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x02, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Enable Quad mode */
    // QE bit in 8bit Configuration Register-1[1], there are three Configuration Registers (1/2/3)
    // opcode 0x01 to write Status Registers 1 and Configuration Registers (1&2&3)
    [4 * NOR_CMD_LUT_SEQ_IDX_ENABLEQE] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x01, kFLEXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x04),

    /* Write Any register - CFR3N */
    [4 * NOR_CMD_LUT_SEQ_IDX_UNIQUECFG] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x71, kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_UNIQUECFG + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00, kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x04),
    [4 * NOR_CMD_LUT_SEQ_IDX_UNIQUECFG + 2] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x01, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

#if SPANSION_DEVICE_S25HS512T
    /* Read Any register */
    // The Read Any Register (RDARG_C_0) transaction is the best way to read all device registers, both nonvolatile and
    //   volatile. This is followed by a number of latency cycles set by (CFR2V[3:0]) for reading nonvolatile registers.
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x65, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DUMMY_SDR, kMIXSPI_1PAD, 0x08, kFLEXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0x01),

    /* Read Any register */
    // The Read Any Register (RDARG_C_0) transaction is the best way to read all device registers, both nonvolatile and
    //   volatile. This is followed by a number of latency cycles set by CFR3V[7:6] for reading volatile registers.
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG2] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x65, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG2 + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0x01, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),
#elif SPANSION_DEVICE_S25FL064L
    /* Read Any register */
    // The Read Any Register (RDAR) command provides a way to read device registers. The instruction is followed by a 3 or 4 Byte
    //   address (depending on the address length configuration CR2V[0]), followed by a number of latency (dummy) cycles set by
    //   CR3V[3:0].
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x65, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DUMMY_SDR, kMIXSPI_1PAD, 0x08, kFLEXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0x01),

    [4 * NOR_CMD_LUT_SEQ_IDX_READREG2] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x65, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG2 + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DUMMY_SDR, kMIXSPI_1PAD, 0x08, kFLEXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0x01),
#endif

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* Enter QPI mode */
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTERQPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x38, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Read status register -QPI-SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_QPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_4PAD, 0x05, kFLEXSPI_Command_READ_SDR,  kMIXSPI_4PAD, 0x01),

    /* Write Enable -QPI-SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_QPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_4PAD, 0x06, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Erase Sector - QPI-SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_QPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_4PAD, 0x20, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_4PAD, 0x18),

    /* Page Program - QPI-SDR */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_4PAD, 0x02, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_4PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kMIXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),
};
#endif

#if SPANSION_DEVICE_S28HS512
const uint32_t s_customLUT_SPANSION_Octal[CUSTOM_LUT_LENGTH] = {
#if !MFB_FLASH_OPI_MODE_DISABLE
    /*  DDR OCTAL I/O FAST READ */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 0] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0xEE, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0xEE),
    // Don't care the dummy value setting here, as it uses external DQS
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kMIXSPI_8PAD, 0x20, kFLEXSPI_Command_DUMMY_DDR, kMIXSPI_8PAD, 0x06),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 2] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR,  kMIXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),
#else
    /*  4-BYTE Fast READ (1S-1S-1S) */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 0] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x0B, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x20),
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DUMMY_SDR, kMIXSPI_1PAD, SPANSION_OCTAL_FLASH_DUMMY_CYCLES, kFLEXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0x04),
#endif

    /* Read status register -SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR,  kMIXSPI_1PAD, 0x01),

    /* Write Enable - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Set Dummy cycle */
    // Volatile Configuration Register address 800003h - Dummy cycle configuration
    //   - Default dummy cycle for OPI DDR is 20, max freq is 200MHz
    [4 * NOR_CMD_LUT_SEQ_IDX_SETDUMMY] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x71, kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x80),
    [4 * NOR_CMD_LUT_SEQ_IDX_SETDUMMY + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00, kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x03),
    [4 * NOR_CMD_LUT_SEQ_IDX_SETDUMMY + 2] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x01, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Erase Sector - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x21, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x20),

    /* 4-BYTE Page Program - SPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x20, kFLEXSPI_Command_RADDR_SDR, kMIXSPI_1PAD, 0x18),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Dummy write, do nothing when AHB write command is triggered. */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITE] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* Enter OPI mode */
    // Volatile Configuration Register address 800006h - I/O mode
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTEROPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x71, kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x80),
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTEROPI + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x00, kFLEXSPI_Command_SDR,       kMIXSPI_1PAD, 0x06),
    [4 * NOR_CMD_LUT_SEQ_IDX_ENTEROPI + 2] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kMIXSPI_1PAD, 0x01, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* Read status register using Octal DDR read */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x05, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x05),
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI + 1] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI + 2] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI + 3] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DUMMY_DDR, kMIXSPI_8PAD, 0x06, kFLEXSPI_Command_READ_DDR,  kMIXSPI_8PAD, 0x01),

    /* Write Enable - OPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_OPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x06, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x06),

    /* Erase Sector - OPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_OPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x21, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x21),
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR_OPI + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kMIXSPI_8PAD, 0x20, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),

    /* 4-BYTE Page Program - OPI */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x12, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x12),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OPI + 1] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kMIXSPI_8PAD, 0x20, kFLEXSPI_Command_WRITE_DDR, kMIXSPI_8PAD, 0x04),

    /* READ ANY REGISTER */
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG + 0] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x65, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x65),
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG + 1] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_RADDR_DDR, kMIXSPI_8PAD, 0x20, kFLEXSPI_Command_DUMMY_DDR, kMIXSPI_8PAD, 0x08),
    [4 * NOR_CMD_LUT_SEQ_IDX_READREG + 2] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR,  kMIXSPI_8PAD, 0x01, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00),
};
#endif

#if SPANSION_DEVICE_HYPERBUS
const uint32_t s_customLUT_SPANSION_Hyper[CUSTOM_LUT_LENGTH] = {
    /* Read Data */
    [4 * NOR_CMD_LUT_SEQ_IDX_READ] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0xA0, kFLEXSPI_Command_RADDR_DDR, kMIXSPI_8PAD, 0x18), // RA (ROW+COL)
    [4 * NOR_CMD_LUT_SEQ_IDX_READ + 1] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_CADDR_DDR, kMIXSPI_8PAD, 0x10, kFLEXSPI_Command_READ_DDR,  kMIXSPI_8PAD, 0x04), // RD

    /* Write Data */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITE] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x20, kFLEXSPI_Command_RADDR_DDR, kMIXSPI_8PAD, 0x18), // WA (ROW+COL)
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITE + 1] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_CADDR_DDR, kMIXSPI_8PAD, 0x10, kFLEXSPI_Command_WRITE_DDR, kMIXSPI_8PAD, 0x02), // WD

    /* Read Status */
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS + 1] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0xAA), // ADDR 0x555
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS + 2] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x05),
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS + 3] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x70), // DATA 0x70
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS + 4] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0xA0, kFLEXSPI_Command_RADDR_DDR, kMIXSPI_8PAD, 0x18), // RA (ROW+COL)
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS + 5] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_CADDR_DDR, kMIXSPI_8PAD, 0x10, kFLEXSPI_Command_DUMMY_RWDS_DDR, kMIXSPI_8PAD, 0x0B),
    [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS + 6] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR,  kMIXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x0),  // RD

    /* Write Enable */
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE + 1] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0xAA), // ADDR 0x555
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE + 2] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x05),
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE + 3] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0xAA), // DATA 0xAA
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE + 4] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE + 5] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x55), // ADDR 0x2AA
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE + 6] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x02),
    [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE + 7] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x55), // DATA 0x55

    /* Erase Sector  */
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 1] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0xAA), // ADDR 0x555
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 2] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x05),
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 3] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x80), // DATA 0x80
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 4] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 5] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0xAA), // ADDR 0x555
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 6] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x05),
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 7] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0xAA), // DATA 0xAA
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 8] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 9] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x55), // ADDR 0x2AA
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 10] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x02),
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 11] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x55), // DATA 0x55
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 12] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_RADDR_DDR, kMIXSPI_8PAD, 0x18), // SA (ROW+COL)
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 13] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_CADDR_DDR, kMIXSPI_8PAD, 0x10, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR + 14] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x30, kFLEXSPI_Command_STOP,      kMIXSPI_1PAD, 0x00), // DATA 0x30

    /* program page with word program command sequence */
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0xAA), // ADDR 0x555
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 2] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x05),
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 3] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0xA0), // DATA 0xA0
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 4] =
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_DDR,       kMIXSPI_8PAD, 0x00, kFLEXSPI_Command_RADDR_DDR, kMIXSPI_8PAD, 0x18), // PA (ROW+COL)
    [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 5] = 
        MIXSPI_LUT_SEQ(kFLEXSPI_Command_CADDR_DDR, kMIXSPI_8PAD, 0x10, kFLEXSPI_Command_WRITE_DDR, kMIXSPI_8PAD, 0x80), // PD
};
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

void mfb_flash_set_param_for_spansion(jedec_id_t *jedecID)
{
    mfb_printf(" -- Spansion Serial Flash.\r\n");
    mfb_printf("MFB: Flash Memory Type ID: 0x%x", jedecID->memoryTypeID);
    switch (jedecID->memoryTypeID)
    {
        /////////////////////////QuadSPI////////////////////////
        case 0x2A:
            g_flashPropertyInfo.flashHasQpiSupport = true;
            g_flashPropertyInfo.mixspiReadSampleClock = kMIXSPI_SampClkLoopbackDqs;
            g_flashPropertyInfo.flashQuadEnableCfg = SPANSION_FLASH_QUAD_ENABLE;
            g_flashPropertyInfo.flashQuadEnableBytes = 2;
            g_flashPropertyInfo.flashUniqueCfg = SPANSION_QUAD_FLASH_UNIQUE_CFG;
            mfb_printf(" -- S25HL-T QuadSPI 3.3V Series.\r\n");
            break;
        case 0x2B:
            g_flashPropertyInfo.flashHasQpiSupport = true;
            g_flashPropertyInfo.mixspiReadSampleClock = kMIXSPI_SampClkLoopbackDqs;
            g_flashPropertyInfo.flashQuadEnableCfg = SPANSION_FLASH_QUAD_ENABLE;
            g_flashPropertyInfo.flashQuadEnableBytes = 2;
            g_flashPropertyInfo.flashUniqueCfg = SPANSION_QUAD_FLASH_UNIQUE_CFG;
            mfb_printf(" -- S25HS-T QuadSPI 1.8V Series.\r\n");
            break;
        case 0x60:
            // S25FL-L QuadSPI
            g_flashPropertyInfo.flashHasQpiSupport = true;
            g_flashPropertyInfo.mixspiReadSampleClock = kMIXSPI_SampClkLoopbackDqs;
            g_flashPropertyInfo.flashQuadEnableCfg = SPANSION_FLASH_QUAD_ENABLE;
            g_flashPropertyInfo.flashQuadEnableBytes = 2;
            mfb_printf(" -- S25FL-L QuadSPI 3.3V Series.\r\n");
            break;
        ////////////////////////OctalSPI////////////////////////
        case 0x5A:
            g_flashPropertyInfo.flashIsOctal = true;
            mfb_printf(" -- S28HL OctalSPI 3.3V Series.\r\n");
            break;
        case 0x5B:
            g_flashPropertyInfo.flashIsOctal = true;
            mfb_printf(" -- S28HS OctalSPI 1.8V Series.\r\n");
            break;
        default:
            mfb_printf(" -- Unsupported Series.\r\n");
            break;
    }
    mfb_flash_show_mem_size(jedecID->capacityID, false);
#if SPANSION_DEVICE_QUAD
    if (!g_flashPropertyInfo.flashIsOctal)
    {
        g_flashPropertyInfo.mixspiPad                 = kMIXSPI_4PAD;
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_100MHz;
        g_flashPropertyInfo.flashBusyStatusPol        = SPANSION_FLASH_BUSY_STATUS_POL;
        g_flashPropertyInfo.flashBusyStatusOffset     = SPANSION_FLASH_BUSY_STATUS_OFFSET;
        g_flashPropertyInfo.mixspiCustomLUTVendor     = s_customLUT_SPANSION_Quad;
    }
#endif
#if SPANSION_DEVICE_OCTAL
    if (g_flashPropertyInfo.flashIsOctal)
    {
        g_flashPropertyInfo.mixspiPad                 = kMIXSPI_8PAD;
        g_flashPropertyInfo.flashBusyStatusPol        = SPANSION_FLASH_BUSY_STATUS_POL;
        g_flashPropertyInfo.flashBusyStatusOffset     = SPANSION_FLASH_BUSY_STATUS_OFFSET;
        g_flashPropertyInfo.flashEnableOctalCmd       = SPANSION_OCTAL_FLASH_ENABLE_DDR_CMD;
        g_flashPropertyInfo.mixspiCustomLUTVendor     = s_customLUT_SPANSION_Octal;
#if MFB_FLASH_OPI_MODE_DISABLE
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_30MHz;
        g_flashPropertyInfo.mixspiReadSampleClock     = kMIXSPI_SampClkLoopbackDqs;
#else
        g_flashPropertyInfo.mixspiRootClkFreq         = kMixspiRootClkFreq_332MHz;
        g_flashPropertyInfo.mixspiReadSampleClock     = kMIXSPI_SampClkExtInputDqs;
        if (g_flashPropertyInfo.mixspiRootClkFreq == kMixspiRootClkFreq_400MHz)
#endif
        {
            g_flashPropertyInfo.flashDummyValue = SPANSION_OCTAL_FLASH_SET_DUMMY_CMD;
        }
    }
#endif
}

void mfb_flash_show_registers_for_spansion(bool isOctalFlash)
{
#if MFB_FLASH_REGS_READBACK_ENABLE
    flash_reg_access_t regAccess;
    if (!isOctalFlash)
    {
        regAccess.regNum = 1;
        regAccess.regAddr = 0x000000;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Non-Volatile Status Register 1 (SR1NV): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x000002;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Non-Volatile Configuration Register 1 (CR1NV): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x000003;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Non-Volatile Configuration Register 2 (CR2NV): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x000004;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Non-Volatile Configuration Register 3 (CR3NV): 0x%x\r\n", regAccess.regValue.B.reg1);
#if SPANSION_DEVICE_S25HS512T
        regAccess.regAddr = 0x000005;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Non-Volatile Configuration Register 4 (CR4NV): 0x%x\r\n", regAccess.regValue.B.reg1);
#endif
        regAccess.regAddr = 0x000030;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Individual and Region Protection Register (IRP[7:0]): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x000031;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Individual and Region Protection Register (IRP[15:8]): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x800000;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG2;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Volatile Status Register 1 (SR1V): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x800001;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG2;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Volatile Status Register 2 (SR2V): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x800002;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG2;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Volatile Configuration Register 1 (CR1V): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x800003;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG2;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Volatile Configuration Register 2 (CR2V): 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x800004;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG2;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Volatile Configuration Register 3 (CR3V): 0x%x\r\n", regAccess.regValue.B.reg1);
#if SPANSION_DEVICE_S25HS512T
        regAccess.regAddr = 0x800005;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG2;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Volatile Configuration Register 4 (CR4V): 0x%x\r\n", regAccess.regValue.B.reg1);
#elif SPANSION_DEVICE_S25FL064L
        regAccess.regAddr = 0x800040;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG2;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Protection Register (PR): 0x%x\r\n", regAccess.regValue.B.reg1);
#endif
    }
    else
    {
        regAccess.regNum = 1;
        regAccess.regAddr = 0x0;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READSTATUS_OPI;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Status Register 1: 0x%x\r\n", regAccess.regValue.B.reg1);

        //regAccess.regAddr = 0x00800000;
        //regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        //mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        //mfb_printf("MFB: Flash Status Register 1: 0x%x\r\n", regAccess.regValue.B.reg1);
        regAccess.regAddr = 0x00800001;
        regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
        mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
        mfb_printf("MFB: Flash Status Register 2: 0x%x\r\n", regAccess.regValue.B.reg1);

        for (uint32_t idx = 2; idx <= 6; idx++)
        {
            regAccess.regAddr = 0x00800000 + idx;
            regAccess.regSeqIdx = NOR_CMD_LUT_SEQ_IDX_READREG;
            mixspi_nor_read_register(EXAMPLE_MIXSPI, &regAccess);
            mfb_printf("MFB: Flash Configuration Register %d: 0x%x\r\n", idx - 1, regAccess.regValue.B.reg1);
        }
    }
#endif
}

void mfb_hyperflash_set_param_for_spansion(void)
{
    g_flashPropertyInfo.mixspiPad             = kMIXSPI_8PAD;
    g_flashPropertyInfo.mixspiRootClkFreq     = kMixspiRootClkFreq_332MHz;
    g_flashPropertyInfo.flashBusyStatusOffset = SPANSION_HYPERFLASH_BUSY_STATUS_OFFSET;
    g_flashPropertyInfo.flashMixStatusMask    = SPANSION_HYPERFLASH_MIX_STATUS_MASK;
    g_flashPropertyInfo.mixspiReadSampleClock = kMIXSPI_SampClkExtInputDqs;
    g_flashPropertyInfo.mixspiCustomLUTVendor = s_customLUT_SPANSION_Hyper;
}

void mfb_hyperflash_show_info_for_spansion(cfi_device_id_t *cfiDeviceId)
{
    mfb_printf("MFB: Flash Manufacturer ID: 0x%x -- Spansion Serial Flash.\r\n", cfiDeviceId->manufacturerID);
    mfb_printf("MFB: Flash Memory Type ID: 0x%x", cfiDeviceId->memoryTypeID);
    switch (cfiDeviceId->capacityID)
    {
        case 0x70:
            mfb_printf(" -- S26KS/L 512Mb HyperFlash 1.8V Series.\r\n");
            break;
        case 0x6F:
            mfb_printf(" -- S26KS/L 512Mb HyperFlash 3.0V Series.\r\n");
            break;
        case 0x72:
            mfb_printf(" -- S26KS/L 256Mb HyperFlash 1.8V Series.\r\n");
            break;
        case 0x71:
            mfb_printf(" -- S26KS/L 256Mb HyperFlash 3.0V Series.\r\n");
            break;
        case 0x74:
            mfb_printf(" -- S26KS/L 128Mb HyperFlash 1.8V Series.\r\n");
            break;
        case 0x73:
            mfb_printf(" -- S26KS/L 128Mb HyperFlash 3.0V Series.\r\n");
            break;
        default:
            mfb_printf(" -- Unsupported Series.\r\n");
            break;
    }
}

#endif // SPANSION_DEVICE_SERIES

