/*
 * Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file ifx_platform_spe_config.h
 * \brief This file contains platform specific configuration used to build secure image.
 *
 * This file is part of Infineon platform configuration files. It's expected
 * that this file provides platform dependent configuration used by Infineon common code.
 */

#ifndef IFX_PLATFORM_SPE_CONFIG_H
#define IFX_PLATFORM_SPE_CONFIG_H

/* Those macro are used by inline assembler and must be in a 0x00UL hex format */

#define IFX_PC_TFM_ID                   0x02UL    /* TFM SPM/PRoT/ARoT/CM33 NSPE */
#define IFX_PC_TFM_SPM_ID               0x02UL    /* TFM SPM */
#define IFX_PC_TFM_PROT_ID              0x02UL    /* TFM PSA RoT */
#define IFX_PC_TFM_AROT_ID              0x02UL    /* TFM Application RoT */
#define IFX_PC_TZ_NSPE_ID               0x02UL    /* NSPE Application */
#define IFX_PC_DEBUGGER_ID              0x07UL    /* Debugger */

#define IFX_PC_NONE                     (0x00)   /* No PC, empty PC mask */
#define IFX_PC_TFM_SPM                  (1UL << IFX_PC_TFM_SPM_ID)
#define IFX_PC_TFM_PROT                 (1UL << IFX_PC_TFM_PROT_ID)
#define IFX_PC_TFM_AROT                 (1UL << IFX_PC_TFM_AROT_ID)
#define IFX_PC_TZ_NSPE                  (1UL << IFX_PC_TZ_NSPE_ID)
#define IFX_PC_DEBUGGER                 (1UL << IFX_PC_DEBUGGER_ID)

/* Used to define set of Protection Context that always has access to peripheral
 * and memory resources of partitions. */
#define IFX_PC_DEFAULT                  IFX_PC_TFM_SPM

/* \brief Platform does not configure MPC to provide memory protection*/
#define IFX_MPC_CONFIGURED_BY_TFM                           0

/* \brief Platform has memory protection controller (MPC) */
#define IFX_PLATFORM_MPC_PRESENT                            1

/* \brief Does not use HW MPC driver */
#define IFX_MPC_DRIVER_HW_MPC                               0

/* \brief Use SW Policy MPC driver */
#define IFX_MPC_DRIVER_SW_POLICY                            1

/* \brief Platform has peripheral protection controller (PPC) */
#define IFX_PLATFORM_PPC_PRESENT                            1

/* \brief Response configuration for ACG and MSC for the referenced bus master */
#define IFX_MSC_ACG_RESP_CONFIG                             1

/* \brief Doesn't use response configuration for ACG and MSC for the referenced bus master in CAT1D devices */
#define IFX_MSC_ACG_RESP_CONFIG_V1                          0

/* Bus master ID of core running TF-M */
#define IFX_MSC_TFM_CORE_BUS_MASTER_ID                      CPUSS_MS_ID_CM33_0

/* Defines whether PSA RoT partitions run in privileged mode */
#define IFX_PSA_ROT_PRIVILEGED                              1

/* Defines whether Application RoT partitions run in privileged mode */
#define IFX_APP_ROT_PRIVILEGED                              0

/* Defines whether NS Agent TZ partition runs in privileged mode */
#define IFX_NS_AGENT_TZ_PRIVILEGED                          1

#endif /* IFX_PLATFORM_SPE_CONFIG_H */
