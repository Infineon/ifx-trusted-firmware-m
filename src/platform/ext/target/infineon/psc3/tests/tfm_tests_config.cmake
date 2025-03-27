#-------------------------------------------------------------------------------
# Copyright (c) 2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(PLATFORM_SLIH_IRQ_TEST_SUPPORT          ON)
set(PLATFORM_FLIH_IRQ_TEST_SUPPORT          ON)

# Disable TFM_CRYPTO_TEST_ECDH as PSA_WANT_ALG_ECDH is not enabled on PSC3
set(TFM_CRYPTO_TEST_ECDH                    OFF     CACHE BOOL      "Test the ECDH key agreement algorithm")

# Disable TFM_CRYPTO_TEST_TLS as PSA_WANT_ALG_TLS12_PRF is not enabled on PSC3
set(TFM_CRYPTO_TEST_TLS                     OFF     CACHE BOOL      "Test the TLS key derivation algorithm")

# Disable TFM_CRYPTO_TEST_HKDF as PSA_WANT_ALG_HKDF is not enabled on PSC3
set(TFM_CRYPTO_TEST_HMAC                    OFF     CACHE BOOL      "Test the HMAC key type and algorithm")

# Disable TFM_CRYPTO_TEST_ALG_CCM as CRYPTO_AEAD_MODULE_ENABLED is not enabled on PSC3
set(TFM_CRYPTO_TEST_ALG_CCM                 OFF     CACHE BOOL      "Test CCM cryptography mode")
