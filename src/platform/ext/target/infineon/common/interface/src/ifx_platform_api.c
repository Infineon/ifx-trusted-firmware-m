/*
 * Copyright (c) 2023-2024 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <string.h>
#include "config_tfm.h"
#include "ifx_platform_api.h"
#include "ifx_platform_private.h"
#include "ifx_utils.h"

uint32_t ifx_platform_log_msg(const uint8_t *msg, uint32_t msg_size)
{
    uint32_t max_chunk_len = (uint32_t)PLATFORM_SERVICE_INPUT_BUFFER_SIZE;
#if defined(IFX_STDIO_PREFIX)
    char buffer[(uint32_t)PLATFORM_SERVICE_INPUT_BUFFER_SIZE];
    uint32_t buffer_msg_offset = sizeof(IFX_STDIO_PREFIX) - 1U;
    if (PLATFORM_SERVICE_INPUT_BUFFER_SIZE <= buffer_msg_offset) {
        /* Don't use prefix if Platform service ioctl buffer is too small */
        buffer_msg_offset = 0;
        max_chunk_len = PLATFORM_SERVICE_INPUT_BUFFER_SIZE;
    } else {
        /* Each message will be prefixed */
        (void)strncpy(buffer, IFX_STDIO_PREFIX, buffer_msg_offset);
        max_chunk_len = (uint32_t)PLATFORM_SERVICE_INPUT_BUFFER_SIZE - buffer_msg_offset;
    }
#endif

    uint32_t out_count = 0U;
    while (msg_size != 0U) {
        uint32_t chunk_len = msg_size < max_chunk_len ?
                             msg_size : max_chunk_len;
#if defined(IFX_STDIO_PREFIX)
        psa_invec in_vec = { .base = buffer, .len = buffer_msg_offset + chunk_len };
        (void)strncpy(buffer + buffer_msg_offset, (const char *)msg, chunk_len);
#else
        psa_invec in_vec = { .base = msg, .len = chunk_len };
#endif
        enum tfm_platform_err_t status =
                tfm_platform_ioctl(IFX_PLATFORM_IOCTL_LOG_MSG, &in_vec, NULL);
        if (status != TFM_PLATFORM_ERR_SUCCESS) {
            return out_count;
        }

        msg += chunk_len;
        msg_size -= chunk_len;
        out_count += chunk_len;
    }

    return out_count;
}
