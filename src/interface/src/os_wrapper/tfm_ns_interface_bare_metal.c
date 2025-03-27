/*
 * Copyright (c) 2023 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* This file provides implementation of TF-M NS os wrapper functions for the
 * bare metal use case.
 *
 * \note  This implementation does not provide multithread safety, so it
 *        shouldn't be used in RTOS environment.
 */

#include "os_wrapper/common.h"
#include "tfm_ns_interface.h"

#ifdef TFM_NSID_IN_SHARED_DATA
#include "tfm_ns_ctx_shm.h"

extern struct tfm_ns_ctx_mgr_t *ns_ctx_mgr;
extern psa_status_t tfm_nsid_shm_init(const struct tfm_ns_ctx_mgr_t *tfm_ns_ctx_mgr);
#endif

int32_t tfm_ns_interface_dispatch(veneer_fn fn,
                                  uint32_t arg0, uint32_t arg1,
                                  uint32_t arg2, uint32_t arg3)
{
    return fn(arg0, arg1, arg2, arg3);
}

uint32_t tfm_ns_interface_init(void)
{
#ifdef TFM_NSID_IN_SHARED_DATA
    /* Tell the SPE where the NSID shared data is */
    tfm_nsid_shm_init(ns_ctx_mgr);
#endif

    /* No initialization is needed */
    return OS_WRAPPER_SUCCESS;
}
