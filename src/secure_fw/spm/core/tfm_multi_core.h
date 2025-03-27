/*
 * Copyright (c) 2019-2025, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_MULTI_CORE_H__
#define __TFM_MULTI_CORE_H__

#include <stdint.h>

#define CLIENT_ID_OWNER_MAGIC           (void *)0xFFFFFFFFU

/**
 * \brief Initialization of the multi core communication.
 *
 * \retval 0                    Operation succeeded.
 * \retval Other return code    Operation failed with an error code.
 */
int32_t tfm_inter_core_comm_init(void);

/**
 * \brief Register a non-secure client ID range.
 *
 * \param[in] owner           Identifier of the non-secure client.
 * \param[in] client_id_base  The minimum client ID for this client.
 * \param[in] client_id_limit The maximum client ID for this client.
 *
 * \return SPM_SUCCESS if the registration is successful, SPM_ERROR_GENERIC
 *         if owner is null, or no free slots left.
 */
int32_t tfm_multi_core_register_client_id_range(void *owner,
                                                int32_t client_id_base,
                                                int32_t client_id_limit);

/**
 * \brief Translate a non-secure client ID range.
 *
 * \param[in]  owner         Identifier of the non-secure client.
 * \param[in]  client_id_in  The input client ID.
 * \param[out] client_id_out The translated client ID. Undefined if
 *                           SPM_ERROR_GENERIC is returned by the function
 *
 * \return SPM_SUCCESS if the translation is successful, SPM_ERROR_GENERIC
 *         otherwise.
 */
int32_t tfm_multi_core_hal_client_id_translate(void *owner,
                                               int32_t client_id_in,
                                               int32_t *client_id_out);

#endif /* __TFM_MULTI_CORE_H__ */
