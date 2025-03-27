/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 * Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdint.h>
#include <stdbool.h>
#include "cmsis.h"
#include "security_defs.h"
#include "tfm_ns_ctx.h"
#include "tfm_nspm.h"
#ifdef TFM_NSID_IN_SHARED_DATA
#include <arm_cmse.h>
#include "compiler_ext_defs.h"
#include "psa/error.h"
#include "current.h"
#include "fih.h"
#include "svc_num.h"
#include "tfm_hal_isolation.h"
#include "utilities.h"
#endif

#include "tfm_ns_ctx_shm.h"

#ifdef TFM_NSID_IN_SHARED_DATA
static struct tfm_ns_ctx_mgr_t *ns_ctx_mgr = NULL;
#else
extern struct tfm_ns_ctx_mgr_t *ns_ctx_mgr;
#endif

#ifdef TFM_NSID_IN_SHARED_DATA
#if defined(__ICCARM__)
void clear_caller_context(void);
#pragma required = clear_caller_context
#endif
__tz_naked_veneer
psa_status_t tfm_nsid_shm_init(const struct tfm_ns_ctx_mgr_t *tfm_ns_ctx_mgr)
{
    __ASM volatile(
        SYNTAX_UNIFIED

        "   push   {r2, r3}                                   \n"
        "   ldr    r2, [sp, #8]                               \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic_nsid_shm                       \n"
        "   pop    {r2, r3}                                   \n"
        "   push   {r4, lr}                                   \n"
        "   svc    "M2S(TFM_SVC_NSID_SHM_INIT)"               \n"
        "   bl     clear_caller_context                       \n"
        "   pop    {r1, r2}                                   \n"
        "   mov    lr, r2                                     \n"
        "   mov    r4, r1                                     \n"
        "   bxns   lr                                         \n"

        "reent_panic_nsid_shm:                                \n"
        "   svc    "M2S(TFM_SVC_PSA_PANIC)"                   \n"
        "   b      .                                          \n"
    );
}

void set_ns_ctx_mgr(struct tfm_ns_ctx_mgr_t *mgr)
{
    /* this value and the data it points to are validated before each use */
    ns_ctx_mgr = mgr;
}
#endif

bool init_ns_ctx(void)
{
    return true;
}

int32_t get_nsid_from_active_ns_ctx(void)
{
    int32_t ret = TFM_NS_CLIENT_INVALID_ID;

    /* Lock out NSPE while we read the struct */
    __disable_irq();

    {
        struct tfm_ns_ctx_mgr_t *mgr_cache = ns_ctx_mgr;

#ifdef TFM_NSID_IN_SHARED_DATA
        fih_int fih_rc = FIH_FAILURE;
        struct partition_t *partition = GET_CURRENT_COMPONENT();
        struct tfm_ns_ctx_mgr_t cache;

        /* ns_ctx_mgr must point to NS-writable memory */
        FIH_CALL(tfm_hal_memory_check, fih_rc,
                 partition->boundary, (uintptr_t)ns_ctx_mgr,
                 sizeof(struct tfm_ns_ctx_mgr_t),
                 TFM_HAL_ACCESS_READWRITE | TFM_HAL_ACCESS_NS);
        if (fih_not_eq(fih_rc, fih_int_encode(PSA_SUCCESS))) {
            tfm_core_panic();
        }

        /* Make a copy to avoid TOCTTOU attacks */
        mgr_cache = &cache;
        memcpy(mgr_cache, ns_ctx_mgr, sizeof(cache));

        /*
         * ns_ctx_mgr->ns_ctx_data must point to NS-writable memory
         * (an array of structs)
         */
        FIH_CALL(tfm_hal_memory_check, fih_rc,
                 partition->boundary, (uintptr_t)mgr_cache->ns_ctx_data,
                 mgr_cache->ns_ctx_limit * sizeof(struct tfm_ns_ctx_t),
                 TFM_HAL_ACCESS_READWRITE | TFM_HAL_ACCESS_NS);
        if (fih_not_eq(fih_rc, fih_int_encode(PSA_SUCCESS))) {
            tfm_core_panic();
        }
#endif

        if (mgr_cache->active_ns_ctx_index < mgr_cache->ns_ctx_limit) {
            ret = mgr_cache->ns_ctx_data[mgr_cache->active_ns_ctx_index].nsid;
        }
    }

    __enable_irq();

    /* IMPROVEMENT: (TFM-3947) add call to platform client_id validation */

    if (ret >= 0) {
        ret = TFM_NS_CLIENT_INVALID_ID;
    }

    return ret;
}
