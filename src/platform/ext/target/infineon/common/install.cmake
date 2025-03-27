#-------------------------------------------------------------------------------
# Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

install(FILES       ${PLATFORM_DIR}/ext/common/uart_stdout.h
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ext/common)

install(DIRECTORY   ${CMAKE_CURRENT_LIST_DIR}/device
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx
        PATTERN     src/armclang EXCLUDE
        PATTERN     src/gcc EXCLUDE
        PATTERN     src/iar EXCLUDE)

# Non-secure build uses platform/linker_scripts folder to lookup for linker script.
install(FILES       ${IFX_COMMON_SOURCE_DIR}/device/src/armclang/ifx_common_ns.sct
                    ${IFX_COMMON_SOURCE_DIR}/device/src/gcc/ifx_common_ns.ld
                    ${IFX_COMMON_SOURCE_DIR}/device/src/iar/ifx_common_ns.icf
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/linker_scripts)

install(DIRECTORY   ${CMAKE_CURRENT_LIST_DIR}/drivers/stdio
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx/drivers)

install(DIRECTORY   ${CMAKE_CURRENT_LIST_DIR}/cmake
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx)

install(DIRECTORY   ${CMAKE_CURRENT_LIST_DIR}/libs
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx
        PATTERN     libs/ifx_gcov_lib EXCLUDE
        PATTERN     libs/ifx_mbedtls_acceleration EXCLUDE
        PATTERN     libs/ifx_se_rt_services_utils EXCLUDE
        PATTERN     spe EXCLUDE)

install(FILES       ${CMAKE_CURRENT_LIST_DIR}/nspe/CMakeLists.txt
                    ${CMAKE_CURRENT_LIST_DIR}/nspe/config.cmake
                    ${CMAKE_CURRENT_LIST_DIR}/post_config.cmake
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx)

# post.cmake is used to add sources to tf-m-tests targets
install(FILES       ${IFX_COMMON_SOURCE_DIR}/nspe/post.cmake
        DESTINATION ${INSTALL_PLATFORM_NS_DIR})

configure_file(${IFX_COMMON_SOURCE_DIR}/nspe/spe_config.cmake.in
               ${INSTALL_PLATFORM_NS_DIR}/ifx/spe_config.cmake @ONLY)

# IMPROVEMENT: Remove OS wrapper when MIDDLEWARE-10689 and TFM-3946 will be resolved
install(FILES       ${CMAKE_CURRENT_LIST_DIR}/nspe/os_wrapper/semaphore.h
                    ${CMAKE_CURRENT_LIST_DIR}/nspe/os_wrapper/thread.h
        DESTINATION ${INSTALL_INTERFACE_INC_DIR}/os_wrapper)

install(FILES       ${CMAKE_CURRENT_LIST_DIR}/nspe/os_wrapper/os_wrapper_cyabs_rtos.c
        DESTINATION ${INSTALL_INTERFACE_SRC_DIR}/os_wrapper)

install(DIRECTORY   ${CMAKE_CURRENT_LIST_DIR}/shared
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx
        PATTERN     mailbox EXCLUDE)

install(DIRECTORY   ${CMAKE_CURRENT_LIST_DIR}/utils
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx)

#################################### Board #####################################

install(FILES       ${IFX_COMMON_SOURCE_DIR}/board/CMakeLists.txt
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx/board)

install(DIRECTORY   ${IFX_COMMON_SOURCE_DIR}/board/nspe
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx/board)

install(FILES       ${IFX_CONFIG_BSP_PATH}/config_bsp.h
                    ${IFX_CONFIG_BSP_PATH}/config.cmake
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/board)

install(DIRECTORY   ${IFX_BOARD_PATH}/nspe
                    ${IFX_BOARD_PATH}/shared
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/board)

################################## Partitions ##################################

if (TFM_PARTITION_CRYPTO)
    # Install Crypto configuration for MTB non-secure interface
    set(TFM_MBEDCRYPTO_CONFIG_CLIENT_PATH ${TFM_MBEDCRYPTO_CONFIG_PATH})
    cmake_path(REMOVE_EXTENSION TFM_MBEDCRYPTO_CONFIG_CLIENT_PATH)
    cmake_path(APPEND_STRING TFM_MBEDCRYPTO_CONFIG_CLIENT_PATH "_client.h")
    install(FILES       ${TFM_MBEDCRYPTO_CONFIG_CLIENT_PATH}
            RENAME      tfm_mbedcrypto_config_client.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})
    install(FILES       ${TFM_MBEDCRYPTO_PSA_CRYPTO_CONFIG_PATH}
            RENAME      tfm_psa_crypto_config_client.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})
    if (MBEDTLS_PSA_CRYPTO_PLATFORM_FILE)
        install(FILES       ${MBEDTLS_PSA_CRYPTO_PLATFORM_FILE}
                RENAME      tfm_mbedtls_psa_crypto_platform.h
                DESTINATION ${INSTALL_INTERFACE_INC_DIR})
    endif()
endif()

if (TFM_PARTITION_NS_AGENT_MAILBOX)
    install(FILES       ${CMAKE_CURRENT_LIST_DIR}/shared/mailbox/platform_multicore.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR}/multi_core)

    install(FILES       ${CMAKE_CURRENT_LIST_DIR}/nspe/mailbox/platform_ns_mailbox.c
                        ${CMAKE_CURRENT_LIST_DIR}/nspe/mailbox/tfm_ns_mailbox_rtos_api.c
                        ${CMAKE_CURRENT_LIST_DIR}/shared/mailbox/platform_multicore.c
            DESTINATION ${INSTALL_INTERFACE_SRC_DIR}/multi_core)
endif()

if (TFM_PARTITION_PLATFORM)
    # Platform service headers
    install(FILES       ${CMAKE_CURRENT_LIST_DIR}/interface/include/ifx_platform_api.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})

    # Platform service sources
    install(FILES       ${CMAKE_CURRENT_LIST_DIR}/interface/src/ifx_platform_private.h
                        ${CMAKE_CURRENT_LIST_DIR}/interface/src/ifx_platform_api.c
                        $<$<BOOL:${IFX_PDL_SECURE_SERVICES}>:${CMAKE_CURRENT_LIST_DIR}/interface/src/ifx_secure_services.c>
            DESTINATION ${INSTALL_INTERFACE_SRC_DIR})
endif()

#################################### Tests #####################################

if (TFM_NS_REG_TEST OR TEST_PSA_API)
    configure_file(${IFX_COMMON_SOURCE_DIR}/tests/non_secure/spe_config.cmake.in
                   ${INSTALL_PLATFORM_NS_DIR}/ifx/tests/spe_config.cmake @ONLY)

    configure_file(${IFX_COMMON_SOURCE_DIR}/tests/non_secure/ifx_tests_spe_config.h.in
                   ${INSTALL_PLATFORM_NS_DIR}/ifx/tests/ifx_tests_spe_config.h @ONLY)

    install(FILES       $<$<OR:$<BOOL:${TFM_NS_REG_TEST}>,$<BOOL:${TEST_PSA_API}>>:${CMAKE_CURRENT_LIST_DIR}/nspe/test/plat_test_ns.c>
                        ${IFX_COMMON_SOURCE_DIR}/tests/non_secure/CMakeLists.txt
            DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx/tests)

    install(FILES       ${IFX_PLATFORM_SOURCE_DIR}/tests/ifx_tests_ns_config.cmake
                        ${IFX_PLATFORM_SOURCE_DIR}/tests/ifx_platform_tests_config.h
                        ${IFX_PLATFORM_SOURCE_DIR}/tests/psa_arch_tests_config.cmake
                        ${IFX_PLATFORM_SOURCE_DIR}/tests/tfm_tests_config.cmake
            DESTINATION ${INSTALL_PLATFORM_NS_DIR}/tests
            OPTIONAL)
endif()

if (TEST_NS_FPU)
    install(FILES   ${CMAKE_CURRENT_LIST_DIR}/nspe/test/ifx_fpu_ns.c
                    ${CMAKE_SOURCE_DIR}/platform/ext/common/test_interrupt.c
                    ${CMAKE_SOURCE_DIR}/platform/ext/common/test_interrupt.h
            DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx/tests)
endif()

if (TEST_NS_IFX_ISOLATION_TEST OR TEST_NS_IFX_IRQ_TEST_MASKED OR TEST_NS_IFX_IRQ_TEST_UNMASKED)
    install(FILES       ${IFX_COMMON_SOURCE_DIR}/tests/services/common/ifx_test_common.h
                        ${IFX_COMMON_SOURCE_DIR}/tests/services/common/ifx_test_common.c
            DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx/tests/extra_ns
    )
endif()

if (TEST_NS_IFX_ISOLATION_TEST)
    install(FILES       ${IFX_PLATFORM_SOURCE_DIR}/tests/ifx_isolation_test_data.c
            DESTINATION ${INSTALL_PLATFORM_NS_DIR}/tests
    )
endif()

if (TEST_NS_IFX_FRAMEWORK OR TEST_NS_IFX_CODE_COVERAGE)
    install(DIRECTORY   ${IFX_COMMON_SOURCE_DIR}/tests/extra_ns
            DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx/tests)
endif()

if (IFX_TEST_PARTITION_AROT_MASTER)
    install(FILES       ${IFX_COMMON_SOURCE_DIR}/tests/services/arot_master/ifx_test_arot_master_api.h
                        ${IFX_COMMON_SOURCE_DIR}/tests/services/arot_master/ifx_test_arot_master_api.c
            DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx/tests/extra_ns
    )
endif()

if (IFX_TEST_PARTITION_AROT_SLAVE)
    install(FILES       ${IFX_COMMON_SOURCE_DIR}/tests/services/arot_slave/ifx_test_arot_slave_api.h
                        ${IFX_COMMON_SOURCE_DIR}/tests/services/arot_slave/ifx_test_arot_slave_api.c
            DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx/tests/extra_ns
    )
endif()

if (IFX_TEST_PARTITION_PROT_MASTER)
    install(FILES       ${IFX_COMMON_SOURCE_DIR}/tests/services/prot_master/ifx_test_prot_master_api.h
                        ${IFX_COMMON_SOURCE_DIR}/tests/services/prot_master/ifx_test_prot_master_api.c
            DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx/tests/extra_ns
    )
endif()

if (IFX_TEST_PARTITION_PROT_SLAVE)
    install(FILES       ${IFX_COMMON_SOURCE_DIR}/tests/services/prot_slave/ifx_test_prot_slave_api.h
                        ${IFX_COMMON_SOURCE_DIR}/tests/services/prot_slave/ifx_test_prot_slave_api.c
            DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx/tests/extra_ns
    )
endif()

if (IFX_TEST_SERVICE_PARTITION)
    install(FILES       ${IFX_COMMON_SOURCE_DIR}/tests/services/ifx_test_service/ifx_test_service_api.h
                        ${IFX_COMMON_SOURCE_DIR}/tests/services/ifx_test_service/ifx_test_service_api.c
            DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx/tests/extra_ns
    )
endif()

if (IFX_TEST_SE_KEY_LOCATION AND TEST_NS_CRYPTO)
    install(FILES       ${IFX_GENERATED_DIR}/test/secure_fw/suites/crypto/crypto_tests_common.c
                        ${IFX_GENERATED_DIR}/test/secure_fw/suites/crypto/non_secure/crypto_ns_interface_testsuite.c
            DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx/tests
    )
endif()

if (IFX_TEST_SE_KEY_LOCATION AND (TEST_PSA_API STREQUAL "CRYPTO"))
    install(FILES       ${IFX_GENERATED_DIR}/platform/targets/common/nspe/crypto/pal_crypto_intf.c
            DESTINATION ${INSTALL_PLATFORM_NS_DIR}/ifx/tests
    )
endif()

# These files are used by tf-m-tests
install(FILES       ${PLATFORM_DIR}/ext/driver/Driver_Common.h
                    ${PLATFORM_DIR}/ext/driver/Driver_USART.h
        DESTINATION ${INSTALL_PLATFORM_NS_DIR}/include)
