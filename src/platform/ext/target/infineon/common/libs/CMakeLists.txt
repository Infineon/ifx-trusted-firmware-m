#-------------------------------------------------------------------------------
# Copyright (c) 2023-2024 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/cmsis)
    add_subdirectory(cmsis)
endif()

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/core-lib)
    add_subdirectory(core-lib)
endif()

if(DEFINED IFX_DEV_SUPPORT_LIB_PATH AND EXISTS ${CMAKE_CURRENT_LIST_DIR}/ifx_dev_support)
    add_subdirectory(ifx_dev_support)
endif()

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/ifx_pdl)
    add_subdirectory(ifx_pdl)
endif()

if (IFX_PDL_SECURE_SERVICES)
    add_subdirectory(ifx_pdl_rpc)
endif()

# Device DB is only used for sources generation and sources generation is only
# done in non-MTB builds thus Device DB is only needed for non-MTB builds
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/ifx_device_db AND NOT IFX_MTB_BUILD)
    add_subdirectory(ifx_device_db)
endif()

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/ifx_gcov_lib)
    add_subdirectory(ifx_gcov_lib)
endif()

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/ifx_se_rt_services_utils AND S_BUILD AND IFX_SE_RT_SERVICES_UTILS_ENABLED)
    # SE RT Services is used by TF-M secure code only
    add_subdirectory(ifx_se_rt_services_utils)
endif()

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/ifx_mbedtls_acceleration AND TFM_PARTITION_CRYPTO AND IFX_MBEDTLS_ACCELERATION_ENABLED AND IFX_MBEDTLS_ACCELERATOR_TYPE)
    add_subdirectory(ifx_mbedtls_acceleration)
endif()
