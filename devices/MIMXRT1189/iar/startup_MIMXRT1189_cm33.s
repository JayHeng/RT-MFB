; -------------------------------------------------------------------------
;  @file:    startup_MIMXRT1189_cm33.s
;  @purpose: CMSIS Cortex-M33 Core Device Startup File
;            MIMXRT1189_cm33
;  @version: 0.1
;  @date:    2021-3-9
;  @build:   b220801
; -------------------------------------------------------------------------
;
; Copyright 1997-2016 Freescale Semiconductor, Inc.
; Copyright 2016-2022 NXP
; All rights reserved.
;
; SPDX-License-Identifier: BSD-3-Clause
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)
        SECTION HEAP:DATA:NOROOT(3)
        SECTION RW:DATA:NOROOT(2)
        SECTION QACCESS_CODE_VAR:DATA:NOROOT(3)
        SECTION QACCESS_DATA_VAR:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__iar_init$$done:              ; The vector table is not needed
                      ; until after copy initialization is done

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler                                   ;NMI Handler
        DCD     HardFault_Handler                             ;Hard Fault Handler
        DCD     MemManage_Handler                             ;MPU Fault Handler
        DCD     BusFault_Handler                              ;Bus Fault Handler
        DCD     UsageFault_Handler                            ;Usage Fault Handler
__vector_table_0x1c
        DCD     SecureFault_Handler                           ;Secure Fault Handler
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     SVC_Handler                                   ;SVCall Handler
        DCD     DebugMon_Handler                              ;Debug Monitor Handler
        DCD     0                                             ;Reserved
        DCD     PendSV_Handler                                ;PendSV Handler
        DCD     SysTick_Handler                               ;SysTick Handler

                                                              ;External Interrupts
        DCD     TMR1_IRQHandler                               ;TMR1 interrupt
        DCD     DAP_IRQHandler                                ;DAP interrupt
        DCD     M7_CTI_TRIGGER_OUTPUT_IRQHandler              ;CTI trigger outputs from CM7
        DCD     M33_CTI_TRIGGER_OUTPUT_IRQHandler             ;CTI trigger outputs from CM33
        DCD     TMR5_IRQHandler                               ;TMR5 interrupt
        DCD     TMR6_IRQHandler                               ;TMR6 interrupt
        DCD     TMR7_IRQHandler                               ;TMR7 interrupt
        DCD     TMR8_IRQHandler                               ;TMR8 interrupt
        DCD     CAN1_IRQHandler                               ;CAN1 interrupt
        DCD     CAN1_ERROR_IRQHandler                         ;CAN1 error interrupt
        DCD     GPIO1_0_IRQHandler                            ;GPIO1 interrupt 0
        DCD     GPIO1_1_IRQHandler                            ;GPIO1 interrupt 1
        DCD     I3C1_IRQHandler                               ;I3C1 interrupt
        DCD     LPI2C1_IRQHandler                             ;LPI2C1 interrupt
        DCD     LPI2C2_IRQHandler                             ;LPI2C2 interrupt
        DCD     LPIT1_IRQHandler                              ;LPIT1 interrupt
        DCD     LPSPI1_IRQHandler                             ;LPSPI1 interrupt
        DCD     LPSPI2_IRQHandler                             ;LPSPI2 interrupt
        DCD     LPTMR1_IRQHandler                             ;LPTMR1 interrupt
        DCD     LPUART1_IRQHandler                            ;LPUART1 interrupt
        DCD     LPUART2_IRQHandler                            ;LPUART2 interrupt
        DCD     MU1_IRQHandler                                ;MU1 interrupt
        DCD     MU2_IRQHandler                                ;MU2 interrupt
        DCD     PWM1_FAULT_IRQHandler                         ;PWM1 fault or reload error interrupt
        DCD     PWM1_0_IRQHandler                             ;PWM1 capture 0, compare 0, or reload 0 interrupt
        DCD     PWM1_1_IRQHandler                             ;PWM1 capture 1, compare 1, or reload 1 interrupt
        DCD     PWM1_2_IRQHandler                             ;PWM1 capture 2, compare 2, or reload 2 interrupt
        DCD     PWM1_3_IRQHandler                             ;PWM1 capture 3, compare 3, or reload 3 interrupt
        DCD     EDGELOCK_TRUST_MUA_RX_FULL_IRQHandler         ;Edgelock Trust MUA RX full interrupt
        DCD     EDGELOCK_TRUST_MUA_TX_EMPTY_IRQHandler        ;Edgelock Trust MUA TX empty interrupt
        DCD     EDGELOCK_APPS_CORE_MUA_RX_FULL_IRQHandler     ;Edgelock Apps Core MUA RX full interrupt
        DCD     EDGELOCK_APPS_CORE_MUA_TX_EMPTY_IRQHandler    ;Edgelock Apps Core MUA TX empty interrupt
        DCD     EDGELOCK_REALTIME_CORE_MUA_RX_FULL_IRQHandler    ;Edgelock Realtime Core MUA RX full interrupt
        DCD     EDGELOCK_REALTIME_CORE_MUA_TX_EMPTY_IRQHandler    ;Edgelock Realtime Core MUA TX empty interrupt
        DCD     EDGELOCK_SECURE_IRQHandler                    ;Edgelock secure interrupt
        DCD     EDGELOCK_NONSECURE_IRQHandler                 ;Edgelock non-secure interrupt
        DCD     TPM1_XBAR1_CH0_CH1_IRQHandler                 ;TPM1 interrupt, XBAR channel 0/1 interrupt
        DCD     TPM2_XBAR1_CH2_CH3_IRQHandler                 ;TPM2 interrupt, XBAR channel 2/3 interrupt
        DCD     RTWDOG1_IRQHandler                            ;RTWDOG1 interrupt
        DCD     RTWDOG2_IRQHandler                            ;RTWDOG2 interrupt
        DCD     Reserved56_IRQHandler                         ;Reserved interrupt
        DCD     PDM_HWVAD_EVENT_IRQHandler                    ;HWVAD event interrupt
        DCD     PDM_HWVAD_ERROR_IRQHandler                    ;HWVAD error interrupt
        DCD     PDM_EVENT_IRQHandler                          ;PDM event interrupt
        DCD     PDM_ERROR_IRQHandler                          ;PDM error interrupt
        DCD     SAI1_IRQHandler                               ;SAI interrupt
        DCD     CM33_PS_IRQHandler                            ;M33 PS Tag/Data Parity Error
        DCD     CM33_TCM_ECC_IRQHandler                       ;M33 TCM ECC interrupt
        DCD     CM33_TCM_ERROR_IRQHandler                     ;M33 TCM Error interrupt
        DCD     CM7_TCM_ECC_IRQHandler                        ;M7 TCM ECC interrupt
        DCD     CM7_TCM_ERROR_IRQHandler                      ;M7 TCM Error interrupt
        DCD     CAN2_IRQHandler                               ;CAN2 interrupt
        DCD     CAN2_ERROR_IRQHandler                         ;CAN2 error interrupt
        DCD     FLEXIO1_IRQHandler                            ;FLEXIO1 interrupt
        DCD     FLEXIO2_IRQHandler                            ;FLEXIO2 interrupt
        DCD     FLEXSPI1_IRQHandler                           ;FLEXSPI1 interrupt
        DCD     FLEXSPI2_IRQHandler                           ;FLEXSPI2 interrupt
        DCD     GPIO2_0_IRQHandler                            ;GPIO2 interrupt 0
        DCD     GPIO2_1_IRQHandler                            ;GPIO2 interrupt 1
        DCD     GPIO3_0_IRQHandler                            ;GPIO3 interrupt 0
        DCD     GPIO3_1_IRQHandler                            ;GPIO3 interrupt 1
        DCD     I3C2_IRQHandler                               ;I3C2 interrupt
        DCD     LPI2C3_IRQHandler                             ;LPI2C3 interrupt
        DCD     LPI2C4_IRQHandler                             ;LPI2C4 interrput
        DCD     LPIT2_IRQHandler                              ;LPIT2 interrupt
        DCD     LPSPI3_IRQHandler                             ;LPSPI3 interrupt
        DCD     LPSPI4_IRQHandler                             ;LPSPI4 interrupt
        DCD     LPTMR2_IRQHandler                             ;LPTMR2 interrupt
        DCD     LPUART3_IRQHandler                            ;LPUART3 interrupt
        DCD     LPUART4_IRQHandler                            ;LPUART4 interrupt
        DCD     LPUART5_IRQHandler                            ;LPUART5 interrupt
        DCD     LPUART6_IRQHandler                            ;LPUART6 interrupt
        DCD     ASRC_IRQHandler                               ;ASRC interrupt
        DCD     BBNSM_IRQHandler                              ;BBNSM iterrupt
        DCD     SYS_CTR1_IRQHandler                           ;System Counter compare interrupt 0 and 1
        DCD     TPM3_EWM_IRQHandler                           ;TPM3 interrupt, EWM reset out interrupt
        DCD     TPM4_SEMC_IRQHandler                          ;TPM4 interrupt, SEMC interrupt
        DCD     TPM5_LPIT3_IRQHandler                         ;TPM5 interrupt, LPIT interrupt
        DCD     TPM6_LPTMR3_IRQHandler                        ;TPM6 interrupt, LPTMR interrupt
        DCD     RTWDOG3_IRQHandler                            ;RTWDOG3 interrupt
        DCD     RTWDOG4_IRQHandler                            ;RTWDOG4 interrupt
        DCD     RTWDOG5_IRQHandler                            ;RTWDOG5 interrupt
        DCD     Reserved98_IRQHandler                         ;Reserved interrupt
        DCD     TMPSNS_INT_IRQHandler                         ;Temperature alarm interrupt
        DCD     Reserved100_IRQHandler                        ;Reserved interrupt
        DCD     LDO_AON_ANA_IRQHandler                        ;Brown out interrupt
        DCD     USDHC1_IRQHandler                             ;USDHC1
        DCD     USDHC2_IRQHandler                             ;USDHC2
        DCD     Reserved104_IRQHandler                        ;Reserved interrupt
        DCD     SFA_TMR2_IRQHandler                           ;Signal Frequency Analyzer interrupt, TMR2 interrupt
        DCD     LDO_AON_DIG_IRQHandler                        ;Brown out interrupt
        DCD     MECC1_IRQHandler                              ;MECC1 interrupt
        DCD     MECC2_IRQHandler                              ;MECC2 interrupt
        DCD     ADC1_IRQHandler                               ;ADC1 interrupt
        DCD     DMA_ERROR_IRQHandler                          ;AON Domain eDMA error interrupt
        DCD     DMA3_CH0_IRQHandler                           ;AON Domain eDMA channel 0 interrupt
        DCD     DMA3_CH1_IRQHandler                           ;AON Domain eDMA channel 1 interrupt
        DCD     DMA3_CH2_IRQHandler                           ;AON Domain eDMA channel 2 interrupt
        DCD     DMA3_CH3_IRQHandler                           ;AON Domain eDMA channel 3 interrupt
        DCD     DMA3_CH4_IRQHandler                           ;AON Domain eDMA channel 4 interrupt
        DCD     DMA3_CH5_IRQHandler                           ;AON Domain eDMA channel 5 interrupt
        DCD     DMA3_CH6_IRQHandler                           ;AON Domain eDMA channel 6 interrupt
        DCD     DMA3_CH7_IRQHandler                           ;AON Domain eDMA channel 7 interrupt
        DCD     DMA3_CH8_IRQHandler                           ;AON Domain eDMA channel 8 interrupt
        DCD     DMA3_CH9_IRQHandler                           ;AON Domain eDMA channel 9 interrupt
        DCD     DMA3_CH10_IRQHandler                          ;AON Domain eDMA channel 10 interrupt
        DCD     DMA3_CH11_IRQHandler                          ;AON Domain eDMA channel 11 interrupt
        DCD     DMA3_CH12_IRQHandler                          ;AON Domain eDMA channel 12 interrupt
        DCD     DMA3_CH13_IRQHandler                          ;AON Domain eDMA channel 13 interrupt
        DCD     DMA3_CH14_IRQHandler                          ;AON Domain eDMA channel 14 interrupt
        DCD     DMA3_CH15_IRQHandler                          ;AON Domain eDMA channel 15 interrupt
        DCD     DMA3_CH16_IRQHandler                          ;AON Domain eDMA channel 16 interrupt
        DCD     DMA3_CH17_IRQHandler                          ;AON Domain eDMA channel 17 interrupt
        DCD     DMA3_CH18_IRQHandler                          ;AON Domain eDMA channel 18 interrupt
        DCD     DMA3_CH19_IRQHandler                          ;AON Domain eDMA channel 19 interrupt
        DCD     DMA3_CH20_IRQHandler                          ;AON Domain eDMA channel 20 interrupt
        DCD     DMA3_CH21_IRQHandler                          ;AON Domain eDMA channel 21 interrupt
        DCD     DMA3_CH22_IRQHandler                          ;AON Domain eDMA channel 22 interrupt
        DCD     DMA3_CH23_IRQHandler                          ;AON Domain eDMA channel 23 interrupt
        DCD     DMA3_CH24_IRQHandler                          ;AON Domain eDMA channel 24 interrupt
        DCD     DMA3_CH25_IRQHandler                          ;AON Domain eDMA channel 25 interrupt
        DCD     DMA3_CH26_IRQHandler                          ;AON Domain eDMA channel 26 interrupt
        DCD     DMA3_CH27_IRQHandler                          ;AON Domain eDMA channel 27 interrupt
        DCD     DMA3_CH28_SINC3_CH0_IRQHandler                ;AON Domain eDMA channel 28 interrupt
        DCD     DMA3_CH29_SINC3_CH1_IRQHandler                ;AON Domain eDMA channel 29 interrupt
        DCD     DMA3_CH30_SINC3_CH2_IRQHandler                ;AON Domain eDMA channel 30 interrupt
        DCD     DMA3_CH31_SINC3_CH3_IRQHandler                ;AON Domain eDMA channel 31 interrupt
        DCD     DMA4_ERROR_IRQHandler                         ;WAKEUP Domain eDMA error interrupt
        DCD     DMA4_CH0_CH1_IRQHandler                       ;WAKEUP Domain eDMA channel 0/1 interrupt
        DCD     DMA4_CH2_CH3_IRQHandler                       ;WAKEUP Domain eDMA channel 2/3 interrupt
        DCD     DMA4_CH4_CH5_IRQHandler                       ;WAKEUP Domain eDMA channel 4/5 interrupt
        DCD     DMA4_CH6_CH7_IRQHandler                       ;WAKEUP Domain eDMA channel 6/7 interrupt
        DCD     DMA4_CH8_CH9_IRQHandler                       ;WAKEUP Domain eDMA channel 8/9 interrupt
        DCD     DMA4_CH10_CH11_IRQHandler                     ;WAKEUP Domain eDMA channel 10/11 interrupt
        DCD     DMA4_CH12_CH13_IRQHandler                     ;WAKEUP Domain eDMA channel 12/13 interrupt
        DCD     DMA4_CH14_CH15_IRQHandler                     ;WAKEUP Domain eDMA channel 14/15 interrupt
        DCD     DMA4_CH16_CH17_IRQHandler                     ;WAKEUP Domain eDMA channel 16/17 interrupt
        DCD     DMA4_CH18_CH19_IRQHandler                     ;WAKEUP Domain eDMA channel 18/19 interrupt
        DCD     DMA4_CH20_CH21_IRQHandler                     ;WAKEUP Domain eDMA channel 20/21 interrupt
        DCD     DMA4_CH22_CH23_IRQHandler                     ;WAKEUP Domain eDMA channel 22/23 interrupt
        DCD     DMA4_CH24_CH25_IRQHandler                     ;WAKEUP Domain eDMA channel 24/25 interrupt
        DCD     DMA4_CH26_CH27_IRQHandler                     ;WAKEUP Domain eDMA channel 26/27 interrupt
        DCD     DMA4_CH28_CH29_IRQHandler                     ;WAKEUP Domain eDMA channel 28/29 interrupt
        DCD     DMA4_CH30_CH31_IRQHandler                     ;WAKEUP Domain eDMA channel 30/31 interrupt
        DCD     DMA4_CH32_CH33_IRQHandler                     ;WAKEUP Domain eDMA channel 32/33 interrupt
        DCD     DMA4_CH34_CH35_IRQHandler                     ;WAKEUP Domain eDMA channel 34/35 interrupt
        DCD     DMA4_CH36_CH37_IRQHandler                     ;WAKEUP Domain eDMA channel 36/37 interrupt
        DCD     DMA4_CH38_CH39_IRQHandler                     ;WAKEUP Domain eDMA channel 38/39 interrupt
        DCD     DMA4_CH40_CH41_IRQHandler                     ;WAKEUP Domain eDMA channel 40/41 interrupt
        DCD     DMA4_CH42_CH43_IRQHandler                     ;WAKEUP Domain eDMA channel 42/43 interrupt
        DCD     DMA4_CH44_CH45_IRQHandler                     ;WAKEUP Domain eDMA channel 44/45 interrupt
        DCD     DMA4_CH46_CH47_IRQHandler                     ;WAKEUP Domain eDMA channel 46/47 interrupt
        DCD     DMA4_CH48_CH49_LPI2C5_IRQHandler              ;WAKEUP Domain eDMA channel 48/49 interrupt, LPI2C5 interrupt
        DCD     DMA4_CH50_CH51_LPI2C6_IRQHandler              ;WAKEUP Domain eDMA channel 50/51 interrupt, LPI2C6 interrupt
        DCD     DMA4_CH52_CH53_SAI4_IRQHandler                ;WAKEUP Domain eDMA channel 46/47 interrupt, SAI4 interrupt
        DCD     DMA4_CH54_CH55_SPDIF_IRQHandler               ;WAKEUP Domain eDMA channel 46/47 interrupt, SPDIF interrupt
        DCD     DMA4_CH56_CH57_LPUART9_IRQHandler             ;WAKEUP Domain eDMA channel 46/47 interrupt, LPUART9 interrupt
        DCD     DMA4_CH58_CH59_LPUART10_IRQHandler            ;WAKEUP Domain eDMA channel 46/47 interrupt, LPUART10 interrupt
        DCD     DMA4_CH60_CH61_LPUART11_IRQHandler            ;WAKEUP Domain eDMA channel 46/47 interrupt, LPUART11 interrupt
        DCD     DMA4_CH62_CH63_LPUART12_IRQHandler            ;WAKEUP Domain eDMA channel 46/47 interrupt, LPUART12 interrupt
        DCD     INTG_BOOTROM_DEBUG_CTRL_IRQHandler            ;CM33, CM7, DAP access IRQ
        DCD     EDGELOCK_REQ1_IRQHandler                      ;
        DCD     EDGELOCK_REQ2_IRQHandler                      ;
        DCD     EDGELOCK_REQ3_IRQHandler                      ;
        DCD     DBG_TRACE_TMR3_IRQHandler                     ;TMR3 interrupt
        DCD     JTAGC_TMR4_IRQHandler                         ;TMR4 interrupt
        DCD     M33_SYSRESET_REQ_IRQHandler                   ;CM33 SYSREQRST SRC reset source
        DCD     M33_LOCKUP_IRQHandler                         ;CM33 LOCKUP SRC reset source
        DCD     M7_SYSRESET_REQ_IRQHandler                    ;CM33 SYSREQRST SRC reset source
        DCD     M7_LOCKUP_IRQHandler                          ;CM33 LOCKUP SRC reset source
        DCD     PWM2_FAULT_IRQHandler                         ;PWM2 fault or reload error interrupt
        DCD     PWM2_0_IRQHandler                             ;PWM2 capture 0, compare 0, or reload 0 interrupt
        DCD     PWM2_1_IRQHandler                             ;PWM2 capture 1, compare 1, or reload 1 interrupt
        DCD     PWM2_2_IRQHandler                             ;PWM2 capture 2, compare 2, or reload 2 interrupt
        DCD     PWM2_3_IRQHandler                             ;PWM2 capture 3, compare 3, or reload 3 interrupt
        DCD     PWM3_FAULT_IRQHandler                         ;PWM3 fault or reload error interrupt
        DCD     PWM3_0_IRQHandler                             ;PWM3 capture 0, compare 0, or reload 0 interrupt
        DCD     PWM3_1_IRQHandler                             ;PWM3 capture 1, compare 1, or reload 1 interrupt
        DCD     PWM3_2_IRQHandler                             ;PWM3 capture 2, compare 2, or reload 2 interrupt
        DCD     PWM3_3_IRQHandler                             ;PWM3 capture 3, compare 3, or reload 3 interrupt
        DCD     PWM4_FAULT_IRQHandler                         ;PWM4 fault or reload error interrupt
        DCD     PWM4_0_IRQHandler                             ;PWM4 capture 0, compare 0, or reload 0 interrupt
        DCD     PWM4_1_IRQHandler                             ;PWM4 capture 1, compare 1, or reload 1 interrupt
        DCD     PWM4_2_IRQHandler                             ;PWM4 capture 2, compare 2, or reload 2 interrupt
        DCD     PWM4_3_IRQHandler                             ;PWM4 capture 3, compare 3, or reload 3 interrupt
        DCD     EQDC1_IRQHandler                              ;EQDC1 interrupt
        DCD     EQDC2_IRQHandler                              ;EQDC2 interrupt
        DCD     EQDC3_IRQHandler                              ;EQDC3 interrupt
        DCD     EQDC4_IRQHandler                              ;EQDC4 interrupt
        DCD     ADC2_IRQHandler                               ;ADC2 interrupt
        DCD     DCDC_IRQHandler                               ;DCDC brown out interrupt
        DCD     CAN3_IRQHandler                               ;CAN3 interrupt
        DCD     CAN3_ERROR_IRQHandler                         ;CAN3 error interrupt
        DCD     DAC_IRQHandler                                ;DAC interrupt
        DCD     LPSPI5_IRQHandler                             ;LPSPI5 interrupt
        DCD     LPSPI6_IRQHandler                             ;LPSPI6 interrupt
        DCD     LPUART7_IRQHandler                            ;LPUART7 interrupt
        DCD     LPUART8_IRQHandler                            ;LPUART8 interrupt
        DCD     SAI2_IRQHandler                               ;SAI2 interrupt
        DCD     SAI3_IRQHandler                               ;SAI3 interrupt
        DCD     ACMP1_IRQHandler                              ;CMP1 interrupt
        DCD     ACMP2_IRQHandler                              ;CMP2 interrupt
        DCD     ACMP3_IRQHandler                              ;CMP3 interrupt
        DCD     ACMP4_IRQHandler                              ;CMP4 interrupt
        DCD     CM7_PS_IRQHandler                             ;M7 PS Tag/Data Parity Error
        DCD     CM7_MCM_IRQHandler                            ;M7 MCM interrupt
        DCD     CM33_MCM_IRQHandler                           ;M33 MCM interrupt
        DCD     ECAT_INT_IRQHandler                           ;EtherCAT interrupt
        DCD     SAFETY_CLK_MON_IRQHandler                     ;Safety clock monitor interrupt
        DCD     GPT1_IRQHandler                               ;GPT1 interrupt
        DCD     GPT2_IRQHandler                               ;GPT2 interrupt
        DCD     KPP_IRQHandler                                ;KPP interrupt
        DCD     USBPHY1_IRQHandler                            ;USBPHY1 interrupt
        DCD     USBPHY2_IRQHandler                            ;USBPHY2 interrupt
        DCD     USB_OTG2_IRQHandler                           ;USBOTG2 interrupt
        DCD     USB_OTG1_IRQHandler                           ;USBOTG1 interrupt
        DCD     FLEXSPI_SLV_IRQHandler                        ;FLEXSPI follower interrupt
        DCD     NETC_IRQHandler                               ;NETC interrupt
        DCD     MSGINTR1_IRQHandler                           ;MSGINTR1 interrupt
        DCD     MSGINTR2_IRQHandler                           ;MSGINTR2 interrupt
        DCD     MSGINTR3_IRQHandler                           ;MSGINTR3 interrupt
        DCD     MSGINTR4_IRQHandler                           ;MSGINTR4 interrupt
        DCD     MSGINTR5_IRQHandler                           ;MSGINTR5 interrupt
        DCD     MSGINTR6_IRQHandler                           ;MSGINTR6 interrupt
        DCD     SINC1_CH0_IRQHandler                          ;SINC Filter Glue 1 channel 0
        DCD     SINC1_CH1_IRQHandler                          ;SINC Filter Glue 1 channel 1
        DCD     SINC1_CH2_IRQHandler                          ;SINC Filter Glue 1 channel 2
        DCD     SINC1_CH3_IRQHandler                          ;SINC Filter Glue 1 channel 3
        DCD     SINC2_CH0_IRQHandler                          ;SINC Filter Glue 2 channel 0
        DCD     SINC2_CH1_IRQHandler                          ;SINC Filter Glue 2 channel 1
        DCD     SINC2_CH2_IRQHandler                          ;SINC Filter Glue 2 channel 2
        DCD     SINC2_CH3_IRQHandler                          ;SINC Filter Glue 2 channel 3
        DCD     GPIO4_0_IRQHandler                            ;GPIO4 interrupt 0
        DCD     GPIO4_1_IRQHandler                            ;GPIO4 interrupt 1
        DCD     GPIO5_0_IRQHandler                            ;GPIO5 interrupt 0
        DCD     GPIO5_1_IRQHandler                            ;GPIO5 interrupt 1
        DCD     GPIO6_0_IRQHandler                            ;GPIO6 interrupt 0
        DCD     GPIO6_1_IRQHandler                            ;GPIO6 interrupt 1
        DCD     ECAT_RST_OUT_IRQHandler                       ;ECAT reset out interrupt
        DCD     DefaultISR                                    ;255
__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        CPSID   I               ; Mask interrupts
        LDR     R0, =0xE000ED08
        LDR     R1, =__vector_table
        STR     R1, [R0]
        LDR     R2, [R1]
        MSR     MSP, R2
        LDR     R0, =sfb(CSTACK)
        MSR     MSPLIM, R0
        LDR     R0, =SystemInit
        BLX     R0
;
; Add RW / stack / heap initializaiton
; TCM controller must perform a read-modify-write for any access < 32-bit to keep the ECC updated.
; The Software must ensure the TCM is ECC clean by initializing all memories that have the potential to be accessed as < 32-bit.
        MOV    R0, #0
        LDR    R1, =SFB(RW)
        LDR    R2, =SFE(RW)
.LC2:
        CMP    R1, R2
        ITT    LT
        STRLT  R0, [R1], #4
        BLT    .LC2

        MOV    R0, #0
        LDR    R1, =SFB(HEAP)
        LDR    R2, =SFE(HEAP)
.LC3:
        CMP    R1, R2
        ITT    LT
        STRLT  R0, [R1], #4
        BLT    .LC3

        LDR     R1, =SFB(CSTACK)
        LDR     R2, =SFE(CSTACK)
.LC4:
        CMP     R1, R2
        ITT     LT
        STRLT   R0, [R1], #4
        BLT     .LC4

#if defined(FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE) && FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE
        LDR     R1, =SFB(QACCESS_CODE_VAR)
        LDR     R2, =SFE(QACCESS_CODE_VAR)
.LC5:
        CMP     R1, R2
        ITT     LT
        STRLT   R0, [R1], #4
        BLT     .LC5

        LDR     R1, =SFB(QACCESS_DATA_VAR)
        LDR     R2, =SFE(QACCESS_DATA_VAR)
.LC6:
        CMP     R1, R2
        ITT     LT
        STRLT   R0, [R1], #4
        BLT     .LC6
#endif

; End RW / stack / heap initialization
;
        CPSIE   I               ; Unmask interrupts
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B .

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B .

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
        B .

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
        B .

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
        B .

        PUBWEAK SecureFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SecureFault_Handler
        B .

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B .

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
        B .

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B .

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B .

        PUBWEAK TMR1_IRQHandler
        PUBWEAK DAP_IRQHandler
        PUBWEAK M7_CTI_TRIGGER_OUTPUT_IRQHandler
        PUBWEAK M33_CTI_TRIGGER_OUTPUT_IRQHandler
        PUBWEAK TMR5_IRQHandler
        PUBWEAK TMR6_IRQHandler
        PUBWEAK TMR7_IRQHandler
        PUBWEAK TMR8_IRQHandler
        PUBWEAK CAN1_IRQHandler
        PUBWEAK CAN1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_IRQHandler
        LDR     R0, =CAN1_DriverIRQHandler
        BX      R0

        PUBWEAK CAN1_ERROR_IRQHandler
        PUBWEAK CAN1_ERROR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN1_ERROR_IRQHandler
        LDR     R0, =CAN1_ERROR_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO1_0_IRQHandler
        PUBWEAK GPIO1_1_IRQHandler
        PUBWEAK I3C1_IRQHandler
        PUBWEAK I3C1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I3C1_IRQHandler
        LDR     R0, =I3C1_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C1_IRQHandler
        PUBWEAK LPI2C1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C1_IRQHandler
        LDR     R0, =LPI2C1_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C2_IRQHandler
        PUBWEAK LPI2C2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C2_IRQHandler
        LDR     R0, =LPI2C2_DriverIRQHandler
        BX      R0

        PUBWEAK LPIT1_IRQHandler
        PUBWEAK LPSPI1_IRQHandler
        PUBWEAK LPSPI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI1_IRQHandler
        LDR     R0, =LPSPI1_DriverIRQHandler
        BX      R0

        PUBWEAK LPSPI2_IRQHandler
        PUBWEAK LPSPI2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI2_IRQHandler
        LDR     R0, =LPSPI2_DriverIRQHandler
        BX      R0

        PUBWEAK LPTMR1_IRQHandler
        PUBWEAK LPUART1_IRQHandler
        PUBWEAK LPUART1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART1_IRQHandler
        LDR     R0, =LPUART1_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART2_IRQHandler
        PUBWEAK LPUART2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART2_IRQHandler
        LDR     R0, =LPUART2_DriverIRQHandler
        BX      R0

        PUBWEAK MU1_IRQHandler
        PUBWEAK MU2_IRQHandler
        PUBWEAK PWM1_FAULT_IRQHandler
        PUBWEAK PWM1_0_IRQHandler
        PUBWEAK PWM1_1_IRQHandler
        PUBWEAK PWM1_2_IRQHandler
        PUBWEAK PWM1_3_IRQHandler
        PUBWEAK EDGELOCK_TRUST_MUA_RX_FULL_IRQHandler
        PUBWEAK EDGELOCK_TRUST_MUA_TX_EMPTY_IRQHandler
        PUBWEAK EDGELOCK_APPS_CORE_MUA_RX_FULL_IRQHandler
        PUBWEAK EDGELOCK_APPS_CORE_MUA_TX_EMPTY_IRQHandler
        PUBWEAK EDGELOCK_REALTIME_CORE_MUA_RX_FULL_IRQHandler
        PUBWEAK EDGELOCK_REALTIME_CORE_MUA_TX_EMPTY_IRQHandler
        PUBWEAK EDGELOCK_SECURE_IRQHandler
        PUBWEAK EDGELOCK_NONSECURE_IRQHandler
        PUBWEAK TPM1_XBAR1_CH0_CH1_IRQHandler
        PUBWEAK TPM2_XBAR1_CH2_CH3_IRQHandler
        PUBWEAK RTWDOG1_IRQHandler
        PUBWEAK RTWDOG2_IRQHandler
        PUBWEAK Reserved56_IRQHandler
        PUBWEAK PDM_HWVAD_EVENT_IRQHandler
        PUBWEAK PDM_HWVAD_EVENT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PDM_HWVAD_EVENT_IRQHandler
        LDR     R0, =PDM_HWVAD_EVENT_DriverIRQHandler
        BX      R0

        PUBWEAK PDM_HWVAD_ERROR_IRQHandler
        PUBWEAK PDM_HWVAD_ERROR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PDM_HWVAD_ERROR_IRQHandler
        LDR     R0, =PDM_HWVAD_ERROR_DriverIRQHandler
        BX      R0

        PUBWEAK PDM_EVENT_IRQHandler
        PUBWEAK PDM_EVENT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PDM_EVENT_IRQHandler
        LDR     R0, =PDM_EVENT_DriverIRQHandler
        BX      R0

        PUBWEAK PDM_ERROR_IRQHandler
        PUBWEAK PDM_ERROR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PDM_ERROR_IRQHandler
        LDR     R0, =PDM_ERROR_DriverIRQHandler
        BX      R0

        PUBWEAK SAI1_IRQHandler
        PUBWEAK SAI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SAI1_IRQHandler
        LDR     R0, =SAI1_DriverIRQHandler
        BX      R0

        PUBWEAK CM33_PS_IRQHandler
        PUBWEAK CM33_TCM_ECC_IRQHandler
        PUBWEAK CM33_TCM_ERROR_IRQHandler
        PUBWEAK CM7_TCM_ECC_IRQHandler
        PUBWEAK CM7_TCM_ERROR_IRQHandler
        PUBWEAK CAN2_IRQHandler
        PUBWEAK CAN2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN2_IRQHandler
        LDR     R0, =CAN2_DriverIRQHandler
        BX      R0

        PUBWEAK CAN2_ERROR_IRQHandler
        PUBWEAK CAN2_ERROR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN2_ERROR_IRQHandler
        LDR     R0, =CAN2_ERROR_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXIO1_IRQHandler
        PUBWEAK FLEXIO1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXIO1_IRQHandler
        LDR     R0, =FLEXIO1_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXIO2_IRQHandler
        PUBWEAK FLEXIO2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXIO2_IRQHandler
        LDR     R0, =FLEXIO2_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXSPI1_IRQHandler
        PUBWEAK FLEXSPI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXSPI1_IRQHandler
        LDR     R0, =FLEXSPI1_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXSPI2_IRQHandler
        PUBWEAK FLEXSPI2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXSPI2_IRQHandler
        LDR     R0, =FLEXSPI2_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO2_0_IRQHandler
        PUBWEAK GPIO2_1_IRQHandler
        PUBWEAK GPIO3_0_IRQHandler
        PUBWEAK GPIO3_1_IRQHandler
        PUBWEAK I3C2_IRQHandler
        PUBWEAK I3C2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I3C2_IRQHandler
        LDR     R0, =I3C2_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C3_IRQHandler
        PUBWEAK LPI2C3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C3_IRQHandler
        LDR     R0, =LPI2C3_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C4_IRQHandler
        PUBWEAK LPI2C4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C4_IRQHandler
        LDR     R0, =LPI2C4_DriverIRQHandler
        BX      R0

        PUBWEAK LPIT2_IRQHandler
        PUBWEAK LPSPI3_IRQHandler
        PUBWEAK LPSPI3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI3_IRQHandler
        LDR     R0, =LPSPI3_DriverIRQHandler
        BX      R0

        PUBWEAK LPSPI4_IRQHandler
        PUBWEAK LPSPI4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI4_IRQHandler
        LDR     R0, =LPSPI4_DriverIRQHandler
        BX      R0

        PUBWEAK LPTMR2_IRQHandler
        PUBWEAK LPUART3_IRQHandler
        PUBWEAK LPUART3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART3_IRQHandler
        LDR     R0, =LPUART3_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART4_IRQHandler
        PUBWEAK LPUART4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART4_IRQHandler
        LDR     R0, =LPUART4_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART5_IRQHandler
        PUBWEAK LPUART5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART5_IRQHandler
        LDR     R0, =LPUART5_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART6_IRQHandler
        PUBWEAK LPUART6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART6_IRQHandler
        LDR     R0, =LPUART6_DriverIRQHandler
        BX      R0

        PUBWEAK ASRC_IRQHandler
        PUBWEAK ASRC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ASRC_IRQHandler
        LDR     R0, =ASRC_DriverIRQHandler
        BX      R0

        PUBWEAK BBNSM_IRQHandler
        PUBWEAK SYS_CTR1_IRQHandler
        PUBWEAK TPM3_EWM_IRQHandler
        PUBWEAK TPM4_SEMC_IRQHandler
        PUBWEAK TPM5_LPIT3_IRQHandler
        PUBWEAK TPM6_LPTMR3_IRQHandler
        PUBWEAK RTWDOG3_IRQHandler
        PUBWEAK RTWDOG4_IRQHandler
        PUBWEAK RTWDOG5_IRQHandler
        PUBWEAK Reserved98_IRQHandler
        PUBWEAK TMPSNS_INT_IRQHandler
        PUBWEAK Reserved100_IRQHandler
        PUBWEAK LDO_AON_ANA_IRQHandler
        PUBWEAK USDHC1_IRQHandler
        PUBWEAK USDHC1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
USDHC1_IRQHandler
        LDR     R0, =USDHC1_DriverIRQHandler
        BX      R0

        PUBWEAK USDHC2_IRQHandler
        PUBWEAK USDHC2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
USDHC2_IRQHandler
        LDR     R0, =USDHC2_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved104_IRQHandler
        PUBWEAK SFA_TMR2_IRQHandler
        PUBWEAK LDO_AON_DIG_IRQHandler
        PUBWEAK MECC1_IRQHandler
        PUBWEAK MECC2_IRQHandler
        PUBWEAK ADC1_IRQHandler
        PUBWEAK DMA_ERROR_IRQHandler
        PUBWEAK DMA_ERROR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA_ERROR_IRQHandler
        LDR     R0, =DMA_ERROR_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH0_IRQHandler
        PUBWEAK DMA3_CH0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH0_IRQHandler
        LDR     R0, =DMA3_CH0_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH1_IRQHandler
        PUBWEAK DMA3_CH1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH1_IRQHandler
        LDR     R0, =DMA3_CH1_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH2_IRQHandler
        PUBWEAK DMA3_CH2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH2_IRQHandler
        LDR     R0, =DMA3_CH2_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH3_IRQHandler
        PUBWEAK DMA3_CH3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH3_IRQHandler
        LDR     R0, =DMA3_CH3_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH4_IRQHandler
        PUBWEAK DMA3_CH4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH4_IRQHandler
        LDR     R0, =DMA3_CH4_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH5_IRQHandler
        PUBWEAK DMA3_CH5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH5_IRQHandler
        LDR     R0, =DMA3_CH5_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH6_IRQHandler
        PUBWEAK DMA3_CH6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH6_IRQHandler
        LDR     R0, =DMA3_CH6_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH7_IRQHandler
        PUBWEAK DMA3_CH7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH7_IRQHandler
        LDR     R0, =DMA3_CH7_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH8_IRQHandler
        PUBWEAK DMA3_CH8_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH8_IRQHandler
        LDR     R0, =DMA3_CH8_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH9_IRQHandler
        PUBWEAK DMA3_CH9_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH9_IRQHandler
        LDR     R0, =DMA3_CH9_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH10_IRQHandler
        PUBWEAK DMA3_CH10_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH10_IRQHandler
        LDR     R0, =DMA3_CH10_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH11_IRQHandler
        PUBWEAK DMA3_CH11_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH11_IRQHandler
        LDR     R0, =DMA3_CH11_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH12_IRQHandler
        PUBWEAK DMA3_CH12_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH12_IRQHandler
        LDR     R0, =DMA3_CH12_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH13_IRQHandler
        PUBWEAK DMA3_CH13_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH13_IRQHandler
        LDR     R0, =DMA3_CH13_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH14_IRQHandler
        PUBWEAK DMA3_CH14_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH14_IRQHandler
        LDR     R0, =DMA3_CH14_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH15_IRQHandler
        PUBWEAK DMA3_CH15_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH15_IRQHandler
        LDR     R0, =DMA3_CH15_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH16_IRQHandler
        PUBWEAK DMA3_CH16_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH16_IRQHandler
        LDR     R0, =DMA3_CH16_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH17_IRQHandler
        PUBWEAK DMA3_CH17_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH17_IRQHandler
        LDR     R0, =DMA3_CH17_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH18_IRQHandler
        PUBWEAK DMA3_CH18_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH18_IRQHandler
        LDR     R0, =DMA3_CH18_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH19_IRQHandler
        PUBWEAK DMA3_CH19_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH19_IRQHandler
        LDR     R0, =DMA3_CH19_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH20_IRQHandler
        PUBWEAK DMA3_CH20_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH20_IRQHandler
        LDR     R0, =DMA3_CH20_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH21_IRQHandler
        PUBWEAK DMA3_CH21_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH21_IRQHandler
        LDR     R0, =DMA3_CH21_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH22_IRQHandler
        PUBWEAK DMA3_CH22_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH22_IRQHandler
        LDR     R0, =DMA3_CH22_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH23_IRQHandler
        PUBWEAK DMA3_CH23_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH23_IRQHandler
        LDR     R0, =DMA3_CH23_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH24_IRQHandler
        PUBWEAK DMA3_CH24_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH24_IRQHandler
        LDR     R0, =DMA3_CH24_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH25_IRQHandler
        PUBWEAK DMA3_CH25_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH25_IRQHandler
        LDR     R0, =DMA3_CH25_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH26_IRQHandler
        PUBWEAK DMA3_CH26_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH26_IRQHandler
        LDR     R0, =DMA3_CH26_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH27_IRQHandler
        PUBWEAK DMA3_CH27_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH27_IRQHandler
        LDR     R0, =DMA3_CH27_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH28_SINC3_CH0_IRQHandler
        PUBWEAK DMA3_CH28_SINC3_CH0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH28_SINC3_CH0_IRQHandler
        LDR     R0, =DMA3_CH28_SINC3_CH0_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH29_SINC3_CH1_IRQHandler
        PUBWEAK DMA3_CH29_SINC3_CH1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH29_SINC3_CH1_IRQHandler
        LDR     R0, =DMA3_CH29_SINC3_CH1_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH30_SINC3_CH2_IRQHandler
        PUBWEAK DMA3_CH30_SINC3_CH2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH30_SINC3_CH2_IRQHandler
        LDR     R0, =DMA3_CH30_SINC3_CH2_DriverIRQHandler
        BX      R0

        PUBWEAK DMA3_CH31_SINC3_CH3_IRQHandler
        PUBWEAK DMA3_CH31_SINC3_CH3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA3_CH31_SINC3_CH3_IRQHandler
        LDR     R0, =DMA3_CH31_SINC3_CH3_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_ERROR_IRQHandler
        PUBWEAK DMA4_ERROR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_ERROR_IRQHandler
        LDR     R0, =DMA4_ERROR_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH0_CH1_IRQHandler
        PUBWEAK DMA4_CH0_CH1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH0_CH1_IRQHandler
        LDR     R0, =DMA4_CH0_CH1_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH2_CH3_IRQHandler
        PUBWEAK DMA4_CH2_CH3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH2_CH3_IRQHandler
        LDR     R0, =DMA4_CH2_CH3_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH4_CH5_IRQHandler
        PUBWEAK DMA4_CH4_CH5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH4_CH5_IRQHandler
        LDR     R0, =DMA4_CH4_CH5_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH6_CH7_IRQHandler
        PUBWEAK DMA4_CH6_CH7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH6_CH7_IRQHandler
        LDR     R0, =DMA4_CH6_CH7_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH8_CH9_IRQHandler
        PUBWEAK DMA4_CH8_CH9_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH8_CH9_IRQHandler
        LDR     R0, =DMA4_CH8_CH9_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH10_CH11_IRQHandler
        PUBWEAK DMA4_CH10_CH11_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH10_CH11_IRQHandler
        LDR     R0, =DMA4_CH10_CH11_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH12_CH13_IRQHandler
        PUBWEAK DMA4_CH12_CH13_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH12_CH13_IRQHandler
        LDR     R0, =DMA4_CH12_CH13_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH14_CH15_IRQHandler
        PUBWEAK DMA4_CH14_CH15_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH14_CH15_IRQHandler
        LDR     R0, =DMA4_CH14_CH15_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH16_CH17_IRQHandler
        PUBWEAK DMA4_CH16_CH17_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH16_CH17_IRQHandler
        LDR     R0, =DMA4_CH16_CH17_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH18_CH19_IRQHandler
        PUBWEAK DMA4_CH18_CH19_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH18_CH19_IRQHandler
        LDR     R0, =DMA4_CH18_CH19_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH20_CH21_IRQHandler
        PUBWEAK DMA4_CH20_CH21_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH20_CH21_IRQHandler
        LDR     R0, =DMA4_CH20_CH21_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH22_CH23_IRQHandler
        PUBWEAK DMA4_CH22_CH23_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH22_CH23_IRQHandler
        LDR     R0, =DMA4_CH22_CH23_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH24_CH25_IRQHandler
        PUBWEAK DMA4_CH24_CH25_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH24_CH25_IRQHandler
        LDR     R0, =DMA4_CH24_CH25_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH26_CH27_IRQHandler
        PUBWEAK DMA4_CH26_CH27_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH26_CH27_IRQHandler
        LDR     R0, =DMA4_CH26_CH27_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH28_CH29_IRQHandler
        PUBWEAK DMA4_CH28_CH29_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH28_CH29_IRQHandler
        LDR     R0, =DMA4_CH28_CH29_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH30_CH31_IRQHandler
        PUBWEAK DMA4_CH30_CH31_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH30_CH31_IRQHandler
        LDR     R0, =DMA4_CH30_CH31_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH32_CH33_IRQHandler
        PUBWEAK DMA4_CH32_CH33_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH32_CH33_IRQHandler
        LDR     R0, =DMA4_CH32_CH33_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH34_CH35_IRQHandler
        PUBWEAK DMA4_CH34_CH35_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH34_CH35_IRQHandler
        LDR     R0, =DMA4_CH34_CH35_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH36_CH37_IRQHandler
        PUBWEAK DMA4_CH36_CH37_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH36_CH37_IRQHandler
        LDR     R0, =DMA4_CH36_CH37_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH38_CH39_IRQHandler
        PUBWEAK DMA4_CH38_CH39_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH38_CH39_IRQHandler
        LDR     R0, =DMA4_CH38_CH39_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH40_CH41_IRQHandler
        PUBWEAK DMA4_CH40_CH41_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH40_CH41_IRQHandler
        LDR     R0, =DMA4_CH40_CH41_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH42_CH43_IRQHandler
        PUBWEAK DMA4_CH42_CH43_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH42_CH43_IRQHandler
        LDR     R0, =DMA4_CH42_CH43_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH44_CH45_IRQHandler
        PUBWEAK DMA4_CH44_CH45_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH44_CH45_IRQHandler
        LDR     R0, =DMA4_CH44_CH45_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH46_CH47_IRQHandler
        PUBWEAK DMA4_CH46_CH47_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH46_CH47_IRQHandler
        LDR     R0, =DMA4_CH46_CH47_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH48_CH49_LPI2C5_IRQHandler
        PUBWEAK DMA4_CH48_CH49_LPI2C5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH48_CH49_LPI2C5_IRQHandler
        LDR     R0, =DMA4_CH48_CH49_LPI2C5_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH50_CH51_LPI2C6_IRQHandler
        PUBWEAK DMA4_CH50_CH51_LPI2C6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH50_CH51_LPI2C6_IRQHandler
        LDR     R0, =DMA4_CH50_CH51_LPI2C6_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH52_CH53_SAI4_IRQHandler
        PUBWEAK DMA4_CH52_CH53_SAI4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH52_CH53_SAI4_IRQHandler
        LDR     R0, =DMA4_CH52_CH53_SAI4_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH54_CH55_SPDIF_IRQHandler
        PUBWEAK DMA4_CH54_CH55_SPDIF_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH54_CH55_SPDIF_IRQHandler
        LDR     R0, =DMA4_CH54_CH55_SPDIF_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH56_CH57_LPUART9_IRQHandler
        PUBWEAK DMA4_CH56_CH57_LPUART9_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH56_CH57_LPUART9_IRQHandler
        LDR     R0, =DMA4_CH56_CH57_LPUART9_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH58_CH59_LPUART10_IRQHandler
        PUBWEAK DMA4_CH58_CH59_LPUART10_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH58_CH59_LPUART10_IRQHandler
        LDR     R0, =DMA4_CH58_CH59_LPUART10_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH60_CH61_LPUART11_IRQHandler
        PUBWEAK DMA4_CH60_CH61_LPUART11_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH60_CH61_LPUART11_IRQHandler
        LDR     R0, =DMA4_CH60_CH61_LPUART11_DriverIRQHandler
        BX      R0

        PUBWEAK DMA4_CH62_CH63_LPUART12_IRQHandler
        PUBWEAK DMA4_CH62_CH63_LPUART12_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA4_CH62_CH63_LPUART12_IRQHandler
        LDR     R0, =DMA4_CH62_CH63_LPUART12_DriverIRQHandler
        BX      R0

        PUBWEAK INTG_BOOTROM_DEBUG_CTRL_IRQHandler
        PUBWEAK EDGELOCK_REQ1_IRQHandler
        PUBWEAK EDGELOCK_REQ2_IRQHandler
        PUBWEAK EDGELOCK_REQ3_IRQHandler
        PUBWEAK DBG_TRACE_TMR3_IRQHandler
        PUBWEAK JTAGC_TMR4_IRQHandler
        PUBWEAK M33_SYSRESET_REQ_IRQHandler
        PUBWEAK M33_LOCKUP_IRQHandler
        PUBWEAK M7_SYSRESET_REQ_IRQHandler
        PUBWEAK M7_LOCKUP_IRQHandler
        PUBWEAK PWM2_FAULT_IRQHandler
        PUBWEAK PWM2_0_IRQHandler
        PUBWEAK PWM2_1_IRQHandler
        PUBWEAK PWM2_2_IRQHandler
        PUBWEAK PWM2_3_IRQHandler
        PUBWEAK PWM3_FAULT_IRQHandler
        PUBWEAK PWM3_0_IRQHandler
        PUBWEAK PWM3_1_IRQHandler
        PUBWEAK PWM3_2_IRQHandler
        PUBWEAK PWM3_3_IRQHandler
        PUBWEAK PWM4_FAULT_IRQHandler
        PUBWEAK PWM4_0_IRQHandler
        PUBWEAK PWM4_1_IRQHandler
        PUBWEAK PWM4_2_IRQHandler
        PUBWEAK PWM4_3_IRQHandler
        PUBWEAK EQDC1_IRQHandler
        PUBWEAK EQDC2_IRQHandler
        PUBWEAK EQDC3_IRQHandler
        PUBWEAK EQDC4_IRQHandler
        PUBWEAK ADC2_IRQHandler
        PUBWEAK DCDC_IRQHandler
        PUBWEAK CAN3_IRQHandler
        PUBWEAK CAN3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN3_IRQHandler
        LDR     R0, =CAN3_DriverIRQHandler
        BX      R0

        PUBWEAK CAN3_ERROR_IRQHandler
        PUBWEAK CAN3_ERROR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAN3_ERROR_IRQHandler
        LDR     R0, =CAN3_ERROR_DriverIRQHandler
        BX      R0

        PUBWEAK DAC_IRQHandler
        PUBWEAK LPSPI5_IRQHandler
        PUBWEAK LPSPI5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI5_IRQHandler
        LDR     R0, =LPSPI5_DriverIRQHandler
        BX      R0

        PUBWEAK LPSPI6_IRQHandler
        PUBWEAK LPSPI6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI6_IRQHandler
        LDR     R0, =LPSPI6_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART7_IRQHandler
        PUBWEAK LPUART7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART7_IRQHandler
        LDR     R0, =LPUART7_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART8_IRQHandler
        PUBWEAK LPUART8_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART8_IRQHandler
        LDR     R0, =LPUART8_DriverIRQHandler
        BX      R0

        PUBWEAK SAI2_IRQHandler
        PUBWEAK SAI2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SAI2_IRQHandler
        LDR     R0, =SAI2_DriverIRQHandler
        BX      R0

        PUBWEAK SAI3_IRQHandler
        PUBWEAK SAI3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SAI3_IRQHandler
        LDR     R0, =SAI3_DriverIRQHandler
        BX      R0

        PUBWEAK ACMP1_IRQHandler
        PUBWEAK ACMP2_IRQHandler
        PUBWEAK ACMP3_IRQHandler
        PUBWEAK ACMP4_IRQHandler
        PUBWEAK CM7_PS_IRQHandler
        PUBWEAK CM7_MCM_IRQHandler
        PUBWEAK CM33_MCM_IRQHandler
        PUBWEAK ECAT_INT_IRQHandler
        PUBWEAK SAFETY_CLK_MON_IRQHandler
        PUBWEAK GPT1_IRQHandler
        PUBWEAK GPT2_IRQHandler
        PUBWEAK KPP_IRQHandler
        PUBWEAK USBPHY1_IRQHandler
        PUBWEAK USBPHY2_IRQHandler
        PUBWEAK USB_OTG2_IRQHandler
        PUBWEAK USB_OTG1_IRQHandler
        PUBWEAK FLEXSPI_SLV_IRQHandler
        PUBWEAK FLEXSPI_SLV_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXSPI_SLV_IRQHandler
        LDR     R0, =FLEXSPI_SLV_DriverIRQHandler
        BX      R0

        PUBWEAK NETC_IRQHandler
        PUBWEAK MSGINTR1_IRQHandler
        PUBWEAK MSGINTR2_IRQHandler
        PUBWEAK MSGINTR3_IRQHandler
        PUBWEAK MSGINTR4_IRQHandler
        PUBWEAK MSGINTR5_IRQHandler
        PUBWEAK MSGINTR6_IRQHandler
        PUBWEAK SINC1_CH0_IRQHandler
        PUBWEAK SINC1_CH1_IRQHandler
        PUBWEAK SINC1_CH2_IRQHandler
        PUBWEAK SINC1_CH3_IRQHandler
        PUBWEAK SINC2_CH0_IRQHandler
        PUBWEAK SINC2_CH1_IRQHandler
        PUBWEAK SINC2_CH2_IRQHandler
        PUBWEAK SINC2_CH3_IRQHandler
        PUBWEAK GPIO4_0_IRQHandler
        PUBWEAK GPIO4_1_IRQHandler
        PUBWEAK GPIO5_0_IRQHandler
        PUBWEAK GPIO5_1_IRQHandler
        PUBWEAK GPIO6_0_IRQHandler
        PUBWEAK GPIO6_1_IRQHandler
        PUBWEAK ECAT_RST_OUT_IRQHandler
        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(1)
TMR1_IRQHandler
DAP_IRQHandler
M7_CTI_TRIGGER_OUTPUT_IRQHandler
M33_CTI_TRIGGER_OUTPUT_IRQHandler
TMR5_IRQHandler
TMR6_IRQHandler
TMR7_IRQHandler
TMR8_IRQHandler
CAN1_DriverIRQHandler
CAN1_ERROR_DriverIRQHandler
GPIO1_0_IRQHandler
GPIO1_1_IRQHandler
I3C1_DriverIRQHandler
LPI2C1_DriverIRQHandler
LPI2C2_DriverIRQHandler
LPIT1_IRQHandler
LPSPI1_DriverIRQHandler
LPSPI2_DriverIRQHandler
LPTMR1_IRQHandler
LPUART1_DriverIRQHandler
LPUART2_DriverIRQHandler
MU1_IRQHandler
MU2_IRQHandler
PWM1_FAULT_IRQHandler
PWM1_0_IRQHandler
PWM1_1_IRQHandler
PWM1_2_IRQHandler
PWM1_3_IRQHandler
EDGELOCK_TRUST_MUA_RX_FULL_IRQHandler
EDGELOCK_TRUST_MUA_TX_EMPTY_IRQHandler
EDGELOCK_APPS_CORE_MUA_RX_FULL_IRQHandler
EDGELOCK_APPS_CORE_MUA_TX_EMPTY_IRQHandler
EDGELOCK_REALTIME_CORE_MUA_RX_FULL_IRQHandler
EDGELOCK_REALTIME_CORE_MUA_TX_EMPTY_IRQHandler
EDGELOCK_SECURE_IRQHandler
EDGELOCK_NONSECURE_IRQHandler
TPM1_XBAR1_CH0_CH1_IRQHandler
TPM2_XBAR1_CH2_CH3_IRQHandler
RTWDOG1_IRQHandler
RTWDOG2_IRQHandler
Reserved56_IRQHandler
PDM_HWVAD_EVENT_DriverIRQHandler
PDM_HWVAD_ERROR_DriverIRQHandler
PDM_EVENT_DriverIRQHandler
PDM_ERROR_DriverIRQHandler
SAI1_DriverIRQHandler
CM33_PS_IRQHandler
CM33_TCM_ECC_IRQHandler
CM33_TCM_ERROR_IRQHandler
CM7_TCM_ECC_IRQHandler
CM7_TCM_ERROR_IRQHandler
CAN2_DriverIRQHandler
CAN2_ERROR_DriverIRQHandler
FLEXIO1_DriverIRQHandler
FLEXIO2_DriverIRQHandler
FLEXSPI1_DriverIRQHandler
FLEXSPI2_DriverIRQHandler
GPIO2_0_IRQHandler
GPIO2_1_IRQHandler
GPIO3_0_IRQHandler
GPIO3_1_IRQHandler
I3C2_DriverIRQHandler
LPI2C3_DriverIRQHandler
LPI2C4_DriverIRQHandler
LPIT2_IRQHandler
LPSPI3_DriverIRQHandler
LPSPI4_DriverIRQHandler
LPTMR2_IRQHandler
LPUART3_DriverIRQHandler
LPUART4_DriverIRQHandler
LPUART5_DriverIRQHandler
LPUART6_DriverIRQHandler
ASRC_DriverIRQHandler
BBNSM_IRQHandler
SYS_CTR1_IRQHandler
TPM3_EWM_IRQHandler
TPM4_SEMC_IRQHandler
TPM5_LPIT3_IRQHandler
TPM6_LPTMR3_IRQHandler
RTWDOG3_IRQHandler
RTWDOG4_IRQHandler
RTWDOG5_IRQHandler
Reserved98_IRQHandler
TMPSNS_INT_IRQHandler
Reserved100_IRQHandler
LDO_AON_ANA_IRQHandler
USDHC1_DriverIRQHandler
USDHC2_DriverIRQHandler
Reserved104_IRQHandler
SFA_TMR2_IRQHandler
LDO_AON_DIG_IRQHandler
MECC1_IRQHandler
MECC2_IRQHandler
ADC1_IRQHandler
DMA_ERROR_DriverIRQHandler
DMA3_CH0_DriverIRQHandler
DMA3_CH1_DriverIRQHandler
DMA3_CH2_DriverIRQHandler
DMA3_CH3_DriverIRQHandler
DMA3_CH4_DriverIRQHandler
DMA3_CH5_DriverIRQHandler
DMA3_CH6_DriverIRQHandler
DMA3_CH7_DriverIRQHandler
DMA3_CH8_DriverIRQHandler
DMA3_CH9_DriverIRQHandler
DMA3_CH10_DriverIRQHandler
DMA3_CH11_DriverIRQHandler
DMA3_CH12_DriverIRQHandler
DMA3_CH13_DriverIRQHandler
DMA3_CH14_DriverIRQHandler
DMA3_CH15_DriverIRQHandler
DMA3_CH16_DriverIRQHandler
DMA3_CH17_DriverIRQHandler
DMA3_CH18_DriverIRQHandler
DMA3_CH19_DriverIRQHandler
DMA3_CH20_DriverIRQHandler
DMA3_CH21_DriverIRQHandler
DMA3_CH22_DriverIRQHandler
DMA3_CH23_DriverIRQHandler
DMA3_CH24_DriverIRQHandler
DMA3_CH25_DriverIRQHandler
DMA3_CH26_DriverIRQHandler
DMA3_CH27_DriverIRQHandler
DMA3_CH28_SINC3_CH0_DriverIRQHandler
DMA3_CH29_SINC3_CH1_DriverIRQHandler
DMA3_CH30_SINC3_CH2_DriverIRQHandler
DMA3_CH31_SINC3_CH3_DriverIRQHandler
DMA4_ERROR_DriverIRQHandler
DMA4_CH0_CH1_DriverIRQHandler
DMA4_CH2_CH3_DriverIRQHandler
DMA4_CH4_CH5_DriverIRQHandler
DMA4_CH6_CH7_DriverIRQHandler
DMA4_CH8_CH9_DriverIRQHandler
DMA4_CH10_CH11_DriverIRQHandler
DMA4_CH12_CH13_DriverIRQHandler
DMA4_CH14_CH15_DriverIRQHandler
DMA4_CH16_CH17_DriverIRQHandler
DMA4_CH18_CH19_DriverIRQHandler
DMA4_CH20_CH21_DriverIRQHandler
DMA4_CH22_CH23_DriverIRQHandler
DMA4_CH24_CH25_DriverIRQHandler
DMA4_CH26_CH27_DriverIRQHandler
DMA4_CH28_CH29_DriverIRQHandler
DMA4_CH30_CH31_DriverIRQHandler
DMA4_CH32_CH33_DriverIRQHandler
DMA4_CH34_CH35_DriverIRQHandler
DMA4_CH36_CH37_DriverIRQHandler
DMA4_CH38_CH39_DriverIRQHandler
DMA4_CH40_CH41_DriverIRQHandler
DMA4_CH42_CH43_DriverIRQHandler
DMA4_CH44_CH45_DriverIRQHandler
DMA4_CH46_CH47_DriverIRQHandler
DMA4_CH48_CH49_LPI2C5_DriverIRQHandler
DMA4_CH50_CH51_LPI2C6_DriverIRQHandler
DMA4_CH52_CH53_SAI4_DriverIRQHandler
DMA4_CH54_CH55_SPDIF_DriverIRQHandler
DMA4_CH56_CH57_LPUART9_DriverIRQHandler
DMA4_CH58_CH59_LPUART10_DriverIRQHandler
DMA4_CH60_CH61_LPUART11_DriverIRQHandler
DMA4_CH62_CH63_LPUART12_DriverIRQHandler
INTG_BOOTROM_DEBUG_CTRL_IRQHandler
EDGELOCK_REQ1_IRQHandler
EDGELOCK_REQ2_IRQHandler
EDGELOCK_REQ3_IRQHandler
DBG_TRACE_TMR3_IRQHandler
JTAGC_TMR4_IRQHandler
M33_SYSRESET_REQ_IRQHandler
M33_LOCKUP_IRQHandler
M7_SYSRESET_REQ_IRQHandler
M7_LOCKUP_IRQHandler
PWM2_FAULT_IRQHandler
PWM2_0_IRQHandler
PWM2_1_IRQHandler
PWM2_2_IRQHandler
PWM2_3_IRQHandler
PWM3_FAULT_IRQHandler
PWM3_0_IRQHandler
PWM3_1_IRQHandler
PWM3_2_IRQHandler
PWM3_3_IRQHandler
PWM4_FAULT_IRQHandler
PWM4_0_IRQHandler
PWM4_1_IRQHandler
PWM4_2_IRQHandler
PWM4_3_IRQHandler
EQDC1_IRQHandler
EQDC2_IRQHandler
EQDC3_IRQHandler
EQDC4_IRQHandler
ADC2_IRQHandler
DCDC_IRQHandler
CAN3_DriverIRQHandler
CAN3_ERROR_DriverIRQHandler
DAC_IRQHandler
LPSPI5_DriverIRQHandler
LPSPI6_DriverIRQHandler
LPUART7_DriverIRQHandler
LPUART8_DriverIRQHandler
SAI2_DriverIRQHandler
SAI3_DriverIRQHandler
ACMP1_IRQHandler
ACMP2_IRQHandler
ACMP3_IRQHandler
ACMP4_IRQHandler
CM7_PS_IRQHandler
CM7_MCM_IRQHandler
CM33_MCM_IRQHandler
ECAT_INT_IRQHandler
SAFETY_CLK_MON_IRQHandler
GPT1_IRQHandler
GPT2_IRQHandler
KPP_IRQHandler
USBPHY1_IRQHandler
USBPHY2_IRQHandler
USB_OTG2_IRQHandler
USB_OTG1_IRQHandler
FLEXSPI_SLV_DriverIRQHandler
NETC_IRQHandler
MSGINTR1_IRQHandler
MSGINTR2_IRQHandler
MSGINTR3_IRQHandler
MSGINTR4_IRQHandler
MSGINTR5_IRQHandler
MSGINTR6_IRQHandler
SINC1_CH0_IRQHandler
SINC1_CH1_IRQHandler
SINC1_CH2_IRQHandler
SINC1_CH3_IRQHandler
SINC2_CH0_IRQHandler
SINC2_CH1_IRQHandler
SINC2_CH2_IRQHandler
SINC2_CH3_IRQHandler
GPIO4_0_IRQHandler
GPIO4_1_IRQHandler
GPIO5_0_IRQHandler
GPIO5_1_IRQHandler
GPIO6_0_IRQHandler
GPIO6_1_IRQHandler
ECAT_RST_OUT_IRQHandler
DefaultISR
        B DefaultISR

        END
