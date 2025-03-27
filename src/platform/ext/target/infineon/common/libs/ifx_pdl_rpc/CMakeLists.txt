#-------------------------------------------------------------------------------
# Copyright (c) 2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

fetch_remote_library(
    LIB_NAME                ifx_pdl_rpc
    LIB_SOURCE_PATH_VAR     IFX_PDL_RPC_LIB_PATH
    # By using ${IFX_LIB_BASE_DIR} as LIB_BASE_DIR repo will only be downloaded once
    # and then reused by subsequent builds. This speeds up the build.
    LIB_BASE_DIR            "${IFX_LIB_BASE_DIR}"
    LIB_PATCH_DIR           ${IFX_PDL_RPC_LIB_PATCH_DIR}
    LIB_FORCE_PATCH         ON
    FETCH_CONTENT_ARGS
        GIT_REPOSITORY      ${IFX_PDL_RPC_LIB_GIT_REMOTE}
        GIT_TAG             ${IFX_PDL_RPC_LIB_VERSION}
        GIT_SHALLOW         TRUE
        GIT_PROGRESS        TRUE
        GIT_SUBMODULES      ""
)

if (S_BUILD)
    target_sources(ifx_pdl_s
        PRIVATE
            ${IFX_PDL_RPC_LIB_PATH}/source/cy_secure_services.c
    )

    target_include_directories(ifx_pdl_inc_s
        INTERFACE
            ${IFX_PDL_RPC_LIB_PATH}/include
    )

    target_compile_definitions(ifx_pdl_inc_s
        INTERFACE
            CY_PDL_CUSTOM_RPC=0
            CY_OMIT_PDL_PPC_SERVICES=1
    )
endif()

if (NS_BUILD)
    target_include_directories(ifx_pdl_inc_ns
        INTERFACE
            ${IFX_PDL_RPC_LIB_PATH}/include
    )

    target_compile_definitions(ifx_pdl_inc_ns
        INTERFACE
            CY_USE_RPC_CALL=1
            CY_USE_FULL_PROTECTION=1
    )

    add_library(ifx_pdl_srv)

    target_sources(ifx_pdl_srv
        PRIVATE
            ${INTERFACE_SRC_DIR}/ifx_secure_services.c
    )

    target_link_libraries(ifx_pdl_srv
        PRIVATE
            ifx_pdl_inc_ns
            psa_interface
    )

    target_link_libraries(ifx_pdl_ns
        PRIVATE
            ifx_pdl_srv
    )
endif()
