/*
 * Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef IFX_PPC_FIX_H
#define IFX_PPC_FIX_H

#include "cy_device_headers.h"

/* IMPROVEMENT: Remove workaround type when DRIVERS-10399 will be fixed */
/* PSC3 has only 1 PPC controller thus cy_en_prot_region_t can be used directly */
typedef cy_en_prot_region_t cy_en_prot_region_fix_t;

#endif /* IFX_PPC_FIX_H */
