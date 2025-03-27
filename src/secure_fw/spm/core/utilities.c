/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon
 * company) or an affiliate of Cypress Semiconductor Corporation. All rights
 * reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <inttypes.h>
#include "config_spm.h"
#include "fih.h"
#include "utilities.h"
#include "tfm_hal_platform.h"
#include "tfm_spm_log.h"
#include "static_checks.h"

void tfm_core_panic(void)
{
#if defined(__ICCARM__)
    uint32_t return_address = (uint32_t)__get_return_address();
#else /* __ICCARM__ */
    TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_11_6, "Intentional pointer cast, void will fit uint32_t")
    uint32_t return_address = (uint32_t)__builtin_return_address(0);
#endif /* __ICCARM__ */
    SPMLOG_ERRMSGVAL("tfm_core_panic() called from: ", return_address);
    (void)return_address; /* Suppress warning if logging is disabled */

    SPMLOG_ERRMSG("tfm_core_panic() called, resetting device!\r\n");

    (void)fih_delay();

/* Suppress Pe111 (statement is unreachable) for IAR as redundant code is needed for FIH */
#if defined(__ICCARM__)
#pragma diag_suppress = Pe111
#endif
#ifdef CONFIG_TFM_HALT_ON_CORE_PANIC

    /*
     * Halt instead of reboot to retain the backtrace that triggered
     * the fault and thereby make it easier to debug.
     */
    tfm_hal_system_halt();

#ifdef TFM_FIH_PROFILE_ON
    (void)fih_delay();

    tfm_hal_system_halt();
#endif

#else /* CONFIG_TFM_HALT_ON_CORE_PANIC */
    /*
     * FixMe: In the first stage, the SPM will restart the entire system when a
     * programmer error is detected in either the SPE or NSPE.
     * In the next stage, the specified error codes are also sent to any NSPE
     * management firmware. The NSPE management firmware can then decide to pass
     * those error codes back to the calling task or to use its own
     * functionality for terminating an execution context.
     */
    tfm_hal_system_reset();

#ifdef TFM_FIH_PROFILE_ON
    (void)fih_delay();

    tfm_hal_system_reset();
#endif

#endif /* CONFIG_TFM_HALT_ON_CORE_PANIC */
#if defined(__ICCARM__)
#pragma diag_default = Pe111
#endif
}
