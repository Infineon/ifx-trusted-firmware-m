/*
 * Copyright (c) 2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis.h"
#include "provisioning.h"

ifx_dap_state_t ifx_get_dap_state(void)
{
    if (_FLD2VAL(CPUSS_AP_CTL_CM33_0_DISABLE, CPUSS->AP_CTL)) {
        /* CM33 AP is permanently disabled */
        return IFX_DAP_DISABLED;
    } else {
        /* CM33 AP is allowed/enabled - thus check Secure debugging capabilities */
        if (_FLD2VAL(CPUSS_AP_CTL_CM33_0_SECURE_DISABLE, CPUSS->AP_CTL)) {
            /* CM33 Secure Debugging is permanently disabled */
            return IFX_DAP_ENABLED_NS_ONLY;
        } else {
            /* CM33 Secure Debugging is allowed/enabled */
            return IFX_DAP_ENABLED_S_NS;
        }
    }
}
