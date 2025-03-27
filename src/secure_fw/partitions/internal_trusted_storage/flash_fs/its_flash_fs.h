/*
 * Copyright (c) 2018-2023, Arm Limited. All rights reserved.
 * Copyright (c) 2020-2022 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 * \file  its_flash_fs.h
 *
 * \brief Internal Trusted Storage service filesystem abstraction APIs.
 *        The purpose of this abstraction is to have the ability to plug-in
 *        other filesystems or filesystem proxies (supplicant).
 */

#ifndef __ITS_FLASH_FS_H__
#define __ITS_FLASH_FS_H__

#include <stddef.h>
#include <stdint.h>

#include "its_flash_fs_mblock.h"
#include "psa/error.h"
#include "../flash/its_flash_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Space reserved for user-defined flags. These are stored in the file metadata
 * and can be retrieved by getting the file info.
 */
#define ITS_FLASH_FS_USER_FLAGS_MASK   ((1UL << 16) - 1)

/* Filesystem flags that can be provided when calling the write function */
#define ITS_FLASH_FS_WRITE_FLAGS_MASK  ((1UL << 24) - (1UL << 16))
/* Create the file if it does not already exist */
#define ITS_FLASH_FS_FLAG_CREATE       (1UL << 16)
/* Remove existing file data if it exists */
#define ITS_FLASH_FS_FLAG_TRUNCATE     (1UL << 17)

/**
 * \struct its_flash_fs_config_t
 *
 * \brief Structure containing the flash filesystem configuration parameters.
 */
struct its_flash_fs_config_t {
    struct its_flash_config_t *flash_cfg;    /**< Pointer to the flash device */
    const struct its_flash_ops_t *ops;    /**< Filesystem flash operations */
    uint16_t max_file_size;   /**< Maximum file size */
    uint16_t max_num_files;   /**< Maximum number of files */
};

/**
 * \brief ITS flash filesystem context type, used to maintain state across FS
 *        operations.
 *
 * \details The user should allocate a variable of this type, initialised to
 *          zero, before calling its_flash_fs_prepare, and then pass it to each
 *          subsequent FS operation. The contents are internal to the
 *          filesystem.
 */
typedef struct its_flash_fs_ctx_t its_flash_fs_ctx_t;

/*!
 * \struct its_flash_fs_file_info_t
 *
 * \brief Structure containing file information.
 *
 * \details This structure is not written to the filesystem, it is used by the
 *          file system functions to simplify accessing the containing
 *          information.
 */
struct its_flash_fs_file_info_t {
    size_t size_current;  /*!< The current size of the file in bytes */
    size_t size_max;      /*!< The maximum size of the file in bytes. */
    uint32_t flags;       /*!< Flags set when the file was created */
#ifdef ITS_ENCRYPTION
    /*!< Additional authenticated data */
    uint8_t add[ITS_FILE_ID_SIZE + ITS_DATA_SIZE_FIELD_SIZE + ITS_FLAG_SIZE];
    uint8_t nonce[12];/*!< Nonce/IV for encrypted files */
    uint8_t tag[16];   /*!< Authentication tag */
#endif
};

/**
 * \brief Initialises the filesystem context. Must be called successfully before
 *        any other filesystem API is called.
 *
 * \param[in,out] fs_ctx  Filesystem context to initialise. Must have been
 *                        allocated by the caller.
 * \param[in,out] fs_cfg  Filesystem configuration to associate with the
 *                        context.
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_init_ctx(its_flash_fs_ctx_t *fs_ctx,
                                   const struct its_flash_fs_config_t *fs_cfg);

/**
 * \brief Prepares the filesystem to accept operations on the files.
 *
 * \param[in,out] fs_ctx  Filesystem context to prepare. Must have been
 *                        initialised by its_flash_fs_init_ctx().
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_prepare(its_flash_fs_ctx_t *fs_ctx);

/**
 * \brief Wipes all files from the filesystem.
 *
 * \param[in,out] fs_ctx  Filesystem context to wipe. Must be prepared again
 *                        before further use.
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_wipe_all(its_flash_fs_ctx_t *fs_ctx);

/**
 * \brief Gets the file information referenced by the file ID.
 *
 * \param[in,out] fs_ctx  Filesystem context
 * \param[in]     fid     File ID
 * \param[out]    info    Pointer to the file information
 *                        structure \ref its_flash_fs_file_info_t
 *
 * \return Returns error code specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_file_get_info(its_flash_fs_ctx_t *fs_ctx,
                                        const uint8_t *fid,
                                        struct its_flash_fs_file_info_t *info);

/**
 * \brief Writes data to a file.
 *
 * \param[in,out] fs_ctx     Filesystem context
 * \param[in]     fid        File ID
 * \param[in]     finfo      Pointer to \ref its_flash_fs_file_info_t
 * \param[in]     data_size  Size of the incoming write data.
 * \param[in]     offset     Offset in the file to write. Must be less than or
 *                           equal to the current file size.
 * \param[in]     data       Pointer to buffer containing data to be written
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_file_write(its_flash_fs_ctx_t *fs_ctx,
                                     const uint8_t *fid,
                                     struct its_flash_fs_file_info_t *finfo,
                                     size_t data_size,
                                     size_t offset,
                                     const uint8_t *data);

/**
 * \brief Reads data from an existing file.
 *
 * \param[in,out] fs_ctx  Filesystem context
 * \param[in]     fid     File ID
 * \param[in]     size    Size to be read
 * \param[in]     offset  Offset in the file
 * \param[out]    data    Pointer to buffer to store the data
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_file_read(its_flash_fs_ctx_t *fs_ctx,
                                    const uint8_t *fid,
                                    size_t size,
                                    size_t offset,
                                    uint8_t *data);

/**
 * \brief Deletes file referenced by the file ID.
 *
 * \param[in,out] fs_ctx  Filesystem context
 * \param[in]     fid     File ID
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
psa_status_t its_flash_fs_file_delete(its_flash_fs_ctx_t *fs_ctx,
                                      const uint8_t *fid);

#ifdef __cplusplus
}
#endif

#endif /* __ITS_FLASH_FS_H__ */
