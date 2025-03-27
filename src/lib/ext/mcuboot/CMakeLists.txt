#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
# Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

fetch_remote_library(
    LIB_NAME                mcuboot
    LIB_SOURCE_PATH_VAR     MCUBOOT_PATH
    FETCH_CONTENT_ARGS
        GIT_REPOSITORY      ${MCUBOOT_GIT_REMOTE}
        GIT_TAG             ${MCUBOOT_VERSION}
        GIT_PROGRESS        TRUE
        GIT_SUBMODULES      "${MCUBOOT_SUBMODULES}"
)
