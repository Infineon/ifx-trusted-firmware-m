#-------------------------------------------------------------------------------
# Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# PDL device category
set(IFX_DEVICE_CATEGORY                     "CAT1B")

#################################### IFX #######################################

# PDL syspm version
set(IFX_PDL_CY_SYSPM_VERSION                "v2")

################################### Platform ###################################

set(PLATFORM_HAS_ISOLATION_L3_SUPPORT       OFF)
set(PLATFORM_HAS_FIRMWARE_UPDATE_SUPPORT    OFF)

################################# Dependencies #################################
set(IFX_CMSIS_LIB_PATH                      "DOWNLOAD"  CACHE PATH      "Path to CMSIS library maintained by Infineon (or DOWNLOAD to fetch automatically)")
set(IFX_CMSIS_LIB_GIT_REMOTE                "https://github.com/Infineon/cmsis.git" CACHE STRING "CMSIS library maintained by Infineon repo URL")
set(IFX_CMSIS_LIB_VERSION                   "release-v5.8.2" CACHE STRING    "The version of Infineon CMSIS library to use")

set(IFX_CORE_LIB_GIT_REMOTE                 "https://github.com/Infineon/core-lib.git" CACHE STRING "Infineon Core library repo URL")
set(IFX_CORE_LIB_VERSION                    "release-v1.4.3"      CACHE STRING    "The version of Infineon Core library to use")

set(IFX_PDL_LIB_PATH                        "DOWNLOAD"        CACHE PATH      "Path to Infineon PDL library (or DOWNLOAD to fetch automatically)")
set(IFX_PDL_LIB_GIT_REMOTE                  "https://github.com/Infineon/mtb-pdl-cat1.git" CACHE STRING "Infineon PDL library repo URL")
set(IFX_PDL_LIB_PATCH_DIR                   "${IFX_PLATFORM_SOURCE_DIR}/libs/ifx_pdl/patch" CACHE STRING "Path to ifx-pdl patches")
set(IFX_PDL_LIB_VERSION                     "release-v3.16.0" CACHE STRING    "The version of Infineon PDL library to use")
