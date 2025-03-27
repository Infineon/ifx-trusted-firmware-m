/*
 * Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef MBEDTLS_TARGET_CONFIG_PSC3_H
#define MBEDTLS_TARGET_CONFIG_PSC3_H

/* *** DO NOT CHANGE ANY SETTINGS IN THIS SECTION *** */

#if IFX_MBEDTLS_ACCELERATION_ENABLED
/* Enable CRYPTOLITE transparent driver */
#define IFX_PSA_CRYPTOLITE_PRESENT
/* Added to enable accelerator implementation of Advanced Encryption Standard */
#define MBEDTLS_AES_ALT
#endif /* IFX_PSA_CRYPTOLITE_PRESENT */

/* Enable support for platform built-in keys */
#define MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS

#ifdef IFX_MBEDTLS_CONFIG_PATH
/* Include project specific ifx-mbedtls configuration header */
#include IFX_MBEDTLS_CONFIG_PATH
#endif

#endif /* MBEDTLS_TARGET_CONFIG_PSC3_H */
