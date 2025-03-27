/*
 * Copyright (c) 2020-2023 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_HAL_PS_H__
#define __TFM_HAL_PS_H__

#include <stddef.h>
#include <stdint.h>

#include "Driver_Flash.h"
#include "flash_layout.h"
#include "tfm_hal_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The name of the ARM_DRIVER_FLASH to use for PS */
#if !PS_RAM_FS && !defined(TFM_HAL_PS_FLASH_DRIVER)
#error "TFM_HAL_PS_FLASH_DRIVER must be defined by the target in flash_layout.h"
#endif

/* The size of the PS flash device's physical program unit. Must be equal to
 * TFM_HAL_PS_FLASH_DRIVER.GetInfo()->program_unit, but required at compile
 * time.
 */
#ifndef TFM_HAL_PS_PROGRAM_UNIT
#error "TFM_HAL_PS_PROGRAM_UNIT must be defined by the target in flash_layout.h"
#elif (TFM_HAL_PS_PROGRAM_UNIT < 1)
#error "TFM_HAL_PS_PROGRAM_UNIT must be greater than 1"
#elif (TFM_HAL_PS_PROGRAM_UNIT & (TFM_HAL_PS_PROGRAM_UNIT - 1) != 0)
#error "TFM_HAL_PS_PROGRAM_UNIT must be a power of two"
#endif

/**
 * \brief Struct containing information required from the platform at runtime
 *        to configure the PS filesystem.
 */
struct tfm_hal_ps_fs_info_t {
    uint32_t flash_area_addr;  /**< Location of the block of flash to use for PS
                                */
    size_t flash_area_size;    /**< Number of bytes of flash to use for PS */
    uint32_t block_size;       /**< Size of logical FS block. Typically it should
                                 *  be multiple to sector erase size of underlying
                                 *  flash driver.
                                 */
};


#if PS_RAM_FS
/* RAM FS: there is nothing to declare */

#elif defined(TFM_HAL_PS_FLASH_OPS)
/* Custom flash driver for Protection Storage is defined through
 * TFM_HAL_PS_FLASH_OPS - `const struct its_flash_ops_t` implementation of
 *      ITS flash operations
 * TFM_HAL_PS_FLASH_DRIVER - this variable is assigned to
 *      \ref its_flash_config_t.context of PS flash driver instance configuration.
 * TFM_HAL_PS_FLASH_DRIVER_TYPE - type of \ref TFM_HAL_PS_FLASH_DRIVER
 */
#ifndef TFM_HAL_PS_FLASH_DRIVER_TYPE
#error "TFM_HAL_PS_FLASH_DRIVER_TYPE must be defined for custom TFM_HAL_PS_FLASH_OPS"
#endif
extern TFM_HAL_PS_FLASH_DRIVER_TYPE TFM_HAL_PS_FLASH_DRIVER;
extern const struct its_flash_ops_t TFM_HAL_PS_FLASH_OPS;

#else
/**
 * \brief The flash driver to use for PS.
 */
extern ARM_DRIVER_FLASH TFM_HAL_PS_FLASH_DRIVER;
#endif

/**
 * \brief Retrieve the filesystem config for PS.
 *
 * Note that this function should ensure that the values returned do
 * not result in a security compromise.
 *
 * \param [out] fs_info  Filesystem config information
 *
 * \return A status code as specified in \ref tfm_hal_status_t
 * If an error is detected within this function, is should leave the
 * content of the parameters unchanged.
 *
 * \retval TFM_HAL_SUCCESS              The operation completed successfully
 * \retval TFM_HAL_ERROR_INVALID_INPUT  Invalid parameter
 */
enum tfm_hal_status_t tfm_hal_ps_fs_info(struct tfm_hal_ps_fs_info_t *fs_info);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_HAL_PS_H__ */
