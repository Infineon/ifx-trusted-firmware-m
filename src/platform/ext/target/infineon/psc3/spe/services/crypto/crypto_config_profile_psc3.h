/*
 * Copyright (c) 2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CRYPTO_CONFIG_PROFILE_PSC3_H
#define CRYPTO_CONFIG_PROFILE_PSC3_H

/*
 * CBC-MAC is not yet supported via the PSA API in Mbed TLS.
 */
//#define PSA_WANT_ALG_CBC_MAC                    1
//#define PSA_WANT_ALG_CBC_NO_PADDING             1
//#define PSA_WANT_ALG_CBC_PKCS7                  1
//#define PSA_WANT_ALG_CCM                        1
#ifdef IFX_MBEDTLS_ACCELERATION_ENABLED
/* ifx-mbedtls/include/psa/crypto_adjust_auto_enabled.h file enables
 * PSA_WANT_KEY_TYPE_DERIVE, while PSA_WANT_KEY_TYPE_DERIVE does not work
 * without PSA_WANT_ALG_CMAC */
#define PSA_WANT_ALG_CMAC                       1 /* PSA_WANT_KEY_TYPE_DERIVE does not work without it */
#else /* IFX_MBEDTLS_ACCELERATION_ENABLED */
/* PSA_WANT_ALG_CCM is required to build secure_fw/partitions/crypto/tfm_mbedcrypto_alt.c */
#define PSA_WANT_ALG_CCM                        1
#endif /* IFX_MBEDTLS_ACCELERATION_ENABLED */
//#define PSA_WANT_ALG_CFB                        1
//#define PSA_WANT_ALG_CHACHA20_POLY1305          1
//#define PSA_WANT_ALG_CTR                        1
//#define PSA_WANT_ALG_DETERMINISTIC_ECDSA        1
//#define PSA_WANT_ALG_ECB_NO_PADDING             1
//#define PSA_WANT_ALG_ECDH                       1
#define PSA_WANT_ALG_ECDSA                      1
//#define PSA_WANT_ALG_GCM                        1
//#define PSA_WANT_ALG_HKDF                       1
//#define PSA_WANT_ALG_HMAC                       1
//#define PSA_WANT_ALG_MD5                        1
//#define PSA_WANT_ALG_OFB                        1
/* PBKDF2-HMAC is not yet supported via the PSA API in Mbed TLS.
 * Note: when adding support, also adjust include/mbedtls/config_psa.h */
//#define PSA_WANT_ALG_PBKDF2_HMAC                1
//#define PSA_WANT_ALG_RIPEMD160                  1
//#define PSA_WANT_ALG_RSA_OAEP                   1
//#define PSA_WANT_ALG_RSA_PKCS1V15_CRYPT         1
//#define PSA_WANT_ALG_RSA_PKCS1V15_SIGN          1
//#define PSA_WANT_ALG_RSA_PSS                    1
//#define PSA_WANT_ALG_SHA_1                      1
//#define PSA_WANT_ALG_SHA_224                    1
#define PSA_WANT_ALG_SHA_256                    1
//#define PSA_WANT_ALG_SHA_384                    1
//#define PSA_WANT_ALG_SHA_512                    1
//#define PSA_WANT_ALG_STREAM_CIPHER              1
//#define PSA_WANT_ALG_TLS12_PRF                  1
//#define PSA_WANT_ALG_TLS12_PSK_TO_MS            1
/* PBKDF2-HMAC is not yet supported via the PSA API in Mbed TLS.
 * Note: when adding support, also adjust include/mbedtls/config_psa.h */
//#define PSA_WANT_ALG_XTS                        1

//#define PSA_WANT_ECC_BRAINPOOL_P_R1_256         1
//#define PSA_WANT_ECC_BRAINPOOL_P_R1_384         1
//#define PSA_WANT_ECC_BRAINPOOL_P_R1_512         1
//#define PSA_WANT_ECC_MONTGOMERY_255             1
//#define PSA_WANT_ECC_MONTGOMERY_448             1
//#define PSA_WANT_ECC_SECP_K1_192                1
/*
 * SECP224K1 is buggy via the PSA API in Mbed TLS
 * (https://github.com/Mbed-TLS/mbedtls/issues/3541). Thus, do not enable it by
 * default.
 */
//#define PSA_WANT_ECC_SECP_K1_224                1
//#define PSA_WANT_ECC_SECP_K1_256                1
//#define PSA_WANT_ECC_SECP_R1_192                1
//#define PSA_WANT_ECC_SECP_R1_224                1
#define PSA_WANT_ECC_SECP_R1_256                1
//#define PSA_WANT_ECC_SECP_R1_384                1
//#define PSA_WANT_ECC_SECP_R1_521                1

//#define PSA_WANT_KEY_TYPE_DERIVE                1
//#define PSA_WANT_KEY_TYPE_HMAC                  1

/* PSA_WANT_KEY_TYPE_AES is required by PSA_WANT_ALG_CMAC and PSA_WANT_ALG_CCM */
#define PSA_WANT_KEY_TYPE_AES                   1 /* PSA_WANT_ALG_CCM does not work without it */
//#define PSA_WANT_KEY_TYPE_ARIA                  1
//#define PSA_WANT_KEY_TYPE_CAMELLIA              1
//#define PSA_WANT_KEY_TYPE_CHACHA20              1
//#define PSA_WANT_KEY_TYPE_DES                   1
//#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR          1 /* Deprecated */
#define PSA_WANT_KEY_TYPE_ECC_PUBLIC_KEY        1
// #define PSA_WANT_KEY_TYPE_RAW_DATA              1
//#define PSA_WANT_KEY_TYPE_RSA_KEY_PAIR          1 /* Deprecated */
//#define PSA_WANT_KEY_TYPE_RSA_PUBLIC_KEY        1

/*
 * The following symbols extend and deprecate the legacy
 * PSA_WANT_KEY_TYPE_xxx_KEY_PAIR ones. They include the usage of that key in
 * the name's suffix. "_USE" is the most generic and it can be used to describe
 * a generic suport, whereas other ones add more features on top of that and
 * they are more specific.
 */
#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_BASIC      1
//#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_IMPORT   1
//#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_EXPORT   1
//#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_GENERATE 1
//#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_DERIVE   1

#ifdef CRYPTO_HW_ACCELERATOR_CONFIG
#include CRYPTO_HW_ACCELERATOR_CONFIG
#endif /* CRYPTO_HW_ACCELERATOR_CONFIG */

#endif /* CRYPTO_CONFIG_PROFILE_PSC3_H */
