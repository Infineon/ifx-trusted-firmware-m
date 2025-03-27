/*
 * Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef IFX_S_PERIPHERALS_DEF_H
#define IFX_S_PERIPHERALS_DEF_H

#include "cy_device.h"
#include "mxs22.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TFM_FPU_S_TEST_IRQ          ioss_interrupts_gpio_1_IRQn

/* SFLASH is Non-Secure for PSC3 */
#define IFX_SFLASH                  IFX_NS_ADDRESS_ALIAS_T(SFLASH_Type*, SFLASH)

/* LCS is stored in EFUSE. Whole EFUSE block is divided into several PPC
 * regions which have different protection settings. Cy_SysLib_GetDeviceLCS
 * reads EFUSE->BOOTROW which is part of PROT_EFUSE_CTL3 PPC region
 * (which is configured as Non-Secure), thus NS alias is used for EFUSE here. */
#define IFX_LCS_BASE                IFX_NS_ADDRESS_ALIAS_T(EFUSE_Type*, EFUSE)

#ifdef __cplusplus
}
#endif

#endif /* IFX_S_PERIPHERALS_DEF_H */
