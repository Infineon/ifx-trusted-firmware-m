/*
 * Copyright (c) 2018-2024, Arm Limited. All rights reserved.
 * Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>

#include "cmsis_compiler.h"
#include "compiler_ext_defs.h"
#include "config_impl.h"
#include "fih.h"
#include "security_defs.h"
#include "svc_num.h"
#include "tfm_psa_call_pack.h"
#include "utilities.h"
#include "psa/client.h"
#include "psa/service.h"
#include "tfm_arch.h"

/*
 * This is the veneers of FF-M Client APIs for Armv8.0-m.
 * The interfaces are written in assembly, and the reasons:
 *
 * - On the 8.0 version of Armv8-M with security extension, a mandatory
 *   software solution needs to be applied because hardware reentrant
 *   detection was not involved until 8.1. It requires TF-M to take full
 *   control of stack usage to perform reentrant detection. Therefore
 *   TF-M shall implement these veneers in the assembly code.
 *
 * - The non-secure context is pushed in the stack. When SPM API returns,
 *   the pushed non-secure context is popped and overrides the returned
 *   context before returning to NSPE. Therefore it is unnecessary to
 *   explicitly clean up the context.
 */

#if defined(__ICCARM__)

#pragma required = psa_framework_version
#pragma required = psa_panic
#pragma required = psa_version
#pragma required = tfm_psa_call_pack
/* Following PSA APIs are only needed by connection-based services */
#if CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1
#pragma required = psa_connect
#pragma required = psa_close
#endif

#endif

__attribute__((naked, used))
void clear_caller_context(void)
{
    __ASM volatile(
#if (CONFIG_TFM_FLOAT_ABI >= 1)
        "   vmov.f32   s0, #1.0                               \n"
        "   vmov.f32   s1, #1.0                               \n"
        "   vmov.f32   s2, #1.0                               \n"
        "   vmov.f32   s3, #1.0                               \n"
        "   vmov.f32   s4, #1.0                               \n"
        "   vmov.f32   s5, #1.0                               \n"
        "   vmov.f32   s6, #1.0                               \n"
        "   vmov.f32   s7, #1.0                               \n"
        "   vmov.f32   s8, #1.0                               \n"
        "   vmov.f32   s9, #1.0                               \n"
        "   vmov.f32   s10, #1.0                              \n"
        "   vmov.f32   s11, #1.0                              \n"
        "   vmov.f32   s12, #1.0                              \n"
        "   vmov.f32   s13, #1.0                              \n"
        "   vmov.f32   s14, #1.0                              \n"
        "   vmov.f32   s15, #1.0                              \n"
        "   vmrs   r12, fpscr                                 \n"
        "   movw   r1, #0x009f                                \n"
        "   movt   r1, #0xf000                                \n"
        "   bics   r12, r1                                    \n"
        "   vmsr   fpscr, r12                                 \n"
#endif
        "   movs    r3, #0x0                                  \n"
        "   msr    APSR_nzcvq, r3                             \n"
        "   bx     lr                                         \n"
    );
}

__tz_naked_veneer
uint32_t tfm_psa_framework_version_veneer(void)
{
    __ASM volatile(
        SYNTAX_UNIFIED
#if (CONFIG_TFM_SECURE_THREAD_MASK_NS_INTERRUPT == 1) && (CONFIG_TFM_SPM_BACKEND_SFN == 1)
        "   ldr    r2, ="M2S(SECURE_THREAD_EXECUTION_PRIORITY)"\n"
        "   msr    basepri, r2                                \n"
#endif
        "   ldr    r2, [sp]                                   \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic1                               \n"
        "   push   {r4, lr}                                   \n"
        "   bl     "M2S(psa_framework_version)"               \n"
        "   bl     clear_caller_context                       \n"
        "   pop    {r1, r2}                                   \n"
        "   mov    lr, r2                                     \n"
        "   mov    r4, r1                                     \n"
#if (CONFIG_TFM_SECURE_THREAD_MASK_NS_INTERRUPT == 1) && (CONFIG_TFM_SPM_BACKEND_SFN == 1)
        "   ldr    r1, =0x00                                  \n"
        "   msr    basepri, r1                                \n"
#endif
#if (CONFIG_TFM_SECURE_THREAD_MASK_NS_INTERRUPT == 1) && defined(TFM_FIH_PROFILE_ON)
        "   mrs    r3, basepri                                \n"
        "   cmp    r3, #0                                     \n"
        "   bne    reent_panic1                               \n"
#endif /* TFM_FIH_PROFILE_ON */
        "   bxns   lr                                         \n"

        "reent_panic1:                                        \n"
        "   bl     psa_panic                                  \n"
    );
}

__tz_naked_veneer
uint32_t tfm_psa_version_veneer(uint32_t sid)
{
    __ASM volatile(
        SYNTAX_UNIFIED
#if (CONFIG_TFM_SECURE_THREAD_MASK_NS_INTERRUPT == 1) && (CONFIG_TFM_SPM_BACKEND_SFN == 1)
        "   ldr    r2, ="M2S(SECURE_THREAD_EXECUTION_PRIORITY)"\n"
        "   msr    basepri, r2                                \n"
#endif
        "   ldr    r2, [sp]                                   \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic2                               \n"

        "   push   {r4, lr}                                   \n"
        "   bl     "M2S(psa_version)"                         \n"
        "   bl     clear_caller_context                       \n"
        "   pop    {r1, r2}                                   \n"
        "   mov    lr, r2                                     \n"
        "   mov    r4, r1                                     \n"
#if (CONFIG_TFM_SECURE_THREAD_MASK_NS_INTERRUPT == 1) && (CONFIG_TFM_SPM_BACKEND_SFN == 1)
        "   ldr    r1, =0x00                                  \n"
        "   msr    basepri, r1                                \n"
#endif
#if (CONFIG_TFM_SECURE_THREAD_MASK_NS_INTERRUPT == 1) && defined(TFM_FIH_PROFILE_ON)
        "   mrs    r3, basepri                                \n"
        "   cmp    r3, #0                                     \n"
        "   bne    reent_panic2                               \n"
#endif /* TFM_FIH_PROFILE_ON */
"   bxns   lr                                         \n"

        "reent_panic2:                                        \n"
        "   bl     psa_panic                                  \n"
    );
}

__tz_naked_veneer
psa_status_t tfm_psa_call_veneer(psa_handle_t handle,
                                 uint32_t ctrl_param,
                                 const psa_invec *in_vec,
                                 psa_outvec *out_vec)
{
    __ASM volatile(
        SYNTAX_UNIFIED
        "   push   {r2, r3}                                   \n"
#if (CONFIG_TFM_SECURE_THREAD_MASK_NS_INTERRUPT == 1) && (CONFIG_TFM_SPM_BACKEND_SFN == 1)
        "   ldr    r2, ="M2S(SECURE_THREAD_EXECUTION_PRIORITY)"\n"
        "   msr    basepri, r2                                \n"
#endif
        "   ldr    r2, [sp, #8]                               \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic4                               \n"
        "   ldr    r3, ="M2S(NS_VEC_DESC_BIT)"                \n"
        "   orrs   r1, r3                                     \n"
        "   pop    {r2, r3}                                   \n"
        "   push   {r4, lr}                                   \n"
        "   bl     "M2S(tfm_psa_call_pack)"                   \n"
        "   bl     clear_caller_context                       \n"
        "   pop    {r1, r2}                                   \n"
        "   mov    lr, r2                                     \n"
        "   mov    r4, r1                                     \n"
#if (CONFIG_TFM_SECURE_THREAD_MASK_NS_INTERRUPT == 1) && (CONFIG_TFM_SPM_BACKEND_SFN == 1)
        "   ldr    r1, =0x00                                  \n"
        "   msr    basepri, r1                                \n"
#endif
#if (CONFIG_TFM_SECURE_THREAD_MASK_NS_INTERRUPT == 1) && defined(TFM_FIH_PROFILE_ON)
        "   mrs    r3, basepri                                \n"
        "   cmp    r3, #0                                     \n"
        "   bne    reent_panic4                               \n"
#endif /* TFM_FIH_PROFILE_ON */
        "   bxns   lr                                         \n"

        "reent_panic4:                                        \n"
        "   bl     psa_panic                                  \n"
    );
}

/* Following veneers are only needed by connection-based services */
#if CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1

__tz_naked_veneer
psa_handle_t tfm_psa_connect_veneer(uint32_t sid, uint32_t version)
{
    __ASM volatile(
        SYNTAX_UNIFIED
#if (CONFIG_TFM_SECURE_THREAD_MASK_NS_INTERRUPT == 1) && (CONFIG_TFM_SPM_BACKEND_SFN == 1)
        "   ldr    r2, ="M2S(SECURE_THREAD_EXECUTION_PRIORITY)"\n"
        "   msr    basepri, r2                                \n"
#endif
        "   ldr    r2, [sp]                                   \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic3                               \n"
        "   push   {r4, lr}                                   \n"
        "   bl     "M2S(psa_connect)"                         \n"
        "   bl     clear_caller_context                       \n"
        "   pop    {r1, r2}                                   \n"
        "   mov    lr, r2                                     \n"
        "   mov    r4, r1                                     \n"
#if (CONFIG_TFM_SECURE_THREAD_MASK_NS_INTERRUPT == 1) && (CONFIG_TFM_SPM_BACKEND_SFN == 1)
        "   ldr    r1, =0x00                                  \n"
        "   msr    basepri, r1                                \n"
#endif
#if (CONFIG_TFM_SECURE_THREAD_MASK_NS_INTERRUPT == 1) && defined(TFM_FIH_PROFILE_ON)
        "   mrs    r3, basepri                                \n"
        "   cmp    r3, #0                                     \n"
        "   bne    reent_panic3                               \n"
#endif /* TFM_FIH_PROFILE_ON */
        "   bxns   lr                                         \n"

        "reent_panic3:                                        \n"
        "   bl     psa_panic                                  \n"
    );
}

__tz_naked_veneer
void tfm_psa_close_veneer(psa_handle_t handle)
{
    __ASM volatile(
        SYNTAX_UNIFIED
#if (CONFIG_TFM_SECURE_THREAD_MASK_NS_INTERRUPT == 1) && (CONFIG_TFM_SPM_BACKEND_SFN == 1)
        "   ldr    r2, ="M2S(SECURE_THREAD_EXECUTION_PRIORITY)"\n"
        "   msr    basepri, r2                                \n"
#endif
        "   ldr    r2, [sp]                                   \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic5                               \n"

        "   push   {r4, lr}                                   \n"
        "   bl     "M2S(psa_close)"                           \n"
        "   bl     clear_caller_context                       \n"
        "   pop    {r1, r2}                                   \n"
        "   mov    lr, r2                                     \n"
        "   mov    r4, r1                                     \n"
#if (CONFIG_TFM_SECURE_THREAD_MASK_NS_INTERRUPT == 1) && (CONFIG_TFM_SPM_BACKEND_SFN == 1)
        "   ldr    r1, =0x00                                  \n"
        "   msr    basepri, r1                                \n"
#endif
#if (CONFIG_TFM_SECURE_THREAD_MASK_NS_INTERRUPT == 1) && defined(TFM_FIH_PROFILE_ON)
        "   mrs    r3, basepri                                \n"
        "   cmp    r3, #0                                     \n"
        "   bne    reent_panic5                               \n"
#endif /* TFM_FIH_PROFILE_ON */
        "   bxns   lr                                         \n"

        "reent_panic5:                                        \n"
        "   bl     psa_panic                                  \n"
    );
}

#else /* CONFIG_TFM_CONNECTION_BASED_SERVICE_API */

/*
 * Define a constant to enable naked tfm_psa_connect_veneer() to return the error code.
 * It is not supported to directly return the error code in psa_status_t type in Basic Asm via
 * M2S.
 * This is defined as a const in order to be placed in .rodata section which
 * normally has least strict protection requirements and can be freely accessed
 * during a call to "tfm_psa_connect_veneer()".
 */
__used const int32_t ret_err = (int32_t)PSA_ERROR_NOT_SUPPORTED;
#if defined(__ICCARM__)
#pragma required = ret_err
#endif

__tz_naked_veneer
psa_handle_t tfm_psa_connect_veneer(uint32_t sid, uint32_t version)
{
    __ASM volatile(
        SYNTAX_UNIFIED
        "   ldr    r2, [sp]                                   \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic3                               \n"

        "   ldr    r1, =ret_err                               \n"
        "   ldr    r0, [r1]                                   \n"
        "   bxns   lr                                         \n"

        "reent_panic3:                                        \n"
        "   bl     psa_panic                                  \n"
    );
}

__tz_naked_veneer
void tfm_psa_close_veneer(psa_handle_t handle)
{
    __ASM volatile(
        SYNTAX_UNIFIED
        "   ldr    r2, [sp]                                   \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic5                               \n"

        "   bxns   lr                                         \n"

        "reent_panic5:                                        \n"
        "   bl     psa_panic                                  \n"
    );
}

#endif /* CONFIG_TFM_CONNECTION_BASED_SERVICE_API */
