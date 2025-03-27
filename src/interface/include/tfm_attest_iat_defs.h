/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_ATTEST_IAT_DEFS_H__
#define __TFM_ATTEST_IAT_DEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "config_tfm.h"

#if ATTEST_TOKEN_PROFILE_PSA_2_0_0

/* In case of PSA_2_0_0 profile */
#define IAT_NONCE                          10  /* EAT nonce */
#define IAT_INSTANCE_ID                    256 /* EAT ueid */
#define IAT_PROFILE_DEFINITION             265 /* EAT eat_profile */
#define IAT_ARM_RANGE_BASE                 (2393)
#define IAT_CLIENT_ID                      (IAT_ARM_RANGE_BASE + 1)
#define IAT_SECURITY_LIFECYCLE             (IAT_ARM_RANGE_BASE + 2)
#define IAT_IMPLEMENTATION_ID              (IAT_ARM_RANGE_BASE + 3)
#define IAT_BOOT_SEED                      (IAT_ARM_RANGE_BASE + 4)
#define IAT_CERTIFICATION_REFERENCE        (IAT_ARM_RANGE_BASE + 5)
#define IAT_SW_COMPONENTS                  (IAT_ARM_RANGE_BASE + 6)
#define IAT_VERIFICATION_SERVICE           (IAT_ARM_RANGE_BASE + 7)

#elif ATTEST_TOKEN_PROFILE_ARM_CCA

/* In case of  ARM_CCA profile */
#define IAT_NONCE                          10  /* EAT nonce*/
#define IAT_INSTANCE_ID                    256 /* EAT ueid */
#define IAT_PROFILE_DEFINITION             265 /* EAT eat_profile */
#define IAT_ARM_RANGE_BASE                 (2393)
#define IAT_CLIENT_ID                      (IAT_ARM_RANGE_BASE + 1)
#define IAT_SECURITY_LIFECYCLE             (IAT_ARM_RANGE_BASE + 2)
#define IAT_IMPLEMENTATION_ID              (IAT_ARM_RANGE_BASE + 3)
#define IAT_BOOT_SEED                      (IAT_ARM_RANGE_BASE + 4)
#define IAT_CERTIFICATION_REFERENCE        (IAT_ARM_RANGE_BASE + 5)
#define IAT_SW_COMPONENTS                  (IAT_ARM_RANGE_BASE + 6)
#define IAT_VERIFICATION_SERVICE           (IAT_ARM_RANGE_BASE + 7)
#define IAT_PLATFORM_CONFIG                (IAT_ARM_RANGE_BASE + 8)
#define IAT_PLATFORM_HASH_ALGO_ID          (IAT_ARM_RANGE_BASE + 9)

#else
#error "Attestation token profile is incorrect"
#endif

#define IAT_SW_COMPONENT_MEASUREMENT_TYPE  (1)
#define IAT_SW_COMPONENT_MEASUREMENT_VALUE (2)
/* Reserved                                (3) */
#define IAT_SW_COMPONENT_VERSION           (4)
#define IAT_SW_COMPONENT_SIGNER_ID         (5)
#define IAT_SW_COMPONENT_MEASUREMENT_DESC  (6)

#ifdef __cplusplus
}
#endif

#endif /* __TFM_ATTEST_IAT_DEFS_H__ */
