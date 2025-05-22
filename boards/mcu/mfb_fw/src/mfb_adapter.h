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

#if MFB_MIXSPI_MODULE == MFB_MIXSPI_MODULE_IS_FLEXSPI
#define mixspi_pad_t                 flexspi_pad_t
#define mixspi_port_t                flexspi_port_t
#define mixspi_read_sample_clock_t   flexspi_read_sample_clock_t
#define MIXSPI_Type                  FLEXSPI_Type

#define kMIXSPI_1PAD                 kFLEXSPI_1PAD
#define kMIXSPI_2PAD                 kFLEXSPI_2PAD
#define kMIXSPI_4PAD                 kFLEXSPI_4PAD
#define kMIXSPI_8PAD                 kFLEXSPI_8PAD

#elif MFB_MIXSPI_MODULE == MFB_MIXSPI_MODULE_IS_QUADSPI
#define mixspi_pad_t                 uint32_t
#define mixspi_port_t                flexspi_port_t
#define mixspi_read_sample_clock_t   qspi_dqs_read_sample_clock_t
#define MIXSPI_Type                  QuadSPI_Type

#define kMIXSPI_1PAD                 0
#define kMIXSPI_2PAD                 0
#define kMIXSPI_4PAD                 0
#define kMIXSPI_8PAD                 0

#elif MFB_MIXSPI_MODULE == MFB_MIXSPI_MODULE_IS_XSPI
#define mixspi_pad_t                 xspi_pad_t
#define mixspi_port_t                uint32_t
#define mixspi_read_sample_clock_t   xspi_sample_clk_source_t
#define MIXSPI_Type                  XSPI_Type

#define kMIXSPI_1PAD                 0
#define kMIXSPI_2PAD                 0
#define kMIXSPI_4PAD                 0
#define kMIXSPI_8PAD                 0

#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_ADAPTER_H_ */
