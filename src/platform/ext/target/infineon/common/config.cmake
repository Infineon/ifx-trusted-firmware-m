#-------------------------------------------------------------------------------
# Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

set(IFX_GENERATED_DIR "${CMAKE_BINARY_DIR}/generated" CACHE PATH "Path to the root directory of files generated during the build of SPE")

#################################### IFX #######################################

set(IFX_PLATFORM_DEFAULT_DEVICE_ID          ON          CACHE BOOL      "Use default implementation of API defined in tfm_plat_device_id.h provided by IFX platform")
set(IFX_PLATFORM_DEFAULT_OTP                ON          CACHE BOOL      "Use default OTP implementation provided by IFX platform")

set(IFX_FAULTS_INFO_DUMP                    OFF         CACHE BOOL      "Enable printing of Infineon Fault information")

set(TEST_NS_IFX_CODE_COVERAGE               OFF         CACHE BOOL      "Whether to enable code coverage.")

if ((IFX_SE_IPC_SERVICE_TYPE STREQUAL "FULL") AND (TFM_PARTITION_CRYPTO OR TFM_PARTITION_PLATFORM OR TFM_PARTITION_INITIAL_ATTESTATION))
    # Enable Infineon SE IPC Service
    set(IFX_PARTITION_SE_IPC_SERVICE        ON)
endif()

set(IFX_PRINT_CORE_PREFIX                   OFF         CACHE BOOL      "Enable printing of core prefix in stdout")

################################## Isolation ###################################

if (IFX_ISOLATION_PC_SWITCHING)
    # Protection Context switching uses platform arch hooks
    set(PLATFORM_ARCH_HOOKS                 ON)
endif()

################################### Drivers ####################################

set(IFX_PLATFORM_DRIVERS_LIST               ${IFX_NV_COUNTERS_TYPE}
                                            ${IFX_ITS_TYPE}
                                            ${IFX_PS_TYPE})

if("FLASH" IN_LIST IFX_PLATFORM_DRIVERS_LIST)
    set(IFX_FLASH_DRIVER_ENABLED            ON          CACHE BOOL      "Enable building flash driver source files")
endif()
if("RRAM" IN_LIST IFX_PLATFORM_DRIVERS_LIST)
    set(IFX_RRAM_DRIVER_ENABLED             ON          CACHE BOOL      "Enable building rram driver source files")
endif()
if("SMIF_MMIO" IN_LIST IFX_PLATFORM_DRIVERS_LIST)
    set(IFX_SMIF_MMIO_DRIVER_ENABLED        ON          CACHE BOOL      "Enable building smif mmio driver source files")
endif()
if("SMIF_XIP" IN_LIST IFX_PLATFORM_DRIVERS_LIST)
    set(IFX_SMIF_XIP_DRIVER_ENABLED         ON          CACHE BOOL      "Enable building smif xip driver source files")
endif()

################################### Platform ###################################

set(CONFIG_TFM_ENABLE_FP                    ON          CACHE BOOL      "Enable FPU (Floating Point Unit)")

set(PLATFORM_DEFAULT_ATTEST_HAL             OFF         CACHE BOOL      "Use default attest hal implementation. Should not be used in production.")
set(PLATFORM_DEFAULT_CRYPTO_KEYS            OFF         CACHE BOOL      "Use default crypto keys implementation.")
set(PLATFORM_DEFAULT_IAK                    OFF         CACHE BOOL      "Use default initial attestation_key.")
set(PLATFORM_DEFAULT_ITS_HAL                OFF         CACHE BOOL      "Use default ITS HAL implementation.")
set(PLATFORM_DEFAULT_NV_COUNTERS            OFF         CACHE BOOL      "Use default NV counters implementation. Should not be used in production.")
set(PLATFORM_DEFAULT_NV_SEED                OFF         CACHE BOOL      "Use default NV seed implementation. Should not be used in production.")
set(PLATFORM_DEFAULT_OTP                    OFF         CACHE BOOL      "Use trusted on-chip flash to implement OTP memory")
set(PLATFORM_DEFAULT_OTP_WRITEABLE          OFF         CACHE BOOL      "Use OTP memory with write support")
set(PLATFORM_DEFAULT_PROVISIONING           OFF         CACHE BOOL      "Use default provisioning implementation")
set(PLATFORM_DEFAULT_PS_HAL                 OFF         CACHE BOOL      "Use default Protected Storage HAL implementation.")
set(PLATFORM_DEFAULT_ROTPK                  OFF         CACHE BOOL      "Use default root of trust public key.")
set(PLATFORM_DEFAULT_UART_STDOUT            OFF         CACHE BOOL      "Use default uart stdout implementation.")
set(TFM_DUMMY_PROVISIONING                  OFF         CACHE BOOL      "Provision with dummy values. NOT to be used in production")

set(PLATFORM_SVC_HANDLERS                   ON)
set(PLATFORM_EXCEPTION_INFO                 ${IFX_FAULTS_INFO_DUMP} CACHE BOOL      "Whether platform provides platform-specific exception info store/dump")

################################# Dependencies #################################

set(MBEDCRYPTO_VERSION                      "release-v3.6.100" CACHE STRING  "The version of Mbed Crypto to use")
set(MBEDCRYPTO_GIT_REMOTE                   "https://github.com/Infineon/ifx-mbedtls.git" CACHE STRING "The URL (or path) to retrieve MbedTLS from.")
set(MBEDCRYPTO_PATCH_DIR                    "${IFX_COMMON_SOURCE_DIR}/libs/mbedcrypto/patch" CACHE STRING "Path to mbedtls patches")
set(MBEDCRYPTO_FORCE_PATCH                  ON          CACHE BOOL      "Always apply MBed Crypto patches")

set(IFX_MBEDTLS_ACCELERATION_ENABLED         OFF        CACHE STRING    "Enable crypto accelerator")

if(NOT DEFINED MBEDCRYPTO_PATH OR MBEDCRYPTO_PATH STREQUAL "DOWNLOAD")
    # MBEDCRYPTO_PATH is still "DOWNLOAD" so ${CMAKE_BINARY_DIR}/lib/ext/mbedcrypto-src is used
    set(MBEDTLS_PSA_CRYPTO_PLATFORM_PATH    "${CMAKE_BINARY_DIR}/lib/ext/mbedcrypto-src/target/infineon/interface"    CACHE PATH      "Infineon features definitions to extend the standard Mbed Crypto platform")
else()
    set(MBEDTLS_PSA_CRYPTO_PLATFORM_PATH    "${MBEDCRYPTO_PATH}/target/infineon/interface"    CACHE PATH      "Infineon features definitions to extend the standard Mbed Crypto platform")
endif()

if(IFX_CRYPTO_SE_RT)
    set(MBEDTLS_PSA_CRYPTO_PLATFORM_FILE    "${MBEDTLS_PSA_CRYPTO_PLATFORM_PATH}/ifx_crypto_platform.h")
endif()

############################# Platform services ################################

set(TFM_EXTRA_MANIFEST_LIST_FILES           ""          CACHE FILEPATH  "Extra manifest list file(s), used to list extra Secure Partition manifests.")
list(APPEND TFM_EXTRA_MANIFEST_LIST_FILES   "${IFX_COMMON_SOURCE_DIR}/spe/services/ifx_tfm_manifest_list.yaml")

set(TFM_EXTRA_PARTITION_PATHS               ""          CACHE PATH      "List of extra Secure Partitions directories. An extra Secure Parition folder contains source code, CMakeLists.txt and manifest files")

################################# Advanced options #############################

if (IFX_MBEDTLS_ACCELERATION_ENABLED)
    if (IFX_MBEDTLS_ACCELERATOR_TYPE STREQUAL "CRYPTOLITE")
        set(CRYPTO_HW_ACCELERATOR_CONFIG "${IFX_COMMON_SOURCE_DIR}/spe/services/crypto/mbedtls_accel_configs/crypto_hw_cryptolite_config.h" CACHE PATH "Mbed-TLS acceleration library config")
    elseif(IFX_MBEDTLS_ACCELERATOR_TYPE STREQUAL "MXCRYPTO")
        set(CRYPTO_HW_ACCELERATOR_CONFIG "${IFX_COMMON_SOURCE_DIR}/spe/services/crypto/mbedtls_accel_configs/crypto_hw_mxcrypto_config.h" CACHE PATH "Mbed-TLS acceleration library config")
    endif()
endif()

################################################################################

include(${IFX_COMMON_SOURCE_DIR}/shared/config.cmake)
include(${IFX_COMMON_SOURCE_DIR}/tests/config.cmake OPTIONAL)
