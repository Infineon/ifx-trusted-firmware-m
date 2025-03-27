/*
 * Copyright (c) 2023 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __OS_WRAPPER_KERNEL_H__
#define __OS_WRAPPER_KERNEL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "os_wrapper/common.h"

/**
 * \brief Returns value that indicates whether RTOS kernel is running.
 *
 * \return true if RTOS kernel is running.
 *         false if RTOS kernel is not running.
 */
bool os_wrapper_is_kernel_running(void);

#ifdef __cplusplus
}
#endif

#endif /* __OS_WRAPPER_KERNEL_H__ */
