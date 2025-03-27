#-------------------------------------------------------------------------------
# Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# BSP config.cmake uses IFX_BSP_DESIGN_FILE_NAME, thus it must be
# defined before BSP config.cmake is included
set(IFX_BSP_DESIGN_FILE_NAME                "flash"     CACHE STRING "Name of BSP design file to use")

#################################### Board #####################################

# Include board specific configuration at the beginning to allow override configuration provided by platform
set(IFX_CONFIG_BSP_PATH                     "${CMAKE_CURRENT_LIST_DIR}/board/KIT_PSC3M5_EVK" CACHE PATH "Path to individual BSP configurations")
include(${IFX_CONFIG_BSP_PATH}/config.cmake)

set(IFX_BOARD_PATH                          ${IFX_CONFIG_BSP_PATH}   CACHE PATH    "Path to board config, override it to support custom board")

set(IFX_BSP_S_COMPONENTS                    "CM33;SECURE_DEVICE" CACHE PATH    "List of BSP components used to build secure image")
set(IFX_BSP_S_EXCLUDE                       ".+/cybsp_dsram\.c" # Power Management (e.g. DSRAM callbacks) is not supported
                                            ".+/cybsp_pm_.+\.c" ".+/cybsp_pm\.c" # Power management is not supported
                                            ".+/startup_cat1b_cm33\.c" # TFM uses own startup files as for now
                                            ".+/system_psc3\.c" # system_psc3.c has data that are shared and linked into ${IFX_SHARED_RO_DATA_TARGET} library
                                                        CACHE PATH      "List of sources excluded from build of secure BSP target")

################################################################################

# Configurations shared by both secure and non-secure images
include(${IFX_PLATFORM_SOURCE_DIR}/shared/config.cmake)

set(IFX_GENERATED_DIR ${CMAKE_BINARY_DIR}/generated CACHE PATH "Path to the root directory of files generated during the build of SPE")

################################## Isolation ###################################

set(TFM_ISOLATION_LEVEL                     2           CACHE STRING    "Isolation level")

# Use single Protection Context for isolation
set(IFX_ISOLATION_PC_SWITCHING              OFF) # Disable Protection Context switching

##################################### BL2 ######################################
set(BL2                                     OFF         CACHE BOOL      "Whether to build BL2")
set(BL2_HEADER_SIZE                         0x400       CACHE STRING    "BL2 Header size")
set(BL2_TRAILER_SIZE                        0x0         CACHE STRING    "BL2 Trailer size")

#################################### IFX #######################################
set(IFX_MPC_DRIVER_SW_POLICY                ON) # TFM uses MPC policy to check region protection
set(IFX_OPTIMIZE_TESTS                      ON          CACHE BOOL      "Optimize tests to be able to fit into memory")
set(IFX_SE_RT_SERVICES_UTILS_ENABLED        OFF)

set(IFX_MBEDTLS_ACCELERATION_LIB_PATH       "DOWNLOAD"  CACHE PATH      "Path to Infineon MBEDTLS Acceleration library (or DOWNLOAD to fetch automatically)")
set(IFX_MBEDTLS_ACCELERATION_LIB_GIT_REMOTE "https://github.com/Infineon/cy-mbedtls-acceleration" CACHE STRING "The URL (or path) to retrieve MBEDTLS Acceleration from.")
set(IFX_MBEDTLS_ACCELERATION_LIB_VERSION    "release-v2.7.0" CACHE STRING "The version of Infineon MBEDTLS Acceleration library to use")
set(IFX_MBEDTLS_ACCELERATOR_VALID_TYPES     "CRYPTOLITE")
set(IFX_MBEDTLS_ACCELERATOR_TYPE            "CRYPTOLITE" CACHE STRING   "Specifies type of accelerator for a project")
set(IFX_MBEDTLS_ACCELERATION_ENABLED        ON           CACHE BOOL     "Enable crypto accelerator")
set(IFX_MBEDTLS_ACCELERATION_PATCH_DIR      "${IFX_COMMON_SOURCE_DIR}/libs/ifx_mbedtls_acceleration/patch/cryptolite" CACHE STRING "Path to mbedtls patches")

set(IFX_BSP_DESIGN_FILE_VALID_NAMES         "flash;test")

################################### Drivers ####################################


set(IFX_NV_COUNTERS_VALID_TYPES             "FLASH")
set(IFX_NV_COUNTERS_TYPE                    "FLASH"     CACHE STRING "Specifies NV counters type for a project")

set(IFX_ITS_VALID_TYPES                     "FLASH")
set(IFX_ITS_TYPE                            "FLASH"     CACHE STRING "Specifies ITS type for a project")

################################### Platform ###################################
set(CONFIG_TFM_USE_TRUSTZONE                ON  CACHE BOOL     "Use TrustZone to transition between NSPE and SPE on the same CPU")
set(TFM_MULTI_CORE_TOPOLOGY                 OFF CACHE BOOL     "NSPE runs on a separate CPU to SPE")

set(IFX_CRYPTO_KEYS_FLASH                   ON) # Crypto key storage in FLASH, platform provides configuration (location and layout)

set(CRYPTO_TFM_BUILTIN_KEYS_DRIVER          ON    CACHE BOOL      "Whether to allow crypto service to store builtin keys. Without this, ALL builtin keys must be stored in a platform-specific location")

set(IFX_PDL_SECURE_SERVICES                 OFF)

################################### Partition ##################################
set(TFM_PARTITION_CRYPTO                    ON    CACHE BOOL "Enable Crypto partition")
set(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE  ON    CACHE BOOL "Enable Internal Trusted Storage partition")
set(TFM_PARTITION_PLATFORM                  ON    CACHE BOOL "Enable the TF-M Platform partition")
set(TFM_PARTITION_PROTECTED_STORAGE         OFF   CACHE BOOL "Enable Protected Storage partition")
set(TFM_PARTITION_INITIAL_ATTESTATION       ON    CACHE BOOL "Enable Initial Attestation partition")
set(SYMMETRIC_INITIAL_ATTESTATION           OFF   CACHE BOOL "Use symmetric crypto for inital attestation")
set(TFM_PARTITION_FIRMWARE_UPDATE           OFF   CACHE BOOL "Enable firmware update partition")

set(TFM_EXTRA_GENERATED_FILE_LIST_PATH      ${CMAKE_CURRENT_LIST_DIR}/generated_file_list.yaml  CACHE PATH "Path to extra generated file list. Appended to stardard TFM generated file list." FORCE)

################################## Advanced options #############################
set(PROJECT_CONFIG_HEADER_FILE              "${IFX_PLATFORM_SOURCE_DIR}/spe/services/crypto/config_profile_psc3.h" CACHE FILEPATH "User defined header file for TF-M config")
set(TFM_MBEDCRYPTO_CONFIG_PATH              "${IFX_PLATFORM_SOURCE_DIR}/spe/services/crypto/tfm_mbedcrypto_config_profile_psc3.h" CACHE PATH "Config to use for Mbed Crypto")
set(TFM_MBEDCRYPTO_PSA_CRYPTO_CONFIG_PATH   "${IFX_PLATFORM_SOURCE_DIR}/spe/services/crypto/crypto_config_profile_psc3.h" CACHE PATH "Config to use psa crypto setting for Mbed Crypto.")

set(TFM_MBEDCRYPTO_PLATFORM_EXTRA_CONFIG_PATH "${IFX_PLATFORM_SOURCE_DIR}/spe/services/crypto/mbedtls_target_config_psc3.h"     CACHE PATH      "Config to append to standard Mbed Crypto config, used by platforms to configure feature support")

set(TFM_PROFILE                             profile_medium CACHE STRING    "Profile to use")

set(IFX_CRYPTOLITE_USER_CONFIG_FILE         "${IFX_PLATFORM_SOURCE_DIR}/spe/services/crypto/ifx_pdl_cryptolite_config.h" CACHE PATH "Config to use for Cryptolite accelerator")

###################################################################################

include(${IFX_COMMON_SOURCE_DIR}/config.cmake)

include(${IFX_COMMON_SOURCE_DIR}/cmake/generate_sources.cmake)
