/*
 * Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef TFM_BUILTIN_KEY_IDS_H
#define TFM_BUILTIN_KEY_IDS_H

#include "psa/crypto.h"
#include "static_checks.h"

/**
 * \brief The persistent key identifiers for TF-M builtin keys.
 *
 * The value of TFM_BUILTIN_KEY_ID_MIN (and therefore of the whole range) is
 * completely arbitrary except for being inside the PSA builtin keys range.
 *
 */

/* MBEDTLS_PSA_KEY_ID_BUILTIN_MIN is defined in crypto_extra.h from MbedTLS
 * but the problem is that TFM version of crypto_extra.h does not have this
 * define. So Crypto service which uses MbedTLS crypto_extra.h has this defined
 * but other partitions and NSPE don't. Thus define
 * MBEDTLS_PSA_KEY_ID_BUILTIN_MIN when needed. */
#ifndef MBEDTLS_PSA_KEY_ID_BUILTIN_MIN
#define MBEDTLS_PSA_KEY_ID_BUILTIN_MIN          ((psa_key_id_t)0x7fff0000)
#endif

enum tfm_builtin_key_id_t {
    TFM_BUILTIN_KEY_ID_MIN = MBEDTLS_PSA_KEY_ID_BUILTIN_MIN,
    TFM_BUILTIN_KEY_ID_HUK = MBEDTLS_PSA_KEY_ID_BUILTIN_MIN,
    TFM_BUILTIN_KEY_ID_IAK, /* IAK private */
    TFM_BUILTIN_KEY_ID_MAX
};

#endif /* TFM_BUILTIN_KEY_IDS_H */
