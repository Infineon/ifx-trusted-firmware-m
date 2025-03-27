/*
 * Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef PLATFORM_OTP_IDS_H
#define PLATFORM_OTP_IDS_H

#include <stdint.h>

/* Infineon TF-M code use tfm_otp_element_id_t only for specific things */
enum tfm_otp_element_id_t {
    /* Is used to read security lifecycle for tfm_spm_get_lifecycle_state */
    PLAT_OTP_ID_LCS = 1,

    PLAT_OTP_ID_MAX = UINT32_MAX,
};

#endif /* PLATFORM_OTP_IDS_H */
