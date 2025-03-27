/*
 * Copyright (c) 2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CRYPTO_HW_CRYPTOLITE_CONFIG_H
#define CRYPTO_HW_CRYPTOLITE_CONFIG_H

#if defined(PSA_WANT_ALG_CBC_MAC) && (PSA_WANT_ALG_CBC_MAC == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_CBC_MAC (not supported)
#endif
#if defined(PSA_WANT_ALG_CBC_NO_PADDING) && (PSA_WANT_ALG_CBC_NO_PADDING == 1)
#define MBEDTLS_PSA_ACCEL_ALG_CBC_NO_PADDING
#endif
#if defined(PSA_WANT_ALG_CBC_PKCS7) && (PSA_WANT_ALG_CBC_PKCS7 == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_CBC_PKCS7 (not supported)
#endif
#if defined(PSA_WANT_ALG_CCM) && (PSA_WANT_ALG_CCM == 1)
#define MBEDTLS_PSA_ACCEL_ALG_CCM
#endif
#if defined(PSA_WANT_ALG_CMAC) && (PSA_WANT_ALG_CMAC == 1)
#define MBEDTLS_PSA_ACCEL_ALG_CMAC
#endif
#if defined(PSA_WANT_ALG_CFB) && (PSA_WANT_ALG_CFB == 1)
#define MBEDTLS_PSA_ACCEL_ALG_CFB
#endif
#if defined(PSA_WANT_ALG_CHACHA20_POLY1305) && (PSA_WANT_ALG_CHACHA20_POLY1305 == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_CHACHA20_POLY1305 (not supported)
#endif
#if defined(PSA_WANT_ALG_CTR) && (PSA_WANT_ALG_CTR == 1)
#define MBEDTLS_PSA_ACCEL_ALG_CTR
#endif
#if defined(PSA_WANT_ALG_DETERMINISTIC_ECDSA) && (PSA_WANT_ALG_DETERMINISTIC_ECDSA == 1)
// #define MBEDTLS_PSA_ACCEL_ALG_DETERMINISTIC_ECDSA (not supported)
#endif
#if defined(PSA_WANT_ALG_ECB_NO_PADDING) && (PSA_WANT_ALG_ECB_NO_PADDING == 1)
#define MBEDTLS_PSA_ACCEL_ALG_ECB_NO_PADDING
#endif
#if defined(PSA_WANT_ALG_ECDH) && (PSA_WANT_ALG_ECDH == 1)
#define MBEDTLS_PSA_ACCEL_ALG_ECDH
#endif
#if defined(PSA_WANT_ALG_ECDSA) && (PSA_WANT_ALG_ECDSA == 1)
#define MBEDTLS_PSA_ACCEL_ALG_ECDSA
#endif
#if defined(PSA_WANT_ALG_GCM) && (PSA_WANT_ALG_GCM == 1)
#define MBEDTLS_PSA_ACCEL_ALG_GCM
#endif
#if defined(PSA_WANT_ALG_HKDF) && (PSA_WANT_ALG_HKDF == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_HKDF (not supported)
#endif
#if defined(PSA_WANT_ALG_HMAC) && (PSA_WANT_ALG_HMAC == 1)
#define MBEDTLS_PSA_ACCEL_ALG_HMAC
#endif
#if defined(PSA_WANT_ALG_MD5) && (PSA_WANT_ALG_MD5 == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_MD5 (not supported)
#endif
#if defined(PSA_WANT_ALG_OFB) && (PSA_WANT_ALG_OFB == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_OFB (not supported)
#endif
/* PBKDF2-HMAC is not yet supported via the PSA API in Mbed TLS.
 * Note: when adding support, also adjust include/mbedtls/config_psa.h */
#if defined(PSA_WANT_ALG_PBKDF2_HMAC) && (PSA_WANT_ALG_PBKDF2_HMAC == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_PBKDF2_HMAC (not supported)
#endif
#if defined(PSA_WANT_ALG_RIPEMD160) && (PSA_WANT_ALG_RIPEMD160 == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_RIPEMD160 (not supported)
#endif
#if defined(PSA_WANT_ALG_RSA_OAEP) && (PSA_WANT_ALG_RSA_OAEP == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_RSA_OAEP (not supported)
#endif
#if defined(PSA_WANT_ALG_RSA_PKCS1V15_CRYPT) && (PSA_WANT_ALG_RSA_PKCS1V15_CRYPT == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_RSA_PKCS1V15_CRYPT (not supported)
#endif
#if defined(PSA_WANT_ALG_RSA_PKCS1V15_SIGN) && (PSA_WANT_ALG_RSA_PKCS1V15_SIGN == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_RSA_PKCS1V15_SIGN (not supported)
#endif
#if defined(PSA_WANT_ALG_RSA_PSS) && (PSA_WANT_ALG_RSA_PSS == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_RSA_PSS (not supported)
#endif
#if defined(PSA_WANT_ALG_SHA_1) && (PSA_WANT_ALG_SHA_1 == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_SHA_1 (not supported)
#endif
#if defined(PSA_WANT_ALG_SHA_224) && (PSA_WANT_ALG_SHA_224 == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_SHA_224 (not supported)
#endif
#if defined(PSA_WANT_ALG_SHA_256) && (PSA_WANT_ALG_SHA_256 == 1)
#define MBEDTLS_PSA_ACCEL_ALG_SHA_256
#endif
#if defined(PSA_WANT_ALG_SHA_384) && (PSA_WANT_ALG_SHA_384 == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_SHA_384 (not supported)
#endif
#if defined(PSA_WANT_ALG_SHA_512) && (PSA_WANT_ALG_SHA_512 == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_SHA_512 (not supported)
#endif
#if defined(PSA_WANT_ALG_STREAM_CIPHER) && (PSA_WANT_ALG_STREAM_CIPHER == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_STREAM_CIPHER (not supported)
#endif
#if defined(PSA_WANT_ALG_TLS12_PRF) && (PSA_WANT_ALG_TLS12_PRF == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_TLS12_PRF (not supported)
#endif
#if defined(PSA_WANT_ALG_TLS12_PSK_TO_MS) && (PSA_WANT_ALG_TLS12_PSK_TO_MS == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_TLS12_PSK_TO_MS (not supported)
#endif
/* PBKDF2-HMAC is not yet supported via the PSA API in Mbed TLS.
 * Note: when adding support, also adjust include/mbedtls/config_psa.h */
#if defined(PSA_WANT_ALG_XTS) && (PSA_WANT_ALG_XTS == 1)
//#define MBEDTLS_PSA_ACCEL_ALG_XTS
#endif

#if defined(PSA_WANT_ECC_BRAINPOOL_P_R1_256) && (PSA_WANT_ECC_BRAINPOOL_P_R1_256 == 1)
//#define MBEDTLS_PSA_ACCEL_ECC_BRAINPOOL_P_R1_256 (not supported)
#endif
#if defined(PSA_WANT_ECC_BRAINPOOL_P_R1_384) && (PSA_WANT_ECC_BRAINPOOL_P_R1_384 == 1)
//#define MBEDTLS_PSA_ACCEL_ECC_BRAINPOOL_P_R1_384 (not supported)
#endif
#if defined(PSA_WANT_ECC_BRAINPOOL_P_R1_512) && (PSA_WANT_ECC_BRAINPOOL_P_R1_512 == 1)
//#define MBEDTLS_PSA_ACCEL_ECC_BRAINPOOL_P_R1_512 (not supported)
#endif
#if defined(PSA_WANT_ECC_MONTGOMERY_255) && (PSA_WANT_ECC_MONTGOMERY_255 == 1)
//#define MBEDTLS_PSA_ACCEL_ECC_MONTGOMERY_255 (not supported)
#endif
#if defined(PSA_WANT_ECC_MONTGOMERY_448) && (PSA_WANT_ECC_MONTGOMERY_448 == 1)
//#define MBEDTLS_PSA_ACCEL_ECC_MONTGOMERY_448 (not supported)
#endif
#if defined(PSA_WANT_ECC_SECP_K1_192) && (PSA_WANT_ECC_SECP_K1_192 == 1)
//#define MBEDTLS_PSA_ACCEL_ECC_SECP_K1_192 (not supported)
#endif
/*
 * SECP224K1 is buggy via the PSA API in Mbed TLS
 * (https://github.com/Mbed-TLS/mbedtls/issues/3541). Thus, do not enable it by
 * default.
 */
#if defined(PSA_WANT_ECC_SECP_K1_224) && (PSA_WANT_ECC_SECP_K1_224 == 1)
//#define MBEDTLS_PSA_ACCEL_ECC_SECP_K1_224 (not supported)
#endif
#if defined(PSA_WANT_ECC_SECP_K1_256) && (PSA_WANT_ECC_SECP_K1_256 == 1)
//#define MBEDTLS_PSA_ACCEL_ECC_SECP_K1_256 (not supported)
#endif
#if defined(PSA_WANT_ECC_SECP_R1_192) && (PSA_WANT_ECC_SECP_R1_192 == 1)
#define MBEDTLS_PSA_ACCEL_ECC_SECP_R1_192
#endif
#if defined(PSA_WANT_ECC_SECP_R1_224) && (PSA_WANT_ECC_SECP_R1_224 == 1)
#define MBEDTLS_PSA_ACCEL_ECC_SECP_R1_224
#endif
#if defined(PSA_WANT_ECC_SECP_R1_256) && (PSA_WANT_ECC_SECP_R1_256 == 1)
#define MBEDTLS_PSA_ACCEL_ECC_SECP_R1_256
#endif
#if defined(PSA_WANT_ECC_SECP_R1_384) && (PSA_WANT_ECC_SECP_R1_384 == 1)
#define MBEDTLS_PSA_ACCEL_ECC_SECP_R1_384
#endif
#if defined(PSA_WANT_ECC_SECP_R1_521) && (PSA_WANT_ECC_SECP_R1_521 == 1)
//#define MBEDTLS_PSA_ACCEL_ECC_SECP_R1_521 (not supported)
#endif

#if defined(PSA_WANT_KEY_TYPE_DERIVE) && (PSA_WANT_KEY_TYPE_DERIVE == 1)
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_DERIVE
#endif
#if defined(PSA_WANT_KEY_TYPE_HMAC) && (PSA_WANT_KEY_TYPE_HMAC == 1)
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_HMAC
#endif
#if defined(PSA_WANT_KEY_TYPE_AES) && (PSA_WANT_KEY_TYPE_AES == 1)
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_AES
#endif
#if defined(PSA_WANT_KEY_TYPE_ARIA) && (PSA_WANT_KEY_TYPE_ARIA == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_ARIA
#endif
#if defined(PSA_WANT_KEY_TYPE_CAMELLIA) && (PSA_WANT_KEY_TYPE_CAMELLIA == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_CAMELLIA
#endif
#if defined(PSA_WANT_KEY_TYPE_CHACHA20) && (PSA_WANT_KEY_TYPE_CHACHA20 == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_CHACHA20
#endif
#if defined(PSA_WANT_KEY_TYPE_DES) && (PSA_WANT_KEY_TYPE_DES == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_DES
#endif
#if defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR) && (PSA_WANT_KEY_TYPE_ECC_KEY_PAIR == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR (Deprecated)
#endif
#if defined(PSA_WANT_KEY_TYPE_ECC_PUBLIC_KEY) && (PSA_WANT_KEY_TYPE_ECC_PUBLIC_KEY == 1)
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_PUBLIC_KEY
#endif
#if defined(PSA_WANT_KEY_TYPE_DH_PUBLIC_KEY) && (PSA_WANT_KEY_TYPE_DH_PUBLIC_KEY == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_PUBLIC_KEY
#endif
#if defined(PSA_WANT_KEY_TYPE_RAW_DATA) && (PSA_WANT_KEY_TYPE_RAW_DATA == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_RAW_DATA
#endif
#if defined(PSA_WANT_KEY_TYPE_RSA_KEY_PAIR) && (PSA_WANT_KEY_TYPE_RSA_KEY_PAIR == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_KEY_PAIR (Deprecated)
#endif
#if defined(PSA_WANT_KEY_TYPE_RSA_PUBLIC_KEY) && (PSA_WANT_KEY_TYPE_RSA_PUBLIC_KEY == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_PUBLIC_KEY
#endif

/*
 * The following symbols extend and deprecate the legacy
 * PSA_WANT_KEY_TYPE_xxx_KEY_PAIR ones. They include the usage of that key in
 * the name's suffix. "_USE" is the most generic and it can be used to describe
 * a generic suport, whereas other ones add more features on top of that and
 * they are more specific.
 */
#if defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_BASIC) && (PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_BASIC == 1)
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_BASIC
#endif
#if defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_IMPORT) && (PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_IMPORT == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_IMPORT (not supported)
#endif
#if defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_EXPORT) && (PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_EXPORT == 1)
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_EXPORT
#endif
#if defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_GENERATE) && (PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_GENERATE == 1)
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_GENERATE
#endif
#if defined(PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_DERIVE) && (PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_DERIVE == 1)
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_ECC_KEY_PAIR_DERIVE
#endif

#if defined(PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_BASIC) && (PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_BASIC == 1)
#define MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_KEY_PAIR_BASIC
#endif
#if defined(PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_IMPORT) && (PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_IMPORT == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_KEY_PAIR_IMPORT
#endif
#if defined(PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_EXPORT) && (PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_EXPORT == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_KEY_PAIR_EXPORT
#endif
#if defined(PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_GENERATE) && (PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_GENERATE == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_KEY_PAIR_GENERATE
#endif
#if defined(PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_DERIVE) && (PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_DERIVE == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_RSA_KEY_PAIR_DERIVE
#endif

#if defined(PSA_WANT_KEY_TYPE_DH_KEY_PAIR_BASIC) && (PSA_WANT_KEY_TYPE_DH_KEY_PAIR_BASIC == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_KEY_PAIR_BASIC
#endif
#if defined(PSA_WANT_KEY_TYPE_DH_KEY_PAIR_IMPORT) && (PSA_WANT_KEY_TYPE_DH_KEY_PAIR_IMPORT == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_KEY_PAIR_IMPORT
#endif
#if defined(PSA_WANT_KEY_TYPE_DH_KEY_PAIR_EXPORT) && (PSA_WANT_KEY_TYPE_DH_KEY_PAIR_EXPORT == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_KEY_PAIR_EXPORT
#endif
#if defined(PSA_WANT_KEY_TYPE_DH_KEY_PAIR_GENERATE) && (PSA_WANT_KEY_TYPE_DH_KEY_PAIR_GENERATE == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_KEY_PAIR_GENERATE
#endif
#if defined(PSA_WANT_KEY_TYPE_DH_KEY_PAIR_DERIVE) && (PSA_WANT_KEY_TYPE_DH_KEY_PAIR_DERIVE == 1)
//#define MBEDTLS_PSA_ACCEL_KEY_TYPE_DH_KEY_PAIR_DERIVE
#endif

#endif /* CRYPTO_HW_CRYPTOLITE_CONFIG_H */
