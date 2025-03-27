#-------------------------------------------------------------------------------
# Copyright (c) 2023-2024 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# secure image
add_compile_definitions(DOMAIN_S=1)
add_compile_definitions(COMPONENT_SECURE_DEVICE)

set(S_BUILD ON)

set(IFX_PLATFORM_SOURCE_DIR                 ${CMAKE_CURRENT_LIST_DIR})
set(IFX_COMMON_SOURCE_DIR                   ${CMAKE_CURRENT_LIST_DIR}/../common)

include(${IFX_PLATFORM_SOURCE_DIR}/shared/cpuarch.cmake)
