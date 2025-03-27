/*
 * Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file ifx_platform_spe_types.h
 * \brief This file contains platform specific types and data declaration used to build secure image.
 *
 * This file is part of Infineon platform configuration files. It's expected
 * that this file provides platform dependent types and data declaration used by Infineon common code.
 */

#ifndef IFX_PLATFORM_SPE_TYPES_H
#define IFX_PLATFORM_SPE_TYPES_H

#include <stdint.h>
#include "cy_mpc.h"
#include "partition_psc3.h"

#define IFX_GET_PC(mask, pc)                 (((mask) >> (pc)) & 1UL)

/* Peripheral region starts from Non-Secure base (MMIO_NS_START)
 * and ends at the end of peripheral Secure alias (MMIO_S_START + MMIO_SIZE - 1)
 * This is needed to be able to access peripherals by both Secure and Non-Secure
 * alias. */
#define IFX_PROTECTION_MPU_PERIPHERAL_REGION_START    ((uint32_t)MMIO_NS_START)
#define IFX_PROTECTION_MPU_PERIPHERAL_REGION_LIMIT    ((uint32_t)(MMIO_S_START + MMIO_SIZE - 1))

 /* \brief Mask of bit values for each PC
  * Current PC number is 8 but in future it may be increased
  */
typedef uint32_t ifx_pc_mask_t;

#endif /* IFX_PLATFORM_SPE_TYPES_H */
