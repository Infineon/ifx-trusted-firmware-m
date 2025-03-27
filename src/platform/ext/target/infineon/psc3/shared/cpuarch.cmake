#-------------------------------------------------------------------------------
# Copyright (c) 2023-2024 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# Use to set/check IFX_CORE
set(IFX_CM33 "cm33")
set(IFX_CORE "cm33")

add_compile_definitions(IFX_CM33=33)
add_compile_definitions(IFX_CORE=33)

# TFM is built without BL2 but external build of bootloader is present on
# device, this bootloader will populate shared data, so BOOT_DATA_AVAILABLE
# needs to be defined.
add_compile_definitions(BOOT_DATA_AVAILABLE)

# FPU for CM33 core
set(CONFIG_TFM_FP_ARCH "fpv5-d16")
set(CONFIG_TFM_FP_ARCH_ASM "FPv5_D16")

set(TFM_SYSTEM_PROCESSOR cortex-m33)
set(TFM_SYSTEM_ARCHITECTURE armv8-m.main)
set(TFM_SYSTEM_DSP OFF)
