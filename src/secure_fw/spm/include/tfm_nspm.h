/*
 * Copyright (c) 2018-2024, Arm Limited. All rights reserved.
 * Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_NSPM_H__
#define __TFM_NSPM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "security_defs.h"

#define TFM_NS_CLIENT_INVALID_ID            ((int32_t)0)

/*
 * NSPM functions can be built either into the SPE or NSPE.
 * They're secure gateway functions only if we're using TZ
 * and we're not using a shared memory interface
 */
#if (defined CONFIG_TFM_USE_TRUSTZONE) && !(defined TFM_NSID_IN_SHARED_DATA)
#else
#undef __tz_c_veneer
#define __tz_c_veneer
#endif /* CONFIG_TFM_USE_TRUSTZONE && !TFM_NSID_IN_SHARED_DATA */

/**
 * \brief initialise the NS context database
 */
void tfm_nspm_ctx_init(void);

/**
 * \brief Get the client ID of the current NS client
 *
 * \return The client id of the current NS client. 0 (invalid client id) is
 *         returned in case of error.
 */
int32_t tfm_nspm_get_current_client_id(void);

/**
 * \brief Register a non-secure client ID range.
 *
 * \param[in] client_id_base  The minimum client ID for this client.
 * \param[in] client_id_limit The maximum client ID for this client.
 */
void tz_ns_agent_register_client_id_range(int32_t client_id_base,
                                          int32_t client_id_limit);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_NSPM_H__ */
