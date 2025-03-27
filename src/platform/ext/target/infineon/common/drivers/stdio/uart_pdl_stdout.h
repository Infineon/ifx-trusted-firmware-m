/*
 * Copyright (c) 2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef UART_PDL_STDOUT_H
#define UART_PDL_STDOUT_H

#include <stdio.h>

#ifdef  __cplusplus
extern "C"
{
#endif

/**
 * \brief Output log for SVC.
 *
 * \ref stdio_output_string_raw is intended to be used by Platform service.
 * \ref ifx_platform_log_msg is a client API to print message through Platform
 * service.
 */
int stdio_output_string_raw(const unsigned char *str, uint32_t len);

#ifdef  __cplusplus
}
#endif

#endif /* UART_PDL_STDOUT_H */
