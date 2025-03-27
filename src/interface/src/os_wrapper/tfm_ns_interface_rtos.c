/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2023 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* This file provides implementation of TF-M NS os wrapper functions for the
 * RTOS use case. This implementation provides multithread safety, so it
 * can be used in RTOS environment.
 */

#include <stdint.h>

#include "os_wrapper/mutex.h"
#include "os_wrapper/kernel.h"

#include "tfm_ns_interface.h"

#ifdef TFM_NSID_IN_SHARED_DATA
#include "tfm_ns_ctx_shm.h"

extern struct tfm_ns_ctx_mgr_t *ns_ctx_mgr;
extern psa_status_t tfm_nsid_shm_init(const struct tfm_ns_ctx_mgr_t *tfm_ns_ctx_mgr);
#endif

/**
 * \brief the ns_lock ID
 */
static void *ns_lock_handle = NULL;

int32_t tfm_ns_interface_dispatch(veneer_fn fn,
                                  uint32_t arg0, uint32_t arg1,
                                  uint32_t arg2, uint32_t arg3)
{
    int32_t result;

    if(!os_wrapper_is_kernel_running()) {
        /* Mutex may not be acquirable if kernel is not running. In such case
         * mutex lock is not called. This is safe because if kernel is not
         * running then no multithread environment exists, thus no multithread
         * safety is needed.
         */
        result = fn(arg0, arg1, arg2, arg3);
    } else {
        /* TFM request protected by NS lock */
        while (os_wrapper_mutex_acquire(ns_lock_handle, OS_WRAPPER_WAIT_FOREVER)
                != OS_WRAPPER_SUCCESS) {
        }

        result = fn(arg0, arg1, arg2, arg3);

        while (os_wrapper_mutex_release(ns_lock_handle) != OS_WRAPPER_SUCCESS) {
        }
    }

    return result;
}

uint32_t tfm_ns_interface_init(void)
{
    void *handle;

#ifdef TFM_NSID_IN_SHARED_DATA
    /* Tell the SPE where the NSID shared data is */
    tfm_nsid_shm_init(ns_ctx_mgr);
#endif

    handle = os_wrapper_mutex_create();
    if (handle == NULL) {
        return OS_WRAPPER_ERROR;
    }

    ns_lock_handle = handle;
    return OS_WRAPPER_SUCCESS;
}
