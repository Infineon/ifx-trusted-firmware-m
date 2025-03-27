/*
 * Copyright (c) 2017-2020 ARM Limited
 * Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <cy_device.h>
#include <cy_scb_uart.h>

#include "config_tfm.h"
#if DOMAIN_S
#include "platform_svc_api.h"
#endif
#include "uart_stdout.h"
#include "uart_pdl_stdout.h"

#include "static_checks.h"

#include <stdio.h>
#include <string.h>
#include <limits.h>

#if (IFX_CORE == IFX_CM33) && DOMAIN_S
#include "tfm_peripherals_def.h"

#define IFX_UART_PDL_SCB                IFX_TFM_SPM_UART
#define IFX_UART_PDL_SCB_CONFIG         IFX_TFM_SPM_UART_config
#ifdef IFX_TFM_SPM_UART_FLUSH
#define IFX_UART_PDL_SCB_FLUSH          IFX_TFM_SPM_UART_FLUSH
#endif

#ifndef IFX_UART_USE_SPM_LOG_MSG
#define IFX_UART_USE_SPM_LOG_MSG        0
#endif

#elif (IFX_CORE == IFX_CM33) && DOMAIN_NS

#ifndef IFX_UART_USE_SPM_LOG_MSG
#define IFX_UART_USE_SPM_LOG_MSG        1
#endif

#if !IFX_UART_USE_SPM_LOG_MSG
#include "cycfg_peripherals.h"
#define IFX_UART_PDL_SCB                IFX_TFM_CM33_NSPE_UART
#endif

#elif (IFX_CORE == IFX_CM55) && DOMAIN_NS

#ifndef IFX_UART_USE_SPM_LOG_MSG
#define IFX_UART_USE_SPM_LOG_MSG        1
#endif

#if !IFX_UART_USE_SPM_LOG_MSG
#include "cycfg_peripherals.h"
#define IFX_UART_PDL_SCB                IFX_TFM_CM55_NSPE_UART
#endif
#endif /* (IFX_CORE == IFX_CM33) && DOMAIN_S */

#if IFX_UART_USE_SPM_LOG_MSG
#include "ifx_platform_api.h"
#endif

#if DOMAIN_S
/**
 * \brief Output log for SVC.
 *
 * \ref stdio_output_string_raw is intended to be used by Platform service.
 * \ref ifx_platform_log_msg is a client API to print message through Platform
 * service.
 */
int stdio_output_string_raw(const unsigned char *str, uint32_t len)
{
#if IFX_UART_ENABLED
    TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_11_8, "DRIVERS-13923, Although we lose const during cast string to void *, function works with string as read only")
    Cy_SCB_UART_PutArrayBlocking(IFX_UART_PDL_SCB, (unsigned char *)str, len);

#if IFX_UART_PDL_SCB_FLUSH
    /* Wait while UART is transmitting */
    while (!Cy_SCB_UART_IsTxComplete(IFX_UART_PDL_SCB)) {}
#endif
#endif /* IFX_UART_ENABLED */

    return len < (uint32_t)INT_MAX ? (int)len : INT_MAX;
}
#endif /* DOMAIN_S */

/**
 * \note Current implementation doesn't work if access to UART is done
 *      by multiple threads.
 *
 * \ref IFX_STDIO_PREFIX optional configuration can be used to provide prefix
 * to the messages. It's useful if system has only one serial port and there is
 * output from multiple cores/images.
 */
int stdio_output_string(const unsigned char *str, uint32_t len)
{
#if IFX_UART_USE_SPM_LOG_MSG
#if DOMAIN_S
    /* ifx_call_platform_uart_log calls SPM to print message via SVC request.
     * Finally it will be handled by \ref stdio_output_string_raw.*/
    ifx_call_platform_uart_log(str, len);
#else
    /* ifx_platform_log_msg calls Platform service to print message, while
     * Platform service redirects this message to SPM through SVC request.
     * Finally it will be handled by \ref stdio_output_string_raw. */
    return (int)ifx_platform_log_msg(str, len);
#endif
#else

#if IFX_UART_ENABLED
#if defined(IFX_STDIO_PREFIX)
    /* Validate that IFX_STDIO_PREFIX is a string not a pointer */
    const char check_prefix[] = IFX_STDIO_PREFIX; (void)check_prefix;
    /* Add prefix */
    TFM_COVERITY_DEVIATE_BLOCK(MISRA_C_2023_Rule_7_4, "DRIVERS-13923, Cy_SCB_UART_PutArrayBlocking() uses the second parameter as const")
    Cy_SCB_UART_PutArrayBlocking(IFX_UART_PDL_SCB,
                                 IFX_STDIO_PREFIX,
                                 sizeof(IFX_STDIO_PREFIX) - 1U);
    TFM_COVERITY_BLOCK_END(MISRA_C_2023_Rule_7_4)
#endif
    /* Consider to use synchronization if there is need to share
     * UART between multiple partitions/threads/cores.
     * Cy_SCB_UART_PutArrayBlocking has problems if there is more than
     * one thread/core which access UART at the same time. */
     TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_11_8, "DRIVERS-13923, Although we lose const during cast string to void *, function works with string as read only")
     Cy_SCB_UART_PutArrayBlocking(IFX_UART_PDL_SCB, (void *)str, len);

#if IFX_UART_PDL_SCB_FLUSH
    /* Wait while UART is transmitting */
    while (!Cy_SCB_UART_IsTxComplete(IFX_UART_PDL_SCB)) {}
#endif
#endif /* IFX_UART_ENABLED */

    return len < (uint32_t)INT_MAX ? (int)len : INT_MAX;
#endif /* IFX_UART_USE_SPM_LOG_MSG */
}

/* __ARMCC_VERSION is only defined starting from Arm compiler version 6 */
#if defined(__ARMCC_VERSION)
/* Struct FILE is implemented in stdio.h. Used to redirect printf to
 * STDIO_DRIVER
 */
FILE __stdout;
/* Redirects printf to STDIO_DRIVER in case of ARMCLANG */
int fputc(int ch, FILE *f)
{
    (void)f;

    /* Send byte to USART */
    (void)stdio_output_string((const unsigned char *)&ch, 1);

    /* Return character written */
    return ch;
}
#elif defined(__GNUC__)
/* Redirects printf to STDIO_DRIVER in case of GNUARM */
TFM_COVERITY_DEVIATE_BLOCK(MISRA_C_2023_Rule_21_2, "This definition overrides weak function _write, that has a _ symbol")
TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_8_4, "This definition overrides weak function")
int _write(int fd, char *str, int len)
{
    (void)fd;

    /* Send string and return the number of characters written */
    return stdio_output_string((const unsigned char *)str, (uint32_t)len);
}
TFM_COVERITY_BLOCK_END(MISRA_C_2023_Rule_21_2)
#elif defined(__ICCARM__)
int putchar(int ch)
{
    /* Send byte to USART */
    (void)stdio_output_string((const unsigned char *)&ch, 1);

    /* Return character written */
    return ch;
}
#endif

void stdio_init(void)
{
#if defined(IFX_UART_PDL_INITIALIZE_UART) && IFX_UART_ENABLED
    cy_en_scb_uart_status_t retval;

    retval = Cy_SCB_UART_Init(IFX_UART_PDL_SCB, &IFX_UART_PDL_SCB_CONFIG, NULL);

    Cy_SCB_UART_ClearRxFifo(IFX_UART_PDL_SCB);
    Cy_SCB_UART_ClearTxFifo(IFX_UART_PDL_SCB);

    if (CY_SCB_UART_SUCCESS == retval) {
        Cy_SCB_UART_Enable(IFX_UART_PDL_SCB);
    }
#endif
}

void stdio_uninit(void)
{
#if defined(IFX_UART_PDL_INITIALIZE_UART) && IFX_UART_ENABLED
    Cy_SCB_UART_Disable(IFX_UART_PDL_SCB, NULL);

    Cy_SCB_UART_DeInit(IFX_UART_PDL_SCB);
#endif
}
