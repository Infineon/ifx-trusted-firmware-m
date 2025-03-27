/*
 * Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stddef.h>

#include "cy_secure_services.h"
#include "ifx_platform_private.h"
#include "tfm_platform_api.h"
#include "ifx_utils.h"
#include "static_checks.h"

static void return_error(cy_rpc_outvec_t *outVec, uint32_t outVecCount)
{
    if ((outVecCount > 0) && (outVec->len == sizeof(cy_rpc_service_args_t))) {
        cy_rpc_service_args_t *rpcOutputArgs = outVec->base;

        /* Don't overwrite any existing return code */
        if (rpcOutputArgs->argc == 0) {
            rpcOutputArgs->argc = 1;
            rpcOutputArgs->argv[0] = CY_RPC_SECURE_SERVICE_ERROR;
        }
    }
}

void Cy_SecureServices_RPC(const cy_rpc_invec_t *inVec, uint32_t inVecCount, cy_rpc_outvec_t *outVec, uint32_t outVecCount)
{
    /* This code relies on these two types matching */
    TFM_COVERITY_DEVIATE_BLOCK(MISRA_C_2023_Rule_2_2, "This code is added to perform compile time check")
    IFX_ASSERT(sizeof(psa_invec) == sizeof(cy_rpc_invec_t));
    IFX_ASSERT(sizeof(psa_outvec) == sizeof(cy_rpc_outvec_t));
    IFX_ASSERT(offsetof(psa_invec, len) == offsetof(cy_rpc_invec_t, len));
    IFX_ASSERT(offsetof(psa_outvec, len) == offsetof(cy_rpc_outvec_t, len));
    TFM_COVERITY_BLOCK_END(MISRA_C_2023_Rule_2_2)

    /* Validate supported numbers of inVec/outVecs */
    if ((inVecCount < 1) ||
        (inVecCount > PSA_MAX_IOVEC) || (outVecCount > PSA_MAX_IOVEC) ||
        (inVecCount + outVecCount > PSA_MAX_IOVEC)) {
        return_error(outVec, outVecCount);
        return;
    }

    /* TF-M's ioctl only supports up to 1 invec and up to 1 outvec */
    if ((inVecCount > 1) || (outVecCount > 1)) {
        return_error(outVec, outVecCount);
        return;
    }

    /* We can't do anything with the return code */
    TFM_COVERITY_DEVIATE_BLOCK(MISRA_C_2023_Rule_11_3, "Intentional pointer type conversion");
    if (tfm_platform_ioctl(IFX_PLATFORM_IOCTL_PDL_RPC,
                           (psa_invec *)inVec,
                           (psa_outvec *)outVec) != TFM_PLATFORM_ERR_SUCCESS) {
        return_error(outVec, outVecCount);
    }
    TFM_COVERITY_BLOCK_END(MISRA_C_2023_Rule_11_3)
}
