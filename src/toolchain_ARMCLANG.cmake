#-------------------------------------------------------------------------------
# Copyright (c) 2020-2024, Arm Limited. All rights reserved.
# Copyright (c) 2022-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.21)

SET(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER armclang)
set(CMAKE_CXX_COMPILER armclang)
set(CMAKE_ASM_COMPILER armasm)

set(LINKER_VENEER_OUTPUT_FLAG --import_cmse_lib_out=)
set(COMPILER_CMSE_FLAG $<$<COMPILE_LANGUAGE:C>:-mcmse>)

# This variable name is a bit of a misnomer. The file it is set to is included
# at a particular step in the compiler initialisation. It is used here to
# configure the extensions for object files. Despite the name, it also works
# with the Ninja generator.
set(CMAKE_USER_MAKE_RULES_OVERRIDE ${CMAKE_CURRENT_LIST_DIR}/cmake/set_extensions.cmake)

if(NOT DEFINED CMAKE_OBJCOPY)
    set(CMAKE_OBJCOPY ${CROSS_COMPILE}-objcopy CACHE FILEPATH "Path to objcopy")
endif()

macro(tfm_toolchain_reset_compiler_flags)
    set_property(DIRECTORY PROPERTY COMPILE_OPTIONS "")

    add_compile_options(
        $<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:-Wno-ignored-optimization-argument>
        $<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:-Wno-unused-command-line-argument>
        $<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:-Wall>
        # Don't error when the MBEDTLS_NULL_ENTROPY warning is shown
        $<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:-Wno-error=cpp>
        $<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:-c>
        $<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:-fdata-sections>
        $<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:-ffunction-sections>
        $<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:-fno-builtin>
        $<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:-fshort-enums>
        $<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:-fshort-wchar>
        $<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:-funsigned-char>
        $<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:-masm=auto>
        $<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:-nostdlib>
        $<$<COMPILE_LANGUAGE:C>:-std=c99>
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>
        $<$<COMPILE_LANGUAGE:ASM>:--cpu=${CMAKE_ASM_CPU_FLAG}>
        $<$<COMPILE_LANGUAGE:ASM>:--diag_suppress=1950> # Suppress "The legacy armasm assembler is deprecated"
        $<$<AND:$<COMPILE_LANGUAGE:C>,$<BOOL:${TFM_DEBUG_SYMBOLS}>>:-g>
        $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<BOOL:${TFM_DEBUG_SYMBOLS}>>:-g>
        $<$<AND:$<COMPILE_LANGUAGE:C,CXX>,$<BOOL:${CONFIG_TFM_WARNINGS_ARE_ERRORS}>>:-Werror>
    )
endmacro()

if(CONFIG_TFM_MEMORY_USAGE_QUIET)
    set(MEMORY_USAGE_FLAG "")
else()
    set(MEMORY_USAGE_FLAG --info=summarysizes,sizes,totals,unused,veneers)
endif()

macro(tfm_toolchain_reset_linker_flags)
    set_property(DIRECTORY PROPERTY LINK_OPTIONS "")

    add_link_options(
        ${MEMORY_USAGE_FLAG}
        --strict
        --symbols
        --xref
        $<$<AND:$<VERSION_GREATER:${TFM_ISOLATION_LEVEL},1>,$<STREQUAL:"${TEST_PSA_API}","IPC">>:--no-merge>
        # Suppress link warnings that are consistant (and therefore hopefully
        # harmless)
        # https://developer.arm.com/documentation/100074/0608/linker-errors-and-warnings/list-of-the-armlink-error-and-warning-messages
        # Empty region description
        --diag_suppress=6312
        # Ns section matches pattern
        --diag_suppress=6314
        # Duplicate input files
        --diag_suppress=6304
        # Pattern only matches removed unused sections.
        --diag_suppress=6329
        # Load region address not aligned on a boundary
        --diag_suppress=6244
    )
endmacro()

macro(tfm_toolchain_set_processor_arch)
    if (DEFINED TFM_SYSTEM_PROCESSOR)
        set(CMAKE_SYSTEM_PROCESSOR       ${TFM_SYSTEM_PROCESSOR})

        if (TFM_SYSTEM_ARCHITECTURE STREQUAL "armv8.1-m.main")
            message(WARNING "MVE is not yet supported using ARMCLANG")
            string(APPEND CMAKE_SYSTEM_PROCESSOR "+nomve")
        endif()

        if (DEFINED TFM_SYSTEM_DSP)
            if(NOT TFM_SYSTEM_DSP)
                string(APPEND CMAKE_SYSTEM_PROCESSOR "+nodsp")
            endif()
        endif()

        # ARMCLANG specifies that '+nofp' is available on following M-profile cpus:
        # 'cortex-m4', 'cortex-m7', 'cortex-m33', 'cortex-m35p', 'cortex-m55' and 'cortex-m85'.
        # Build fails if other M-profile cpu, such as 'cortex-m23', is added with '+nofp'.
        # Explicitly list those cpu to align with ARMCLANG description.
        if (NOT CONFIG_TFM_FLOAT_ABI STREQUAL "hard" AND
            (TFM_SYSTEM_PROCESSOR STREQUAL "cortex-m4"
            OR TFM_SYSTEM_PROCESSOR STREQUAL "cortex-m7"
            OR TFM_SYSTEM_PROCESSOR STREQUAL "cortex-m33"
            OR TFM_SYSTEM_PROCESSOR STREQUAL "cortex-m35p"
            OR TFM_SYSTEM_PROCESSOR STREQUAL "cortex-m55"
            OR TFM_SYSTEM_PROCESSOR STREQUAL "cortex-m85"))
                string(APPEND CMAKE_SYSTEM_PROCESSOR "+nofp")
        endif()

        string(REGEX REPLACE "\\+nodsp" ".no_dsp" CMAKE_ASM_CPU_FLAG "${CMAKE_SYSTEM_PROCESSOR}")
        string(REGEX REPLACE "\\+nomve" ".no_mve" CMAKE_ASM_CPU_FLAG "${CMAKE_ASM_CPU_FLAG}")
        string(REGEX REPLACE "\\+nofp" ".no_fp" CMAKE_ASM_CPU_FLAG "${CMAKE_ASM_CPU_FLAG}")
    else()
        set(CMAKE_ASM_CPU_FLAG  ${TFM_SYSTEM_ARCHITECTURE})

        # Armasm uses different syntax than armclang for architecture targets
        string(REGEX REPLACE "\\armv" "" CMAKE_ASM_CPU_FLAG "${CMAKE_ASM_CPU_FLAG}")
        string(REGEX REPLACE "\\armv" "" CMAKE_ASM_CPU_FLAG "${CMAKE_ASM_CPU_FLAG}")

        # Modifiers are additive instead of subtractive (.fp Vs .no_fp)
        if (TFM_SYSTEM_DSP)
            string(APPEND CMAKE_ASM_CPU_FLAG ".dsp")
        endif()

        if (CONFIG_TFM_FLOAT_ABI STREQUAL "hard")
            string(APPEND CMAKE_ASM_CPU_FLAG ".fp")
        endif()
    endif()

    # CMAKE_SYSTEM_ARCH is an ARMCLANG CMAKE internal variable, used to set
    # compile and linker flags up until CMake 3.21 where CMP0123 was introduced:
    # https://cmake.org/cmake/help/latest/policy/CMP0123.html
    # This behavior is overwritten by setting CMAKE_C_FLAGS in
    # tfm_toolchain_reload_compiler.
    # Another use of this variable is to statisfy a requirement for ARMCLANG to
    # set either the target CPU or the Architecture. This variable needs to be
    # set to allow targeting architectures without a specific CPU.
    set(CMAKE_SYSTEM_ARCH            ${TFM_SYSTEM_ARCHITECTURE})

    set(CMAKE_C_COMPILER_TARGET      arm-${CROSS_COMPILE})
    set(CMAKE_CXX_COMPILER_TARGET    arm-${CROSS_COMPILE})
    set(CMAKE_ASM_COMPILER_TARGET    arm-${CROSS_COMPILE})

    # MVE is currently not supported in case of armclang
    if (TFM_SYSTEM_ARCHITECTURE STREQUAL "armv8.1-m.main")
        string(APPEND CMAKE_SYSTEM_ARCH "+nomve")
    endif()

    if (DEFINED TFM_SYSTEM_DSP)
        if(NOT TFM_SYSTEM_DSP)
            string(APPEND CMAKE_SYSTEM_ARCH "+nodsp")
        endif()
    endif()

    # Cmake's ARMClang support has several issues with compiler validation. To
    # avoid these, we set the list of supported -mcpu and -march variables to
    # the ones we intend to use so that the validation will never fail.
    include(Compiler/ARMClang)
    set(CMAKE_C_COMPILER_PROCESSOR_LIST ${CMAKE_SYSTEM_PROCESSOR})
    set(CMAKE_C_COMPILER_ARCH_LIST ${CMAKE_SYSTEM_ARCH})
    set(CMAKE_CXX_COMPILER_PROCESSOR_LIST ${CMAKE_SYSTEM_PROCESSOR})
    set(CMAKE_CXX_COMPILER_ARCH_LIST ${CMAKE_SYSTEM_ARCH})
    set(CMAKE_ASM_COMPILER_PROCESSOR_LIST ${CMAKE_SYSTEM_PROCESSOR})
    set(CMAKE_ASM_COMPILER_ARCH_LIST ${CMAKE_SYSTEM_ARCH})
endmacro()

macro(tfm_toolchain_reload_compiler)
    tfm_toolchain_set_processor_arch()
    tfm_toolchain_reset_compiler_flags()
    tfm_toolchain_reset_linker_flags()

    unset(CMAKE_C_FLAGS_INIT)
    unset(CMAKE_CXX_FLAGS_INIT)
    unset(CMAKE_C_LINK_FLAGS)
    unset(CMAKE_ASM_FLAGS_INIT)
    unset(CMAKE_ASM_LINK_FLAGS)
    unset(__mcpu_flag_set)
    unset(__march_flag_set)

    include(Compiler/ARMClang)
    __compiler_armclang(C)
    include(Compiler/ARMCC-ASM)
    __compiler_armcc(ASM)

    if (CMAKE_C_COMPILER_VERSION VERSION_LESS 6.13)
        message(FATAL_ERROR "Please select newer Arm compiler version starting from 6.13.")
    endif()

    if (CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 6.15 AND
        CMAKE_C_COMPILER_VERSION VERSION_LESS 6.18)
        message(FATAL_ERROR "Armclang 6.15~6.17 may cause MemManage fault."
                            " This defect has been fixed since Armclang 6.18."
                            " See [SDCOMP-59788] in Armclang 6.18 release note for details."
                            " Please use other Armclang versions instead.")
    endif()

    set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS_INIT})

    if (DEFINED TFM_SYSTEM_PROCESSOR)
        set(CMAKE_C_FLAGS "-mcpu=${CMAKE_SYSTEM_PROCESSOR}")
        set(CMAKE_CXX_FLAGS "-mcpu=${CMAKE_SYSTEM_PROCESSOR}")
        set(CMAKE_C_LINK_FLAGS   "--cpu=${CMAKE_SYSTEM_PROCESSOR}")
        set(CMAKE_CXX_LINK_FLAGS "--cpu=${CMAKE_SYSTEM_PROCESSOR}")
        set(CMAKE_ASM_LINK_FLAGS "--cpu=${CMAKE_SYSTEM_PROCESSOR}")
        # But armlink doesn't support this +dsp syntax
        string(REGEX REPLACE "\\+nodsp" "" CMAKE_C_LINK_FLAGS   "${CMAKE_C_LINK_FLAGS}")
        string(REGEX REPLACE "\\+nodsp" "" CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS}")
        string(REGEX REPLACE "\\+nodsp" "" CMAKE_ASM_LINK_FLAGS "${CMAKE_ASM_LINK_FLAGS}")
        # And uses different syntax for +nofp
        string(REGEX REPLACE "\\+nofp" ".no_fp" CMAKE_C_LINK_FLAGS   "${CMAKE_C_LINK_FLAGS}")
        string(REGEX REPLACE "\\+nofp" ".no_fp" CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS}")
        string(REGEX REPLACE "\\+nofp" ".no_fp" CMAKE_ASM_LINK_FLAGS "${CMAKE_ASM_LINK_FLAGS}")

        string(REGEX REPLACE "\\+nomve" ".no_mve" CMAKE_C_LINK_FLAGS   "${CMAKE_C_LINK_FLAGS}")
        string(REGEX REPLACE "\\+nomve" ".no_mve" CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS}")
        string(REGEX REPLACE "\\+nomve" ".no_mve" CMAKE_ASM_LINK_FLAGS "${CMAKE_ASM_LINK_FLAGS}")
    else()
        set(CMAKE_C_FLAGS "-march=${CMAKE_SYSTEM_ARCH}")
        set(CMAKE_CXX_FLAGS "-march=${CMAKE_SYSTEM_ARCH}")
    endif()

    set(BL2_COMPILER_CP_FLAG
        $<$<COMPILE_LANGUAGE:C>:-mfpu=softvfp>
        $<$<COMPILE_LANGUAGE:ASM>:--fpu=softvfp>
    )
    # As BL2 does not use hardware FPU, specify '--fpu=SoftVFP' explicitly to use software
    # library functions for BL2 to override any implicit FPU option, such as '--cpu' option.
    # Because the implicit hardware FPU option enforces BL2 to initialize FPU but hardware FPU
    # is not actually enabled in BL2, it will cause BL2 runtime fault.
    set(BL2_LINKER_CP_OPTION --fpu=SoftVFP)

    set(BL1_COMPILER_CP_FLAG
        $<$<COMPILE_LANGUAGE:C>:-mfpu=softvfp>
        $<$<COMPILE_LANGUAGE:ASM>:--fpu=softvfp>
    )
    set(BL1_LINKER_CP_OPTION --fpu=SoftVFP)

    if (CONFIG_TFM_FLOAT_ABI STREQUAL "hard")
        set(COMPILER_CP_FLAG
            $<$<COMPILE_LANGUAGE:C>:-mfloat-abi=hard>
        )
        if (CONFIG_TFM_ENABLE_FP)
            set(COMPILER_CP_FLAG
                $<$<COMPILE_LANGUAGE:C>:-mfpu=${CONFIG_TFM_FP_ARCH}>
                $<$<COMPILE_LANGUAGE:C>:-mfloat-abi=hard>
                $<$<COMPILE_LANGUAGE:ASM>:--fpu=${CONFIG_TFM_FP_ARCH_ASM}>
            )
            # armasm and armlink have the same option "--fpu" and are both used to
            # specify the target FPU architecture. So the supported FPU architecture
            # names can be shared by armasm and armlink.
            set(LINKER_CP_OPTION --fpu=${CONFIG_TFM_FP_ARCH_ASM})
        endif()
    else()
        set(COMPILER_CP_FLAG
            $<$<COMPILE_LANGUAGE:C>:-mfpu=softvfp>
            $<$<COMPILE_LANGUAGE:ASM>:--fpu=softvfp>
        )
        set(LINKER_CP_OPTION --fpu=SoftVFP)
    endif()

    # Workaround for issues with --depend-single-line with armasm and Ninja
    if (CMAKE_GENERATOR STREQUAL "Ninja")
        set( CMAKE_DEPFILE_FLAGS_ASM "--depend=<OBJECT>.d")
    endif()

    set(CMAKE_C_FLAGS_MINSIZEREL "-Oz -DNDEBUG")
endmacro()

# Configure environment for the compiler setup run by cmake at the first
# `project` call in <tfm_root>/CMakeLists.txt. After this mandatory setup is
# done, all further compiler setup is done via tfm_toolchain_reload_compiler()
tfm_toolchain_set_processor_arch()
tfm_toolchain_reset_compiler_flags()
tfm_toolchain_reset_linker_flags()

# Behaviour for handling scatter files is so wildly divergent between compilers
# that this macro is required.
macro(target_add_scatter_file target)
    target_link_options(${target}
        PRIVATE
        --scatter=$<TARGET_OBJECTS:${target}_scatter>
    )

    add_library(${target}_scatter OBJECT)
    foreach(scatter_file ${ARGN})
        target_sources(${target}_scatter
            PRIVATE
                ${scatter_file}
        )
        # Cmake cannot use generator expressions in the
        # set_source_file_properties command, so instead we just parse the regex
        # for the filename and set the property on all files, regardless of if
        # the generator expression would evaluate to true or not.
        string(REGEX REPLACE ".*>:(.*)>$" "\\1" SCATTER_FILE_PATH "${scatter_file}")
        set_source_files_properties(${SCATTER_FILE_PATH}
            PROPERTIES
            LANGUAGE C
        )
    endforeach()

    add_dependencies(${target}
        ${target}_scatter
    )

    set_target_properties(${target} PROPERTIES LINK_DEPENDS $<TARGET_OBJECTS:${target}_scatter>)

    target_link_libraries(${target}_scatter
        platform_region_defs
        psa_interface
        tfm_config
    )

    target_compile_options(${target}_scatter
        PRIVATE
            -E
            -xc
    )
endmacro()

macro(add_convert_to_bin_target target)
    get_target_property(bin_dir ${target} RUNTIME_OUTPUT_DIRECTORY)

    add_custom_target(${target}_bin
        SOURCES ${bin_dir}/${target}.bin
    )
    add_custom_command(OUTPUT ${bin_dir}/${target}.bin
        DEPENDS ${target}
        COMMAND fromelf
            --bincombined $<TARGET_FILE:${target}>
            --output=${bin_dir}/${target}.bin
    )

    add_custom_target(${target}_elf
        SOURCES ${bin_dir}/${target}.elf
    )
    add_custom_command(OUTPUT ${bin_dir}/${target}.elf
        DEPENDS ${target}
        COMMAND fromelf
            --elf $<TARGET_FILE:${target}>
            --output=${bin_dir}/${target}.elf
    )

    add_custom_target(${target}_hex
        SOURCES ${bin_dir}/${target}.hex
    )
    add_custom_command(OUTPUT ${bin_dir}/${target}.hex
        DEPENDS ${target}
        COMMAND fromelf
            --i32combined $<TARGET_FILE:${target}>
            --output=${bin_dir}/${target}.hex
    )

    add_custom_target(${target}_binaries
        ALL
        DEPENDS ${target}_bin
        DEPENDS ${target}_elf
        DEPENDS ${target}_hex
    )
endmacro()

macro(target_share_symbols target)
    get_target_property(TARGET_TYPE ${target} TYPE)
    if (NOT TARGET_TYPE STREQUAL "EXECUTABLE")
        message(FATAL_ERROR "${target} is not an executable. Symbols cannot be shared from libraries.")
    endif()

    foreach(symbol_file ${ARGN})
        FILE(STRINGS ${symbol_file} SYMBOLS
            LENGTH_MINIMUM 1
        )
        list(APPEND KEEP_SYMBOL_LIST ${SYMBOLS})
    endforeach()


    # strip all the symbols except those proveded as arguments. Long inline
    # python scripts aren't ideal, but this is both portable and possibly easier
    # to maintain than trying to filter files at build time in cmake.
    add_custom_command(TARGET ${target}
        POST_BUILD
        VERBATIM
        COMMAND python3 -c "from sys import argv; import re; f = open(argv[1], 'rt'); p = [x.replace('*', '.*') for x in argv[2:]]; l = [x for x in f.readlines() if re.search(r'(?=('+'$|'.join(p + ['SYMDEFS']) + r'))', x)]; f.close(); f = open(argv[1], 'wt'); f.writelines(l); f.close();" $<TARGET_FILE_DIR:${target}>/${target}${CODE_SHARING_OUTPUT_FILE_SUFFIX} ${KEEP_SYMBOL_LIST})

    # Force the target to not remove the symbols if they're unused.
    list(TRANSFORM KEEP_SYMBOL_LIST PREPEND --undefined=)
    target_link_options(${target}
        PRIVATE
            ${KEEP_SYMBOL_LIST}
    )

    # Ask armclang to produce a symdefs file that will
    target_link_options(${target}
        PRIVATE
            --symdefs=$<TARGET_FILE_DIR:${target}>/${target}${CODE_SHARING_OUTPUT_FILE_SUFFIX}
    )
endmacro()

macro(target_link_shared_code target)
    get_target_property(TARGET_SOURCE_DIR ${target} SOURCE_DIR)

    foreach(symbol_provider ${ARGN})
        if (TARGET ${symbol_provider})
            get_target_property(SYMBOL_PROVIDER_TYPE ${symbol_provider} TYPE)
            if (NOT SYMBOL_PROVIDER_TYPE STREQUAL "EXECUTABLE")
                message(FATAL_ERROR "${symbol_provider} is not an executable. Symbols cannot be shared from libraries.")
            endif()
        endif()

        add_dependencies(${target} ${symbol_provider})
        target_link_options(${target} PRIVATE LINKER:$<TARGET_FILE_DIR:${symbol_provider}>/${symbol_provider}${CODE_SHARING_INPUT_FILE_SUFFIX})
    endforeach()
endmacro()

macro(target_strip_symbols target)
    set(SYMBOL_LIST "${ARGN}")
    list(TRANSFORM SYMBOL_LIST PREPEND  --strip-symbol=)

    # TODO we assume that arm-none-eabi-objcopy is available - since we're using
    # armclang this isn't necessarily true.
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY}
        ARGS $<TARGET_FILE:${target}> --wildcard ${SYMBOL_LIST} $<TARGET_FILE:${target}>
    )
endmacro()

macro(target_strip_symbols_from_dependency target dependency)
    set(SYMBOL_LIST "${ARGN}")
    list(TRANSFORM SYMBOL_LIST PREPEND  --strip-symbol=)

    # TODO we assume that arm-none-eabi-objcopy is available - since we're using
    # armclang this isn't necessarily true.
    add_custom_command(
        TARGET ${target}
        PRE_LINK
        COMMAND ${CMAKE_OBJCOPY}
        ARGS $<TARGET_FILE:${dependency}> --wildcard ${SYMBOL_LIST} $<TARGET_FILE:${dependency}>
    )
endmacro()

macro(target_weaken_symbols target)
    set(SYMBOL_LIST "${ARGN}")
    list(TRANSFORM SYMBOL_LIST PREPEND  --weaken-symbol=)

    # TODO we assume that arm-none-eabi-objcopy is available - since we're using
    # armclang this isn't necessarily true.
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY}
        ARGS $<TARGET_FILE:${target}> --wildcard ${SYMBOL_LIST} $<TARGET_FILE:${target}>
    )
endmacro()

macro(target_weaken_symbols_from_dependency target dependency)
    set(SYMBOL_LIST "${ARGN}")
    list(TRANSFORM SYMBOL_LIST PREPEND  --weaken-symbol=)

    # TODO we assume that arm-none-eabi-objcopy is available - since we're using
    # armclang this isn't necessarily true.
    add_custom_command(
        TARGET ${target}
        PRE_LINK
        COMMAND ${CMAKE_OBJCOPY}
        ARGS $<TARGET_FILE:${dependency}> --wildcard ${SYMBOL_LIST} $<TARGET_FILE:${dependency}>
    )
endmacro()
