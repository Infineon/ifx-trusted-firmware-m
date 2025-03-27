#-------------------------------------------------------------------------------
# Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(IFX_PDL_PART_NUMBER                     "PSC3M5FDS2AFQ1"                CACHE STRING    "Part number, see PDL for more details")

set(IFX_BSP_LIB_PATH                        "DOWNLOAD"                      CACHE PATH      "Path to target BSP library (or DOWNLOAD to fetch automatically)")
set(IFX_BSP_LIB_GIT_REMOTE                  "https://github.com/Infineon/target_kit_psc3m5_evk.git" CACHE STRING "Target BSP library repo URL")
set(IFX_BSP_LIB_VERSION                     "release-v2.0.1"                CACHE STRING    "The version of target BSP library to use")

set(IFX_DEVICE_DB_LIB_PATH                  "DOWNLOAD"                      CACHE PATH      "Path to target Device DB library (or DOWNLOAD to fetch automatically)")
set(IFX_DEVICE_DB_LIB_GIT_REMOTE            "https://github.com/Infineon/device-db.git" CACHE STRING "Target Device DB library repo URL")
set(IFX_DEVICE_DB_LIB_VERSION               "release-v4.21.0"                   CACHE STRING    "The version of target Device DB library to use")

set(IFX_BSP_DESIGN_FILE_PATH        "${CMAKE_CURRENT_LIST_DIR}/shared/design.modus"  CACHE FILEPATH "Path to design.modus file to use for BSP code generation by Device Configurator.")
