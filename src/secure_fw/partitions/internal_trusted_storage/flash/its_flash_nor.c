/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2020-2022 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <string.h>
#include "its_flash_nor.h"

/* Valid entries for data item width */
static const uint32_t data_width_byte[] = {
    sizeof(uint8_t),
    sizeof(uint16_t),
    sizeof(uint32_t),
};

/**
 * \brief Gets physical address of the given block ID.
 *
 * \param[in] instance  Flash driver configuration
 * \param[in] block_id  Block ID
 * \param[in] offset    Offset position from the init of the block
 *
 * \returns Returns physical address for the given block ID.
 */
static uint32_t get_phys_address(const struct its_flash_config_t *cfg,
                                 uint32_t block_id, size_t offset)
{
    return cfg->flash_area_addr + (block_id * cfg->block_size) + offset;
}

static psa_status_t its_flash_nor_init(struct its_flash_config_t *cfg)
{
    int32_t err;
    ARM_DRIVER_FLASH *flash_dev = cfg->context;
    ARM_FLASH_INFO *flash_info;

    err = flash_dev->Initialize(NULL);
    if (err != ARM_DRIVER_OK) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    /* Retrieve flash properties from the flash driver */
    flash_info = flash_dev->GetInfo();
    if (flash_info == NULL) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    /* Check the compile-time program unit matches the runtime value */
    if (cfg->program_unit != flash_info->program_unit) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    /* Validate block size */
    if ((cfg->block_size == 0) ||
        (flash_info->sector_size == 0) ||
        (cfg->block_size % flash_info->sector_size != 0)) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    /* Save device specific properties */
    cfg->erase_val = flash_info->erased_value;

    return PSA_SUCCESS;
}

static psa_status_t flash_read_unaligned(
                                    const struct its_flash_config_t *cfg,
                                    uint32_t addr, uint8_t *buff, size_t size)
{
    uint32_t remaining_len, read_length = 0;
    uint32_t aligned_addr;
    uint32_t item_number;

    /* The max size of flash data_width is 4 bytes. */
    uint8_t temp_buffer[sizeof(uint32_t)];
    ARM_FLASH_CAPABILITIES DriverCapabilities;
    uint8_t data_width;
    const ARM_DRIVER_FLASH *flash_dev = cfg->context;
    int ret;

    DriverCapabilities = flash_dev->GetCapabilities();
    data_width = data_width_byte[DriverCapabilities.data_width];

    /*
     * CMSIS ARM_FLASH_ReadData API requires the `addr` data type size
     * aligned. Data type size is specified by the data_width in
     * ARM_FLASH_CAPABILITIES.
     */
    aligned_addr = (addr / data_width) * data_width;
    remaining_len = size;

    /* Read the first data_width bytes data if `addr` is not aligned. */
    if (aligned_addr != addr) {
        ret = flash_dev->ReadData(aligned_addr, temp_buffer, 1);
        if (ret < 0) {
            return PSA_ERROR_STORAGE_FAILURE;
        }

        /* Record how many target data have been read. */
        read_length = ((addr - aligned_addr + size >= data_width) ?
         (data_width - (addr - aligned_addr)) : size);

        /* Copy the read data. */
        memcpy(buff, temp_buffer + addr - aligned_addr, read_length);
        remaining_len -= read_length;
    }

    /*
     * The `cnt` parameter in CMSIS ARM_FLASH_ReadData indicates number of
     * data items to read.
     */
    if (remaining_len) {
        item_number = remaining_len / data_width;
        if (item_number) {
            ret = flash_dev->ReadData(
                                              addr + read_length,
                                              (uint8_t *)buff + read_length,
                                              item_number);
            if (ret < 0) {
                return PSA_ERROR_STORAGE_FAILURE;
            }
            read_length += item_number * data_width;
            remaining_len -= item_number * data_width;
        }
    }

    /* Read the last data item if there is still remaing data. */
    if (remaining_len) {
        ret = flash_dev->ReadData(addr + read_length, temp_buffer, 1);
        if (ret < 0) {
            return PSA_ERROR_STORAGE_FAILURE;
        }
        /* Copy the read data. */
        memcpy(buff + read_length, temp_buffer, remaining_len);
    }

    return PSA_SUCCESS;
}
static psa_status_t its_flash_nor_read(const struct its_flash_config_t *cfg,
                                       uint32_t block_id, uint8_t *buff,
                                       size_t offset, size_t size)
{
    uint32_t addr;

    if (size == 0) {
        return PSA_SUCCESS;
    }
    addr = get_phys_address(cfg, block_id, offset);
    return flash_read_unaligned(cfg, addr, buff, size);
}

static psa_status_t its_flash_nor_write(const struct its_flash_config_t *cfg,
                                        uint32_t block_id, const uint8_t *buff,
                                        size_t offset, size_t size)
{
    int32_t err;
    ARM_FLASH_CAPABILITIES DriverCapabilities;
    uint32_t addr;
    uint8_t data_width;
    const ARM_DRIVER_FLASH *flash_dev = cfg->context;

    DriverCapabilities = flash_dev->GetCapabilities();
    data_width = data_width_byte[DriverCapabilities.data_width];

    addr = get_phys_address(cfg, block_id, offset);

    err = flash_dev->ProgramData(addr, buff, size / data_width);
    if (err < 0) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    return PSA_SUCCESS;
}

static psa_status_t its_flash_nor_flush(const struct its_flash_config_t *cfg,
                                        uint32_t block_id)
{
    /* Nothing needs to be done for NOR flash, as writes are commited to flash
     * immediately.
     */
    (void)cfg;
    (void)block_id;
    return PSA_SUCCESS;
}

static psa_status_t its_flash_nor_erase(const struct its_flash_config_t *cfg,
                                        uint32_t block_id)
{
    int32_t err;
    uint32_t addr;
    size_t offset;
    const ARM_DRIVER_FLASH *flash_dev = cfg->context;
    ARM_FLASH_INFO *flash_info = flash_dev->GetInfo();
    uint32_t sector_size = flash_info != NULL ? flash_info->sector_size : 0;
    if (sector_size == 0) {
        return PSA_ERROR_STORAGE_FAILURE;
    }

    for (offset = 0; offset < cfg->block_size; offset += sector_size) {
        addr = get_phys_address(cfg, block_id, offset);

        err = flash_dev->EraseSector(addr);
        if (err != ARM_DRIVER_OK) {
            return PSA_ERROR_STORAGE_FAILURE;
        }
    }

    return PSA_SUCCESS;
}

const struct its_flash_ops_t its_flash_ops_nor = {
    .init = its_flash_nor_init,
    .read = its_flash_nor_read,
    .write = its_flash_nor_write,
    .flush = its_flash_nor_flush,
    .erase = its_flash_nor_erase,
};
