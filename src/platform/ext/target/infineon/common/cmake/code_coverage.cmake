#-------------------------------------------------------------------------------
# Copyright (c) 2023-2024 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

include(${IFX_COMMON_SOURCE_DIR}/cmake/utils.cmake)

# Adds code coverage flags to all infineon files that are built, excluding EXCLUDE
#
# EXCLUDE - list of regex match expressions to exclude, see string (REGEX MATCH)
function(ifx_add_code_coverage EXCLUDE)
    # Get all build targets
    ifx_get_all_targets(ifx_all_targets)

    # Create a list of directories where targets ware created
    foreach(target ${ifx_all_targets})
        get_target_property(target_source_dir ${target} SOURCE_DIR)
        list(APPEND ifx_all_target_source_directories ${target_source_dir})
    endforeach()

    # Get a list of all files relative to target ()
    file(READ ${IFX_COMMON_SOURCE_DIR}/deploy/tfm_files_to_deploy.txt IFX_FILES_LIST)

    # Convert string to a list
    string(REPLACE "\n" ";" ifx_target_sources ${IFX_FILES_LIST})

    # Include only infineon files
    list(FILTER ifx_target_sources INCLUDE REGEX "platform/ext/target/infineon")

    # Exclude test (we measure coverage of the secure code, not the code that tests it)
    list(FILTER ifx_target_sources EXCLUDE REGEX "platform/ext/target/infineon/common/tests")
    list(FILTER ifx_target_sources EXCLUDE REGEX "platform/ext/target/infineon/pse84/common/tests")

    # Exclude coverage library itself
    list(FILTER ifx_target_sources EXCLUDE REGEX "platform/ext/target/infineon/common/libs/ifx_gcov_lib")

    # Exclude interrupt handler since it requires special handling of enter/exit by exclusion of protection_interrupts.c.
    # This file also has protection context switching assembler function and initialization of H/W PC handler.
    # It's ok to exclude interrupt handler and protection context switching functions because coverage tool
    # doesn't properly handle pure assembler code.
    list(FILTER ifx_target_sources EXCLUDE REGEX "protection_pc.c")

    # Exclude required files
    foreach(exclude_item ${EXCLUDE})
        list(FILTER ifx_target_sources EXCLUDE REGEX ${exclude_item})
    endforeach()

    # Add --coverage flag for each of the files. This will add code coverage
    # code to the specified files.
    foreach(ifx_source_file ${ifx_target_sources})
        set_source_files_properties(${CMAKE_SOURCE_DIR}/${ifx_source_file}
            # Specify PROPERTIES scope (to be sure that PROPERTY is added,
            # PROPERTY must visible at the scope of the directory where target
            # was created). Simplest solution is just to add all of the
            # directories
            DIRECTORY ${ifx_all_target_source_directories}
            PROPERTIES
                COMPILE_FLAGS
                    --coverage
        )
    endforeach()
endfunction()
