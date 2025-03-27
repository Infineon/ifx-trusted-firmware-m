/*
 * Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file protection_ppc.h
 * \brief This file contains types/API used by PPC driver.
 *
 * \note Don't include this file directly, include protection_types.h instead !!!
 * It's expected that this file is included by protection_types.h if platform is configured
 * to use PPC driver.
 */

#ifndef PROTECTION_PPC_H
#define PROTECTION_PPC_H

#include <stdbool.h>

#include "ifx_spe_config.h"
#include "tfm_hal_defs.h"
#include "cy_ppc.h"
#include "load/partition_defs.h"
#include "load/asset_defs.h"
#include "ifx_ppc_fix.h"

/* IMPROVEMENT: move IFX_GET_PPC_GROUP_REGION(), IFX_GET_PPC_GROUP_ID() and Cy_Ppc_GetAttrib() to PDL (DRIVERS-10521) */

/* Tools for get region in PPC and which one PPC assigned from cy_en_prot_region_fix_t */
#define IFX_PPC_REGION_MASK 0x3FFFFFFU
#define IFX_GET_PPC_GROUP_REGION(prot_region) ((cy_en_prot_region_t)((uint32_t)(prot_region) & IFX_PPC_REGION_MASK))

#define IFX_PPC_GROUP_SHIFT 28u
/* Upper bit (31-28) contain index of PPCs */
#define IFX_GET_PPC_GROUP_ID(prot_region) ((uint32_t)(prot_region) >> IFX_PPC_GROUP_SHIFT)

typedef struct {
    cy_en_ppc_sec_attribute_t  sec_attr;      /**< Security attribute */
    bool                       allow_unpriv;  /**< Whether unprivileged access is permitted */
    ifx_pc_mask_t              pc_mask;       /**< PC mask */
} ifx_ppc_named_mmio_config_t;

typedef struct {
    cy_en_ppc_sec_attribute_t      sec_attr;      /**< Security attribute */
    bool                           allow_unpriv;  /**< Whether unprivileged access is permitted */
    ifx_pc_mask_t                  pc_mask;       /**< PC mask */
    const cy_en_prot_region_fix_t* regions;       /**< Array of PPC regions */
    size_t                         region_count;  /**< Number of items in /ref regions */
} ifx_ppcx_config_t;

/* SPM protection configuration for \ref ifx_ppc_named_mmio_config_t */
#define IFX_PPC_NAMED_MMIO_SPM_ROT_CFG \
    .ppc_cfg = { \
        .sec_attr       = CY_PPC_SECURE, \
        .allow_unpriv   = false, \
        .pc_mask        = IFX_PC_DEFAULT, \
    },

#if IFX_PSA_ROT_PRIVILEGED
/* Privileged PSA RoT partition protection configuration for \ref ifx_ppc_named_mmio_config_t */
#define IFX_PPC_NAMED_MMIO_PSA_ROT_CFG \
    .ppc_cfg = { \
        .sec_attr       = CY_PPC_SECURE, \
        .allow_unpriv   = false, \
        .pc_mask        = IFX_PC_DEFAULT | IFX_PC_TFM_PROT, \
    },
#else
/* Unprivileged PSA RoT partition protection configuration for \ref ifx_ppc_named_mmio_config_t */
#define IFX_PPC_NAMED_MMIO_PSA_ROT_CFG \
    .ppc_cfg = { \
        .sec_attr       = CY_PPC_SECURE, \
        .allow_unpriv   = true, \
        .pc_mask        = IFX_PC_DEFAULT | IFX_PC_TFM_PROT, \
    },
#endif /* IFX_PSA_ROT_PRIVILEGED */

#if IFX_APP_ROT_PRIVILEGED
/* Privileged App RoT partition protection configuration for \ref ifx_ppc_named_mmio_config_t */
#define IFX_PPC_NAMED_MMIO_APP_ROT_CFG \
    .ppc_cfg = { \
        .sec_attr       = CY_PPC_SECURE, \
        .allow_unpriv   = false, \
        .pc_mask        = IFX_PC_DEFAULT | IFX_PC_TFM_AROT, \
    },
#else
/* Unprivileged App RoT partition protection configuration for \ref ifx_ppc_named_mmio_config_t */
#define IFX_PPC_NAMED_MMIO_APP_ROT_CFG \
    .ppc_cfg = { \
        .sec_attr       = CY_PPC_SECURE, \
        .allow_unpriv   = true, \
        .pc_mask        = IFX_PC_DEFAULT | IFX_PC_TFM_AROT, \
    },
#endif /* IFX_APP_ROT_PRIVILEGED */

#ifdef TFM_PARTITION_NS_AGENT_TZ
/* Disable access to secure peripheral within NS Agent TZ by setting pc_mask
 * to IFX_PC_NONE and sec_priv_attr to CY_PPC_SEC_PRIV. */
#define IFX_PPC_NAMED_MMIO_NS_AGENT_TZ_CFG \
    .ppc_cfg = { \
        .sec_attr       = CY_PPC_SECURE, \
        .allow_unpriv   = false, \
        .pc_mask        = IFX_PC_NONE, \
    },
#endif /* TFM_PARTITION_NS_AGENT_TZ */

#if TFM_ISOLATION_LEVEL == 3
/* Suspended partition configuration for L3 isolation for \ref ifx_ppc_named_mmio_config_t */
#define IFX_PPC_NAMED_MMIO_L3_DISABLED_CFG \
    .ppc_cfg = { \
        .sec_attr       = CY_PPC_SECURE, \
        .allow_unpriv   = false, \
        .pc_mask        = IFX_PC_DEFAULT, \
    },
#endif /* TFM_ISOLATION_LEVEL == 3 */

#endif /* PROTECTION_PPC_H */
