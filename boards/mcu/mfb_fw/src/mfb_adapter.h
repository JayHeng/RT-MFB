/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_ADAPTER_H_
#define _MFB_ADAPTER_H_

#include "port_mixspi_info.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

// Definition to select spi peripheral
#ifndef MFB_MIXSPI_MODULE
#define MFB_MIXSPI_MODULE MFB_MIXSPI_MODULE_IS_FLEXSPI
#endif
////////////////////////////////////////////////////////////////////////////////
#if MFB_MIXSPI_MODULE == MFB_MIXSPI_MODULE_IS_FLEXSPI
#define MIXSPI_Type                     FLEXSPI_Type
#define MIXSPI_LUT_SEQ(cmd0, pad0, op0, cmd1, pad1, op1)                                                              \
    (FLEXSPI_LUT_OPERAND0(op0) | FLEXSPI_LUT_NUM_PADS0(pad0) | FLEXSPI_LUT_OPCODE0(cmd0) | FLEXSPI_LUT_OPERAND1(op1) | \
     FLEXSPI_LUT_NUM_PADS1(pad1) | FLEXSPI_LUT_OPCODE1(cmd1))
#define MIXSPI_LUT_SUB_SEQ_LEN          (4)
#define kMIXSPI_Command_STOP            kFLEXSPI_Command_STOP
#define kMIXSPI_Command_SDR             kFLEXSPI_Command_SDR
#define kMIXSPI_Command_RADDR_SDR       kFLEXSPI_Command_RADDR_SDR
#define kMIXSPI_Command_CADDR_SDR       kFLEXSPI_Command_CADDR_SDR
#define kMIXSPI_Command_MODE1_SDR       kFLEXSPI_Command_MODE1_SDR
#define kMIXSPI_Command_MODE2_SDR       kFLEXSPI_Command_MODE2_SDR
#define kMIXSPI_Command_MODE4_SDR       kFLEXSPI_Command_MODE4_SDR
#define kMIXSPI_Command_MODE8_SDR       kFLEXSPI_Command_MODE8_SDR
#define kMIXSPI_Command_WRITE_SDR       kFLEXSPI_Command_WRITE_SDR
#define kMIXSPI_Command_READ_SDR        kFLEXSPI_Command_READ_SDR
#define kMIXSPI_Command_LEARN_SDR       kFLEXSPI_Command_LEARN_SDR
#define kMIXSPI_Command_DATSZ_SDR       kFLEXSPI_Command_DATSZ_SDR
#define kMIXSPI_Command_DUMMY_SDR       kFLEXSPI_Command_DUMMY_SDR
#define kMIXSPI_Command_DUMMY_RWDS_SDR  kFLEXSPI_Command_DUMMY_RWDS_SDR
#define kMIXSPI_Command_DDR             kFLEXSPI_Command_DDR
#define kMIXSPI_Command_RADDR_DDR       kFLEXSPI_Command_RADDR_DDR
#define kMIXSPI_Command_CADDR_DDR       kFLEXSPI_Command_CADDR_DDR
#define kMIXSPI_Command_MODE1_DDR       kFLEXSPI_Command_MODE1_DDR
#define kMIXSPI_Command_MODE2_DDR       kFLEXSPI_Command_MODE2_DDR
#define kMIXSPI_Command_MODE4_DDR       kFLEXSPI_Command_MODE4_DDR
#define kMIXSPI_Command_MODE8_DDR       kFLEXSPI_Command_MODE8_DDR
#define kMIXSPI_Command_WRITE_DDR       kFLEXSPI_Command_WRITE_DDR
#define kMIXSPI_Command_READ_DDR        kFLEXSPI_Command_READ_DDR
#define kMIXSPI_Command_LEARN_DDR       kFLEXSPI_Command_LEARN_DDR
#define kMIXSPI_Command_DATSZ_DDR       kFLEXSPI_Command_DATSZ_DDR
#define kMIXSPI_Command_DUMMY_DDR       kFLEXSPI_Command_DUMMY_DDR
#define kMIXSPI_Command_DUMMY_RWDS_DDR  kFLEXSPI_Command_DUMMY_RWDS_DDR
#define kMIXSPI_Command_JUMP_ON_CS      kFLEXSPI_Command_JUMP_ON_CS
//#define kMIXSPI_Command_JUMP_TO_SEQ
#define mixspi_pad_t                    flexspi_pad_t
#define kMIXSPI_1PAD                    kFLEXSPI_1PAD
#define kMIXSPI_2PAD                    kFLEXSPI_2PAD
#define kMIXSPI_4PAD                    kFLEXSPI_4PAD
#define kMIXSPI_8PAD                    kFLEXSPI_8PAD
#define mixspi_port_t                   flexspi_port_t
#define mixspi_read_sample_clock_t      flexspi_read_sample_clock_t
#define kMIXSPI_SampClkLoopbackDummy0   kFLEXSPI_ReadSampleClkLoopbackInternally
#define kMIXSPI_SampClkLoopbackDqs      kFLEXSPI_ReadSampleClkLoopbackFromDqsPad
#define kMIXSPI_SampClkLoopbackSck      kFLEXSPI_ReadSampleClkLoopbackFromSckPad
#define kMIXSPI_SampClkExtInputDqs      kFLEXSPI_ReadSampleClkExternalInputFromDqsPad

////////////////////////////////////////////////////////////////////////////////
#elif MFB_MIXSPI_MODULE == MFB_MIXSPI_MODULE_IS_QUADSPI
#define MIXSPI_Type                     QuadSPI_Type
#define MIXSPI_LUT_SEQ(cmd0, pad0, op0, cmd1, pad1, op1)                                                        \
    (QuadSPI_LUT_INSTR0(cmd0) | QuadSPI_LUT_PAD0(pad0) | QuadSPI_LUT_OPRND0(op0) | QuadSPI_LUT_INSTR1(cmd1) | \
     QuadSPI_LUT_PAD1(pad1) | QuadSPI_LUT_OPRND1(op1))
#define MIXSPI_LUT_SUB_SEQ_LEN          (4)
#define kMIXSPI_Command_STOP            0
#define kMIXSPI_Command_SDR             1
#define kMIXSPI_Command_RADDR_SDR       2  // QSPI_ADDR
#define kMIXSPI_Command_CADDR_SDR       0x12
#define kMIXSPI_Command_MODE2_SDR       5
#define kMIXSPI_Command_MODE4_SDR       6
#define kMIXSPI_Command_MODE8_SDR       4  // QSPI_MODE
#define kMIXSPI_Command_WRITE_SDR       8
#define kMIXSPI_Command_READ_SDR        7
#define kMIXSPI_Command_LEARN_SDR       0x10  // QSPI_DATA_LEARN
//#define kMIXSPI_Command_DATSZ_SDR
#define kMIXSPI_Command_DUMMY_SDR       3
//#define kMIXSPI_Command_DUMMY_RWDS_SDR
#define kMIXSPI_Command_DDR             0x11u
#define kMIXSPI_Command_RADDR_DDR       0xA
#define kMIXSPI_Command_CADDR_DDR       0x13
//#define kMIXSPI_Command_MODE1_DDR
#define kMIXSPI_Command_MODE2_DDR       0xC
#define kMIXSPI_Command_MODE4_DDR       0xD
#define kMIXSPI_Command_MODE8_DDR       0xB
#define kMIXSPI_Command_WRITE_DDR       0xF
#define kMIXSPI_Command_READ_DDR        0xE
//#define kMIXSPI_Command_LEARN_DDR
//#define kMIXSPI_Command_DATSZ_DDR
#define kMIXSPI_Command_DUMMY_DDR       3
#define kMIXSPI_Command_DUMMY_RWDS_DDR  3
#define kMIXSPI_Command_JUMP_ON_CS      9
//#define kMIXSPI_Command_JUMP_TO_SEQ
#define mixspi_pad_t                    uint32_t
#define kMIXSPI_1PAD                    0 // QSPI_PAD_1
#define kMIXSPI_2PAD                    1 // QSPI_PAD_2
#define kMIXSPI_4PAD                    2 // QSPI_PAD_4
#define kMIXSPI_8PAD                    3 // QSPI_PAD_8
#define mixspi_port_t                   uint32_t
#define mixspi_read_sample_clock_t      qspi_dqs_read_sample_clock_t
#define kMIXSPI_SampClkLoopbackDummy0   kQSPI_ReadSampleClkInternalLoopback
#define kMIXSPI_SampClkLoopbackDqs      kQSPI_ReadSampleClkLoopbackFromDqsPad
#define kMIXSPI_SampClkExtInputDqs      kQSPI_ReadSampleClkExternalInputFromDqsPad

////////////////////////////////////////////////////////////////////////////////
#elif MFB_MIXSPI_MODULE == MFB_MIXSPI_MODULE_IS_XSPI
#define MIXSPI_Type                     XSPI_Type
#define MIXSPI_LUT_SEQ(cmd0, pad0, op0, cmd1, pad1, op1)                                            \
    (XSPI_LUT_INSTR0(cmd0) | XSPI_LUT_PAD0(pad0) | XSPI_LUT_OPRND0(op0) | XSPI_LUT_INSTR1(cmd1) | \
     XSPI_LUT_PAD1(pad1) | XSPI_LUT_OPRND1(op1))
#define MIXSPI_LUT_SUB_SEQ_LEN          (5)
#define kMIXSPI_Command_STOP            kXSPI_Command_STOP
#define kMIXSPI_Command_SDR             kXSPI_Command_SDR
#define kMIXSPI_Command_RADDR_SDR       kXSPI_Command_RADDR_SDR
#define kMIXSPI_Command_CADDR_SDR       kXSPI_Command_CADDR_SDR
//#define kMIXSPI_Command_MODE1_SDR
#define kMIXSPI_Command_MODE2_SDR       kXSPI_Command_MODE2_SDR
#define kMIXSPI_Command_MODE4_SDR       kXSPI_Command_MODE4_SDR
#define kMIXSPI_Command_MODE8_SDR       kXSPI_Command_MODE_SDR
#define kMIXSPI_Command_WRITE_SDR       kXSPI_Command_WRITE_SDR
#define kMIXSPI_Command_READ_SDR        kXSPI_Command_READ_SDR
//#define kMIXSPI_Command_LEARN_SDR
//#define kMIXSPI_Command_DATSZ_SDR
#define kMIXSPI_Command_DUMMY_SDR       kXSPI_Command_DUMMY_SDR
//#define kMIXSPI_Command_DUMMY_RWDS_SDR
#define kMIXSPI_Command_DDR             kXSPI_Command_DDR
#define kMIXSPI_Command_RADDR_DDR       kXSPI_Command_RADDR_DDR
#define kMIXSPI_Command_CADDR_DDR       kXSPI_Command_CADDR_DDR
//#define kMIXSPI_Command_MODE1_DDR
#define kMIXSPI_Command_MODE2_DDR       kXSPI_Command_MODE2_DDR
#define kMIXSPI_Command_MODE4_DDR       kXSPI_Command_MODE4_DDR
#define kMIXSPI_Command_MODE8_DDR       kXSPI_Command_MODE_DDR
#define kMIXSPI_Command_WRITE_DDR       kXSPI_Command_WRITE_DDR
#define kMIXSPI_Command_READ_DDR        kXSPI_Command_READ_DDR
#define kMIXSPI_Command_LEARN_DDR       kXSPI_Command_LEARN_DDR
//#define kMIXSPI_Command_DATSZ_DDR
#define kMIXSPI_Command_DUMMY_DDR       kXSPI_Command_DUMMY_SDR
#define kMIXSPI_Command_DUMMY_RWDS_DDR  kXSPI_Command_DUMMY_SDR
#define kMIXSPI_Command_JUMP_ON_CS      kXSPI_Command_JMP_ON_CS
#define kMIXSPI_Command_JUMP_TO_SEQ     kXSPI_Command_JUMP_TO_SEQ
#define mixspi_pad_t                    xspi_pad_t
#define kMIXSPI_1PAD                    kXSPI_1PAD
#define kMIXSPI_2PAD                    kXSPI_2PAD
#define kMIXSPI_4PAD                    kXSPI_4PAD
#define kMIXSPI_8PAD                    kXSPI_8PAD
#define mixspi_port_t                   xspi_target_group_t
#define mixspi_read_sample_clock_t      xspi_sample_clk_source_t
#define kMIXSPI_SampClkLoopbackDummy0   kXSPI_SampleClkFromNonInvertedFullySpeedDummyPadLoopback
#define kMIXSPI_SampClkLoopbackDummy1   kXSPI_SampleClkFromInvertedFullySpeedDummyPadLoopback
#define kMIXSPI_SampClkLoopbackDummy2   kXSPI_SampleClkFromHalfSpeedDummyPadLoopback
#define kMIXSPI_SampClkLoopbackDqs      kXSPI_SampleClkFromDqsPadLoopback
#define kMIXSPI_SampClkExtInputDqs      kXSPI_SampleClkFromExternalDQS

#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_ADAPTER_H_ */
