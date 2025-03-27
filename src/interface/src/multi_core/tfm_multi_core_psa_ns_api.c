/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "psa/client.h"
#include "psa/error.h"
#include "tfm_ns_mailbox.h"


#ifdef TFM_NS_MANAGE_NSID
#include "tfm_ns_ctx.h"

/*
 * Retrieve the client id provided by the NSPE
 */
#define NON_SECURE_CLIENT_ID            get_nsid_from_active_ns_ctx()
#else
/*
 * Force all the non-secure client to share the same ID.
 */
#define NON_SECURE_CLIENT_ID            (-1)
#endif

/*
 * TODO
 * Require a formal definition of errors related to mailbox in PSA client call.
 */
#define PSA_INTER_CORE_COMM_ERR         (INT32_MIN + 0xFF)

/**** API functions ****/

uint32_t psa_framework_version(void)
{
    struct psa_client_params_t params = {0};
    uint32_t version = PSA_VERSION_NONE;
    int32_t ret;
    struct mailbox_reply_t reply = {.return_val = MAILBOX_GENERIC_ERROR};

    ret = tfm_ns_mailbox_client_call(MAILBOX_PSA_FRAMEWORK_VERSION,
                                     &params, NON_SECURE_CLIENT_ID,
                                     &reply);
    version = (uint32_t)reply.return_val;
    if (ret != MAILBOX_SUCCESS) {
        version = PSA_VERSION_NONE;
    }

    return version;
}

uint32_t psa_version(uint32_t sid)
{
    struct psa_client_params_t params;
    uint32_t version;
    int32_t ret;
    struct mailbox_reply_t reply = {.return_val = MAILBOX_GENERIC_ERROR};

    params.psa_params.psa_version_params.sid = sid;

    ret = tfm_ns_mailbox_client_call(MAILBOX_PSA_VERSION, &params,
                                     NON_SECURE_CLIENT_ID,
                                     &reply);
    version = (uint32_t)reply.return_val;
    if (ret != MAILBOX_SUCCESS) {
        version = PSA_VERSION_NONE;
    }

    return version;
}

psa_handle_t psa_connect(uint32_t sid, uint32_t version)
{
    struct psa_client_params_t params;
    psa_handle_t psa_handle;
    int32_t ret;
    struct mailbox_reply_t reply = {.return_val = MAILBOX_GENERIC_ERROR};

    params.psa_params.psa_connect_params.sid = sid;
    params.psa_params.psa_connect_params.version = version;

    ret = tfm_ns_mailbox_client_call(MAILBOX_PSA_CONNECT, &params,
                                     NON_SECURE_CLIENT_ID,
                                     &reply);
    psa_handle = (psa_handle_t)reply.return_val;
    if (ret != MAILBOX_SUCCESS) {
        psa_handle = PSA_NULL_HANDLE;
    }

    return psa_handle;
}

psa_status_t psa_call(psa_handle_t handle, int32_t type,
                      const psa_invec *in_vec, size_t in_len,
                      psa_outvec *out_vec, size_t out_len)
{
    struct psa_client_params_t params;
    int32_t ret;
    struct mailbox_reply_t reply = {.return_val = MAILBOX_GENERIC_ERROR, .out_vec_len = {0}};
    psa_status_t status;

    if ((in_len > IOVEC_LEN(params.psa_params.psa_call_params.in_vec)) ||
        (out_len > IOVEC_LEN(params.psa_params.psa_call_params.out_vec)) ||
        (in_len + out_len > PSA_MAX_IOVEC)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    params.psa_params.psa_call_params.handle = handle;
    params.psa_params.psa_call_params.type = type;
    memcpy(params.psa_params.psa_call_params.in_vec, in_vec, sizeof(in_vec[0]) * in_len);
    params.psa_params.psa_call_params.in_len = in_len;
    memcpy(params.psa_params.psa_call_params.out_vec, out_vec, sizeof(out_vec[0]) * out_len);
    params.psa_params.psa_call_params.out_len = out_len;

    ret = tfm_ns_mailbox_client_call(MAILBOX_PSA_CALL, &params,
                                     NON_SECURE_CLIENT_ID,
                                     &reply);
    status = (psa_status_t)reply.return_val;
    if (ret != MAILBOX_SUCCESS) {
        status = PSA_INTER_CORE_COMM_ERR;
    }

    for (size_t i = 0; i < out_len; i++) {
        out_vec[i].len = reply.out_vec_len[i];
    }

    return status;
}

void psa_close(psa_handle_t handle)
{
    struct psa_client_params_t params;
    struct mailbox_reply_t reply = {.return_val = MAILBOX_GENERIC_ERROR};

    params.psa_params.psa_close_params.handle = handle;

    (void)tfm_ns_mailbox_client_call(MAILBOX_PSA_CLOSE, &params,
                                     NON_SECURE_CLIENT_ID, &reply);
}
