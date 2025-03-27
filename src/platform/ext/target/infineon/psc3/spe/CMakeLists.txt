#-------------------------------------------------------------------------------
# Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

add_subdirectory(${IFX_COMMON_SOURCE_DIR}/spe common)

target_include_directories(platform_s
    PUBLIC
        ${IFX_COMMON_SOURCE_DIR}/spe/v80m
        ${IFX_PLATFORM_SOURCE_DIR}/spe
        ${IFX_PLATFORM_SOURCE_DIR}/shared
        ${IFX_PLATFORM_SOURCE_DIR}/shared/device/include
        ${IFX_PLATFORM_SOURCE_DIR}/shared/device/config
        ${IFX_PLATFORM_SOURCE_DIR}/config
)

target_sources(platform_s
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/provisioning/ifx_platform_provisioning.c
)

target_compile_definitions(tfm_s_scatter
    PRIVATE
        ${IFX_PDL_PART_NUMBER}
)

################################ tfm_spm #############################################

target_include_directories(tfm_spm
    PUBLIC
        ${IFX_COMMON_SOURCE_DIR}/spe/protection
)

target_sources(tfm_spm
    PRIVATE
        ${IFX_COMMON_SOURCE_DIR}/spe/v80m/target_cfg.c
        ${IFX_COMMON_SOURCE_DIR}/spe/v80m/tfm_hal_platform.c
)

if(TFM_PARTITION_CRYPTO)
    target_include_directories(crypto_service_mbedcrypto_config
        INTERFACE
            ${CMAKE_SOURCE_DIR}/lib/ext/mbedcrypto/mbedcrypto_config
    )

    target_compile_definitions(crypto_service_mbedcrypto_config
        INTERFACE
            $<$<BOOL:${IFX_CRYPTOLITE_USER_CONFIG_FILE}>:CY_CRYPTOLITE_USER_CONFIG_FILE="${IFX_CRYPTOLITE_USER_CONFIG_FILE}">
    )
endif()
