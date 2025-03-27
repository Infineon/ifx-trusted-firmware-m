#-------------------------------------------------------------------------------
# Copyright (c) 2024 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.27.7)

include(remote_library)

cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)

# Vendor's common build of non-secure image
add_subdirectory(${IFX_COMMON_SOURCE_DIR} ifx)

# Common libraries
add_subdirectory(${IFX_COMMON_SOURCE_DIR}/libs libs)

# Board
add_subdirectory(${IFX_COMMON_SOURCE_DIR}/board board)

############################## Platform Non-Secure #############################

target_include_directories(platform_ns
    PUBLIC
        ${IFX_PLATFORM_SOURCE_DIR}/ext/common
        ${IFX_PLATFORM_SOURCE_DIR}/include
        ${IFX_PLATFORM_SOURCE_DIR}/shared
        ${IFX_PLATFORM_SOURCE_DIR}/shared/device/include
        ${IFX_PLATFORM_SOURCE_DIR}/shared/device/config
)

target_link_libraries(platform_ns
    PUBLIC
        ifx_cmsis_lib
)
