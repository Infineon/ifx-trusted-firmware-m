/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2022-2024 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file its_flash_nor.h
 *
 * \brief Implementations of the flash interface functions for a NOR flash
 *        device. See its_flash_ops_t for full documentation of functions.
 */

#ifndef __ITS_FLASH_NOR_H__
#define __ITS_FLASH_NOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "driver/Driver_Flash.h"
#include "its_flash_hal.h"

/**
 * \brief ITS NOR flash driver.
 *
 * \ref its_flash_config_t.context should point to \ref ARM_DRIVER_FLASH.
 */
extern const struct its_flash_ops_t its_flash_ops_nor;

#ifdef __cplusplus
}
#endif

#endif /* __ITS_FLASH_NOR_H__ */
