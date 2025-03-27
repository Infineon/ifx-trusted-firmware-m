/*
 * Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef IFX_PDL_CRYPTOLITE_CONFIG_H
#define IFX_PDL_CRYPTOLITE_CONFIG_H

/* Default configuration:
 *
 * - SHA256
 * - HMAC
 * - RSA encrypt/decrypt
 * - RSA PKCS#1.l5 verification
 * - ECP multiplication
 * - ECDSA verification
 */

/*SHA and HMAC*/
#define CY_CRYPTOLITE_CFG_SHA_C
#define CY_CRYPTOLITE_CFG_HMAC_C
#define CY_CRYPTOLITE_CFG_SHA2_256_ENABLED
#define CY_CRYPTOLITE_CFG_HKDF_C

#define CY_CRYPTOLITE_CFG_TRNG_C
#define CY_CRYPTOLITE_CFG_AES_C
#define CY_CRYPTOLITE_CFG_CBC_MAC_C
#define CY_CRYPTOLITE_CFG_CMAC_C

/* AES Cipher modes */
#define CY_CRYPTOLITE_CFG_CIPHER_MODE_CBC
// #define CY_CRYPTOLITE_CFG_CIPHER_MODE_CFB
#define CY_CRYPTOLITE_CFG_CIPHER_MODE_CTR
#define CY_CRYPTOLITE_CFG_CIPHER_MODE_CCM

/* RSA functionality */
// #define CY_CRYPTOLITE_CFG_RSA_C
/* RSA PKCS 1.5 verification */
// #define CY_CRYPTOLITE_CFG_RSA_VERIFY_ENABLED
/* RSA Max Key size */
// #define CY_CRYPTOLITE_CFG_RSA_MAX_KEY_4096
/* RSA verify SHA mode */
// #define CY_CRYPTOLITE_CFG_RSA_VERIFY_SHA1
// #define CY_CRYPTOLITE_CFG_RSA_VERIFY_SHA256
// #define CY_CRYPTOLITE_CFG_RSA_VERIFY_SHA512

/* Only NIST-P curves are currently supported */
#define CY_CRYPTOLITE_CFG_ECP_C

// #define CY_CRYPTOLITE_CFG_ECP_DP_SECP192R1_ENABLED
// #define CY_CRYPTOLITE_CFG_ECP_DP_SECP224R1_ENABLED
#define CY_CRYPTOLITE_CFG_ECP_DP_SECP256R1_ENABLED
// #define CY_CRYPTOLITE_CFG_ECP_DP_SECP384R1_ENABLED
// #define CY_CRYPTOLITE_CFG_ECP_DP_SECP521R1_ENABLED

/* ECDSA functionality */
#define CY_CRYPTOLITE_CFG_ECDSA_C
/* ECDSA Sign */
#define CY_CRYPTOLITE_CFG_ECDSA_SIGN_C
/* ECDSA verification */
#define CY_CRYPTOLITE_CFG_ECDSA_VERIFY_C

/* Use stack memory allocation */
#define IFX_PSA_CRYPTOLITE_USE_STACK_MEM

#endif /* IFX_PDL_CRYPTOLITE_CONFIG_H */
