/*
 * Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis.h"
#include "cy_pdl.h"
#ifdef IFX_PDL_SECURE_SERVICES
#include "cy_secure_services.h"
#endif
#include "ifx_platform_api.h"
#include "ifx_platform_private.h"
#include "platform_svc_api.h"
#include "tfm_platform_system.h"


void tfm_platform_hal_system_reset(void)
{
    /* Platform partition may be unprivileged, which means that it does not
     * have access to reset control register, thus SVC call is used. */
    ifx_call_platform_system_reset();
}

enum tfm_platform_err_t tfm_platform_hal_ioctl(tfm_platform_ioctl_req_t request,
                                               psa_invec  *in_vec,
                                               psa_outvec *out_vec)
{
    enum tfm_platform_err_t status = TFM_PLATFORM_ERR_NOT_SUPPORTED;
    switch (request) {
        case IFX_PLATFORM_IOCTL_LOG_MSG: {
            status = (enum tfm_platform_err_t)ifx_call_platform_uart_log(
                    (const uint8_t *)in_vec[0].base, in_vec[0].len);
            break;
        }

#ifdef IFX_PDL_SECURE_SERVICES
        case IFX_PLATFORM_IOCTL_PDL_RPC: {
            /* Note that PDL packages parameters by grouping the simple ones (numbers, pointers to PPUs
             * and pointers to PPCs) together into one invec. Actual blocks of memory should always be
             * passed in their own invec/outvec.
             * Pointers to PPUs and PPCs are validated within Cy_Secure_services().
             * If additional functions are added, care must be taken to ensure that parameters are
             * passed such that they can be validated correctly by TF-M.
             */

            cy_rpc_msg_t rpcMsg = {
                .inVecCount = 0,
                .outVecCount = 0,
            };

            /* TF-M's ioctl currently only supports up to 1 invec and up to 1 outvec */
            if (in_vec != NULL) {
                rpcMsg.inVec[0].base = in_vec->base;
                rpcMsg.inVec[0].len = in_vec->len;
                rpcMsg.inVecCount = 1;
            }
            if (out_vec != NULL) {
                rpcMsg.outVec[0].base = out_vec->base;
                rpcMsg.outVec[0].len = out_vec->len;
                rpcMsg.outVecCount = 1;
            }
            Cy_SecureServices(&rpcMsg);
            return TFM_PLATFORM_ERR_SUCCESS;
        }
#endif /* IFX_PDL_SECURE_SERVICES */

        default: {
#if IFX_CUSTOM_PLATFORM_HAL_IOCTL == 1
            if ((request >= IFX_PLATFORM_IOCTL_APP_MIN) &&
                (request <= IFX_PLATFORM_IOCTL_APP_MIN)) {
                /* Handle application platform-specific IOCTL */
                status = ifx_platform_hal_ioctl(request, in_vec, out_vec);
                break;
            }
#endif /* IFX_CUSTOM_PLATFORM_HAL_IOCTL == 1 */

            status = TFM_PLATFORM_ERR_NOT_SUPPORTED;
            break;
        }
    }

    return status; 
}
