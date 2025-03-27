#-------------------------------------------------------------------------------
# Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

############################# Platform Non-Secure #############################

add_library(platform_ns)

target_include_directories(platform_ns
    PRIVATE
        ${IFX_COMMON_SOURCE_DIR}/interface/src
    PUBLIC
        ${IFX_COMMON_SOURCE_DIR}/device/include
        $<$<BOOL:${TEST_NS_FPU}>:${IFX_COMMON_SOURCE_DIR}/tests>
)

target_sources(platform_ns
    PRIVATE
        $<$<OR:$<BOOL:${TFM_NS_REG_TEST}>,$<BOOL:${TEST_PSA_API}>>:${IFX_COMMON_SOURCE_DIR}/tests/plat_test_ns.c>

        ${IFX_COMMON_SOURCE_DIR}/device/src/device_definition.c
        $<$<BOOL:${IFX_UART_ENABLED}>:${IFX_COMMON_SOURCE_DIR}/drivers/stdio/uart_pdl_stdout.c>
        $<$<BOOL:${TFM_PARTITION_PLATFORM}>:${INTERFACE_SRC_DIR}/ifx_platform_api.c>
        $<$<BOOL:${IFX_NS_INTERFACE_MAILBOX}>:${INTERFACE_SRC_DIR}/multi_core/platform_ns_mailbox.c>
        $<$<BOOL:${IFX_NS_INTERFACE_MAILBOX}>:${INTERFACE_SRC_DIR}/multi_core/platform_multicore.c>

        $<$<BOOL:${TEST_NS_FPU}>:${IFX_COMMON_SOURCE_DIR}/tests/ifx_fpu_ns.c>
        $<$<BOOL:${TEST_NS_FPU}>:${IFX_COMMON_SOURCE_DIR}/tests/test_interrupt.c>
)

target_compile_definitions(platform_ns
    PUBLIC
        $<$<BOOL:${IFX_UART_ENABLED}>:IFX_UART_ENABLED=1>
        $<$<BOOL:${TEST_NS_FPU}>:TEST_NS_FPU=1>
)

target_link_libraries(platform_ns
    PUBLIC
        ifx_pdl_ns
        psa_interface
)

target_compile_definitions(tfm_config
    INTERFACE
        ${IFX_PDL_PART_NUMBER}
)

target_include_directories(tfm_config
    INTERFACE
        ${IFX_COMMON_SOURCE_DIR}/utils
)
