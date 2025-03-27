#-------------------------------------------------------------------------------
# Copyright (c) 2023-2024 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.27.7)

# Library with shared read-only data, only SPM/PRoT can write it
set(IFX_SHARED_RO_DATA_TARGET "ifx_tfm_sp_meta")
add_library(${IFX_SHARED_RO_DATA_TARGET} STATIC)

add_subdirectory(${IFX_PLATFORM_SOURCE_DIR}/libs libs)

add_subdirectory(${IFX_COMMON_SOURCE_DIR}/board board)

############################# Platform region defs #############################
target_include_directories(platform_region_defs
    INTERFACE
        shared/partition
)

############################## TF-M configuration ##############################
target_include_directories(tfm_config
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/config>
)

########################## Platform Common Interface ###########################
target_compile_definitions(tfm_config
    INTERFACE
        IFX_BSP_CONFIG_HEADER_FILE="${IFX_CONFIG_BSP_PATH}/config_bsp.h"
        IFX_PROJECT_MEMORY_LAYOUT_FILE="${IFX_BSP_DESIGN_FILE_NAME}_project_memory_layout.h"
)

############################### Platform Secure ###############################
add_subdirectory(spe)

if(TFM_PARTITION_CRYPTO)
    target_include_directories(platform_crypto_keys
        PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/spe/services/crypto
    )

    target_include_directories(psa_interface
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/spe/services/crypto>
    )
endif()

################################################################################

include(./install.cmake)
