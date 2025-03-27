#-------------------------------------------------------------------------------
# Copyright (c) 2023-2024 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

include(${IFX_COMMON_SOURCE_DIR}/install.cmake)

if (IFX_MTB_BUILD)
    install(FILES       ${IFX_COMMON_SOURCE_DIR}/mtb/config/PSC3.mk
            DESTINATION ${CMAKE_INSTALL_PREFIX}/config)
endif()

install(FILES       ${IFX_PLATFORM_SOURCE_DIR}/nspe/CMakeLists.txt
                    ${IFX_PLATFORM_SOURCE_DIR}/nspe/cpuarch.cmake
        DESTINATION ${INSTALL_PLATFORM_NS_DIR})

configure_file(${IFX_PLATFORM_SOURCE_DIR}/nspe/config.cmake.in
               ${INSTALL_PLATFORM_NS_DIR}/config.cmake @ONLY)

install(DIRECTORY   ${IFX_PLATFORM_SOURCE_DIR}/shared
        DESTINATION ${INSTALL_PLATFORM_NS_DIR})

if (NOT PLATFORM_DEFAULT_CRYPTO_KEYS)
    # Platform specific key IDs header
    install(FILES       ${IFX_PLATFORM_SOURCE_DIR}/spe/services/crypto/tfm_builtin_key_ids.h
            DESTINATION ${INSTALL_INTERFACE_INC_DIR})
endif()
