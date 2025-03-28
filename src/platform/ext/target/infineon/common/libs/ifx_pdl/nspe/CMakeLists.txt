#-------------------------------------------------------------------------------
# Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)

################################ IFX_PDL_LIB_NS ################################

# There are two libraries for non-secure PDL. This is needed to resolve circular dependency between
# BSP and PDL.
# - ifx_pdl_inc_ns is used to provide includes and compile definitions for secure PDL.
# - ifx_pdl_ns is used to build PDL library for TF-M.
add_library(ifx_pdl_inc_ns INTERFACE)
add_library(ifx_pdl_ns STATIC EXCLUDE_FROM_ALL)

target_sources(ifx_pdl_ns
    PUBLIC
    PRIVATE
        $<$<C_COMPILER_ID:ARMClang>:${IFX_PDL_LIB_PATH}/drivers/source/TOOLCHAIN_ARM/cy_syslib_ext.s>
        $<$<C_COMPILER_ID:GNU>:${IFX_PDL_LIB_PATH}/drivers/source/TOOLCHAIN_GCC_ARM/cy_syslib_ext.S>
        $<$<C_COMPILER_ID:IAR>:${IFX_PDL_LIB_PATH}/drivers/source/TOOLCHAIN_IAR/cy_syslib_ext.s>
        ${IFX_PDL_LIB_PATH}/devices/COMPONENT_${IFX_DEVICE_CATEGORY}/source/cy_device.c
        ${IFX_PDL_LIB_PATH}/drivers/source/cy_gpio.c
        ${IFX_PDL_LIB_PATH}/drivers/source/cy_ipc_drv.c
        ${IFX_PDL_LIB_PATH}/drivers/source/cy_scb_common.c
        ${IFX_PDL_LIB_PATH}/drivers/source/cy_scb_uart.c
        ${IFX_PDL_LIB_PATH}/drivers/source/cy_sysclk_v2.c
        ${IFX_PDL_LIB_PATH}/drivers/source/cy_syslib.c
        ${IFX_PDL_LIB_PATH}/drivers/source/cy_syspm_btss.c
        ${IFX_PDL_LIB_PATH}/drivers/source/cy_syspm_pdcm.c
        ${IFX_PDL_LIB_PATH}/drivers/source/cy_syspm_ppu.c
        ${IFX_PDL_LIB_PATH}/drivers/source/cy_syspm_v2.c
        ${IFX_PDL_LIB_PATH}/drivers/source/cy_tcpwm_counter.c
        ${IFX_PDL_LIB_PATH}/drivers/source/cy_wdt.c
        ${IFX_PDL_LIB_PATH}/drivers/source/ppu_v1.c
)

target_link_libraries(ifx_pdl_ns
    PUBLIC
        ifx_pdl_inc_ns
)

target_include_directories(ifx_pdl_inc_ns
    INTERFACE
        ${IFX_PDL_LIB_PATH}/devices/COMPONENT_${IFX_DEVICE_CATEGORY}/include
        ${IFX_PDL_LIB_PATH}/devices/COMPONENT_${IFX_DEVICE_CATEGORY}/include/ip
        ${IFX_PDL_LIB_PATH}/drivers/include
)

target_compile_definitions(ifx_pdl_inc_ns
    INTERFACE
        ${IFX_PDL_PART_NUMBER}
        $<$<STREQUAL:${IFX_CORE},${IFX_CM33}>:COMPONENT_CM33>
        $<$<STREQUAL:${IFX_CORE},${IFX_CM33}>:CORE_NAME_CM33_0>
        $<$<STREQUAL:${IFX_CORE},${IFX_CM55}>:COMPONENT_CM55>
        $<$<STREQUAL:${IFX_CORE},${IFX_CM55}>:CORE_NAME_CM55_0>
)

target_compile_options(ifx_pdl_inc_ns
    INTERFACE
        $<$<AND:$<C_COMPILER_ID:ARMClang>,$<COMPILE_LANGUAGE:C>>:-Wno-documentation>
)

target_link_libraries(ifx_pdl_inc_ns
    INTERFACE
        # Use CMSIS library maintained by Infineon for NS image
        ifx_cmsis_lib
        ifx_core_lib
        tfm_config
)
