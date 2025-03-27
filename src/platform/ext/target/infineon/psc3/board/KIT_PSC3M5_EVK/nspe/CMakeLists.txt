#-------------------------------------------------------------------------------
# Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)
# Set CMP0118: The GENERATED source file property is now visible in all
# directories for all targets created in this file
cmake_policy(SET CMP0118 NEW)

###################### BSP library for Non-Secure image ########################

target_compile_definitions(ifx_pdl_inc_ns
    INTERFACE
        CORE_NAME_CM33_0
)

################################################################################

# Add startup file for NSPE
target_sources(tfm_ns
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../shared/device/source/startup_psc3.c
)

# Add config file definition for linker script.
# Note that target_add_scatter_file creates ${target}_scatter CMake target
# which is used to build scatter file, but this target is not available for platform.
# So, tfm_config is used instead which is linked with ${target}_scatter target.
target_compile_definitions(tfm_config
    INTERFACE
        IFX_TARGET_LINKER_CONFIG_HEADER="region_defs.h"
)

target_include_directories(tfm_config
    INTERFACE
        ${IFX_PLATFORM_SOURCE_DIR}/shared/partition
)
