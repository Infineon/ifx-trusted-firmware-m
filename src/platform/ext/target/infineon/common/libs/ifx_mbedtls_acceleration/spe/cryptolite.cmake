#-------------------------------------------------------------------------------
# Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

target_sources(ifx_mbedtls_acceleration
    PUBLIC
    PRIVATE
        ${IFX_MBEDTLS_ACCELERATION_LIB_PATH}/COMPONENT_CAT1/mbedtls_MXCRYPTOLITE/aes_alt_mxcryptolite.c
        ${IFX_MBEDTLS_ACCELERATION_LIB_PATH}/COMPONENT_CAT1/mbedtls_MXCRYPTOLITE/ecdsa_alt_mxcryptolite.c
        ${IFX_MBEDTLS_ACCELERATION_LIB_PATH}/COMPONENT_CAT1/mbedtls_MXCRYPTOLITE/sha256_alt_mxcryptolite.c
        ${IFX_MBEDTLS_ACCELERATION_LIB_PATH}/COMPONENT_CAT1/mbedtls_MXCRYPTOLITE/trng_alt_mxcryptolite.c
        ${IFX_MBEDTLS_ACCELERATION_LIB_PATH}/COMPONENT_CAT1/mbedtls_psa_MXCryptolite/ifx_cryptolite_common.c
        ${IFX_MBEDTLS_ACCELERATION_LIB_PATH}/COMPONENT_CAT1/mbedtls_psa_MXCryptolite/ifx_cryptolite_transparent_aead.c
        ${IFX_MBEDTLS_ACCELERATION_LIB_PATH}/COMPONENT_CAT1/mbedtls_psa_MXCryptolite/ifx_cryptolite_transparent_cipher.c
        ${IFX_MBEDTLS_ACCELERATION_LIB_PATH}/COMPONENT_CAT1/mbedtls_psa_MXCryptolite/ifx_cryptolite_transparent_hash.c
        ${IFX_MBEDTLS_ACCELERATION_LIB_PATH}/COMPONENT_CAT1/mbedtls_psa_MXCryptolite/ifx_cryptolite_transparent_key_agreement.c
        ${IFX_MBEDTLS_ACCELERATION_LIB_PATH}/COMPONENT_CAT1/mbedtls_psa_MXCryptolite/ifx_cryptolite_transparent_key_generation.c
        ${IFX_MBEDTLS_ACCELERATION_LIB_PATH}/COMPONENT_CAT1/mbedtls_psa_MXCryptolite/ifx_cryptolite_transparent_mac.c
        ${IFX_MBEDTLS_ACCELERATION_LIB_PATH}/COMPONENT_CAT1/mbedtls_psa_MXCryptolite/ifx_cryptolite_transparent_public_key_export.c
        ${IFX_MBEDTLS_ACCELERATION_LIB_PATH}/COMPONENT_CAT1/mbedtls_psa_MXCryptolite/ifx_cryptolite_transparent_sign.c
        ${IFX_MBEDTLS_ACCELERATION_LIB_PATH}/COMPONENT_CAT1/mbedtls_psa_MXCryptolite/ifx_cryptolite_transparent_signature_verify.c
)

target_include_directories(ifx_mbedtls_acceleration
    PUBLIC
        ${IFX_MBEDTLS_ACCELERATION_LIB_PATH}/COMPONENT_CAT1/mbedtls_psa_MXCryptolite
        ${IFX_MBEDTLS_ACCELERATION_LIB_PATH}/COMPONENT_CAT1/mbedtls_MXCRYPTOLITE
        ${MBEDCRYPTO_PATH}/target/infineon/mbedtls
)
