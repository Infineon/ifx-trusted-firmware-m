/*
 * Copyright (c) 2017-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2022-2024 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file  its_flash_hal.h
 *
 * \brief Internal Trusted Storage file system driver abstraction APIs.
 *        The purpose of this abstraction is to provide interface between
 *        ITS file system and implementation of storage back-end.
 *
 * \par Encryption of ITS storage
 * \parblock
 * TF-M can use this layer to implement additional encryption layer for Internal
 * Trusted Storage by implementing of ITS FS driver that is storage independent
 * and sits between ITS file system and storage located on a physical device.
 *
 * Write sequence with encryption of ITS:
 *   -# TFM ITS FS calls storage write operation (\ref its_flash_ops_t.write)
 *      that is handled by encryption driver.
 *   -# Encryption driver uses PSA Crypto API to encrypt chunk and pass encrypted
 *      data to underlying storage driver by calling \ref its_flash_ops_t.write.
 *   -# Underlying storage driver (NOR/NAND/RAM or platform specific) store
 *      encrypted data on the physical device.
 *
 * Read sequence with encryption of ITS:
 *   -# TFM ITS FS calls storage read operation (\ref its_flash_ops_t.read) that
 *      is handled by encryption driver.
 *   -# Encryption driver uses underlying storage driver to read encrypted chunk
 *      by calling \ref its_flash_ops_t.read.
 *   -# Underlying storage driver (NOR/NAND/RAM or platform specific) read
 *      encrypted data from the physical device.
 *   -# Encryption driver uses PSA Crypto API to decrypt chunk of data into result
 *      buffer of \ref its_flash_ops_t.read request.
 *
 * So, from ITS FS point of view there is no need to change anything. All magic
 * could be implemented during initialization of ITS service.
 * \endparblock.
 *
 * \todo ITS file system driver currently depends on sector size, see
 * \ref its_flash_info_t.sector_size. This dependency make no sense for ITS file
 * system or Internal Storage service. Sector size is used only by Internal
 * Storage service to calculate block size during initialization. It make sense
 * to remove \ref its_flash_info_t and \ref its_flash_ops_t.get_info.
 */

#ifndef __ITS_FLASH_HAL_H__
#define __ITS_FLASH_HAL_H__

#include <stdint.h>
#include <stddef.h>
#include <psa/error.h>


/* Invalid block index */
#define ITS_BLOCK_INVALID_ID 0xFFFFFFFFU

/**
 * \brief Structure containing ITS flash driver instance configuration used by
 * ITS flash FS layer.
 *
 * There are values which should be initialized by \ref its_flash_ops_t.init of
 * \ref ops or set to constant values during allocation of this structure:
 * \li \ref erase_val
 * \li \ref program_unit
 *
 * Following members are setup by ITS:
 * \li \ref block_size
 * \li \ref flash_area_addr
 * \li \ref num_blocks
 */
struct its_flash_config_t {
    const void *context;            /**< ITS flash driver context */
    uint32_t flash_area_addr; /**< Base address of the flash region in flash
                               *   memory space. It's not mandatory that this
                               *   address is mapped to MCU address space.
                               */
    uint32_t block_size;      /**< Size of a logical filesystem erase unit, a
                               *   multiple of sector_size.
                               */
    uint16_t num_blocks;      /**< Number of logical erase blocks */
    uint16_t program_unit;    /**< Minimum size of a program operation */
    uint8_t erase_val;        /**< Value of a byte after erase (usually 0xFF) */
};

/**
 * \brief Structure containing the ITS flash driver operations.
 */
struct its_flash_ops_t {
    /**
     * \brief Initializes the flash device.
     *
     * \param[in/out] cfg  Flash driver configuration
     *
     * \return Returns PSA_SUCCESS if the function is executed correctly.
     *         Otherwise, it returns PSA_ERROR_STORAGE_FAILURE.
     */
    psa_status_t (*init)(struct its_flash_config_t *cfg);

    /**
     * \brief Reads block data from the position specified by block ID and
     *        offset.
     *
     * \param[in]  cfg       Flash driver configuration
     * \param[in]  block_id  Block ID
     * \param[out] buf       Buffer pointer to store the data read
     * \param[in]  offset    Offset position from the init of the block
     * \param[in]  size      Number of bytes to read
     *
     * \note This function assumes all input values are valid. That is, the
     *       address range, based on block_id, offset and size, is a valid range
     *       in flash.
     *
     * \return Returns PSA_SUCCESS if the function is executed correctly.
     *         Otherwise, it returns PSA_ERROR_STORAGE_FAILURE.
     */
    psa_status_t (*read)(const struct its_flash_config_t *cfg,
                         uint32_t block_id, uint8_t *buf, size_t offset,
                         size_t size);

    /**
     * \brief Writes block data to the position specified by block ID and
     *        offset.
     *
     * \param[in] cfg       Flash driver configuration
     * \param[in] block_id  Block ID
     * \param[in] buf       Buffer pointer to the write data
     * \param[in] offset    Offset position from the init of the block
     * \param[in] size      Number of bytes to write
     *
     * \note This function assumes all input values are valid. That is, the
     *       address range, based on block_id, offset and size, is a valid range
     *       in flash.
     *
     * \return Returns PSA_SUCCESS if the function is executed correctly.
     *         Otherwise, it returns PSA_ERROR_STORAGE_FAILURE.
     */
    psa_status_t (*write)(const struct its_flash_config_t *cfg,
                          uint32_t block_id, const uint8_t *buf, size_t offset,
                          size_t size);

    /**
     * \brief Flushes modifications to a block to flash. Must be called after a
     *        sequence of calls to write() (including via
     *        its_flash_block_to_block_move()) for one block ID, before any call
     *        to the same functions for a different block ID.
     *
     * \param[in] cfg  Flash driver configuration
     *
     * \param[in] block_id  Block ID
     *
     * \note It is permitted for write() to commit block updates immediately, in
     *       which case this function is a no-op.
     *
     * \return Returns PSA_SUCCESS if the function is executed correctly.
     *         Otherwise, it returns PSA_ERROR_STORAGE_FAILURE.
     */
    psa_status_t (*flush)(const struct its_flash_config_t *cfg,
                          uint32_t block_id);

    /**
     * \brief Erases block ID data.
     *
     * \param[in] cfg       Flash driver configuration
     * \param[in] block_id  Block ID
     *
     * \note This function assumes the input value is valid.
     *
     * \return Returns PSA_SUCCESS if the function is executed correctly.
     *         Otherwise, it returns PSA_ERROR_STORAGE_FAILURE.
     */
    psa_status_t (*erase)(const struct its_flash_config_t *cfg,
                          uint32_t block_id);
};

#endif /* __ITS_FLASH_HAL_H__ */
