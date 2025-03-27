/*
 * Copyright (c) 2022-2024 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdbool.h>
#include "ifx_regions.h"

int32_t ifx_is_region_inside_other(uintptr_t first_region_start,
                                   uintptr_t first_region_limit,
                                   uintptr_t second_region_start,
                                   uintptr_t second_region_limit)
{
    bool range_inside_other;

    /* Check parameters of the first region */
    if (first_region_start > first_region_limit) {
        return SPM_ERROR_GENERIC;
    }

    /* Check parameters of the second region */
    if (second_region_start > second_region_limit) {
        return SPM_ERROR_GENERIC;
    }

    /* Calculate the result */
    range_inside_other = (first_region_start >= second_region_start) &&
                         (first_region_limit <= second_region_limit);
    if (range_inside_other) {
        return SPM_SUCCESS;
    } else {
        return SPM_ERROR_GENERIC;
    }
}

int32_t ifx_is_region_overlap_other(uintptr_t first_region_start,
                                    uintptr_t first_region_limit,
                                    uintptr_t second_region_start,
                                    uintptr_t second_region_limit)
{
    /* Check parameters of the first region */
    if (first_region_start > first_region_limit) {
        return SPM_ERROR_GENERIC;
    }

    /* Check parameters of the second region */
    if (second_region_start > second_region_limit) {
        return SPM_ERROR_GENERIC;
    }

    /* Calculate the result */
    bool region_overlap_other = (first_region_start <= second_region_limit) &&
                                (first_region_limit >= second_region_start);

    if (region_overlap_other) {
        return SPM_SUCCESS;
    } else {
        return SPM_ERROR_GENERIC;
    }
}
