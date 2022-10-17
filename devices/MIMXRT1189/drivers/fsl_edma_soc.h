/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FSL_EDMA_SOC_H_
#define _FSL_EDMA_SOC_H_

#include "fsl_common.h"

/*!
 * @addtogroup edma_soc
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @name Driver version */
/*@{*/
/*! @brief Driver version 2.0.0. */
#define FSL_EDMA_SOC_DRIVER_VERSION (MAKE_VERSION(2, 0, 0))
/*@}*/

/*!@brief DMA IP version */
#define FSL_EDMA_SOC_IP_DMA3 (1)
#define FSL_EDMA_SOC_IP_DMA4 (1)

/*!@brief DMA base table */
#define EDMA_BASE_PTRS \
    {                  \
        DMA3, DMA4     \
    }

#define EDMA_CHN_IRQS                                                                                             \
    {                                                                                                             \
        {DMA3_CH0_IRQn,                                                                                           \
         DMA3_CH1_IRQn,                                                                                           \
         DMA3_CH2_IRQn,                                                                                           \
         DMA3_CH3_IRQn,                                                                                           \
         DMA3_CH4_IRQn,                                                                                           \
         DMA3_CH5_IRQn,                                                                                           \
         DMA3_CH6_IRQn,                                                                                           \
         DMA3_CH7_IRQn,                                                                                           \
         DMA3_CH8_IRQn,                                                                                           \
         DMA3_CH9_IRQn,                                                                                           \
         DMA3_CH10_IRQn,                                                                                          \
         DMA3_CH11_IRQn,                                                                                          \
         DMA3_CH12_IRQn,                                                                                          \
         DMA3_CH13_IRQn,                                                                                          \
         DMA3_CH14_IRQn,                                                                                          \
         DMA3_CH15_IRQn,                                                                                          \
         DMA3_CH16_IRQn,                                                                                          \
         DMA3_CH17_IRQn,                                                                                          \
         DMA3_CH18_IRQn,                                                                                          \
         DMA3_CH19_IRQn,                                                                                          \
         DMA3_CH20_IRQn,                                                                                          \
         DMA3_CH21_IRQn,                                                                                          \
         DMA3_CH22_IRQn,                                                                                          \
         DMA3_CH23_IRQn,                                                                                          \
         DMA3_CH24_IRQn,                                                                                          \
         DMA3_CH25_IRQn,                                                                                          \
         DMA3_CH26_IRQn,                                                                                          \
         DMA3_CH27_IRQn,                                                                                          \
         DMA3_CH28_SINC3_CH0_IRQn,                                                                                \
         DMA3_CH29_SINC3_CH1_IRQn,                                                                                \
         DMA3_CH30_SINC3_CH2_IRQn,                                                                                \
         DMA3_CH31_SINC3_CH3_IRQn},                                                                               \
        {                                                                                                         \
            DMA4_CH0_CH1_IRQn, DMA4_CH0_CH1_IRQn, DMA4_CH2_CH3_IRQn, DMA4_CH2_CH3_IRQn, DMA4_CH4_CH5_IRQn,        \
                DMA4_CH4_CH5_IRQn, DMA4_CH6_CH7_IRQn, DMA4_CH6_CH7_IRQn, DMA4_CH8_CH9_IRQn, DMA4_CH8_CH9_IRQn,    \
                DMA4_CH10_CH11_IRQn, DMA4_CH10_CH11_IRQn, DMA4_CH12_CH13_IRQn, DMA4_CH12_CH13_IRQn,               \
                DMA4_CH14_CH15_IRQn, DMA4_CH14_CH15_IRQn, DMA4_CH16_CH17_IRQn, DMA4_CH16_CH17_IRQn,               \
                DMA4_CH18_CH19_IRQn, DMA4_CH18_CH19_IRQn, DMA4_CH20_CH21_IRQn, DMA4_CH20_CH21_IRQn,               \
                DMA4_CH22_CH23_IRQn, DMA4_CH22_CH23_IRQn, DMA4_CH24_CH25_IRQn, DMA4_CH24_CH25_IRQn,               \
                DMA4_CH26_CH27_IRQn, DMA4_CH26_CH27_IRQn, DMA4_CH28_CH29_IRQn, DMA4_CH28_CH29_IRQn,               \
                DMA4_CH30_CH31_IRQn, DMA4_CH30_CH31_IRQn, DMA4_CH32_CH33_IRQn, DMA4_CH34_CH35_IRQn,               \
                DMA4_CH36_CH37_IRQn, DMA4_CH38_CH39_IRQn, DMA4_CH40_CH41_IRQn, DMA4_CH42_CH43_IRQn,               \
                DMA4_CH44_CH45_IRQn, DMA4_CH46_CH47_IRQn, DMA4_CH48_CH49_LPI2C5_IRQn, DMA4_CH50_CH51_LPI2C6_IRQn, \
                DMA4_CH52_CH53_SAI4_IRQn, DMA4_CH54_CH55_SPDIF_IRQn, DMA4_CH56_CH57_LPUART9_IRQn,                 \
                DMA4_CH58_CH59_LPUART10_IRQn, DMA4_CH60_CH61_LPUART11_IRQn, DMA4_CH62_CH63_LPUART12_IRQn          \
        }                                                                                                         \
    }

/*!@brief dma request source */
typedef enum _dma_request_source
{
    kDmaRequestDisabled                            = 0U,           /**< DSisabled*/
    kDma3RequestMuxCAN1                            = 1 | 0x100U,   /**< CAN1 */
    kDma3RequestMuxGPIO1Request0                   = 3 | 0x100U,   /**< GPIO1 channel 0 */
    kDma3RequestMuxGPIO1Request1                   = 4 | 0x100U,   /**< GPIO1 channel 1 */
    kDma3RequestMuxI3C1ToBusRequest                = 5 | 0x100U,   /**< I3C1 To-bus Request */
    kDma3RequestMuxI3C1FromBusRequest              = 6 | 0x100U,   /**< I3C1 From-bus Request */
    kDma3RequestMuxLPI2C1Tx                        = 7 | 0x100U,   /**< LPI2C1 */
    kDma3RequestMuxLPI2C1Rx                        = 8 | 0x100U,   /**< LPI2C1 */
    kDma3RequestMuxLPI2C2Tx                        = 9 | 0x100U,   /**< LPI2C2 */
    kDma3RequestMuxLPI2C2Rx                        = 10 | 0x100U,  /**< LPI2C2 */
    kDma3RequestMuxLPSPI1Tx                        = 11 | 0x100U,  /**< LPSPI1 Transmit */
    kDma3RequestMuxLPSPI1Rx                        = 12 | 0x100U,  /**< LPSPI1 Receive */
    kDma3RequestMuxLPSPI2Tx                        = 13 | 0x100U,  /**< LPSPI2 Transmit */
    kDma3RequestMuxLPSPI2Rx                        = 14 | 0x100U,  /**< LPSPI2 Receive */
    kDma3RequestMuxLPTMR1Request                   = 15 | 0x100U,  /**< LPTMR1 Request */
    kDma3RequestMuxLPUART1Tx                       = 16 | 0x100U,  /**< LPUART1 Transmit */
    kDma3RequestMuxLPUART1Rx                       = 17 | 0x100U,  /**< LPUART1 Receive */
    kDma3RequestMuxLPUART2Tx                       = 18 | 0x100U,  /**< LPUART2 Transmit */
    kDma3RequestMuxLPUART2Rx                       = 19 | 0x100U,  /**< LPUART2 Receive */
    kDma3RequestMuxEdgelockRequest                 = 20 | 0x100U,  /**< Edgelock enclave DMA Request */
    kDma3RequestMuxSai1Tx                          = 21 | 0x100U,  /**< SAI1 Transmit */
    kDma3RequestMuxSai1Rx                          = 22 | 0x100U,  /**< SAI1 Receive */
    kDma3RequestMuxTPM1Request0Request2            = 23 | 0x100U,  /**< TPM1 request 0 and request 2 */
    kDma3RequestMuxTPM1Request1Request3            = 24 | 0x100U,  /**< TPM1 request 1 and request 3 */
    kDma3RequestMuxTPM1OverflowRequest             = 25 | 0x100U,  /**< TPM1 Overflow request */
    kDma3RequestMuxTPM2Request0Request2            = 26 | 0x100U,  /**< TPM2 request 0 and request 2 */
    kDma3RequestMuxTPM2Request1Request3            = 27 | 0x100U,  /**< TPM2 request 1 and request 3 */
    kDma3RequestMuxTPM2OverflowRequest             = 28 | 0x100U,  /**< TPM2 Overflow request */
    kDma3RequestMuxLPUART7Tx                       = 29 | 0x100U,  /**< LPUART7 Transmit */
    kDma3RequestMuxLPUART7Rx                       = 30 | 0x100U,  /**< LPUART7 Receive */
    kDma3RequestMuxLPUART12Tx                      = 31 | 0x100U,  /**< LPUART12 Transmit */
    kDma3RequestMuxLPUART12Rx                      = 32 | 0x100U,  /**< LPUART12 Receive */
    kDma3RequestMuxFlexSPI2Tx                      = 33 | 0x100U,  /**< FlexSPI2 Transmit */
    kDma3RequestMuxFlexSPI2Rx                      = 34 | 0x100U,  /**< FlexSPI2 Receive */
    kDma3RequestMuxMotorControlRequest1            = 35 | 0x100U,  /**< Motor control request 1 */
    kDma3RequestMuxMotorControlRequest2            = 36 | 0x100U,  /**< Motor control request 2 */
    kDma3RequestMuxMotorControlRequest3            = 37 | 0x100U,  /**< Motor Control request 3 */
    kDma3RequestMuxCAN3                            = 38 | 0x100U,  /**< CAN3 */
    kDma4RequestMuxCAN2                            = 1 | 0x200U,   /**< CAN2 */
    kDma4RequestMuxGPIO2Request0                   = 2 | 0x200U,   /**< GPIO2 channel 0 */
    kDma4RequestMuxGPIO2Request1                   = 3 | 0x200U,   /**< GPIO2 channel 1 */
    kDma4RequestMuxGPIO3Request0                   = 4 | 0x200U,   /**< GPIO3 channel 0 */
    kDma4RequestMuxGPIO3Request1                   = 5 | 0x200U,   /**< GPIO3 channel 1 */
    kDma4RequestMuxI3C2ToBusRequest                = 6 | 0x200U,   /**< I3C2 To-bus Request */
    kDma4RequestMuxI3C2FromBusRequest              = 7 | 0x200U,   /**< I3C2 From-bus Request */
    kDma4RequestMuxLPI2C3Tx                        = 8 | 0x200U,   /**< LPI2C3 */
    kDma4RequestMuxLPI2C3Rx                        = 9 | 0x200U,   /**< LPI2C3 */
    kDma4RequestMuxLPI2C4Tx                        = 10 | 0x200U,  /**< LPI2C4 */
    kDma4RequestMuxLPI2C4Rx                        = 11 | 0x200U,  /**< LPI2C2 */
    kDma4RequestMuxLPSPI3Tx                        = 12 | 0x200U,  /**< LPSPI3 Transmit */
    kDma4RequestMuxLPSPI3Rx                        = 13 | 0x200U,  /**< LPSPI3 Receive */
    kDma4RequestMuxLPSPI4Tx                        = 14 | 0x200U,  /**< LPSPI4 Transmit */
    kDma4RequestMuxLPSPI4Rx                        = 15 | 0x200U,  /**< LPSPI4 Receive */
    kDma4RequestMuxLPTMR2Request                   = 16 | 0x200U,  /**< LPTMR2 Request */
    kDma4RequestMuxLPUART3Tx                       = 17 | 0x200U,  /**< LPUART3 Transmit */
    kDma4RequestMuxLPUART3Rx                       = 18 | 0x200U,  /**< LPUART3 Receive */
    kDma4RequestMuxLPUART4Tx                       = 19 | 0x200U,  /**< LPUART4 Transmit */
    kDma4RequestMuxLPUART4Rx                       = 20 | 0x200U,  /**< LPUART4 Receive */
    kDma4RequestMuxLPUART5Tx                       = 21 | 0x200U,  /**< LPUART5 Transmit */
    kDma4RequestMuxLPUART5Rx                       = 22 | 0x200U,  /**< LPUART5 Receive */
    kDma4RequestMuxLPUART6Tx                       = 23 | 0x200U,  /**< LPUART6 Transmit */
    kDma4RequestMuxLPUART6Rx                       = 24 | 0x200U,  /**< LPUART6 Receive */
    kDma4RequestMuxTPM3Request0Request2            = 25 | 0x200U,  /**< TPM3 request 0 and request 2 */
    kDma4RequestMuxTPM3Request1Request3            = 26 | 0x200U,  /**< TPM3 request 1 and request 3 */
    kDma4RequestMuxTPM3OverflowRequest             = 27 | 0x200U,  /**< TPM3 Overflow request */
    kDma4RequestMuxTPM4Request0Request2            = 28 | 0x200U,  /**< TPM4 request 0 and request 2 */
    kDma4RequestMuxTPM4Request1Request3            = 29 | 0x200U,  /**< TPM4 request 1 and request 3 */
    kDma4RequestMuxTPM4OverflowRequest             = 30 | 0x200U,  /**< TPM4 Overflow request */
    kDma4RequestMuxTPM5Request0Request2            = 31 | 0x200U,  /**< TPM5 request 0 and request 2 */
    kDma4RequestMuxTPM5Request1Request3            = 32 | 0x200U,  /**< TPM5 request 1 and request 3 */
    kDma4RequestMuxTPM5OverflowRequest             = 33 | 0x200U,  /**< TPM5 Overflow request */
    kDma4RequestMuxTPM6Request0Request2            = 34 | 0x200U,  /**< TPM6 request 0 and request 2 */
    kDma4RequestMuxTPM6Request1Request3            = 35 | 0x200U,  /**< TPM6 request 1 and request 3 */
    kDma4RequestMuxTPM6OverflowRequest             = 36 | 0x200U,  /**< TPM6 Overflow request */
    kDma4RequestMuxFlexIO1Request0                 = 37 | 0x200U,  /**< FlexIO1 Request0 */
    kDma4RequestMuxFlexIO1Request1                 = 38 | 0x200U,  /**< FlexIO1 Request1 */
    kDma4RequestMuxFlexIO1Request2                 = 39 | 0x200U,  /**< FlexIO1 Request2 */
    kDma4RequestMuxFlexIO1Request3                 = 40 | 0x200U,  /**< FlexIO1 Request3 */
    kDma4RequestMuxFlexIO1Request4                 = 41 | 0x200U,  /**< FlexIO1 Request4 */
    kDma4RequestMuxFlexIO1Request5                 = 42 | 0x200U,  /**< FlexIO1 Request5 */
    kDma4RequestMuxFlexIO1Request6                 = 43 | 0x200U,  /**< FlexIO1 Request6 */
    kDma4RequestMuxFlexIO1Request7                 = 44 | 0x200U,  /**< FlexIO1 Request7 */
    kDma4RequestMuxFlexIO2Request0                 = 45 | 0x200U,  /**< FlexIO2 Request0 */
    kDma4RequestMuxFlexIO2Request1                 = 46 | 0x200U,  /**< FlexIO2 Request1 */
    kDma4RequestMuxFlexIO2Request2                 = 47 | 0x200U,  /**< FlexIO2 Request2 */
    kDma4RequestMuxFlexIO2Request3                 = 48 | 0x200U,  /**< FlexIO2 Request3 */
    kDma4RequestMuxFlexIO2Request4                 = 49 | 0x200U,  /**< FlexIO2 Request4 */
    kDma4RequestMuxFlexIO2Request5                 = 50 | 0x200U,  /**< FlexIO2 Request5 */
    kDma4RequestMuxFlexIO2Request6                 = 51 | 0x200U,  /**< FlexIO2 Request6 */
    kDma4RequestMuxFlexIO2Request7                 = 52 | 0x200U,  /**< FlexIO2 Request7 */
    kDma4RequestMuxFlexSPI1Tx                      = 53 | 0x200U,  /**< FlexSPI1 Transmit */
    kDma4RequestMuxFlexSPI1Rx                      = 54 | 0x200U,  /**< FlexSPI1 Receive */
    kDma4RequestMuxADC1Request0                    = 57 | 0x200U,  /**< ADC1 Reqest 0 */
    kDma4RequestMuxFlexPWM1CaptureSub0             = 58 | 0x200U,  /**< FlexPWM1 Capture sub-module0 */
    kDma4RequestMuxFlexPWM1CaptureSub1             = 59 | 0x200U,  /**< FlexPWM1 Capture sub-module1 */
    kDma4RequestMuxFlexPWM1CaptureSub2             = 60 | 0x200U,  /**< FlexPWM1 Capture sub-module2 */
    kDma4RequestMuxFlexPWM1CaptureSub3             = 61 | 0x200U,  /**< FlexPWM1 Capture sub-module3 */
    kDma4RequestMuxFlexPWM1ValueSub0               = 62 | 0x200U,  /**< FlexPWM1 Value sub-module 0 */
    kDma4RequestMuxFlexPWM1ValueSub1               = 63 | 0x200U,  /**< FlexPWM1 Value sub-module 1 */
    kDma4RequestMuxFlexPWM1ValueSub2               = 64 | 0x200U,  /**< FlexPWM1 Value sub-module 2 */
    kDma4RequestMuxFlexPWM1ValueSub3               = 65 | 0x200U,  /**< FlexPWM1 Value sub-module 3 */
    kDma4RequestMuxFlexPWM2CaptureSub0             = 66 | 0x200U,  /**< FlexPWM2 Capture sub-module0 */
    kDma4RequestMuxFlexPWM2CaptureSub1             = 67 | 0x200U,  /**< FlexPWM2 Capture sub-module1 */
    kDma4RequestMuxFlexPWM2CaptureSub2             = 68 | 0x200U,  /**< FlexPWM2 Capture sub-module2 */
    kDma4RequestMuxFlexPWM2CaptureSub3             = 69 | 0x200U,  /**< FlexPWM2 Capture sub-module3 */
    kDma4RequestMuxFlexPWM2ValueSub0               = 70 | 0x200U,  /**< FlexPWM2 Value sub-module 0 */
    kDma4RequestMuxFlexPWM2ValueSub1               = 71 | 0x200U,  /**< FlexPWM2 Value sub-module 1 */
    kDma4RequestMuxFlexPWM2ValueSub2               = 72 | 0x200U,  /**< FlexPWM2 Value sub-module 2 */
    kDma4RequestMuxFlexPWM2ValueSub3               = 73 | 0x200U,  /**< FlexPWM2 Value sub-module 3 */
    kDma4RequestMuxFlexPWM3CaptureSub0             = 74 | 0x200U,  /**< FlexPWM3 Capture sub-module0 */
    kDma4RequestMuxFlexPWM3CaptureSub1             = 75 | 0x200U,  /**< FlexPWM3 Capture sub-module1 */
    kDma4RequestMuxFlexPWM3CaptureSub2             = 76 | 0x200U,  /**< FlexPWM3 Capture sub-module2 */
    kDma4RequestMuxFlexPWM3CaptureSub3             = 77 | 0x200U,  /**< FlexPWM3 Capture sub-module3 */
    kDma4RequestMuxFlexPWM3ValueSub0               = 78 | 0x200U,  /**< FlexPWM3 Value sub-module 0 */
    kDma4RequestMuxFlexPWM3ValueSub1               = 79 | 0x200U,  /**< FlexPWM3 Value sub-module 1 */
    kDma4RequestMuxFlexPWM3ValueSub2               = 80 | 0x200U,  /**< FlexPWM3 Value sub-module 2 */
    kDma4RequestMuxFlexPWM3ValueSub3               = 81 | 0x200U,  /**< FlexPWM3 Value sub-module 3 */
    kDma4RequestMuxFlexPWM4CaptureSub0             = 82 | 0x200U,  /**< FlexPWM4 Capture sub-module0 */
    kDma4RequestMuxFlexPWM4CaptureSub1             = 83 | 0x200U,  /**< FlexPWM4 Capture sub-module1 */
    kDma4RequestMuxFlexPWM4CaptureSub2             = 84 | 0x200U,  /**< FlexPWM4 Capture sub-module2 */
    kDma4RequestMuxFlexPWM4CaptureSub3             = 85 | 0x200U,  /**< FlexPWM4 Capture sub-module3 */
    kDma4RequestMuxFlexPWM4ValueSub0               = 86 | 0x200U,  /**< FlexPWM4 Value sub-module 0 */
    kDma4RequestMuxFlexPWM4ValueSub1               = 87 | 0x200U,  /**< FlexPWM4 Value sub-module 1 */
    kDma4RequestMuxFlexPWM4ValueSub2               = 88 | 0x200U,  /**< FlexPWM4 Value sub-module 2 */
    kDma4RequestMuxFlexPWM4ValueSub3               = 89 | 0x200U,  /**< FlexPWM4 Value sub-module 3 */
    kDma4RequestMuxQTIMER1CaptTimer0               = 90 | 0x200U,  /**< TMR1 Capture timer 0 */
    kDma4RequestMuxQTIMER1CaptTimer1               = 91 | 0x200U,  /**< TMR1 Capture timer 1 */
    kDma4RequestMuxQTIMER1CaptTimer2               = 92 | 0x200U,  /**< TMR1 Capture timer 2 */
    kDma4RequestMuxQTIMER1CaptTimer3               = 93 | 0x200U,  /**< TMR1 Capture timer 3 */
    kDma4RequestMuxQTIMER1Cmpld1Timer0Cmpld2Timer1 = 94 | 0x200U,  /**< TMR1 cmpld1 in timer 0 or cmpld2 in timer 1 */
    kDma4RequestMuxQTIMER1Cmpld1Timer1Cmpld2Timer0 = 95 | 0x200U,  /**< TMR1 cmpld1 in timer 1 or cmpld2 in timer 0 */
    kDma4RequestMuxQTIMER1Cmpld1Timer2Cmpld2Timer3 = 96 | 0x200U,  /**< TMR1 cmpld1 in timer 2 or cmpld2 in timer 3 */
    kDma4RequestMuxQTIMER1Cmpld1Timer3Cmpld2Timer2 = 97 | 0x200U,  /**< TMR1 cmpld1 in timer 3 or cmpld2 in timer 2 */
    kDma4RequestMuxQTIMER2CaptTimer0               = 98 | 0x200U,  /**< TMR2 Capture timer 0 */
    kDma4RequestMuxQTIMER2CaptTimer1               = 99 | 0x200U,  /**< TMR2 Capture timer 1 */
    kDma4RequestMuxQTIMER2CaptTimer2               = 100 | 0x200U, /**< TMR2 Capture timer 2 */
    kDma4RequestMuxQTIMER2CaptTimer3               = 101 | 0x200U, /**< TMR2 Capture timer 3 */
    kDma4RequestMuxQTIMER2Cmpld1Timer0Cmpld2Timer1 = 102 | 0x200U, /**< TMR2 cmpld1 in timer 0 or cmpld2 in timer 1 */
    kDma4RequestMuxQTIMER2Cmpld1Timer1Cmpld2Timer0 = 103 | 0x200U, /**< TMR2 cmpld1 in timer 1 or cmpld2 in timer 0 */
    kDma4RequestMuxQTIMER2Cmpld1Timer2Cmpld2Timer3 = 104 | 0x200U, /**< TMR2 cmpld1 in timer 2 or cmpld2 in timer 3 */
    kDma4RequestMuxQTIMER2Cmpld1Timer3Cmpld2Timer2 = 105 | 0x200U, /**< TMR2 cmpld1 in timer 3 or cmpld2 in timer 2 */
    kDma4RequestMuxQTIMER3CaptTimer0               = 106 | 0x200U, /**< TMR3 Capture timer 0 */
    kDma4RequestMuxQTIMER3CaptTimer1               = 107 | 0x200U, /**< TMR3 Capture timer 1 */
    kDma4RequestMuxQTIMER3CaptTimer2               = 108 | 0x200U, /**< TMR3 Capture timer 2 */
    kDma4RequestMuxQTIMER3CaptTimer3               = 109 | 0x200U, /**< TMR3 Capture timer 3 */
    kDma4RequestMuxQTIMER3Cmpld1Timer0Cmpld2Timer1 = 110 | 0x200U, /**< TMR3 cmpld1 in timer 0 or cmpld2 in timer 1 */
    kDma4RequestMuxQTIMER3Cmpld1Timer1Cmpld2Timer0 = 111 | 0x200U, /**< TMR3 cmpld1 in timer 1 or cmpld2 in timer 0 */
    kDma4RequestMuxQTIMER3Cmpld1Timer2Cmpld2Timer3 = 112 | 0x200U, /**< TMR3 cmpld1 in timer 2 or cmpld2 in timer 3 */
    kDma4RequestMuxQTIMER3Cmpld1Timer3Cmpld2Timer2 = 113 | 0x200U, /**< TMR3 cmpld1 in timer 3 or cmpld2 in timer 2 */
    kDma4RequestMuxQTIMER4CaptTimer0               = 114 | 0x200U, /**< TMR4 Capture timer 0 */
    kDma4RequestMuxQTIMER4CaptTimer1               = 115 | 0x200U, /**< TMR4 Capture timer 1 */
    kDma4RequestMuxQTIMER4CaptTimer2               = 116 | 0x200U, /**< TMR4 Capture timer 2 */
    kDma4RequestMuxQTIMER4CaptTimer3               = 117 | 0x200U, /**< TMR4 Capture timer 3 */
    kDma4RequestMuxQTIMER4Cmpld1Timer0Cmpld2Timer1 = 118 | 0x200U, /**< TMR4 cmpld1 in timer 0 or cmpld2 in timer 1 */
    kDma4RequestMuxQTIMER4Cmpld1Timer1Cmpld2Timer0 = 119 | 0x200U, /**< TMR4 cmpld1 in timer 1 or cmpld2 in timer 0 */
    kDma4RequestMuxQTIMER4Cmpld1Timer2Cmpld2Timer3 = 120 | 0x200U, /**< TMR4 cmpld1 in timer 2 or cmpld2 in timer 3 */
    kDma4RequestMuxQTIMER4Cmpld1Timer3Cmpld2Timer2 = 121 | 0x200U, /**< TMR4 cmpld1 in timer 3 or cmpld2 in timer 2 */
    kDma4RequestMuxQTIMER5CaptTimer0               = 122 | 0x200U, /**< TMR5 Capture timer 0 */
    kDma4RequestMuxQTIMER5CaptTimer1               = 123 | 0x200U, /**< TMR5 Capture timer 1 */
    kDma4RequestMuxQTIMER5CaptTimer2               = 124 | 0x200U, /**< TMR5 Capture timer 2 */
    kDma4RequestMuxQTIMER5CaptTimer3               = 125 | 0x200U, /**< TMR5 Capture timer 3 */
    kDma4RequestMuxQTIMER5Cmpld1Timer0Cmpld2Timer1 = 126 | 0x200U, /**< TMR5 cmpld1 in timer 0 or cmpld2 in timer 1 */
    kDma4RequestMuxQTIMER5Cmpld1Timer1Cmpld2Timer0 = 127 | 0x200U, /**< TMR5 cmpld1 in timer 1 or cmpld2 in timer 0 */
    kDma4RequestMuxQTIMER5Cmpld1Timer2Cmpld2Timer3 = 128 | 0x200U, /**< TMR5 cmpld1 in timer 2 or cmpld2 in timer 3 */
    kDma4RequestMuxQTIMER5Cmpld1Timer3Cmpld2Timer2 = 129 | 0x200U, /**< TMR5 cmpld1 in timer 3 or cmpld2 in timer 2 */
    kDma4RequestMuxQTIMER6CaptTimer0               = 130 | 0x200U, /**< TMR6 Capture timer 0 */
    kDma4RequestMuxQTIMER6CaptTimer1               = 131 | 0x200U, /**< TMR6 Capture timer 1 */
    kDma4RequestMuxQTIMER6CaptTimer2               = 132 | 0x200U, /**< TMR6 Capture timer 2 */
    kDma4RequestMuxQTIMER6CaptTimer3               = 133 | 0x200U, /**< TMR6 Capture timer 3 */
    kDma4RequestMuxQTIMER6Cmpld1Timer0Cmpld2Timer1 = 134 | 0x200U, /**< TMR6 cmpld1 in timer 0 or cmpld2 in timer 1 */
    kDma4RequestMuxQTIMER6Cmpld1Timer1Cmpld2Timer0 = 135 | 0x200U, /**< TMR6 cmpld1 in timer 1 or cmpld2 in timer 0 */
    kDma4RequestMuxQTIMER6Cmpld1Timer2Cmpld2Timer3 = 136 | 0x200U, /**< TMR6 cmpld1 in timer 2 or cmpld2 in timer 3 */
    kDma4RequestMuxQTIMER6Cmpld1Timer3Cmpld2Timer2 = 137 | 0x200U, /**< TMR6 cmpld1 in timer 3 or cmpld2 in timer 2 */
    kDma4RequestMuxQTIMER7CaptTimer0               = 138 | 0x200U, /**< TMR7 Capture timer 0 */
    kDma4RequestMuxQTIMER7CaptTimer1               = 139 | 0x200U, /**< TMR7 Capture timer 1 */
    kDma4RequestMuxQTIMER7CaptTimer2               = 140 | 0x200U, /**< TMR7 Capture timer 2 */
    kDma4RequestMuxQTIMER7CaptTimer3               = 141 | 0x200U, /**< TMR7 Capture timer 3 */
    kDma4RequestMuxQTIMER7Cmpld1Timer0Cmpld2Timer1 = 142 | 0x200U, /**< TMR7 cmpld1 in timer 0 or cmpld2 in timer 1 */
    kDma4RequestMuxQTIMER7Cmpld1Timer1Cmpld2Timer0 = 143 | 0x200U, /**< TMR7 cmpld1 in timer 1 or cmpld2 in timer 0 */
    kDma4RequestMuxQTIMER7Cmpld1Timer2Cmpld2Timer3 = 144 | 0x200U, /**< TMR7 cmpld1 in timer 2 or cmpld2 in timer 3 */
    kDma4RequestMuxQTIMER7Cmpld1Timer3Cmpld2Timer2 = 145 | 0x200U, /**< TMR7 cmpld1 in timer 3 or cmpld2 in timer 2 */
    kDma4RequestMuxQTIMER8CaptTimer0               = 146 | 0x200U, /**< TMR8 Capture timer 0 */
    kDma4RequestMuxQTIMER8CaptTimer1               = 147 | 0x200U, /**< TMR8 Capture timer 1 */
    kDma4RequestMuxQTIMER8CaptTimer2               = 148 | 0x200U, /**< TMR8 Capture timer 2 */
    kDma4RequestMuxQTIMER8CaptTimer3               = 149 | 0x200U, /**< TMR8 Capture timer 3 */
    kDma4RequestMuxQTIMER8Cmpld1Timer0Cmpld2Timer1 = 150 | 0x200U, /**< TMR8 cmpld1 in timer 0 or cmpld2 in timer 1 */
    kDma4RequestMuxQTIMER8Cmpld1Timer1Cmpld2Timer0 = 151 | 0x200U, /**< TMR8 cmpld1 in timer 1 or cmpld2 in timer 0 */
    kDma4RequestMuxQTIMER8Cmpld1Timer2Cmpld2Timer3 = 152 | 0x200U, /**< TMR8 cmpld1 in timer 2 or cmpld2 in timer 3 */
    kDma4RequestMuxQTIMER8Cmpld1Timer3Cmpld2Timer2 = 153 | 0x200U, /**< TMR8 cmpld1 in timer 3 or cmpld2 in timer 2 */
    kDma4RequestMuxXBAR1Request0                   = 154 | 0x200U, /**< XBAR1 Request 0 */
    kDma4RequestMuxXBAR1Request1                   = 155 | 0x200U, /**< XBAR1 Request 1 */
    kDma4RequestMuxXBAR1Request2                   = 156 | 0x200U, /**< XBAR1 Request 2 */
    kDma4RequestMuxXBAR1Request3                   = 157 | 0x200U, /**< XBAR1 Request 3 */
    kDma4RequestMuxADC2Request0                    = 158 | 0x200U, /**< ADC2 Reqest 0 */
    kDma4RequestMuxEQDC1                           = 159 | 0x200U, /**< EQDC1 */
    kDma4RequestMuxEQDC2                           = 160 | 0x200U, /**< EQDC2 */
    kDma4RequestMuxEQDC3                           = 161 | 0x200U, /**< EQDC3 */
    kDma4RequestMuxEQDC4                           = 162 | 0x200U, /**< EQDC4 */
    kDma4RequestMuxLPI2C5Tx                        = 163 | 0x200U, /**< LPI2C5 */
    kDma4RequestMuxLPI2C5Rx                        = 164 | 0x200U, /**< LPI2C5 */
    kDma4RequestMuxLPI2C2Tx                        = 165 | 0x200U, /**< LPI2C6 */
    kDma4RequestMuxLPI2C6Rx                        = 166 | 0x200U, /**< LPI2C6 */
    kDma4RequestMuxLPSPI5Tx                        = 167 | 0x200U, /**< LPSPI5 Transmit */
    kDma4RequestMuxLPSPI5Rx                        = 168 | 0x200U, /**< LPSPI5 Receive */
    kDma4RequestMuxLPSPI6Tx                        = 169 | 0x200U, /**< LPSPI6 Transmit */
    kDma4RequestMuxLPSPI6Rx                        = 170 | 0x200U, /**< LPSPI6 Receive */
    kDma4RequestMuxLPTMR3Request                   = 171 | 0x200U, /**< LPTMR3 Request */
    kDma4RequestMuxLPUART9Tx                       = 172 | 0x200U, /**< LPUART9 Transmit */
    kDma4RequestMuxLPUART9Rx                       = 173 | 0x200U, /**< LPUART9 Receive */
    kDma4RequestMuxLPUART10Tx                      = 174 | 0x200U, /**< LPUART10 Transmit */
    kDma4RequestMuxLPUART10Rx                      = 175 | 0x200U, /**< LPUART10 Receive */
    kDma4RequestMuxLPUART11Tx                      = 176 | 0x200U, /**< LPUART11 Transmit */
    kDma4RequestMuxLPUART11Rx                      = 177 | 0x200U, /**< LPUART11 Receive */
    kDma4RequestMuxLPUART8Tx                       = 178 | 0x200U, /**< LPUART8 Transmit */
    kDma4RequestMuxLPUART8Rx                       = 179 | 0x200U, /**< LPUART8 Receive */
    kDma4RequestMuxSai2Tx                          = 180 | 0x200U, /**< SAI2 Transmit */
    kDma4RequestMuxSai2Rx                          = 181 | 0x200U, /**< SAI2 Receive */
    kDma4RequestMuxSai3Tx                          = 182 | 0x200U, /**< SAI3 Transmit */
    kDma4RequestMuxSai3Rx                          = 183 | 0x200U, /**< SAI3 Receive */
    kDma4RequestMuxSai4Tx                          = 184 | 0x200U, /**< SAI4 Transmit */
    kDma4RequestMuxSai4Rx                          = 185 | 0x200U, /**< SAI4 Receive */
    kDma4RequestMuxDAC                             = 186 | 0x200U, /**< DAC */
    kDma4RequestMuxACMP1                           = 187 | 0x200U, /**< ACMP1 */
    kDma4RequestMuxACMP2                           = 188 | 0x200U, /**< ACMP2 */
    kDma4RequestMuxACMP3                           = 189 | 0x200U, /**< ACMP3 */
    kDma4RequestMuxACMP4                           = 190 | 0x200U, /**< ACMP4 */
    kDma4RequestMuxASRCRequest1                    = 191 | 0x200U, /**< ASRC request 1 pair A input request */
    kDma4RequestMuxASRCRequest2                    = 192 | 0x200U, /**< ASRC request 2 pair B input request */
    kDma4RequestMuxASRCRequest3                    = 193 | 0x200U, /**< ASRC request 3 pair C input request */
    kDma4RequestMuxASRCRequest4                    = 194 | 0x200U, /**< ASRC request 4 pair A output request */
    kDma4RequestMuxASRCRequest5                    = 195 | 0x200U, /**< ASRC request 5 pair B output request */
    kDma4RequestMuxASRCRequest6                    = 196 | 0x200U, /**< ASRC request 6 pair C output request */
    kDma4RequestMuxSpdifRx                         = 197 | 0x200U, /**< SPDIF Receive */
    kDma4RequestMuxSpdifTx                         = 198 | 0x200U, /**< SPDIF Transmit */
    kDma4RequestMuxPdmTx                           = 199 | 0x200U, /**< PDM */
    kDma4RequestMuxGPIO4Request0                   = 200 | 0x200U, /**< GPIO4 channel 0 */
    kDma4RequestMuxGPIO4Request1                   = 201 | 0x200U, /**< GPIO4 channel 1 */
    kDma4RequestMuxGPIO5Request0                   = 202 | 0x200U, /**< GPIO5 channel 0 */
    kDma4RequestMuxGPIO5Request1                   = 203 | 0x200U, /**< GPIO5 channel 1 */
    kDma4RequestMuxGPIO6Request0                   = 204 | 0x200U, /**< GPIO6 channel 0 */
    kDma4RequestMuxGPIO6Request1                   = 205 | 0x200U, /**< GPIO6 channel 1 */
    kDma4RequestMuxSINC1Request0                   = 208 | 0x200U, /**< SINC1 Reqest 0 */
    kDma4RequestMuxSINC1Request1                   = 209 | 0x200U, /**< SINC1 Reqest 1 */
    kDma4RequestMuxSINC1Request2                   = 210 | 0x200U, /**< SINC1 Reqest 2 */
    kDma4RequestMuxSINC1Request3                   = 211 | 0x200U, /**< SINC1 Reqest 3 */
    kDma4RequestMuxSINC2Request0                   = 212 | 0x200U, /**< SINC2 Reqest 0 */
    kDma4RequestMuxSINC2Request1                   = 213 | 0x200U, /**< SINC2 Reqest 1 */
    kDma4RequestMuxSINC2Request2                   = 214 | 0x200U, /**< SINC2 Reqest 2 */
    kDma4RequestMuxSINC2Request3                   = 215 | 0x200U, /**< SINC2 Reqest 3 */
    kDma4RequestMuxSINC3Request0                   = 216 | 0x200U, /**< SINC3 Reqest 0 */
    kDma4RequestMuxSINC3Request1                   = 217 | 0x200U, /**< SINC3 Reqest 1 */
    kDma4RequestMuxSINC3Request2                   = 218 | 0x200U, /**< SINC3 Reqest 2 */
    kDma4RequestMuxSINC3Request3                   = 219 | 0x200U, /**< SINC3 Reqest 3 */
    kDma4RequestMuxADC1Request1                    = 220 | 0x200U, /**< ADC1 Reqest 1 */
    kDma4RequestMuxADC2Request1                    = 221 | 0x200U, /**< ADC2 Reqest 1 */
} dma_request_source_t;

/*!< Verify dma base and request source */
#define EDMA_CHANNEL_HAS_REQUEST_SOURCE(base, source) ((base) == DMA3 ? ((source)&0x100U) : ((source)&0x200U))

#define FSL_FEATURE_EDMA_MODULE_CHANNEL(base)                    (base == DMA3 ? 32U : 64U)
#define FSL_FEATURE_EDMA_MODULE_MAX_CHANNEL                      (64)
#define FSL_FEATURE_EDMA_HAS_GLOBAL_MASTER_ID_REPLICATION        (1)
#define FSL_FEATURE_EDMA_HAS_CONTINUOUS_LINK_MODE                (0)
#define FSL_FEATURE_EDMA_MODULE_COUNT                            (2)
#define FSL_FEATURE_EDMA_HAS_CHANNEL_CONFIG                      (1)
#define FSL_FEATURE_EDMA_HAS_CHANNEL_SWAP_SIZE                   (1)
#define FSL_FEATURE_EDMA_HAS_CHANNEL_ACCESS_TYPE                 (1)
#define FSL_FEATURE_EDMA_HAS_CHANNEL_MEMRORY_ATTRIBUTE           (1)
#define FSL_FEATURE_EDMA_HAS_CHANNEL_SIGN_EXTENSION              (1)
#define FSL_FEATURE_EDMA_MODULE_SUPPORT_MATTR(base)              (base == DMA3 ? 0U : 1U)
#define FSL_FEATURE_EDMA_MODULE_SUPPORT_SIGN_EXTENSION(base)     (base == DMA3 ? 0U : 1U)
#define FSL_FEATURE_EDMA_MODULE_SUPPORT_SWAP(base)               (base == DMA3 ? 0U : 1U)
#define FSL_FEATURE_EDMA_MODULE_SUPPORT_INSTR(base)              (base == DMA3 ? 0U : 1U)
#define FSL_FEATURE_EDMA_SUPPORT_128_BYTES_TRANSFER              (1)
#define FSL_FEATURE_EDMA_MODULE_SUPPORT_128_BYTES_TRANSFER(base) (base == DMA3 ? 0U : 1U)

/*!@brief EDMA base address convert macro */
#define EDMA_CHANNEL_OFFSET           0x10000U
#define EDMA_CHANNEL_ARRAY_STEP(base) ((base) == DMA3 ? 0x10000U : 0x8000U)

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*!
 * @}
 */

#endif /* _FSL_EDMA_SOC_H_ */
