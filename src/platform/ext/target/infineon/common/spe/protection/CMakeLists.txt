#-------------------------------------------------------------------------------
# Copyright (c) 2023-2024 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

target_sources(tfm_spm
    PRIVATE
        $<$<BOOL:${IFX_MPC_DRIVER_HW_MPC}>:${CMAKE_CURRENT_LIST_DIR}/protection_mpc_hw_mpc.c>
        $<$<BOOL:${IFX_MPC_DRIVER_SW_POLICY}>:${CMAKE_CURRENT_LIST_DIR}/protection_mpc_sw_policy.c>
        protection_mpu.c
        protection_msc.c
        protection_ppc.c
        protection_sau.c
        protection_tz.c
        $<$<BOOL:${IFX_MPC_DRIVER_HW_MPC}>:${CMAKE_CURRENT_LIST_DIR}/protection_utils.c>
        tfm_hal_isolation.c
        $<$<BOOL:${PLATFORM_ARCH_HOOKS}>:${CMAKE_CURRENT_LIST_DIR}/tfm_platform_arch_hooks.c>
        ${IFX_GENERATED_DIR}/secure_fw/ifx/partition_info.c
)

target_include_directories(tfm_spm
    PRIVATE
        ${IFX_COMMON_SOURCE_DIR}/drivers/protection
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(ns_agent_tz
    PRIVATE
        $<$<BOOL:${IFX_ISOLATION_PC_SWITCHING}>:${CMAKE_CURRENT_LIST_DIR}/protection_pc.c>
)

target_include_directories(tfm_sprt
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(platform_s
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${IFX_SHARED_RO_DATA_TARGET}
    PRIVATE
        protection_shared_data.c
)

target_compile_options(${IFX_SHARED_RO_DATA_TARGET}
    PRIVATE
        ${COMPILER_CP_FLAG}
)
