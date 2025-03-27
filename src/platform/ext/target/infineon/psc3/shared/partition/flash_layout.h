/*
 * Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ifx_driver_flash.h is used for definition of IFX_DRIVER_RRAM_PROGRAM_UNIT and
 * is only needed for tfm_hal_its.h file. Thus, to avoid adding RRAM driver
 * to include directories of all other libs that use flash_layout.h, include
 * of ifx_driver_flash.h is made conditional.
 * Also it is done outside of FLASH_LAYOUT_H include guard as tfm_hal_its.h
 * may not be the first file that includes flash_layout.h */
#ifdef TFM_HAL_ITS_H
#include "ifx_driver_flash.h"
#endif /* TFM_HAL_ITS_H */

#ifndef FLASH_LAYOUT_H
#define FLASH_LAYOUT_H

#include "config_tfm.h"
/* Include Trust Zone configurator generated file with memory areas definitions */
#include "project_memory_layout.h"

/*******************************************************************************
************************************* ITS **************************************
*******************************************************************************/

#ifdef TFM_PARTITION_INTERNAL_TRUSTED_STORAGE
#if ITS_RAM_FS

#ifndef ITS_RAM_FS_SIZE
/* Size of RAM allocated for RAM ITS flash driver */
#define ITS_RAM_FS_SIZE                 0x800 /* 2 KB */
#endif
#ifndef ITS_RAM_FS_BLOCK_SIZE
/* Block size of RAM ITS flash driver */
#define ITS_RAM_FS_BLOCK_SIZE           0x400 /* 1 KB */
#endif

/* Smallest flash programmable unit in bytes */
#define TFM_HAL_ITS_PROGRAM_UNIT        (0x4)

#else /* ITS_RAM_FS */

/* Internal Trusted Storage (ITS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M ITS Integration Guide. */
#define TFM_HAL_ITS_FLASH_DRIVER        ifx_its_cmsis_flash_instance
/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address. */
/* Base address of dedicated flash area for ITS */
#define TFM_HAL_ITS_FLASH_AREA_ADDR     IFX_TFM_ITS_ADDR
/* Size of dedicated flash area for ITS */
#define TFM_HAL_ITS_FLASH_AREA_SIZE     IFX_TFM_ITS_SIZE
/* Size of logical FS block */
#ifndef TFM_HAL_ITS_BLOCK_SIZE
#define TFM_HAL_ITS_BLOCK_SIZE          (0x800) /* 2 KB */
#endif /* TFM_HAL_ITS_BLOCK_SIZE */
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_ITS_PROGRAM_UNIT        IFX_DRIVER_FLASH_PROGRAM_UNIT

#endif /* ITS_RAM_FS */
#endif /* TFM_PARTITION_INTERNAL_TRUSTED_STORAGE */

#endif /* FLASH_LAYOUT_H */
