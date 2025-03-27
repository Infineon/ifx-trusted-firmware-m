/*
 * Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef PROTECTION_REGIONS_CFG_H
#define PROTECTION_REGIONS_CFG_H

#include "cy_ms_ctl.h"
#include "cy_sysfault.h"
#include "ifx_spm_init.h"
#include "partition_psc3.h"
#include "project_memory_layout.h"
#include "protection_types.h"

/* In SAU config all peripherals with NS IDAU alias are marked as Non-Secure, this is needed to
 * allow some peripheral to be Non-Secure when S/NS attr for each peripheral is set by PPC */
#define IFX_PROTECTION_SAU_PERIPHERAL_REGION_START    ((uint32_t)MMIO_NS_START)
#define IFX_PROTECTION_SAU_PERIPHERAL_REGION_SIZE     ((uint32_t)MMIO_SIZE)

typedef struct {
    en_ms_ctl_master_sc_acg_t      bus_master;
    cy_en_ms_ctl_cfg_gate_resp_t   gate_resp;
    cy_en_ms_ctl_sec_resp_t        sec_resp;
} ifx_msc_agc_resp_config_t;

/* Structure used to configure SAU region */
typedef struct {
    uint32_t    base_addr; /* Base address of SAU region. Must be aligned to 32 bytes */
    uint32_t    size;      /* Size of SAU region. Must be aligned to 32 bytes */
    bool        nsc;       /* Whether this region is Non-Secure Callable */
} ifx_sau_config_t;

extern PPC_Type* const                      ifx_ppcx_region_ptrs[];
extern const size_t                         ifx_ppcx_region_ptrs_count;
extern const ifx_ppcx_config_t              ifx_ppcx_static_config[];
extern const size_t                         ifx_ppcx_static_config_count;
extern const ifx_msc_agc_resp_config_t      ifx_msc_agc_resp_config[];
extern const size_t                         ifx_msc_agc_resp_config_count;
extern const ifx_sau_config_t               SAU_config[];
extern const size_t                         CY_SAU_REGION_CNT;
extern const IRQn_Type                      ifx_secure_interrupts_config[];
extern const size_t                         ifx_secure_interrupts_config_count;
extern const cy_en_SysFault_source_t        ifx_fault_sources_fault_struct0[];
extern const size_t                         ifx_fault_sources_fault_struct0_count;

#endif /* PROTECTION_REGIONS_CFG_H */
