/*
 * Copyright (c) 2017-2023, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PLAT_PS_CRYPTO_H__
#define __TFM_PLAT_PS_CRYPTO_H__

#include <stdint.h>
#include "psa/crypto.h"
#include "tfm_plat_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PS_KEY_LEN_BYTES  16
#define PS_TAG_LEN_BYTES  16
#define PS_IV_LEN_BYTES   12

#ifndef PS_CRYPTO_AEAD_ALG
#define PS_CRYPTO_AEAD_ALG PSA_ALG_GCM
#endif

/* The PSA key type used by this implementation */
#define PS_KEY_TYPE PSA_KEY_TYPE_AES
/* The PSA key usage required by this implementation */
#define PS_KEY_USAGE (PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT)

/* The PSA algorithm used by this implementation */
#define PS_CRYPTO_ALG \
    PSA_ALG_AEAD_WITH_SHORTENED_TAG(PS_CRYPTO_AEAD_ALG, PS_TAG_LEN_BYTES)

/**
 * \brief Sets the key to use for crypto operations for Protected Storage.
 *
 * This function can be used by platform to provide custom setup of crypto keys 
 * used by Protected Storage. 
 * Is called by Protected Storage if PS_DEFAULT_CRYPTO_CONFIG is set to zero.
 *
 * \param[out]    ps_key          Identifier for the newly created key
 * \param[in]     key_label       Pointer to the key label
 * \param[in]     key_label_len   Length of the key label
 *
 * \return Returns values as described in \ref psa_status_t
 */
psa_status_t tfm_platform_ps_set_key(psa_key_id_t *ps_key, const uint8_t *key_label, size_t key_label_len);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_PLAT_PS_CRYPTO_H__ */
