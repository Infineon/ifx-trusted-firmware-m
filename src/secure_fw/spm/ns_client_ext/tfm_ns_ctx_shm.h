/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 * Copyright (c) 2023-2024 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdint.h>

/* Non-secure context structure */
struct tfm_ns_ctx_t {
    int32_t nsid;       /* Non-secure Client ID, must be < 0 */
    uint8_t gid;        /* Group ID. Threads in same group share one context */
    uint8_t tid;        /* Thread ID. Used to identify threads in same group */
    uint8_t ref_cnt;    /* The number of threads sharing this context */
};

struct tfm_ns_ctx_mgr_t {
    /* Number of supported contexts. Also the size of the ns_ctx_data array */
    uint8_t ns_ctx_limit;

    /* Current active NS context index. Default is invalid index */
    uint8_t active_ns_ctx_index;

    /*
     * NS context. Initialized to 0.
     * All contexts are not used as the reference counter is 0 in initialization.
     */
    struct tfm_ns_ctx_t *ns_ctx_data;
};
