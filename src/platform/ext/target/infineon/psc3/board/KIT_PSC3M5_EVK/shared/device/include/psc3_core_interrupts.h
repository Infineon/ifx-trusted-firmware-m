/*
 * Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef PSC3_CORE_INTERRUPTS_H
#define PSC3_CORE_INTERRUPTS_H

/* SPE and NSPE are on the same core so the vector table is also the same */
#define IFX_CORE_DEFINE_EXCEPTIONS_LIST \
    DEFAULT_IRQ_HANDLER(NMI_Handler) \
    DEFAULT_IRQ_HANDLER(HardFault_Handler) \
    DEFAULT_IRQ_HANDLER(MemManage_Handler) \
    DEFAULT_IRQ_HANDLER(BusFault_Handler) \
    DEFAULT_IRQ_HANDLER(UsageFault_Handler) \
    DEFAULT_IRQ_HANDLER(SecureFault_Handler) \
    DEFAULT_IRQ_HANDLER(SVC_Handler) \
    DEFAULT_IRQ_HANDLER(DebugMon_Handler) \
    DEFAULT_IRQ_HANDLER(PendSV_Handler) \
    DEFAULT_IRQ_HANDLER(SysTick_Handler)

#define IFX_CORE_DEFINE_INTERRUPTS_LIST \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_gpio_0_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_gpio_1_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_gpio_2_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_gpio_3_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_gpio_4_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_gpio_5_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_gpio_6_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_gpio_7_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_gpio_8_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_gpio_9_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_sec_gpio_0_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_sec_gpio_1_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_sec_gpio_2_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_sec_gpio_3_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_sec_gpio_4_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_sec_gpio_5_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_sec_gpio_6_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_sec_gpio_7_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_sec_gpio_8_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupts_sec_gpio_9_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupt_vdd_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupt_gpio_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(ioss_interrupt_sec_gpio_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(scb_0_interrupt_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(srss_interrupt_mcwdt_0_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(srss_interrupt_backup_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_ipc_dpslp_0_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_ipc_dpslp_1_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(srss_interrupt_wdt_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(lpcomp_interrupt_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(srss_interrupt_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(scb_1_interrupt_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(scb_2_interrupt_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(scb_3_interrupt_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(scb_4_interrupt_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(scb_5_interrupt_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupt_fm_cbus_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw0_0_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw0_1_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw0_2_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw0_3_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw0_4_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw0_5_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw0_6_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw0_7_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw0_8_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw0_9_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw0_10_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw0_11_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw0_12_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw0_13_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw0_14_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw0_15_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw1_0_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw1_1_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw1_2_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw1_3_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw1_4_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw1_5_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw1_6_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw1_7_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw1_8_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw1_9_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw1_10_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw1_11_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw1_12_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw1_13_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw1_14_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_dw1_15_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupt_ppu_sramc0_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupt_cm33_0_fp_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_cm33_0_cti_0_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_cm33_0_cti_1_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupts_fault_0_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupt_ppu_cpuss_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cpuss_interrupt_msc_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_0_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_1_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_2_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_3_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_256_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_257_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_258_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_259_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_260_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_261_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_262_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_263_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_512_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_513_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_514_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_515_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_516_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_517_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_518_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_interrupts_519_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(srss_interrupt_main_ppu_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cryptolite_interrupt_error_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cryptolite_interrupt_trng_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(canfd_0_interrupts0_0_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(canfd_0_interrupts1_0_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(canfd_0_interrupts0_1_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(canfd_0_interrupts1_1_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(canfd_0_interrupt0_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(canfd_0_interrupt1_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(cordic_interrupt_mxcordic_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_motif_interrupt_64_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(tcpwm_0_motif_interrupt_65_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_mcpass_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_results_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_entry_done_0_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_entry_done_1_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_entry_done_2_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_entry_done_3_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_entry_done_4_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_entry_done_5_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_entry_done_6_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_entry_done_7_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_fir_done_0_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_fir_done_1_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_ranges_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_range_0_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_range_1_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_range_2_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_range_3_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_range_4_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_range_5_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_range_6_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_sar_range_7_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_csg_dacs_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_csg_dac_0_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_csg_dac_1_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_csg_dac_2_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_csg_dac_3_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_csg_dac_4_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_csg_dac_5_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_csg_dac_6_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_csg_dac_7_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_csg_cmps_IRQn_Handler) \
    DEFAULT_IRQ_HANDLER(pass_interrupt_fifos_IRQn_Handler)

#define IFX_CORE_EXCEPTIONS_LIST \
    (VECTOR_TABLE_Type)(&__INITIAL_SP),         /*       Initial Stack Pointer */ \
    Reset_Handler,                              /*   -15 Reset Handler */ \
    NMI_Handler,                                /*   -14 NMI Handler */ \
    HardFault_Handler,                          /*   -13 Hard Fault Handler */ \
    MemManage_Handler,                          /*   -12 MPU Fault Handler */ \
    BusFault_Handler,                           /*   -11 Bus Fault Handler */ \
    UsageFault_Handler,                         /*   -10 Usage Fault Handler */ \
    SecureFault_Handler,                        /*    -9 Secure Fault Handler */ \
    0,                                          /*    -8 Reserved */ \
    0,                                          /*    -7 Reserved */ \
    0,                                          /*    -6 Reserved */ \
    SVC_Handler,                                /*    -5 SVCall Handler */ \
    DebugMon_Handler,                           /*    -4 Debug Monitor Handler */ \
    0,                                          /*    -3 Reserved */ \
    PendSV_Handler,                             /*    -2 PendSV Handler */ \
    SysTick_Handler,                            /*    -1 SysTick Handler */

#define IFX_CORE_INTERRUPTS_LIST \
    ioss_interrupts_gpio_0_IRQn_Handler,                /*!<   0 [DeepSleep] GPIO Port Interrupt #0 */ \
    ioss_interrupts_gpio_1_IRQn_Handler,                /*!<   1 [DeepSleep] GPIO Port Interrupt #1 */ \
    ioss_interrupts_gpio_2_IRQn_Handler,                /*!<   2 [DeepSleep] GPIO Port Interrupt #2 */ \
    ioss_interrupts_gpio_3_IRQn_Handler,                /*!<   3 [DeepSleep] GPIO Port Interrupt #3 */ \
    ioss_interrupts_gpio_4_IRQn_Handler,                /*!<   4 [DeepSleep] GPIO Port Interrupt #4 */ \
    ioss_interrupts_gpio_5_IRQn_Handler,                /*!<   5 [DeepSleep] GPIO Port Interrupt #5 */ \
    ioss_interrupts_gpio_6_IRQn_Handler,                /*!<   6 [DeepSleep] GPIO Port Interrupt #6 */ \
    ioss_interrupts_gpio_7_IRQn_Handler,                /*!<   7 [DeepSleep] GPIO Port Interrupt #7 */ \
    ioss_interrupts_gpio_8_IRQn_Handler,                /*!<   8 [DeepSleep] GPIO Port Interrupt #8 */ \
    ioss_interrupts_gpio_9_IRQn_Handler,                /*!<   9 [DeepSleep] GPIO Port Interrupt #9 */ \
    ioss_interrupts_sec_gpio_0_IRQn_Handler,            /*!<  10 [DeepSleep] GPIO Port Secure Interrupt #0 */ \
    ioss_interrupts_sec_gpio_1_IRQn_Handler,            /*!<  11 [DeepSleep] GPIO Port Secure Interrupt #1 */ \
    ioss_interrupts_sec_gpio_2_IRQn_Handler,            /*!<  12 [DeepSleep] GPIO Port Secure Interrupt #2 */ \
    ioss_interrupts_sec_gpio_3_IRQn_Handler,            /*!<  13 [DeepSleep] GPIO Port Secure Interrupt #3 */ \
    ioss_interrupts_sec_gpio_4_IRQn_Handler,            /*!<  14 [DeepSleep] GPIO Port Secure Interrupt #4 */ \
    ioss_interrupts_sec_gpio_5_IRQn_Handler,            /*!<  15 [DeepSleep] GPIO Port Secure Interrupt #5 */ \
    ioss_interrupts_sec_gpio_6_IRQn_Handler,            /*!<  16 [DeepSleep] GPIO Port Secure Interrupt #6 */ \
    ioss_interrupts_sec_gpio_7_IRQn_Handler,            /*!<  17 [DeepSleep] GPIO Port Secure Interrupt #7 */ \
    ioss_interrupts_sec_gpio_8_IRQn_Handler,            /*!<  18 [DeepSleep] GPIO Port Secure Interrupt #8 */ \
    ioss_interrupts_sec_gpio_9_IRQn_Handler,            /*!<  19 [DeepSleep] GPIO Port Secure Interrupt #9 */ \
    ioss_interrupt_vdd_IRQn_Handler,                    /*!<  20 [DeepSleep] GPIO Supply Detect Interrupt */ \
    ioss_interrupt_gpio_IRQn_Handler,                   /*!<  21 [DeepSleep] GPIO All Ports - Interrupts */ \
    ioss_interrupt_sec_gpio_IRQn_Handler,               /*!<  22 [DeepSleep] GPIO All Ports  - Secure  Interrupts */ \
    scb_0_interrupt_IRQn_Handler,                       /*!<  23 [DeepSleep] Serial Communication Block #0 (DeepSleep capable) */ \
    srss_interrupt_mcwdt_0_IRQn_Handler,                /*!<  24 [DeepSleep] Multi Counter Watchdog Timer interrupt */ \
    srss_interrupt_backup_IRQn_Handler,                 /*!<  25 [DeepSleep] Backup domain interrupt */ \
    cpuss_interrupts_ipc_dpslp_0_IRQn_Handler,          /*!<  26 [DeepSleep] cpuss Inter Process Communication Interrupt #0 */ \
    cpuss_interrupts_ipc_dpslp_1_IRQn_Handler,          /*!<  27 [DeepSleep] cpuss Inter Process Communication Interrupt #1 */ \
    srss_interrupt_wdt_IRQn_Handler,                    /*!<  28 [DeepSleep] Interrupt from WDT */ \
    lpcomp_interrupt_IRQn_Handler,                      /*!<  29 [DeepSleep] LPCOMP */ \
    srss_interrupt_IRQn_Handler,                        /*!<  30 [Active] Other combined Interrupts for srss (LVD and CLKCAL, CLKCAL only supported in Active mode) */ \
    scb_1_interrupt_IRQn_Handler,                       /*!<  31 [Active] Serial Communication Block #1 */ \
    scb_2_interrupt_IRQn_Handler,                       /*!<  32 [Active] Serial Communication Block #2 */ \
    scb_3_interrupt_IRQn_Handler,                       /*!<  33 [Active] Serial Communication Block #3 */ \
    scb_4_interrupt_IRQn_Handler,                       /*!<  34 [Active] Serial Communication Block #4 */ \
    scb_5_interrupt_IRQn_Handler,                       /*!<  35 [Active] Serial Communication Block #5 */ \
    cpuss_interrupt_fm_cbus_IRQn_Handler,               /*!<  36 [Active] FLASH Macro interrupt */ \
    cpuss_interrupts_dw0_0_IRQn_Handler,                /*!<  37 [Active] cpuss DataWire #0, Channel #0 */ \
    cpuss_interrupts_dw0_1_IRQn_Handler,                /*!<  38 [Active] cpuss DataWire #0, Channel #1 */ \
    cpuss_interrupts_dw0_2_IRQn_Handler,                /*!<  39 [Active] cpuss DataWire #0, Channel #2 */ \
    cpuss_interrupts_dw0_3_IRQn_Handler,                /*!<  40 [Active] cpuss DataWire #0, Channel #3 */ \
    cpuss_interrupts_dw0_4_IRQn_Handler,                /*!<  41 [Active] cpuss DataWire #0, Channel #4 */ \
    cpuss_interrupts_dw0_5_IRQn_Handler,                /*!<  42 [Active] cpuss DataWire #0, Channel #5 */ \
    cpuss_interrupts_dw0_6_IRQn_Handler,                /*!<  43 [Active] cpuss DataWire #0, Channel #6 */ \
    cpuss_interrupts_dw0_7_IRQn_Handler,                /*!<  44 [Active] cpuss DataWire #0, Channel #7 */ \
    cpuss_interrupts_dw0_8_IRQn_Handler,                /*!<  45 [Active] cpuss DataWire #0, Channel #8 */ \
    cpuss_interrupts_dw0_9_IRQn_Handler,                /*!<  46 [Active] cpuss DataWire #0, Channel #9 */ \
    cpuss_interrupts_dw0_10_IRQn_Handler,               /*!<  47 [Active] cpuss DataWire #0, Channel #10 */ \
    cpuss_interrupts_dw0_11_IRQn_Handler,               /*!<  48 [Active] cpuss DataWire #0, Channel #11 */ \
    cpuss_interrupts_dw0_12_IRQn_Handler,               /*!<  49 [Active] cpuss DataWire #0, Channel #12 */ \
    cpuss_interrupts_dw0_13_IRQn_Handler,               /*!<  50 [Active] cpuss DataWire #0, Channel #13 */ \
    cpuss_interrupts_dw0_14_IRQn_Handler,               /*!<  51 [Active] cpuss DataWire #0, Channel #14 */ \
    cpuss_interrupts_dw0_15_IRQn_Handler,               /*!<  52 [Active] cpuss DataWire #0, Channel #15 */ \
    cpuss_interrupts_dw1_0_IRQn_Handler,                /*!<  53 [Active] cpuss DataWire #1, Channel #0 */ \
    cpuss_interrupts_dw1_1_IRQn_Handler,                /*!<  54 [Active] cpuss DataWire #1, Channel #1 */ \
    cpuss_interrupts_dw1_2_IRQn_Handler,                /*!<  55 [Active] cpuss DataWire #1, Channel #2 */ \
    cpuss_interrupts_dw1_3_IRQn_Handler,                /*!<  56 [Active] cpuss DataWire #1, Channel #3 */ \
    cpuss_interrupts_dw1_4_IRQn_Handler,                /*!<  57 [Active] cpuss DataWire #1, Channel #4 */ \
    cpuss_interrupts_dw1_5_IRQn_Handler,                /*!<  58 [Active] cpuss DataWire #1, Channel #5 */ \
    cpuss_interrupts_dw1_6_IRQn_Handler,                /*!<  59 [Active] cpuss DataWire #1, Channel #6 */ \
    cpuss_interrupts_dw1_7_IRQn_Handler,                /*!<  60 [Active] cpuss DataWire #1, Channel #7 */ \
    cpuss_interrupts_dw1_8_IRQn_Handler,                /*!<  61 [Active] cpuss DataWire #1, Channel #8 */ \
    cpuss_interrupts_dw1_9_IRQn_Handler,                /*!<  62 [Active] cpuss DataWire #1, Channel #9 */ \
    cpuss_interrupts_dw1_10_IRQn_Handler,               /*!<  63 [Active] cpuss DataWire #1, Channel #10 */ \
    cpuss_interrupts_dw1_11_IRQn_Handler,               /*!<  64 [Active] cpuss DataWire #1, Channel #11 */ \
    cpuss_interrupts_dw1_12_IRQn_Handler,               /*!<  65 [Active] cpuss DataWire #1, Channel #12 */ \
    cpuss_interrupts_dw1_13_IRQn_Handler,               /*!<  66 [Active] cpuss DataWire #1, Channel #13 */ \
    cpuss_interrupts_dw1_14_IRQn_Handler,               /*!<  67 [Active] cpuss DataWire #1, Channel #14 */ \
    cpuss_interrupts_dw1_15_IRQn_Handler,               /*!<  68 [Active] cpuss DataWire #1, Channel #15 */ \
    cpuss_interrupt_ppu_sramc0_IRQn_Handler,            /*!<  69 [Active] PPU SRAM0 */ \
    cpuss_interrupt_cm33_0_fp_IRQn_Handler,             /*!<  70 [Active] CM33 0 Floating Point Interrupt */ \
    cpuss_interrupts_cm33_0_cti_0_IRQn_Handler,         /*!<  71 [Active] CM33-0 CTI interrupt outputs */ \
    cpuss_interrupts_cm33_0_cti_1_IRQn_Handler,         /*!<  72 [Active] CM33-1 CTI interrupt outputs */ \
    cpuss_interrupts_fault_0_IRQn_Handler,              /*!<  73 [Active] cpuss Faults interrupt */ \
    cpuss_interrupt_ppu_cpuss_IRQn_Handler,             /*!<  74 [Active] cpuss PPU Interrupt */ \
    cpuss_interrupt_msc_IRQn_Handler,                   /*!<  75 [Active] cpuss Master Security Controller Interrupt */ \
    tcpwm_0_interrupts_0_IRQn_Handler,                  /*!<  76 [Active] TCPWM #0, Counter #0 */ \
    tcpwm_0_interrupts_1_IRQn_Handler,                  /*!<  77 [Active] TCPWM #0, Counter #1 */ \
    tcpwm_0_interrupts_2_IRQn_Handler,                  /*!<  78 [Active] TCPWM #0, Counter #2 */ \
    tcpwm_0_interrupts_3_IRQn_Handler,                  /*!<  79 [Active] TCPWM #0, Counter #3 */ \
    tcpwm_0_interrupts_256_IRQn_Handler,                /*!<  80 [Active] TCPWM #0, Counter #256 */ \
    tcpwm_0_interrupts_257_IRQn_Handler,                /*!<  81 [Active] TCPWM #0, Counter #257 */ \
    tcpwm_0_interrupts_258_IRQn_Handler,                /*!<  82 [Active] TCPWM #0, Counter #258 */ \
    tcpwm_0_interrupts_259_IRQn_Handler,                /*!<  83 [Active] TCPWM #0, Counter #259 */ \
    tcpwm_0_interrupts_260_IRQn_Handler,                /*!<  84 [Active] TCPWM #0, Counter #260 */ \
    tcpwm_0_interrupts_261_IRQn_Handler,                /*!<  85 [Active] TCPWM #0, Counter #261 */ \
    tcpwm_0_interrupts_262_IRQn_Handler,                /*!<  86 [Active] TCPWM #0, Counter #262 */ \
    tcpwm_0_interrupts_263_IRQn_Handler,                /*!<  87 [Active] TCPWM #0, Counter #263 */ \
    tcpwm_0_interrupts_512_IRQn_Handler,                /*!<  88 [Active] TCPWM #0, Counter #512 */ \
    tcpwm_0_interrupts_513_IRQn_Handler,                /*!<  89 [Active] TCPWM #0, Counter #513 */ \
    tcpwm_0_interrupts_514_IRQn_Handler,                /*!<  90 [Active] TCPWM #0, Counter #514 */ \
    tcpwm_0_interrupts_515_IRQn_Handler,                /*!<  91 [Active] TCPWM #0, Counter #515 */ \
    tcpwm_0_interrupts_516_IRQn_Handler,                /*!<  92 [Active] TCPWM #0, Counter #516 */ \
    tcpwm_0_interrupts_517_IRQn_Handler,                /*!<  93 [Active] TCPWM #0, Counter #517 */ \
    tcpwm_0_interrupts_518_IRQn_Handler,                /*!<  94 [Active] TCPWM #0, Counter #518 */ \
    tcpwm_0_interrupts_519_IRQn_Handler,                /*!<  95 [Active] TCPWM #0, Counter #519 */ \
    srss_interrupt_main_ppu_IRQn_Handler,               /*!<  96 [Active] SRSS Main PPU Interrupt */ \
    cryptolite_interrupt_error_IRQn_Handler,            /*!<  97 [Active] Crypto Interrupt */ \
    cryptolite_interrupt_trng_IRQn_Handler,             /*!<  98 [Active] TRNG interrupt */ \
    canfd_0_interrupts0_0_IRQn_Handler,                 /*!<  99 [Active] CAN #0, Interrupt #0, Channel #0 */ \
    canfd_0_interrupts1_0_IRQn_Handler,                 /*!< 100 [Active] CAN #0, Interrupt #1, Channel #0 */ \
    canfd_0_interrupts0_1_IRQn_Handler,                 /*!< 101 [Active] CAN #0, Interrupt #0, Channel #1 */ \
    canfd_0_interrupts1_1_IRQn_Handler,                 /*!< 102 [Active] CAN #0, Interrupt #1, Channel #1 */ \
    canfd_0_interrupt0_IRQn_Handler,                    /*!< 103 [Active] Can #0, Consolidated interrupt #0 */ \
    canfd_0_interrupt1_IRQn_Handler,                    /*!< 104 [Active] Can #0, Consolidated interrupt #1 */ \
    cordic_interrupt_mxcordic_IRQn_Handler,             /*!< 105 [Active] CORDIC */ \
    tcpwm_0_motif_interrupt_64_IRQn_Handler,            /*!< 106 [Active] TCPWM #0, MOTIF #1 SR0 */ \
    tcpwm_0_motif_interrupt_65_IRQn_Handler,            /*!< 107 [Active] TCPWM #0, MOTIF #1 SR1 */ \
    pass_interrupt_mcpass_IRQn_Handler,                 /*!< 108 [Active] Combined MCPASS interrupt, AC, Error conditions */ \
    pass_interrupt_sar_results_IRQn_Handler,            /*!< 109 [Active] Combined SAR Entry and FIR results interrupt */ \
    pass_interrupt_sar_entry_done_0_IRQn_Handler,       /*!< 110 [Active] Individual SAR Entry result interrupts [0] */ \
    pass_interrupt_sar_entry_done_1_IRQn_Handler,       /*!< 111 [Active] Individual SAR Entry result interrupts [1] */ \
    pass_interrupt_sar_entry_done_2_IRQn_Handler,       /*!< 112 [Active] Individual SAR Entry result interrupts [2] */ \
    pass_interrupt_sar_entry_done_3_IRQn_Handler,       /*!< 113 [Active] Individual SAR Entry result interrupts [3] */ \
    pass_interrupt_sar_entry_done_4_IRQn_Handler,       /*!< 114 [Active] Individual SAR Entry result interrupts [4] */ \
    pass_interrupt_sar_entry_done_5_IRQn_Handler,       /*!< 115 [Active] Individual SAR Entry result interrupts [5] */ \
    pass_interrupt_sar_entry_done_6_IRQn_Handler,       /*!< 116 [Active] Individual SAR Entry result interrupts [6] */ \
    pass_interrupt_sar_entry_done_7_IRQn_Handler,       /*!< 117 [Active] Individual SAR Entry result interrupts [7] */ \
    pass_interrupt_sar_fir_done_0_IRQn_Handler,         /*!< 118 [Active] Individual FIR result interrupts[0] */ \
    pass_interrupt_sar_fir_done_1_IRQn_Handler,         /*!< 119 [Active] Individual FIR result interrupts[1] */ \
    pass_interrupt_sar_ranges_IRQn_Handler,             /*!< 120 [Active] Combined SAR range interrupt */ \
    pass_interrupt_sar_range_0_IRQn_Handler,            /*!< 121 [Active] Individual SAR range interrupts[0] */ \
    pass_interrupt_sar_range_1_IRQn_Handler,            /*!< 122 [Active] Individual SAR range interrupts[1] */ \
    pass_interrupt_sar_range_2_IRQn_Handler,            /*!< 123 [Active] Individual SAR range interrupts[2] */ \
    pass_interrupt_sar_range_3_IRQn_Handler,            /*!< 124 [Active] Individual SAR range interrupts[3] */ \
    pass_interrupt_sar_range_4_IRQn_Handler,            /*!< 125 [Active] Individual SAR range interrupts[4] */ \
    pass_interrupt_sar_range_5_IRQn_Handler,            /*!< 126 [Active] Individual SAR range interrupts[5] */ \
    pass_interrupt_sar_range_6_IRQn_Handler,            /*!< 127 [Active] Individual SAR range interrupts[6] */ \
    pass_interrupt_sar_range_7_IRQn_Handler,            /*!< 128 [Active] Individual SAR range interrupts[7] */ \
    pass_interrupt_csg_dacs_IRQn_Handler,               /*!< 129 [Active] Combined CSG DAC interrupt */ \
    pass_interrupt_csg_dac_0_IRQn_Handler,              /*!< 130 [Active] Individual CSG DAC interrupts ( 5 in PSC3 )[0] */ \
    pass_interrupt_csg_dac_1_IRQn_Handler,              /*!< 131 [Active] Individual CSG DAC interrupts ( 5 in PSC3 )[1] */ \
    pass_interrupt_csg_dac_2_IRQn_Handler,              /*!< 132 [Active] Individual CSG DAC interrupts ( 5 in PSC3 )[2] */ \
    pass_interrupt_csg_dac_3_IRQn_Handler,              /*!< 133 [Active] Individual CSG DAC interrupts ( 5 in PSC3 )[3] */ \
    pass_interrupt_csg_dac_4_IRQn_Handler,              /*!< 134 [Active] Individual CSG DAC interrupts ( 5 in PSC3 )[4] */ \
    pass_interrupt_csg_dac_5_IRQn_Handler,              /*!< 135 [Active] Individual CSG DAC interrupts ( 5 in PSC3 )[5] */ \
    pass_interrupt_csg_dac_6_IRQn_Handler,              /*!< 136 [Active] Individual CSG DAC interrupts ( 5 in PSC3 )[6] */ \
    pass_interrupt_csg_dac_7_IRQn_Handler,              /*!< 137 [Active] Individual CSG DAC interrupts ( 5 in PSC3 )[7] */ \
    pass_interrupt_csg_cmps_IRQn_Handler,               /*!< 138 [Active] Combined CSG CMP interrupts */ \
    pass_interrupt_fifos_IRQn_Handler,                  /*!< 139 [Active] Combined FIFO interrupts */

#endif /* PSC3_CORE_INTERRUPTS_H */
