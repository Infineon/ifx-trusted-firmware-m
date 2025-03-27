/*
 * Copyright (c) 2021-2024, Arm Limited. All rights reserved.
 * Copyright (c) 2023-2024 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdint.h>
#include <stdbool.h>
#include "tfm_hal_device_header.h"
#include "tfm_ns_ctx.h"

#include "tfm_ns_ctx_shm.h"

#define TFM_NS_CONTEXT_MAX_TID 0xff

/* Supported maximum context for NS. Only support single context for now. */
#define TFM_NS_CONTEXT_MAX                  1

/*
 * NS context. Initialized to 0.
 * All contexts are not used as the reference counter is 0 in initialization.
 */
static struct tfm_ns_ctx_t ns_ctx_data[TFM_NS_CONTEXT_MAX] = {0};

static struct tfm_ns_ctx_mgr_t tfm_ns_ctx_mgr = {
    .ns_ctx_limit = TFM_NS_CONTEXT_MAX,
    .active_ns_ctx_index = TFM_NS_CONTEXT_MAX,
    .ns_ctx_data = ns_ctx_data,
};

struct tfm_ns_ctx_mgr_t *ns_ctx_mgr = &tfm_ns_ctx_mgr;

/*
 * All the code in this file access the above structs directly.
 * Code in other files accesses them through a pointer.
 */

bool acquire_ns_ctx(uint8_t gid, uint8_t *idx)
{
    /* Only support one context slot for now */
    uint32_t i;
    uint32_t empty_ctx_idx = TFM_NS_CONTEXT_MAX; /* default is invalid */

    __disable_irq();
    /*
     * Go through all context slots to get the context for the given group ID.
     * It will not take a long time as the context number should be limited.
     */
    for (i = 0; i < TFM_NS_CONTEXT_MAX; i++) {
        if (tfm_ns_ctx_mgr.ns_ctx_data[i].ref_cnt > 0) {  /* This context has been taken */
            if (tfm_ns_ctx_mgr.ns_ctx_data[i].gid == gid) {
                /*
                 * Found the context associated with the input group ID.
                 * Check if the thread number reached the limit.
                 */
                if (tfm_ns_ctx_mgr.ns_ctx_data[i].ref_cnt < TFM_NS_CONTEXT_MAX_TID) {
                    /* Reuse this context and increase the reference number */
                    tfm_ns_ctx_mgr.ns_ctx_data[i].ref_cnt++;
                    *idx = i;
                    __enable_irq();
                    return true;
                } else {
                    /* No more thread for this group */
                    __enable_irq();
                    return false;
                }
            }
        } else {    /* Found an empty context slot */
            /* Save the first unused context index and continue */
            if (empty_ctx_idx == TFM_NS_CONTEXT_MAX) {
                empty_ctx_idx = i;
            }
            /*
             * Still need to go through the remaining slots to check if there
             * is an existing context assigned to the given group ID.
             */
        }
    }

    /* No existing context for the group ID, use the first free context */
    if (empty_ctx_idx < TFM_NS_CONTEXT_MAX) {
        tfm_ns_ctx_mgr.ns_ctx_data[empty_ctx_idx].ref_cnt++;
        tfm_ns_ctx_mgr.ns_ctx_data[empty_ctx_idx].gid = gid;
        *idx = empty_ctx_idx;
        __enable_irq();
        return true;
    } else {
        __enable_irq();
        return false;   /* No available context */
    }
}

bool release_ns_ctx(uint8_t gid, uint8_t tid, uint8_t idx)
{
    /* Check if the index is in range */
    if (idx >= TFM_NS_CONTEXT_MAX) {
        return false;
    }

    __disable_irq();

    /* Check if the context belongs to that group  */
    if (tfm_ns_ctx_mgr.ns_ctx_data[idx].gid != gid) {
        __enable_irq();
        return false;
    }

    /*
     * If it is to release the current active context, then set active context
     * to invalid.
     * Otherwise, just de-reference the context
     */
    if (idx == tfm_ns_ctx_mgr.active_ns_ctx_index) {
        if (tfm_ns_ctx_mgr.ns_ctx_data[idx].tid == tid) {
            /* Release the currrent active thread */
            if (tfm_ns_ctx_mgr.ns_ctx_data[idx].ref_cnt > 0) {
                tfm_ns_ctx_mgr.ns_ctx_data[idx].ref_cnt--;
            }
            tfm_ns_ctx_mgr.active_ns_ctx_index = TFM_NS_CONTEXT_MAX;
        } else {
            /*
             * Release for another thread in the active context
             * As there's an active thread ongoing, so the ref_counter should
             * be at least 1 after the release.
             */
            if (tfm_ns_ctx_mgr.ns_ctx_data[idx].ref_cnt > 1) {
                tfm_ns_ctx_mgr.ns_ctx_data[idx].ref_cnt--;
            }
        }
    } else {
        /* Release in the non-active context */
        if (tfm_ns_ctx_mgr.ns_ctx_data[idx].ref_cnt > 0) {
            tfm_ns_ctx_mgr.ns_ctx_data[idx].ref_cnt--;
        }
    }

    __enable_irq();
    return true;
}

bool load_ns_ctx(uint8_t gid, uint8_t tid, int32_t nsid, uint8_t idx)
{
    /* Check if the index is in range */
    if (idx >= TFM_NS_CONTEXT_MAX) {
        return false;
    }

    __disable_irq();

    /* Check group ID and reference counter */
    if ((tfm_ns_ctx_mgr.ns_ctx_data[idx].gid != gid) || (tfm_ns_ctx_mgr.ns_ctx_data[idx].ref_cnt == 0)) {
        __enable_irq();
        return false;
    }

    tfm_ns_ctx_mgr.ns_ctx_data[idx].tid = tid;
    tfm_ns_ctx_mgr.ns_ctx_data[idx].nsid = nsid;
    tfm_ns_ctx_mgr.active_ns_ctx_index = idx;
    __enable_irq();
    return true;
}

bool save_ns_ctx(uint8_t gid, uint8_t tid, uint8_t idx)
{
    __disable_irq();
    /* Check if the given index is valid */
    if ((idx != tfm_ns_ctx_mgr.active_ns_ctx_index) || (idx >= TFM_NS_CONTEXT_MAX)) {
        __enable_irq();
        return false;
    }

    /* Check group, thread ID and reference counter */
    if ((tfm_ns_ctx_mgr.ns_ctx_data[idx].gid != gid)
        || (tfm_ns_ctx_mgr.ns_ctx_data[idx].tid != tid)
        || (tfm_ns_ctx_mgr.ns_ctx_data[idx].ref_cnt == 0)) {
        __enable_irq();
        return false;
    }

    /* Set active context index to invalid */
    tfm_ns_ctx_mgr.active_ns_ctx_index = TFM_NS_CONTEXT_MAX;
    __enable_irq();
    return true;
}
