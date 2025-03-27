/*
 * Copyright (c) 2020-2023 Cypress Semiconductor Corporation. All rights reserved.
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_hal_ps.h"

#include "cmsis_compiler.h"
#include "flash_layout.h"

#if PS_RAM_FS

/* The size of flash area emulated in RAM for PS */
#ifndef PS_RAM_FS_SIZE
#error "PS_RAM_FS_SIZE must be defined by the target in flash_layout.h"
#endif

/* The size in bytes of the logical block for flash emulated in RAM for ITS */
#ifndef PS_RAM_FS_BLOCK_SIZE
#error "PS_RAM_FS_BLOCK_SIZE must be defined by the target in flash_layout.h"
#endif

#else /* PS_RAM_FS */

/* The base address of the dedicated flash area for PS */
#ifndef TFM_HAL_PS_FLASH_AREA_ADDR
#error "TFM_HAL_PS_FLASH_AREA_ADDR must be defined by the target in flash_layout.h"
#endif

/* The size of the dedicated flash area for PS in bytes */
#ifndef TFM_HAL_PS_FLASH_AREA_SIZE
#error "TFM_HAL_PS_FLASH_AREA_SIZE must be defined by the target in flash_layout.h"
#endif

#endif /* PS_RAM_FS */

__WEAK enum tfm_hal_status_t
tfm_hal_ps_fs_info(struct tfm_hal_ps_fs_info_t *fs_info)
{
    if (!fs_info) {
        return TFM_HAL_ERROR_INVALID_INPUT;
    }

#if PS_RAM_FS
    fs_info->flash_area_addr = 0;
    fs_info->flash_area_size = PS_RAM_FS_SIZE;
    fs_info->block_size = PS_RAM_FS_BLOCK_SIZE;
#elif defined(TFM_HAL_PS_BLOCK_SIZE)
    /* TFM_HAL_PS_BLOCK_SIZE - The size of contiguous physical flash logical
     * file system erase block. Adjust so that the maximum required asset size
     * will fit in one logical block. */
    fs_info->flash_area_addr = TFM_HAL_PS_FLASH_AREA_ADDR;
    fs_info->flash_area_size = TFM_HAL_PS_FLASH_AREA_SIZE;
    fs_info->block_size = TFM_HAL_PS_BLOCK_SIZE;
#elif defined(TFM_HAL_PS_SECTORS_PER_BLOCK)
    /* TFM_HAL_PS_SECTORS_PER_BLOCK - The number of contiguous physical flash
     * erase sectors per logical file system erase block. Adjust so that the
     * maximum required asset size will fit in one logical block.
     * TFM_HAL_PS_SECTORS_PER_BLOCK is supported for CMSIS flash driver only. */
    ARM_DRIVER_FLASH *cmsis_driver = &TFM_HAL_PS_FLASH_DRIVER;
    fs_info->flash_area_addr = TFM_HAL_PS_FLASH_AREA_ADDR;
    fs_info->flash_area_size = TFM_HAL_PS_FLASH_AREA_SIZE;
    fs_info->block_size = TFM_HAL_PS_SECTORS_PER_BLOCK *
            cmsis_driver->GetInfo()->sector_size;
#else
#error "TFM_HAL_PS_BLOCK_SIZE or TFM_HAL_PS_SECTORS_PER_BLOCK must be defined by the target in flash_layout.h"
#endif

    return TFM_HAL_SUCCESS;
}
