#-------------------------------------------------------------------------------
# Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0118 NEW)

######################### BSP library for Secure image #########################

target_compile_definitions(ifx_pdl_inc_s
    INTERFACE
        CYBSP_SKIP_PM_REGISTRATION # Power Management is not supported in TFM for SPC3
)

##################### TFM library with shared R/O data #########################

target_sources(${IFX_SHARED_RO_DATA_TARGET}
    PRIVATE
        ${IFX_BSP_LIB_PATH}/system_psc3.c
)

###################### TFM libraries for Secure image ##########################

target_sources(platform_s
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/source/ifx_spm_init.c
)

# Add startup file for SPE
target_sources(tfm_s
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../shared/device/source/startup_psc3.c
)

#################################### tfm_spm ###################################

target_sources(tfm_spm
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/source/protection_regions_cfg.c
)
