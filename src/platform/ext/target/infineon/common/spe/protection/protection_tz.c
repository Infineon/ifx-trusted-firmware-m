/*
 * Copyright (c) 2015-2025, Arm Limited. All rights reserved.
 * Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <arm_cmse.h>
#include <stdint.h>
#include <stdbool.h>
#include "config_impl.h"
#include "config_tfm.h"
#include "protection_tz.h"
#include "protection_utils.h"
#include "static_checks.h"
#include "tfm_hal_isolation.h"
#include "ifx_utils.h"

/**
 * \brief FIH implementation of cmse_check_address_range.
 *
 * \param[in]   p             The base address of the region.
 * \param[in]   size          The size of the region.
 * \param[in]   flags         The memory access types to be checked between
 *                            given memory and boundaries.
 *
 * \return TFM_HAL_SUCCESS - The memory region has the access permissions by TrustZone.
 *         TFM_HAL_ERROR_MEM_FAULT - The memory region has not the access
 *                                   permissions by TrustZone.
 *         TFM_HAL_ERROR_INVALID_INPUT - Invalid inputs.
 */
static FIH_RET_TYPE(enum tfm_hal_status_t) ifx_cmse_check_address_range(uintptr_t p, size_t size, uint32_t flags)
{
    cmse_address_info_t permb, perme;
    char *pb = (char *) p, *pe;

    /* Check if the range wraps around.  */
    if (UINTPTR_MAX - p < size) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_INVALID_INPUT));
    }

    /* Check if an unknown flag is present.  */
    uint32_t known = ((uint32_t)CMSE_MPU_UNPRIV) | ((uint32_t)CMSE_MPU_READWRITE) | ((uint32_t)CMSE_MPU_READ);
    uint32_t known_secure_level = CMSE_MPU_UNPRIV;
    #if (__ARM_FEATURE_CMSE & 2) != 0
    known |= ((uint32_t)CMSE_AU_NONSECURE) | ((uint32_t)CMSE_MPU_NONSECURE);
    known_secure_level |= (uint32_t)CMSE_MPU_NONSECURE;
    #endif
    if (flags & (~known)) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_INVALID_INPUT));
    }

    /* Execute the right variant of the TT instructions.  */
    pe = pb + size - 1;
    const bool singleCheck = (((uintptr_t)pb ^ (uintptr_t)pe) < 32U);
    TFM_COVERITY_DEVIATE_BLOCK(MISRA_C_2023_Rule_16_3, "Return used as unconditional break statement, discuss with Coverity why this is flagged.")
    TFM_COVERITY_DEVIATE_BLOCK(MISRA_C_2023_Rule_16_1, "Same as above, return used as unconditional break statement")
    switch (flags & known_secure_level) {
    case 0:
        permb = cmse_TT (pb);
        perme = singleCheck ? permb : cmse_TT (pe);
        break;
    case CMSE_MPU_UNPRIV:
        permb = cmse_TTT (pb);
        perme = singleCheck ? permb : cmse_TTT (pe);
        break;
    #if (__ARM_FEATURE_CMSE & 2) != 0
    case CMSE_MPU_NONSECURE:
        permb = cmse_TTA (pb);
        perme = singleCheck ? permb : cmse_TTA (pe);
        break;
    case (((uint32_t)CMSE_MPU_UNPRIV) | ((uint32_t)CMSE_MPU_NONSECURE)):
        permb = cmse_TTAT (pb);
        perme = singleCheck ? permb : cmse_TTAT (pe);
        break;
    #endif
    default:
        /* Invalid flag, eg.  CMSE_MPU_NONSECURE specified but __ARM_FEATURE_CMSE & 2 == 0.  */
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_INVALID_INPUT));
    }
    TFM_COVERITY_BLOCK_END(MISRA_C_2023_Rule_16_1)
    TFM_COVERITY_BLOCK_END(MISRA_C_2023_Rule_16_3)

    /* Check that the range does not cross MPU, SAU, or IDAU boundaries.  */
    if (permb.value != perme.value) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
    }

    /* Removes MISRA C:2012 Rule 2.2 */
    (void)perme;

    /* Check the permissions on the range.  */
    TFM_COVERITY_DEVIATE_BLOCK(MISRA_C_2023_Rule_16_3, "Return used as unconditional break statement, discuss with Coverity why this is flagged.")
    TFM_COVERITY_DEVIATE_BLOCK(MISRA_C_2023_Rule_16_1, "Same as above, return used as unconditional break statement")
    switch (flags & (~known_secure_level))
    {
    #if (__ARM_FEATURE_CMSE & 2) != 0
    case (((uint32_t)CMSE_MPU_READ) | ((uint32_t)CMSE_MPU_READWRITE) | ((uint32_t)CMSE_AU_NONSECURE)):
    case (((uint32_t)CMSE_MPU_READWRITE) | ((uint32_t)CMSE_AU_NONSECURE)):
        if (permb.flags.nonsecure_readwrite_ok) {
            FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
        }
        break;
    case (((uint32_t)CMSE_MPU_READ) | ((uint32_t)CMSE_AU_NONSECURE)):
        if (permb.flags.nonsecure_read_ok) {
            FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
        }
        break;
    case CMSE_AU_NONSECURE:
        if (permb.flags.secure == 0U) {
            FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
        }
        break;
    #endif
    case (((uint32_t)CMSE_MPU_READ) | ((uint32_t)CMSE_MPU_READWRITE)):
    case CMSE_MPU_READWRITE:
        if (permb.flags.readwrite_ok) {
            FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
        }
        break;
    case CMSE_MPU_READ:
        if (permb.flags.read_ok) {
            FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
        }
        break;
    default:
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
    }
    TFM_COVERITY_BLOCK_END(MISRA_C_2023_Rule_16_1)
    TFM_COVERITY_BLOCK_END(MISRA_C_2023_Rule_16_3)

    FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
}

FIH_RET_TYPE(enum tfm_hal_status_t) ifx_tz_memory_check(const ifx_partition_info_t *p_info,
                                                        uintptr_t base,
                                                        size_t size,
                                                        uint32_t access_type)
{
    FIH_RET_TYPE(enum tfm_hal_status_t) fih_rc = fih_int_encode(TFM_HAL_ERROR_GENERIC);
    fih_int is_non_secure = (access_type & TFM_HAL_ACCESS_NS) != 0u ? FIH_SUCCESS : FIH_FAILURE;
    /* ifx_tz_memory_check only checks on-core protections (SAU, IDAU, MPU),
     * so this check is not applicable for off core request coming from NS side of mailbox. */
    TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_10_1, "external macro IS_NS_AGENT_MAILBOX interprets non-boolean type as a boolean")
    if (IS_NS_AGENT_MAILBOX(p_info->p_ldinfo) && fih_eq(is_non_secure, FIH_SUCCESS)) {
        FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
    }

    uint32_t flags = 0;

    (void)fih_delay();
    if ((access_type & TFM_HAL_ACCESS_READWRITE) == TFM_HAL_ACCESS_READWRITE) {
        flags |= (uint32_t)CMSE_MPU_READWRITE;
    } else if ((access_type & TFM_HAL_ACCESS_READABLE) != 0u) {
        flags |= (uint32_t)CMSE_MPU_READ;
    } else {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_INVALID_INPUT));
    }

    (void)fih_delay();
    TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_10_1, "external macro IS_NS_AGENT_TZ interprets non-boolean type as a boolean")
    if (IS_NS_AGENT_TZ(p_info->p_ldinfo)) {
        CONTROL_Type ctrl;
        ctrl.w = __TZ_get_CONTROL_NS();
        if (ctrl.b.nPRIV == 1U) {
            flags |= (uint32_t)CMSE_MPU_UNPRIV;
        } else {
            flags &= ~(uint32_t)CMSE_MPU_UNPRIV;
        }
        flags |= (uint32_t)CMSE_NONSECURE;
    } else { /* Partition is Secure */
        if (IFX_IS_PARTITION_PRIVILEGED(p_info)) {
            flags &= ~(uint32_t)CMSE_MPU_UNPRIV;
        } else {
            flags |= (uint32_t)CMSE_MPU_UNPRIV;
        }
    }

    TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_20_7, "Cannot wrap with parentheses due to Fault injection architecture and define FIH_RET_TYPE")
    FIH_CALL(ifx_cmse_check_address_range, fih_rc,
             base, size, flags);
    if (fih_not_eq(fih_rc, fih_int_encode(TFM_HAL_SUCCESS))) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
    }

#if defined(TFM_FIH_PROFILE_ON) && !defined(TFM_FIH_PROFILE_LOW)
    (void)fih_delay();
    FIH_CALL(ifx_cmse_check_address_range, fih_rc,
             base, size, flags);
    if (fih_not_eq(fih_rc, fih_int_encode(TFM_HAL_SUCCESS))) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
    }
#endif

    FIH_RET(fih_rc);
}
