; -------------------------------------------------------------------------
;  @file:    startup_RW612.s
;  @purpose: CMSIS Cortex-M33 Core Device Startup File
;            RW612
;  @version: 1.0
;  @date:    2021-3-16
;  @build:   b221116
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
        SECTION RO:CODE:NOROOT(2)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

;;The vector table is not needed for initialization.
__iar_init$$done

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
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
        DCD     0x100000 ;Image length
#else
        DCD     sfe(RO) - __vector_table ;Image length
#endif
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     SVC_Handler                                   ;SVCall Handler
        DCD     DebugMon_Handler                              ;Debug Monitor Handler
        DCD     __vector_table                                ;Image load address
        DCD     PendSV_Handler                                ;PendSV Handler
        DCD     SysTick_Handler                               ;SysTick Handler

                                                              ;External Interrupts
        DCD     WDT0_IRQHandler                               ;Windowed watchdog timer 0 (CM33 watchdog)
        DCD     DMA0_IRQHandler                               ;DMA controller 0 (secure or CM33 DMA)
        DCD     GPIO_INTA_IRQHandler                          ;GPIO interrupt A
        DCD     GPIO_INTB_IRQHandler                          ;GPIO interrupt B
        DCD     PIN_INT0_IRQHandler                           ;Pin interrupt 0 or pattern match engine slice 0 int
        DCD     PIN_INT1_IRQHandler                           ;Pin interrupt 1 or pattern match engine slice 1 int
        DCD     PIN_INT2_IRQHandler                           ;Pin interrupt 2 or pattern match engine slice 2 int
        DCD     PIN_INT3_IRQHandler                           ;Pin interrupt 3 or pattern match engine slice 3 int
        DCD     UTICK_IRQHandler                              ;Micro-tick Timer
        DCD     MRT0_IRQHandler                               ;Multi-Rate Timer. Global MRT interrupts
        DCD     CTIMER0_IRQHandler                            ;Standard counter/timer CTIMER0
        DCD     CTIMER1_IRQHandler                            ;Standard counter/timer CTIMER1
        DCD     SCT0_IRQHandler                               ;SCTimer/PWM
        DCD     CTIMER3_IRQHandler                            ;Standard counter/timer CTIMER3
        DCD     FLEXCOMM0_IRQHandler                          ;Flexcomm Interface 0 (USART, SPI, I2C, I2S)
        DCD     FLEXCOMM1_IRQHandler                          ;Flexcomm Interface 1 (USART, SPI, I2C, I2S)
        DCD     FLEXCOMM2_IRQHandler                          ;Flexcomm Interface 2 (USART, SPI, I2C, I2S)
        DCD     FLEXCOMM3_IRQHandler                          ;Flexcomm Interface 3 (USART, SPI, I2C, I2S)
        DCD     Reserved34_IRQHandler                         ;xxx Interrupt 34
        DCD     Reserved35_IRQHandler                         ;xxx Interrupt 35
        DCD     FLEXCOMM14_IRQHandler                         ;Flexcomm Interface 14 (USART, SPI, I2C, I2S)
        DCD     Reserved37_IRQHandler                         ;xxx Interrupt 37
        DCD     Reserved38_IRQHandler                         ;xxx Interrupt 38
        DCD     MRT1_IRQHandler                               ;Free Multi-rate timer. Global MRT interrupts
        DCD     Reserved40_IRQHandler                         ;xxx Interrupt 40
        DCD     DMIC0_IRQHandler                              ;Digital microphone and DMIC subsystem
        DCD     WFD_IRQHandler                                ;Wakeup From Deepsleep
        DCD     HYPERVISOR_IRQHandler                         ;Hypervisor service software interrupt
        DCD     SECUREVIOLATION_IRQHandler                    ;Secure violation
        DCD     HWVAD0_IRQHandler                             ;Hardware Voice Activity Detector
        DCD     Reserved46_IRQHandler                         ;xxx Interrupt 46
        DCD     Reserved47_IRQHandler                         ;xxx Interrupt 47
        DCD     RTC_IRQHandler                                ;RTC alarm and wake-up
        DCD     Reserved49_IRQHandler                         ;xxx Interrupt 49
        DCD     Reserved50_IRQHandler                         ;xxx Interrupt 50
        DCD     PIN_INT4_IRQHandler                           ;Pin interrupt 4 or pattern match engine slice 4 int
        DCD     PIN_INT5_IRQHandler                           ;Pin interrupt 5 or pattern match engine slice 5 int
        DCD     PIN_INT6_IRQHandler                           ;Pin interrupt 6 or pattern match engine slice 6 int
        DCD     PIN_INT7_IRQHandler                           ;Pin interrupt 7 or pattern match engine slice 7 int
        DCD     CTIMER2_IRQHandler                            ;Standard counter/timer CTIMER2
        DCD     CTIMER4_IRQHandler                            ;Standard counter/timer CTIMER4
        DCD     OS_EVENT_IRQHandler                           ;OS event timer
        DCD     FLEXSPI_IRQHandler                            ;FLEXSPI interface
        DCD     Reserved59_IRQHandler                         ;xxx Interrupt 59
        DCD     Reserved60_IRQHandler                         ;xxx Interrupt 60
        DCD     Reserved61_IRQHandler                         ;xxx Interrupt 61
        DCD     SDU_IRQHandler                                ;SDIO
        DCD     SGPIO_INTA_IRQHandler                         ;Secure GPIO interrupt A
        DCD     SGPIO_INTB_IRQHandler                         ;Secure GPIO interrupt B
        DCD     Reserved65_IRQHandler                         ;xxx Interrupt 65
        DCD     USB_IRQHandler                                ;High-speed USB device/host
        DCD     Reserved67_IRQHandler                         ;xxx Interrupt 67
        DCD     Reserved68_IRQHandler                         ;xxx Interrupt 68
        DCD     Reserved69_IRQHandler                         ;xxx Interrupt 69
        DCD     DMA1_IRQHandler                               ;DMA controller 1 (non-secure or HiFi 4 DMA)
        DCD     PUF_IRQHandler                                ;Physical Unclonable Function
        DCD     POWERQUAD_IRQHandler                          ;PowerQuad math coprocessor
        DCD     Reserved73_IRQHandler                         ;xxx Interrupt 73
        DCD     Reserved74_IRQHandler                         ;xxx Interrupt 74
        DCD     Reserved75_IRQHandler                         ;xxx Interrupt 75
        DCD     Reserved76_IRQHandler                         ;xxx Interrupt 76
        DCD     LCDIC_IRQHandler                              ;LCDIC
        DCD     CAPT_PULSE_IRQHandler                         ;Capture timer
        DCD     Reserved79_IRQHandler                         ;xxx Interrupt 79
        DCD     WL_MCI_WAKEUP_DONE0_IRQHandler                ;WL to MCI, Wakeup done 0
        DCD     WL_MCI_WAKEUP_DONE1_IRQHandler                ;WL to MCI, Wakeup done 1
        DCD     WL_MCI_WAKEUP_DONE2_IRQHandler                ;WL to MCI, Wakeup done 2
        DCD     WL_MCI_WAKEUP_DONE3_IRQHandler                ;WL to MCI, Wakeup done 3
        DCD     WL_MCI_WAKEUP_DONE4_IRQHandler                ;WL to MCI, Wakeup done 4
        DCD     WL_MCI_WAKEUP_DONE5_IRQHandler                ;WL to MCI, Wakeup done 5
        DCD     WL_MCI_WAKEUP_DONE6_IRQHandler                ;WL to MCI, Wakeup done 6
        DCD     WL_MCI_WAKEUP_DONE7_IRQHandler                ;WL to MCI, Wakeup done 7
        DCD     WL_MCI_WAKEUP0_IRQHandler                     ;IMU_INT0: Cpu1_to_cpu3_msg_rdy_imu wl_mci_wakeup[0]
        DCD     WL_MCI_WAKEUP1_IRQHandler                     ;GP_INT from WL
        DCD     WL_MCI_INT0_IRQHandler                        ;IMU_INT: Imu13_cpu3_msg_space_avail
        DCD     WL_MCI_INT1_IRQHandler                        ;reserved
        DCD     WL_MCI_INT2_IRQHandler                        ;reserved
        DCD     WL_MCI_INT3_IRQHandler                        ;reserved
        DCD     WL_MCI_INT4_IRQHandler                        ;reserved
        DCD     WL_MCI_INT5_IRQHandler                        ;reserved
        DCD     WL_MCI_INT6_IRQHandler                        ;reserved
        DCD     WL_MCI_INT7_IRQHandler                        ;reserved
        DCD     BLE_MCI_WAKEUP_DONE0_IRQHandler               ;BLE to MCI, Wakeup done 0
        DCD     BLE_MCI_WAKEUP_DONE1_IRQHandler               ;BLE to MCI, Wakeup done 1
        DCD     BLE_MCI_WAKEUP_DONE2_IRQHandler               ;BLE to MCI, Wakeup done 2
        DCD     BLE_MCI_WAKEUP_DONE3_IRQHandler               ;BLE to MCI, Wakeup done 3
        DCD     BLE_MCI_WAKEUP_DONE4_IRQHandler               ;BLE to MCI, Wakeup done 4
        DCD     BLE_MCI_WAKEUP_DONE5_IRQHandler               ;BLE to MCI, Wakeup done 5
        DCD     BLE_MCI_WAKEUP_DONE6_IRQHandler               ;BLE to MCI, Wakeup done 6
        DCD     BLE_MCI_WAKEUP_DONE7_IRQHandler               ;BLE to MCI, Wakeup done 7
        DCD     BLE_MCI_WAKEUP0_IRQHandler                    ;IMU_INT0: Cpu2_to_cpu3_msg_rdy_imu wl_mci_wakeup[0]
        DCD     BLE_MCI_WAKEUP1_IRQHandler                    ;GP_INT from BLE
        DCD     BLE_MCI_INT0_IRQHandler                       ;IMU_INT: Imu13_cpu3_msg_space_avail
        DCD     BLE_MCI_INT1_IRQHandler                       ;reserved
        DCD     BLE_MCI_INT2_IRQHandler                       ;reserved
        DCD     BLE_MCI_INT3_IRQHandler                       ;reserved
        DCD     BLE_MCI_INT4_IRQHandler                       ;reserved
        DCD     BLE_MCI_INT5_IRQHandler                       ;reserved
        DCD     BLE_MCI_INT6_IRQHandler                       ;reserved
        DCD     BLE_MCI_INT7_IRQHandler                       ;reserved
        DCD     PIN0_INT_IRQHandler                           ;From AON GPIO
        DCD     PIN1_INT_IRQHandler                           ;From AON GPIO
        DCD     ELS_IRQHandler                                ;ELS
        DCD     ELS_GDET_IRQ_IRQHandler                       ;ELS IRQ line for GDET error
        DCD     ELS_GDET_ERR_IRQHandler                       ;ELS Ungated latched error
        DCD     PKC_IRQHandler                                ;PKC interrupt
        DCD     PKC_ERR_IRQHandler                            ;PKC error
        DCD     CDOG_IRQHandler                               ;Code watch dog timmer
        DCD     GAU_GPDAC_INT_FUNC11_IRQHandler               ;GAU
        DCD     GAU_ACOMP_INT_WKUP11_IRQHandler               ;GAU
        DCD     GAU_ACOMP_INT_FUNC11_IRQHandler               ;GAU
        DCD     GAU_GPADC1_INT_FUNC11_IRQHandler              ;GAU
        DCD     GAU_GPADC0_INT_FUNC11_IRQHandler              ;GAU
        DCD     USIM_IRQHandler                               ;USIM
        DCD     OCOTP_IRQHandler                              ;OTP
        DCD     ENET_IRQHandler                               ;ENET
        DCD     ENET_TIMER_IRQHandler                         ;ENET
        DCD     BOD_1_85_INT_IRQHandler                       ;PMIP
        DCD     BOD_1_85_NEG_IRQHandler                       ;Bod_1_85_int negedge
        DCD     ITRC_IRQHandler                               ;ITRC
        DCD     BTU_HOST_TRIGGER0_IRQHandler                  ;Btu host trigger0
        DCD     BTU_HOST_TRIGGER1_IRQHandler                  ;Btu host trigger1
        DCD     BTU_HOST_TRIGGER2_IRQHandler                  ;Btu host trigger2
        DCD     TRNG_IRQHandler                               ;TRNG
        DCD     AHB_MEM_ACC_CHECKER_VIO_INT_C_OR_IRQHandler    ;ahb memory access checker - CM33 code bus
        DCD     AHB_MEM_ACC_CHECKER_VIO_INT_S_OR_IRQHandler    ;ahb memory access checker - CM33 sys bus
        DCD     WL_ACC_INT_IRQHandler                         ;Cpu access wlan when wlan is powered off
        DCD     BLE_ACC_INT_IRQHandler                        ;Cpu access wlan when ble is powered off
        DCD     GDMA_IRQHandler                               ;GDMA
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
        CPSIE   I               ; Unmask interrupts
        LDR     R0, =SystemInit
        BLX     R0
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

        PUBWEAK WDT0_IRQHandler
        PUBWEAK WDT0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WDT0_IRQHandler
        LDR     R0, =WDT0_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_IRQHandler
        PUBWEAK DMA0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_IRQHandler
        LDR     R0, =DMA0_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_INTA_IRQHandler
        PUBWEAK GPIO_INTA_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_INTA_IRQHandler
        LDR     R0, =GPIO_INTA_DriverIRQHandler
        BX      R0

        PUBWEAK GPIO_INTB_IRQHandler
        PUBWEAK GPIO_INTB_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GPIO_INTB_IRQHandler
        LDR     R0, =GPIO_INTB_DriverIRQHandler
        BX      R0

        PUBWEAK PIN_INT0_IRQHandler
        PUBWEAK PIN_INT0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN_INT0_IRQHandler
        LDR     R0, =PIN_INT0_DriverIRQHandler
        BX      R0

        PUBWEAK PIN_INT1_IRQHandler
        PUBWEAK PIN_INT1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN_INT1_IRQHandler
        LDR     R0, =PIN_INT1_DriverIRQHandler
        BX      R0

        PUBWEAK PIN_INT2_IRQHandler
        PUBWEAK PIN_INT2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN_INT2_IRQHandler
        LDR     R0, =PIN_INT2_DriverIRQHandler
        BX      R0

        PUBWEAK PIN_INT3_IRQHandler
        PUBWEAK PIN_INT3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN_INT3_IRQHandler
        LDR     R0, =PIN_INT3_DriverIRQHandler
        BX      R0

        PUBWEAK UTICK_IRQHandler
        PUBWEAK UTICK_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
UTICK_IRQHandler
        LDR     R0, =UTICK_DriverIRQHandler
        BX      R0

        PUBWEAK MRT0_IRQHandler
        PUBWEAK MRT0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MRT0_IRQHandler
        LDR     R0, =MRT0_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER0_IRQHandler
        PUBWEAK CTIMER0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER0_IRQHandler
        LDR     R0, =CTIMER0_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER1_IRQHandler
        PUBWEAK CTIMER1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER1_IRQHandler
        LDR     R0, =CTIMER1_DriverIRQHandler
        BX      R0

        PUBWEAK SCT0_IRQHandler
        PUBWEAK SCT0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SCT0_IRQHandler
        LDR     R0, =SCT0_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER3_IRQHandler
        PUBWEAK CTIMER3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER3_IRQHandler
        LDR     R0, =CTIMER3_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXCOMM0_IRQHandler
        PUBWEAK FLEXCOMM0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXCOMM0_IRQHandler
        LDR     R0, =FLEXCOMM0_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXCOMM1_IRQHandler
        PUBWEAK FLEXCOMM1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXCOMM1_IRQHandler
        LDR     R0, =FLEXCOMM1_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXCOMM2_IRQHandler
        PUBWEAK FLEXCOMM2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXCOMM2_IRQHandler
        LDR     R0, =FLEXCOMM2_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXCOMM3_IRQHandler
        PUBWEAK FLEXCOMM3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXCOMM3_IRQHandler
        LDR     R0, =FLEXCOMM3_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved34_IRQHandler
        PUBWEAK Reserved34_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved34_IRQHandler
        LDR     R0, =Reserved34_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved35_IRQHandler
        PUBWEAK Reserved35_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved35_IRQHandler
        LDR     R0, =Reserved35_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXCOMM14_IRQHandler
        PUBWEAK FLEXCOMM14_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXCOMM14_IRQHandler
        LDR     R0, =FLEXCOMM14_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved37_IRQHandler
        PUBWEAK Reserved37_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved37_IRQHandler
        LDR     R0, =Reserved37_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved38_IRQHandler
        PUBWEAK Reserved38_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved38_IRQHandler
        LDR     R0, =Reserved38_DriverIRQHandler
        BX      R0

        PUBWEAK MRT1_IRQHandler
        PUBWEAK MRT1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
MRT1_IRQHandler
        LDR     R0, =MRT1_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved40_IRQHandler
        PUBWEAK Reserved40_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved40_IRQHandler
        LDR     R0, =Reserved40_DriverIRQHandler
        BX      R0

        PUBWEAK DMIC0_IRQHandler
        PUBWEAK DMIC0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMIC0_IRQHandler
        LDR     R0, =DMIC0_DriverIRQHandler
        BX      R0

        PUBWEAK WFD_IRQHandler
        PUBWEAK WFD_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WFD_IRQHandler
        LDR     R0, =WFD_DriverIRQHandler
        BX      R0

        PUBWEAK HYPERVISOR_IRQHandler
        PUBWEAK HYPERVISOR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
HYPERVISOR_IRQHandler
        LDR     R0, =HYPERVISOR_DriverIRQHandler
        BX      R0

        PUBWEAK SECUREVIOLATION_IRQHandler
        PUBWEAK SECUREVIOLATION_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SECUREVIOLATION_IRQHandler
        LDR     R0, =SECUREVIOLATION_DriverIRQHandler
        BX      R0

        PUBWEAK HWVAD0_IRQHandler
        PUBWEAK HWVAD0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
HWVAD0_IRQHandler
        LDR     R0, =HWVAD0_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved46_IRQHandler
        PUBWEAK Reserved46_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved46_IRQHandler
        LDR     R0, =Reserved46_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved47_IRQHandler
        PUBWEAK Reserved47_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved47_IRQHandler
        LDR     R0, =Reserved47_DriverIRQHandler
        BX      R0

        PUBWEAK RTC_IRQHandler
        PUBWEAK RTC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
RTC_IRQHandler
        LDR     R0, =RTC_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved49_IRQHandler
        PUBWEAK Reserved49_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved49_IRQHandler
        LDR     R0, =Reserved49_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved50_IRQHandler
        PUBWEAK Reserved50_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved50_IRQHandler
        LDR     R0, =Reserved50_DriverIRQHandler
        BX      R0

        PUBWEAK PIN_INT4_IRQHandler
        PUBWEAK PIN_INT4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN_INT4_IRQHandler
        LDR     R0, =PIN_INT4_DriverIRQHandler
        BX      R0

        PUBWEAK PIN_INT5_IRQHandler
        PUBWEAK PIN_INT5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN_INT5_IRQHandler
        LDR     R0, =PIN_INT5_DriverIRQHandler
        BX      R0

        PUBWEAK PIN_INT6_IRQHandler
        PUBWEAK PIN_INT6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN_INT6_IRQHandler
        LDR     R0, =PIN_INT6_DriverIRQHandler
        BX      R0

        PUBWEAK PIN_INT7_IRQHandler
        PUBWEAK PIN_INT7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN_INT7_IRQHandler
        LDR     R0, =PIN_INT7_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER2_IRQHandler
        PUBWEAK CTIMER2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER2_IRQHandler
        LDR     R0, =CTIMER2_DriverIRQHandler
        BX      R0

        PUBWEAK CTIMER4_IRQHandler
        PUBWEAK CTIMER4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTIMER4_IRQHandler
        LDR     R0, =CTIMER4_DriverIRQHandler
        BX      R0

        PUBWEAK OS_EVENT_IRQHandler
        PUBWEAK OS_EVENT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
OS_EVENT_IRQHandler
        LDR     R0, =OS_EVENT_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXSPI_IRQHandler
        PUBWEAK FLEXSPI_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXSPI_IRQHandler
        LDR     R0, =FLEXSPI_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved59_IRQHandler
        PUBWEAK Reserved59_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved59_IRQHandler
        LDR     R0, =Reserved59_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved60_IRQHandler
        PUBWEAK Reserved60_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved60_IRQHandler
        LDR     R0, =Reserved60_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved61_IRQHandler
        PUBWEAK Reserved61_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved61_IRQHandler
        LDR     R0, =Reserved61_DriverIRQHandler
        BX      R0

        PUBWEAK SDU_IRQHandler
        PUBWEAK SDU_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SDU_IRQHandler
        LDR     R0, =SDU_DriverIRQHandler
        BX      R0

        PUBWEAK SGPIO_INTA_IRQHandler
        PUBWEAK SGPIO_INTA_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SGPIO_INTA_IRQHandler
        LDR     R0, =SGPIO_INTA_DriverIRQHandler
        BX      R0

        PUBWEAK SGPIO_INTB_IRQHandler
        PUBWEAK SGPIO_INTB_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
SGPIO_INTB_IRQHandler
        LDR     R0, =SGPIO_INTB_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved65_IRQHandler
        PUBWEAK Reserved65_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved65_IRQHandler
        LDR     R0, =Reserved65_DriverIRQHandler
        BX      R0

        PUBWEAK USB_IRQHandler
        PUBWEAK USB_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
USB_IRQHandler
        LDR     R0, =USB_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved67_IRQHandler
        PUBWEAK Reserved67_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved67_IRQHandler
        LDR     R0, =Reserved67_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved68_IRQHandler
        PUBWEAK Reserved68_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved68_IRQHandler
        LDR     R0, =Reserved68_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved69_IRQHandler
        PUBWEAK Reserved69_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved69_IRQHandler
        LDR     R0, =Reserved69_DriverIRQHandler
        BX      R0

        PUBWEAK DMA1_IRQHandler
        PUBWEAK DMA1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA1_IRQHandler
        LDR     R0, =DMA1_DriverIRQHandler
        BX      R0

        PUBWEAK PUF_IRQHandler
        PUBWEAK PUF_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PUF_IRQHandler
        LDR     R0, =PUF_DriverIRQHandler
        BX      R0

        PUBWEAK POWERQUAD_IRQHandler
        PUBWEAK POWERQUAD_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
POWERQUAD_IRQHandler
        LDR     R0, =POWERQUAD_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved73_IRQHandler
        PUBWEAK Reserved73_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved73_IRQHandler
        LDR     R0, =Reserved73_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved74_IRQHandler
        PUBWEAK Reserved74_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved74_IRQHandler
        LDR     R0, =Reserved74_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved75_IRQHandler
        PUBWEAK Reserved75_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved75_IRQHandler
        LDR     R0, =Reserved75_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved76_IRQHandler
        PUBWEAK Reserved76_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved76_IRQHandler
        LDR     R0, =Reserved76_DriverIRQHandler
        BX      R0

        PUBWEAK LCDIC_IRQHandler
        PUBWEAK LCDIC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LCDIC_IRQHandler
        LDR     R0, =LCDIC_DriverIRQHandler
        BX      R0

        PUBWEAK CAPT_PULSE_IRQHandler
        PUBWEAK CAPT_PULSE_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CAPT_PULSE_IRQHandler
        LDR     R0, =CAPT_PULSE_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved79_IRQHandler
        PUBWEAK Reserved79_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reserved79_IRQHandler
        LDR     R0, =Reserved79_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_WAKEUP_DONE0_IRQHandler
        PUBWEAK WL_MCI_WAKEUP_DONE0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_WAKEUP_DONE0_IRQHandler
        LDR     R0, =WL_MCI_WAKEUP_DONE0_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_WAKEUP_DONE1_IRQHandler
        PUBWEAK WL_MCI_WAKEUP_DONE1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_WAKEUP_DONE1_IRQHandler
        LDR     R0, =WL_MCI_WAKEUP_DONE1_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_WAKEUP_DONE2_IRQHandler
        PUBWEAK WL_MCI_WAKEUP_DONE2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_WAKEUP_DONE2_IRQHandler
        LDR     R0, =WL_MCI_WAKEUP_DONE2_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_WAKEUP_DONE3_IRQHandler
        PUBWEAK WL_MCI_WAKEUP_DONE3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_WAKEUP_DONE3_IRQHandler
        LDR     R0, =WL_MCI_WAKEUP_DONE3_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_WAKEUP_DONE4_IRQHandler
        PUBWEAK WL_MCI_WAKEUP_DONE4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_WAKEUP_DONE4_IRQHandler
        LDR     R0, =WL_MCI_WAKEUP_DONE4_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_WAKEUP_DONE5_IRQHandler
        PUBWEAK WL_MCI_WAKEUP_DONE5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_WAKEUP_DONE5_IRQHandler
        LDR     R0, =WL_MCI_WAKEUP_DONE5_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_WAKEUP_DONE6_IRQHandler
        PUBWEAK WL_MCI_WAKEUP_DONE6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_WAKEUP_DONE6_IRQHandler
        LDR     R0, =WL_MCI_WAKEUP_DONE6_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_WAKEUP_DONE7_IRQHandler
        PUBWEAK WL_MCI_WAKEUP_DONE7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_WAKEUP_DONE7_IRQHandler
        LDR     R0, =WL_MCI_WAKEUP_DONE7_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_WAKEUP0_IRQHandler
        PUBWEAK WL_MCI_WAKEUP0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_WAKEUP0_IRQHandler
        LDR     R0, =WL_MCI_WAKEUP0_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_WAKEUP1_IRQHandler
        PUBWEAK WL_MCI_WAKEUP1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_WAKEUP1_IRQHandler
        LDR     R0, =WL_MCI_WAKEUP1_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_INT0_IRQHandler
        PUBWEAK WL_MCI_INT0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_INT0_IRQHandler
        LDR     R0, =WL_MCI_INT0_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_INT1_IRQHandler
        PUBWEAK WL_MCI_INT1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_INT1_IRQHandler
        LDR     R0, =WL_MCI_INT1_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_INT2_IRQHandler
        PUBWEAK WL_MCI_INT2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_INT2_IRQHandler
        LDR     R0, =WL_MCI_INT2_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_INT3_IRQHandler
        PUBWEAK WL_MCI_INT3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_INT3_IRQHandler
        LDR     R0, =WL_MCI_INT3_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_INT4_IRQHandler
        PUBWEAK WL_MCI_INT4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_INT4_IRQHandler
        LDR     R0, =WL_MCI_INT4_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_INT5_IRQHandler
        PUBWEAK WL_MCI_INT5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_INT5_IRQHandler
        LDR     R0, =WL_MCI_INT5_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_INT6_IRQHandler
        PUBWEAK WL_MCI_INT6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_INT6_IRQHandler
        LDR     R0, =WL_MCI_INT6_DriverIRQHandler
        BX      R0

        PUBWEAK WL_MCI_INT7_IRQHandler
        PUBWEAK WL_MCI_INT7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_MCI_INT7_IRQHandler
        LDR     R0, =WL_MCI_INT7_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_WAKEUP_DONE0_IRQHandler
        PUBWEAK BLE_MCI_WAKEUP_DONE0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_WAKEUP_DONE0_IRQHandler
        LDR     R0, =BLE_MCI_WAKEUP_DONE0_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_WAKEUP_DONE1_IRQHandler
        PUBWEAK BLE_MCI_WAKEUP_DONE1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_WAKEUP_DONE1_IRQHandler
        LDR     R0, =BLE_MCI_WAKEUP_DONE1_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_WAKEUP_DONE2_IRQHandler
        PUBWEAK BLE_MCI_WAKEUP_DONE2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_WAKEUP_DONE2_IRQHandler
        LDR     R0, =BLE_MCI_WAKEUP_DONE2_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_WAKEUP_DONE3_IRQHandler
        PUBWEAK BLE_MCI_WAKEUP_DONE3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_WAKEUP_DONE3_IRQHandler
        LDR     R0, =BLE_MCI_WAKEUP_DONE3_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_WAKEUP_DONE4_IRQHandler
        PUBWEAK BLE_MCI_WAKEUP_DONE4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_WAKEUP_DONE4_IRQHandler
        LDR     R0, =BLE_MCI_WAKEUP_DONE4_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_WAKEUP_DONE5_IRQHandler
        PUBWEAK BLE_MCI_WAKEUP_DONE5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_WAKEUP_DONE5_IRQHandler
        LDR     R0, =BLE_MCI_WAKEUP_DONE5_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_WAKEUP_DONE6_IRQHandler
        PUBWEAK BLE_MCI_WAKEUP_DONE6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_WAKEUP_DONE6_IRQHandler
        LDR     R0, =BLE_MCI_WAKEUP_DONE6_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_WAKEUP_DONE7_IRQHandler
        PUBWEAK BLE_MCI_WAKEUP_DONE7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_WAKEUP_DONE7_IRQHandler
        LDR     R0, =BLE_MCI_WAKEUP_DONE7_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_WAKEUP0_IRQHandler
        PUBWEAK BLE_MCI_WAKEUP0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_WAKEUP0_IRQHandler
        LDR     R0, =BLE_MCI_WAKEUP0_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_WAKEUP1_IRQHandler
        PUBWEAK BLE_MCI_WAKEUP1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_WAKEUP1_IRQHandler
        LDR     R0, =BLE_MCI_WAKEUP1_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_INT0_IRQHandler
        PUBWEAK BLE_MCI_INT0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_INT0_IRQHandler
        LDR     R0, =BLE_MCI_INT0_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_INT1_IRQHandler
        PUBWEAK BLE_MCI_INT1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_INT1_IRQHandler
        LDR     R0, =BLE_MCI_INT1_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_INT2_IRQHandler
        PUBWEAK BLE_MCI_INT2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_INT2_IRQHandler
        LDR     R0, =BLE_MCI_INT2_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_INT3_IRQHandler
        PUBWEAK BLE_MCI_INT3_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_INT3_IRQHandler
        LDR     R0, =BLE_MCI_INT3_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_INT4_IRQHandler
        PUBWEAK BLE_MCI_INT4_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_INT4_IRQHandler
        LDR     R0, =BLE_MCI_INT4_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_INT5_IRQHandler
        PUBWEAK BLE_MCI_INT5_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_INT5_IRQHandler
        LDR     R0, =BLE_MCI_INT5_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_INT6_IRQHandler
        PUBWEAK BLE_MCI_INT6_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_INT6_IRQHandler
        LDR     R0, =BLE_MCI_INT6_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_MCI_INT7_IRQHandler
        PUBWEAK BLE_MCI_INT7_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_MCI_INT7_IRQHandler
        LDR     R0, =BLE_MCI_INT7_DriverIRQHandler
        BX      R0

        PUBWEAK PIN0_INT_IRQHandler
        PUBWEAK PIN0_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN0_INT_IRQHandler
        LDR     R0, =PIN0_INT_DriverIRQHandler
        BX      R0

        PUBWEAK PIN1_INT_IRQHandler
        PUBWEAK PIN1_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PIN1_INT_IRQHandler
        LDR     R0, =PIN1_INT_DriverIRQHandler
        BX      R0

        PUBWEAK ELS_IRQHandler
        PUBWEAK ELS_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ELS_IRQHandler
        LDR     R0, =ELS_DriverIRQHandler
        BX      R0

        PUBWEAK ELS_GDET_IRQ_IRQHandler
        PUBWEAK ELS_GDET_IRQ_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ELS_GDET_IRQ_IRQHandler
        LDR     R0, =ELS_GDET_IRQ_DriverIRQHandler
        BX      R0

        PUBWEAK ELS_GDET_ERR_IRQHandler
        PUBWEAK ELS_GDET_ERR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ELS_GDET_ERR_IRQHandler
        LDR     R0, =ELS_GDET_ERR_DriverIRQHandler
        BX      R0

        PUBWEAK PKC_IRQHandler
        PUBWEAK PKC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PKC_IRQHandler
        LDR     R0, =PKC_DriverIRQHandler
        BX      R0

        PUBWEAK PKC_ERR_IRQHandler
        PUBWEAK PKC_ERR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
PKC_ERR_IRQHandler
        LDR     R0, =PKC_ERR_DriverIRQHandler
        BX      R0

        PUBWEAK CDOG_IRQHandler
        PUBWEAK CDOG_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CDOG_IRQHandler
        LDR     R0, =CDOG_DriverIRQHandler
        BX      R0

        PUBWEAK GAU_GPDAC_INT_FUNC11_IRQHandler
        PUBWEAK GAU_GPDAC_INT_FUNC11_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GAU_GPDAC_INT_FUNC11_IRQHandler
        LDR     R0, =GAU_GPDAC_INT_FUNC11_DriverIRQHandler
        BX      R0

        PUBWEAK GAU_ACOMP_INT_WKUP11_IRQHandler
        PUBWEAK GAU_ACOMP_INT_WKUP11_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GAU_ACOMP_INT_WKUP11_IRQHandler
        LDR     R0, =GAU_ACOMP_INT_WKUP11_DriverIRQHandler
        BX      R0

        PUBWEAK GAU_ACOMP_INT_FUNC11_IRQHandler
        PUBWEAK GAU_ACOMP_INT_FUNC11_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GAU_ACOMP_INT_FUNC11_IRQHandler
        LDR     R0, =GAU_ACOMP_INT_FUNC11_DriverIRQHandler
        BX      R0

        PUBWEAK GAU_GPADC1_INT_FUNC11_IRQHandler
        PUBWEAK GAU_GPADC1_INT_FUNC11_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GAU_GPADC1_INT_FUNC11_IRQHandler
        LDR     R0, =GAU_GPADC1_INT_FUNC11_DriverIRQHandler
        BX      R0

        PUBWEAK GAU_GPADC0_INT_FUNC11_IRQHandler
        PUBWEAK GAU_GPADC0_INT_FUNC11_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GAU_GPADC0_INT_FUNC11_IRQHandler
        LDR     R0, =GAU_GPADC0_INT_FUNC11_DriverIRQHandler
        BX      R0

        PUBWEAK USIM_IRQHandler
        PUBWEAK USIM_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
USIM_IRQHandler
        LDR     R0, =USIM_DriverIRQHandler
        BX      R0

        PUBWEAK OCOTP_IRQHandler
        PUBWEAK OCOTP_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
OCOTP_IRQHandler
        LDR     R0, =OCOTP_DriverIRQHandler
        BX      R0

        PUBWEAK ENET_IRQHandler
        PUBWEAK ENET_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET_IRQHandler
        LDR     R0, =ENET_DriverIRQHandler
        BX      R0

        PUBWEAK ENET_TIMER_IRQHandler
        PUBWEAK ENET_TIMER_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ENET_TIMER_IRQHandler
        LDR     R0, =ENET_TIMER_DriverIRQHandler
        BX      R0

        PUBWEAK BOD_1_85_INT_IRQHandler
        PUBWEAK BOD_1_85_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BOD_1_85_INT_IRQHandler
        LDR     R0, =BOD_1_85_INT_DriverIRQHandler
        BX      R0

        PUBWEAK BOD_1_85_NEG_IRQHandler
        PUBWEAK BOD_1_85_NEG_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BOD_1_85_NEG_IRQHandler
        LDR     R0, =BOD_1_85_NEG_DriverIRQHandler
        BX      R0

        PUBWEAK ITRC_IRQHandler
        PUBWEAK ITRC_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
ITRC_IRQHandler
        LDR     R0, =ITRC_DriverIRQHandler
        BX      R0

        PUBWEAK BTU_HOST_TRIGGER0_IRQHandler
        PUBWEAK BTU_HOST_TRIGGER0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BTU_HOST_TRIGGER0_IRQHandler
        LDR     R0, =BTU_HOST_TRIGGER0_DriverIRQHandler
        BX      R0

        PUBWEAK BTU_HOST_TRIGGER1_IRQHandler
        PUBWEAK BTU_HOST_TRIGGER1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BTU_HOST_TRIGGER1_IRQHandler
        LDR     R0, =BTU_HOST_TRIGGER1_DriverIRQHandler
        BX      R0

        PUBWEAK BTU_HOST_TRIGGER2_IRQHandler
        PUBWEAK BTU_HOST_TRIGGER2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BTU_HOST_TRIGGER2_IRQHandler
        LDR     R0, =BTU_HOST_TRIGGER2_DriverIRQHandler
        BX      R0

        PUBWEAK TRNG_IRQHandler
        PUBWEAK TRNG_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
TRNG_IRQHandler
        LDR     R0, =TRNG_DriverIRQHandler
        BX      R0

        PUBWEAK AHB_MEM_ACC_CHECKER_VIO_INT_C_OR_IRQHandler
        PUBWEAK AHB_MEM_ACC_CHECKER_VIO_INT_C_OR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AHB_MEM_ACC_CHECKER_VIO_INT_C_OR_IRQHandler
        LDR     R0, =AHB_MEM_ACC_CHECKER_VIO_INT_C_OR_DriverIRQHandler
        BX      R0

        PUBWEAK AHB_MEM_ACC_CHECKER_VIO_INT_S_OR_IRQHandler
        PUBWEAK AHB_MEM_ACC_CHECKER_VIO_INT_S_OR_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
AHB_MEM_ACC_CHECKER_VIO_INT_S_OR_IRQHandler
        LDR     R0, =AHB_MEM_ACC_CHECKER_VIO_INT_S_OR_DriverIRQHandler
        BX      R0

        PUBWEAK WL_ACC_INT_IRQHandler
        PUBWEAK WL_ACC_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
WL_ACC_INT_IRQHandler
        LDR     R0, =WL_ACC_INT_DriverIRQHandler
        BX      R0

        PUBWEAK BLE_ACC_INT_IRQHandler
        PUBWEAK BLE_ACC_INT_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
BLE_ACC_INT_IRQHandler
        LDR     R0, =BLE_ACC_INT_DriverIRQHandler
        BX      R0

        PUBWEAK GDMA_IRQHandler
        PUBWEAK GDMA_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
GDMA_IRQHandler
        LDR     R0, =GDMA_DriverIRQHandler
        BX      R0

WDT0_DriverIRQHandler
DMA0_DriverIRQHandler
GPIO_INTA_DriverIRQHandler
GPIO_INTB_DriverIRQHandler
PIN_INT0_DriverIRQHandler
PIN_INT1_DriverIRQHandler
PIN_INT2_DriverIRQHandler
PIN_INT3_DriverIRQHandler
UTICK_DriverIRQHandler
MRT0_DriverIRQHandler
CTIMER0_DriverIRQHandler
CTIMER1_DriverIRQHandler
SCT0_DriverIRQHandler
CTIMER3_DriverIRQHandler
FLEXCOMM0_DriverIRQHandler
FLEXCOMM1_DriverIRQHandler
FLEXCOMM2_DriverIRQHandler
FLEXCOMM3_DriverIRQHandler
Reserved34_DriverIRQHandler
Reserved35_DriverIRQHandler
FLEXCOMM14_DriverIRQHandler
Reserved37_DriverIRQHandler
Reserved38_DriverIRQHandler
MRT1_DriverIRQHandler
Reserved40_DriverIRQHandler
DMIC0_DriverIRQHandler
WFD_DriverIRQHandler
HYPERVISOR_DriverIRQHandler
SECUREVIOLATION_DriverIRQHandler
HWVAD0_DriverIRQHandler
Reserved46_DriverIRQHandler
Reserved47_DriverIRQHandler
RTC_DriverIRQHandler
Reserved49_DriverIRQHandler
Reserved50_DriverIRQHandler
PIN_INT4_DriverIRQHandler
PIN_INT5_DriverIRQHandler
PIN_INT6_DriverIRQHandler
PIN_INT7_DriverIRQHandler
CTIMER2_DriverIRQHandler
CTIMER4_DriverIRQHandler
OS_EVENT_DriverIRQHandler
FLEXSPI_DriverIRQHandler
Reserved59_DriverIRQHandler
Reserved60_DriverIRQHandler
Reserved61_DriverIRQHandler
SDU_DriverIRQHandler
SGPIO_INTA_DriverIRQHandler
SGPIO_INTB_DriverIRQHandler
Reserved65_DriverIRQHandler
USB_DriverIRQHandler
Reserved67_DriverIRQHandler
Reserved68_DriverIRQHandler
Reserved69_DriverIRQHandler
DMA1_DriverIRQHandler
PUF_DriverIRQHandler
POWERQUAD_DriverIRQHandler
Reserved73_DriverIRQHandler
Reserved74_DriverIRQHandler
Reserved75_DriverIRQHandler
Reserved76_DriverIRQHandler
LCDIC_DriverIRQHandler
CAPT_PULSE_DriverIRQHandler
Reserved79_DriverIRQHandler
WL_MCI_WAKEUP_DONE0_DriverIRQHandler
WL_MCI_WAKEUP_DONE1_DriverIRQHandler
WL_MCI_WAKEUP_DONE2_DriverIRQHandler
WL_MCI_WAKEUP_DONE3_DriverIRQHandler
WL_MCI_WAKEUP_DONE4_DriverIRQHandler
WL_MCI_WAKEUP_DONE5_DriverIRQHandler
WL_MCI_WAKEUP_DONE6_DriverIRQHandler
WL_MCI_WAKEUP_DONE7_DriverIRQHandler
WL_MCI_WAKEUP0_DriverIRQHandler
WL_MCI_WAKEUP1_DriverIRQHandler
WL_MCI_INT0_DriverIRQHandler
WL_MCI_INT1_DriverIRQHandler
WL_MCI_INT2_DriverIRQHandler
WL_MCI_INT3_DriverIRQHandler
WL_MCI_INT4_DriverIRQHandler
WL_MCI_INT5_DriverIRQHandler
WL_MCI_INT6_DriverIRQHandler
WL_MCI_INT7_DriverIRQHandler
BLE_MCI_WAKEUP_DONE0_DriverIRQHandler
BLE_MCI_WAKEUP_DONE1_DriverIRQHandler
BLE_MCI_WAKEUP_DONE2_DriverIRQHandler
BLE_MCI_WAKEUP_DONE3_DriverIRQHandler
BLE_MCI_WAKEUP_DONE4_DriverIRQHandler
BLE_MCI_WAKEUP_DONE5_DriverIRQHandler
BLE_MCI_WAKEUP_DONE6_DriverIRQHandler
BLE_MCI_WAKEUP_DONE7_DriverIRQHandler
BLE_MCI_WAKEUP0_DriverIRQHandler
BLE_MCI_WAKEUP1_DriverIRQHandler
BLE_MCI_INT0_DriverIRQHandler
BLE_MCI_INT1_DriverIRQHandler
BLE_MCI_INT2_DriverIRQHandler
BLE_MCI_INT3_DriverIRQHandler
BLE_MCI_INT4_DriverIRQHandler
BLE_MCI_INT5_DriverIRQHandler
BLE_MCI_INT6_DriverIRQHandler
BLE_MCI_INT7_DriverIRQHandler
PIN0_INT_DriverIRQHandler
PIN1_INT_DriverIRQHandler
ELS_DriverIRQHandler
ELS_GDET_IRQ_DriverIRQHandler
ELS_GDET_ERR_DriverIRQHandler
PKC_DriverIRQHandler
PKC_ERR_DriverIRQHandler
CDOG_DriverIRQHandler
GAU_GPDAC_INT_FUNC11_DriverIRQHandler
GAU_ACOMP_INT_WKUP11_DriverIRQHandler
GAU_ACOMP_INT_FUNC11_DriverIRQHandler
GAU_GPADC1_INT_FUNC11_DriverIRQHandler
GAU_GPADC0_INT_FUNC11_DriverIRQHandler
USIM_DriverIRQHandler
OCOTP_DriverIRQHandler
ENET_DriverIRQHandler
ENET_TIMER_DriverIRQHandler
BOD_1_85_INT_DriverIRQHandler
BOD_1_85_NEG_DriverIRQHandler
ITRC_DriverIRQHandler
BTU_HOST_TRIGGER0_DriverIRQHandler
BTU_HOST_TRIGGER1_DriverIRQHandler
BTU_HOST_TRIGGER2_DriverIRQHandler
TRNG_DriverIRQHandler
AHB_MEM_ACC_CHECKER_VIO_INT_C_OR_DriverIRQHandler
AHB_MEM_ACC_CHECKER_VIO_INT_S_OR_DriverIRQHandler
WL_ACC_INT_DriverIRQHandler
BLE_ACC_INT_DriverIRQHandler
GDMA_DriverIRQHandler
DefaultISR
        B .

        END
