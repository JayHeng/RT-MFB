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
#define mixspi_pad_t                    uint32_t
#define kMIXSPI_1PAD                    0
#define kMIXSPI_2PAD                    0
#define kMIXSPI_4PAD                    0
#define kMIXSPI_8PAD                    0
#define mixspi_port_t                   flexspi_port_t
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
#define mixspi_pad_t                    xspi_pad_t
#define kMIXSPI_1PAD                    kXSPI_1PAD
#define kMIXSPI_2PAD                    kXSPI_2PAD
#define kMIXSPI_4PAD                    kXSPI_4PAD
#define kMIXSPI_8PAD                    kXSPI_8PAD
#define mixspi_port_t                   uint32_t
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
