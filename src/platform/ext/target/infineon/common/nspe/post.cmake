#-------------------------------------------------------------------------------
# Copyright (c) 2023-2024 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Include tests on post step of tf-m-tests suite
if(TFM_NS_REG_TEST)
    add_subdirectory(${IFX_COMMON_SOURCE_DIR}/tests tests)
endif()
