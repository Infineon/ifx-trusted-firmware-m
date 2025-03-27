/*
 * Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CRYPTO_KEYS_FLASH_H
#define CRYPTO_KEYS_FLASH_H

#include <stdint.h>

typedef struct {
    uint8_t huk[32];    /* PSA_KEY_EXPORT_ECC_KEY_PAIR_MAX_SIZE(256)*/
    /* IAK private */
    uint8_t iak[32];    /* PSA_KEY_EXPORT_ECC_KEY_PAIR_MAX_SIZE(256)*/
} ifx_plat_psc3_keys_t;

#define IFX_CRYPTO_KEYS_PTR                         ((const ifx_plat_psc3_keys_t *)0x13407604U)

#define IFX_CRYPTO_KEYS_HUK_KEY_BITS                256
#define IFX_CRYPTO_KEYS_HUK_KEY_LEN                 PSA_KEY_EXPORT_ECC_KEY_PAIR_MAX_SIZE(256u)
#define IFX_CRYPTO_KEYS_HUK_ALGORITHM               PSA_ALG_HKDF(PSA_ALG_SHA_256)
#define IFX_CRYPTO_KEYS_HUK_TYPE                    PSA_KEY_TYPE_DERIVE

#define IFX_CRYPTO_KEYS_IAK_KEY_BITS                256
#define IFX_CRYPTO_KEYS_IAK_KEY_LEN                 PSA_KEY_EXPORT_ECC_KEY_PAIR_MAX_SIZE(256u)
#define IFX_CRYPTO_KEYS_IAK_ALGORITHM               PSA_ALG_ECDSA(PSA_ALG_SHA_256)
#define IFX_CRYPTO_KEYS_IAK_TYPE                    PSA_KEY_TYPE_ECC_KEY_PAIR(PSA_ECC_FAMILY_SECP_R1)

#endif /* CRYPTO_KEYS_FLASH_H */
