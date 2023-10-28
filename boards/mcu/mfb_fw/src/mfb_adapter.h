/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MFB_ADAPTER_H_
#define _MFB_ADAPTER_H_

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MFB_MIXSPI_MODULE_IS_FLEXSPI 0U
#define MFB_MIXSPI_MODULE_IS_QUADSPI 1U

// Definition to select spi peripheral
#ifndef MFB_MIXSPI_MODULE
#define MFB_MIXSPI_MODULE MFB_MIXSPI_MODULE_IS_FLEXSPI
#endif

#if MFB_MIXSPI_MODULE == MFB_MIXSPI_MODULE_IS_FLEXSPI
#define flexspi_pad_t                mixspi_pad_t
#define flexspi_read_sample_clock_t  mixspi_read_sample_clock_t
#define FLEXSPI_Type                 MIXSPI_Type
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


#endif /* _MFB_ADAPTER_H_ */
