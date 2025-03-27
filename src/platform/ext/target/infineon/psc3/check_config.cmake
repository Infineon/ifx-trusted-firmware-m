#-------------------------------------------------------------------------------
# Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

include(${IFX_COMMON_SOURCE_DIR}/check_config.cmake)

################################## Isolation ###################################

# Isolation Level 1 is not supported
tfm_invalid_config(TFM_ISOLATION_LEVEL EQUAL 1)
# Isolation Level 3 is not supported
tfm_invalid_config(TFM_ISOLATION_LEVEL EQUAL 3)

########################## Platform ############################################

# Protected storage is not supported
tfm_invalid_config(TFM_PARTITION_PROTECTED_STORAGE)

# RRAM is not present on the PSC3 device
tfm_invalid_config(IFX_RRAM_DRIVER_ENABLED)

# SMIF is not present on the PSC3 device
tfm_invalid_config(IFX_SMIF_MMIO_DRIVER_ENABLED)
tfm_invalid_config(IFX_SMIF_XIP_DRIVER_ENABLED)
