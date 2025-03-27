/*
 * Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef TFM_PERIPHERALS_DEF_H
#define TFM_PERIPHERALS_DEF_H

#include <cycfg.h>

/* ifx_peripherals_def.h should be provided by BSP to define peripheral
 * configuration for SPE and NSPE */
#include "ifx_peripherals_def.h"

#if DOMAIN_S
/* ifx_s_peripherals_def.h should be provided by BSP to define peripheral
 * configuration for SPE */
#include "ifx_s_peripherals_def.h"

/* Defines partition MMIO asset which is defined in TF-M/tf-m-tests/psa-arch-tests
 * partition manifest, but is not used by platform.
 */
#define IFX_UNUSED_MMIO                     (0xFFFFFFFFu)

#if IFX_UART_ENABLED
/* UART block to use */
#define IFX_TFM_SPM_UART                    (IFX_TFM_SPM_UART_HW)
#ifndef IFX_TFM_SPM_UART_FLUSH
#define IFX_TFM_SPM_UART_FLUSH               0
#endif /* IFX_TFM_SPM_UART_FLUSH */
#endif /* IFX_UART_ENABLED */
#endif /* DOMAIN_S */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Quantized default IRQ priority, the value is:
 * (Number of configurable priority) / 4: (1UL << __NVIC_PRIO_BITS) / 4
 */
#define DEFAULT_IRQ_PRIORITY    (1UL << (__NVIC_PRIO_BITS - 2))

/* Section that defines peripherals used by tests */
#if DOMAIN_S
/* TF-M Regression Tests use UART provided by SPM */
#define TFM_PERIPHERAL_STD_UART             (IFX_UNUSED_MMIO)
#define TFM_PERIPHERAL_TIMER0               (IFX_UNUSED_MMIO)
/* FF Tests use UART provided by SPM */
#define FF_TEST_UART_REGION                 (IFX_UNUSED_MMIO)

#define IFX_IRQ_TEST_TIMER_S_SYSTICK        1
#if IFX_IRQ_TEST_TIMER_S_SYSTICK
#define TFM_TIMER0_IRQ                      SysTick_IRQn
#else
#error only SysTick is supported for TFM tests
#endif

/* TFM regression tests and PSA arch tests are mutually exclusive thus
 * TFM_TIMER0_IRQ can be reused for FF_TEST_UART_IRQ to save up the resources */
#define FF_TEST_UART_IRQ                    TFM_TIMER0_IRQ

/* Watchdog is not supported for psa-arch-tests */
#define FF_TEST_WATCHDOG_REGION             IFX_UNUSED_MMIO

/* Following FF_* memory regions are assigned to FF tests partitions as NAMED
 * mmio_regions, but in PSC3 NAMED mmio_regions represent peripherals.
 * Thus following FF_* defines are set to IFX_UNUSED_MMIO and instead respective
 * memory areas are added to partitions as NUMBERED mmio_regions in
 * partition_info.template */
#define FF_TEST_DRIVER_PARTITION_MMIO       IFX_UNUSED_MMIO
#define FF_TEST_SERVER_PARTITION_MMIO       IFX_UNUSED_MMIO
#define FF_TEST_NVMEM_REGION                IFX_UNUSED_MMIO
#endif /* DOMAIN_S */

/* Free peripheral that does not require configuration is used */
#define IFX_TEST_PERIPHERAL_1               PROT_GPIO_PRT0_PRT
#define IFX_TEST_PERIPHERAL_1_BASE          GPIO_PRT0
#define IFX_TEST_PERIPHERAL_1_SIZE          0x40U

/* Free peripheral that does not require configuration is used */
#define IFX_TEST_PERIPHERAL_2               PROT_GPIO_PRT1_PRT
#define IFX_TEST_PERIPHERAL_2_BASE          GPIO_PRT1
#define IFX_TEST_PERIPHERAL_2_SIZE          0x40U

#ifdef __cplusplus
}
#endif

#endif /* TFM_PERIPHERALS_DEF_H */
