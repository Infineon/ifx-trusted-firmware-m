/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2022-2024 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <string.h>
#include "its_flash_ram.h"

/**
 * \brief Gets physical address of the given block ID.
 *
 * \param[in] cfg       Flash driver configuration
 * \param[in] block_id  Block ID
 * \param[in] offset    Offset position from the init of the block
 *
 * \returns Returns physical address for the given block ID.
 */
static uint32_t get_phys_address(const struct its_flash_config_t *cfg,
                                 uint32_t block_id, size_t offset)
{
    return (block_id * cfg->block_size) + offset;
}

static psa_status_t its_flash_ram_init(struct its_flash_config_t *cfg)
{
    const struct its_flash_ram_dev_t *dev = cfg->context;

    /* Validate block size */
    if ((cfg->block_size > dev->size) ||
        (dev->size / cfg->block_size < cfg->num_blocks)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    return PSA_SUCCESS;
}

static psa_status_t its_flash_ram_read(const struct its_flash_config_t *cfg,
                                       uint32_t block_id, uint8_t *buff,
                                       size_t offset, size_t size)
{
    const struct its_flash_ram_dev_t *dev = cfg->context;
    uint32_t idx = get_phys_address(cfg, block_id, offset);

    /* Validate arguments */
    if ((idx >= dev->size) ||
        (size > dev->size) || (idx > dev->size - size)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    (void)memcpy(buff, dev->buffer + idx, size);

    return PSA_SUCCESS;
}

static psa_status_t its_flash_ram_write(const struct its_flash_config_t *cfg,
                                        uint32_t block_id, const uint8_t *buff,
                                        size_t offset, size_t size)
{
    const struct its_flash_ram_dev_t *dev = cfg->context;
    uint32_t idx = get_phys_address(cfg, block_id, offset);

    /* Validate arguments */
    if ((idx >= dev->size) ||
        (size > dev->size) || (idx > dev->size - size)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    (void)memcpy(dev->buffer + idx, buff, size);

    return PSA_SUCCESS;
}

static psa_status_t its_flash_ram_flush(const struct its_flash_config_t *cfg,
                                        uint32_t block_id)

{
    /* Nothing needs to be done for flash emulated in RAM, as writes are
     * commited immediately.
     */
    (void)cfg;
    (void)block_id;
    return PSA_SUCCESS;
}

static psa_status_t its_flash_ram_erase(const struct its_flash_config_t *cfg,
                                        uint32_t block_id)
{
    const struct its_flash_ram_dev_t *dev = cfg->context;
    uint32_t idx = get_phys_address(cfg, block_id, 0);

    /* Validate arguments */
    if ((idx >= dev->size) || (idx > dev->size - cfg->block_size)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    (void)memset(dev->buffer + idx, cfg->erase_val,
                 cfg->block_size);

    return PSA_SUCCESS;
}

const struct its_flash_ops_t its_flash_fs_ops_ram = {
    .init = its_flash_ram_init,
    .read = its_flash_ram_read,
    .write = its_flash_ram_write,
    .flush = its_flash_ram_flush,
    .erase = its_flash_ram_erase,
};
