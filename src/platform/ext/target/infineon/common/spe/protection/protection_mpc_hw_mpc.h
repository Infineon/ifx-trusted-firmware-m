/*
 * Copyright (c) 2024 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file protection_mpc_hw_mpc.h
 * \brief This file contains types/API used by HW MPC driver, see cy_mpc.h in PDL.
 *
 * \note Don't include this file directly, include protection_types.h instead !!!
 * It's expected that this file is included by protection_types.h if platform is configured
 * to use HW MPC driver via IFX_MPC_DRIVER_HW_MPC option.
 */

#ifndef __PROTECTION_MPC_HW_MPC_H__
#define __PROTECTION_MPC_HW_MPC_H__

#include <stdint.h>

#include "ifx_platform_spe_types.h"
#include "ifx_spe_config.h"

typedef struct {
    ifx_pc_mask_t   pc_mask; /**< Mask that define what PCs must be modified.
                              *   1 - must be applied new value,
                              *   0 - must be left old value no matter what it was */
    ifx_pc_mask_t   ns_mask; /**< NS bit mask for each PC*/
} ifx_mpc_numbered_mmio_config_t;

typedef struct {
    uint32_t             address;
    uint32_t             size;
    ifx_pc_mask_t        pc_mask; /**< Mask that define what PCs must be modified.
                                   *   1 - must be applied new value,
                                   *   0 - must be left old value no matter what it was */
    ifx_pc_mask_t        ns_mask; /**< NS bit mask for each PC*/
    ifx_pc_mask_t        r_mask;  /**< R  bit mask for each PC*/
    ifx_pc_mask_t        w_mask;  /**< W  bit mask for each PC*/
} ifx_mpc_region_config_t;

/* SPM protection configuration for \ref ifx_mpc_numbered_mmio_config_t */
#define IFX_MPC_NAMED_MMIO_SPM_ROT_CFG \
    .mpc_cfg = { \
        .pc_mask        = IFX_PC_DEFAULT, \
        .ns_mask        = IFX_PC_NONE, \
    },

/* PSA RoT partition protection configuration for \ref ifx_mpc_numbered_mmio_config_t */
#define IFX_MPC_NAMED_MMIO_PSA_ROT_CFG \
    .mpc_cfg = { \
        .pc_mask        = IFX_PC_DEFAULT | IFX_PC_TFM_PROT, \
        .ns_mask        = IFX_PC_NONE, \
    },

#if TFM_ISOLATION_LEVEL == 2
/* App RoT partition protection configuration for \ref ifx_mpc_numbered_mmio_config_t */
#define IFX_MPC_NAMED_MMIO_APP_ROT_CFG \
    .mpc_cfg = { \
        /* In L2 PRoT should be able to access ARoT so IFX_PC_TFM_PROT is added */ \
        .pc_mask        = IFX_PC_DEFAULT | IFX_PC_TFM_PROT | IFX_PC_TFM_AROT, \
        .ns_mask        = IFX_PC_NONE, \
    },
#else
/* App RoT partition protection configuration for \ref ifx_mpc_numbered_mmio_config_t */
#define IFX_MPC_NAMED_MMIO_APP_ROT_CFG \
    .mpc_cfg = { \
        .pc_mask        = IFX_PC_DEFAULT | IFX_PC_TFM_AROT, \
        .ns_mask        = IFX_PC_NONE, \
    },
#endif /* TFM_ISOLATION_LEVEL == 2 */

#ifdef TFM_PARTITION_NS_AGENT_TZ
/* NS Agent TZ partition protection configuration for \ref ifx_mpc_numbered_mmio_config_t */
#define IFX_MPC_NAMED_MMIO_NS_AGENT_TZ_CFG \
    .mpc_cfg = { \
        .pc_mask        = IFX_PC_DEFAULT | IFX_PC_TZ_NSPE, \
        .ns_mask        = IFX_PC_NONE, \
    },
#endif /* TFM_PARTITION_NS_AGENT_TZ */

#if TFM_ISOLATION_LEVEL == 3
/* Suspended partition configuration for L3 isolation for \ref ifx_mpc_named_mmio_config_t */
#define IFX_MPC_NAMED_MMIO_L3_DISABLED_CFG \
    .mpc_cfg = { \
        .pc_mask        = IFX_PC_DEFAULT, \
        .ns_mask        = IFX_PC_NONE, \
    },
#endif

#endif /* __PROTECTION_MPC_HW_MPC_H__ */
