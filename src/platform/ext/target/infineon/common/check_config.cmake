#-------------------------------------------------------------------------------
# Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

##################################### FWU ######################################

# FWU is not supported for Infineon platforms
tfm_invalid_config(TFM_PARTITION_FIRMWARE_UPDATE)

#################################### Crypto ####################################

# IFX_CRYPTO_SE_RT must be enabled to support crypto keys in SE RT
tfm_invalid_config(NOT IFX_CRYPTO_SE_RT AND IFX_CRYPTO_KEYS_SE_RT)

tfm_invalid_config(IFX_MBEDTLS_ACCELERATION_ENABLED AND IFX_MBEDTLS_ACCELERATOR_VALID_TYPES AND NOT IFX_MBEDTLS_ACCELERATOR_TYPE IN_LIST IFX_MBEDTLS_ACCELERATOR_VALID_TYPES)

################################### Drivers ####################################

tfm_invalid_config(IFX_NV_COUNTERS_VALID_TYPES AND NOT IFX_NV_COUNTERS_TYPE IN_LIST IFX_NV_COUNTERS_VALID_TYPES)

# Only FULL SE_RT service provides NV counters
tfm_invalid_config(IFX_NV_COUNTERS_TYPE STREQUAL "SE_RT" AND NOT IFX_SE_IPC_SERVICE_TYPE STREQUAL "FULL")

tfm_invalid_config(IFX_ITS_VALID_TYPES AND NOT IFX_ITS_TYPE IN_LIST IFX_ITS_VALID_TYPES)

tfm_invalid_config(IFX_PS_VALID_TYPES AND NOT IFX_PS_TYPE IN_LIST IFX_PS_VALID_TYPES)

################################### Platform ###################################

# Default UART stdout driver is not supported
tfm_invalid_config(PLATFORM_DEFAULT_UART_STDOUT)

# IFX_UART_ENABLED must be enabled to support SPM log
tfm_invalid_config(NOT IFX_UART_ENABLED AND TFM_SPM_LOG_RAW_ENABLED)

tfm_invalid_config(IFX_BSP_DESIGN_FILE_VALID_NAMES AND NOT IFX_BSP_DESIGN_FILE_NAME IN_LIST IFX_BSP_DESIGN_FILE_VALID_NAMES)

# Secure Services is implemented through Platform partition
tfm_invalid_config(IFX_PDL_SECURE_SERVICES AND NOT TFM_PARTITION_PLATFORM)

# Masked and unmasked non-secure IRQ tests are mutually exclusive
tfm_invalid_config(TEST_NS_IFX_IRQ_TEST_MASKED AND TEST_NS_IFX_IRQ_TEST_UNMASKED)

# Mailbox tests can not be executed when mailbox partition is disabled
tfm_invalid_config(TEST_NS_IFX_MAILBOX AND NOT TFM_PARTITION_NS_AGENT_MAILBOX)

################################### Services ###################################

# Validate IFX_SE_IPC_SERVICE_TYPE
set(IFX_SE_IPC_SERVICE_VALID_TYPES "BASE;FULL")
tfm_invalid_config(IFX_SE_IPC_SERVICE_TYPE AND NOT IFX_SE_IPC_SERVICE_TYPE IN_LIST IFX_SE_IPC_SERVICE_VALID_TYPES)

# SE IPC Service supports FULL type only
tfm_invalid_config(IFX_PARTITION_SE_IPC_SERVICE AND NOT IFX_SE_IPC_SERVICE_TYPE STREQUAL "FULL")
