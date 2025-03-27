/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2022-2024 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file its_flash_ram.h
 *
 * \brief Implementations of the flash interface functions for an emulated flash
 *        device using RAM. See its_flash_ops_t for full documentation of
 *        functions.
 */

#ifndef __ITS_FLASH_RAM_H__
#define __ITS_FLASH_RAM_H__

#include <stdint.h>
#include "its_flash_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief ITS RAM driver context.
 */
struct its_flash_ram_dev_t {
    /* Memory buffer for RAM driver */
    uint8_t *buffer;

    /* Size of the memory buffer */
    uint32_t size;
};

extern const struct its_flash_ops_t its_flash_fs_ops_ram;

#ifdef __cplusplus
}
#endif

#endif /* __ITS_FLASH_RAM_H__ */
