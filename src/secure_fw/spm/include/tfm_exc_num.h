/*
 * Copyright (c) 2018-2023, Arm Limited. All rights reserved.
 * Copyright (c) 2022-2023 Cypress Semiconductor Corporation (an Infineon
 * company) or an affiliate of Cypress Semiconductor Corporation. All rights
 * reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_EXC_NUM_H__
#define __TFM_EXC_NUM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "cmsis_compiler.h"

/* Exception numbers */
#define EXC_NUM_THREAD_MODE                     (0)
#define EXC_NUM_SVCALL                          (11)
#define EXC_NUM_PENDSV                          (14)

__attribute__ ((always_inline))
__STATIC_INLINE uint32_t __get_active_exc_num(void)
{
    IPSR_Type IPSR;

    /* if non-zero, exception is active. NOT banked S/NS */
    IPSR.w = __get_IPSR();
    return IPSR.b.ISR;
}

#ifdef __cplusplus
}
#endif

#endif /* __TFM_EXC_NUM_H__ */
