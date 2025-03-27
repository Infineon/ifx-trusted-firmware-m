/*
 * Copyright (c) 2023-2024 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdbool.h>

#include "config_tfm.h"
#include "ifx_regions.h"
#include "protection_sau.h"
#include "protection_regions_cfg.h"
#include "static_checks.h"
#include "region.h"
#include "utilities.h"

#if CONFIG_TFM_USE_TRUSTZONE
/* Number of additional SAU regions for Veneer regions */
#define IFX_SAU_VEENER_REGION_COUNT      1U
#else
/* There is no Veener region without CM33 NS image */
#define IFX_SAU_VEENER_REGION_COUNT      0U
#endif

/* Number of reserved SAU regions */
#define IFX_SAU_RESERVED_REGION_COUNT    IFX_SAU_VEENER_REGION_COUNT     /* Veener region */

TFM_COVERITY_DEVIATE_BLOCK(overrun,      "CY_SAU_REGION_CNT comparisons and SAU_config operations are fair")
TFM_COVERITY_DEVIATE_BLOCK(cert_arr30_c, "CY_SAU_REGION_CNT comparisons are fair")
TFM_COVERITY_DEVIATE_BLOCK(cert_int30_c, "CY_SAU_REGION_CNT comparisons and SAU_config operations are fair")
TFM_COVERITY_DEVIATE_BLOCK(cert_str31_c, "CY_SAU_REGION_CNT comparisons are fair")
FIH_RET_TYPE(enum tfm_hal_status_t) ifx_sau_cfg(void)
{
    if (CY_SAU_REGION_CNT > (CPUSS_CM33_0_SAU_REGION_NR - IFX_SAU_RESERVED_REGION_COUNT)) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_INVALID_INPUT));
    }

    /*
    * Starts changing actual configuration so issue DMB to ensure every
    * transaction has completed by now
    */
    __DMB();

    /* Disable SAU */
    TZ_SAU_Disable();

    uint32_t idx;
    /* Configures SAU regions to be Non-Secure */
    for(idx = 0UL; idx < CY_SAU_REGION_CNT; idx++) {
        /* Check are address and size aligned to 32 bytes */
        SPM_ASSERT((SAU_config[idx].base_addr & ~SAU_RBAR_BADDR_Msk) == 0U);
        SPM_ASSERT((SAU_config[idx].size & ~SAU_RLAR_LADDR_Msk) == 0U);

        /* Non-Secure Callable region is configured by TFM only */
        SPM_ASSERT(!SAU_config[idx].nsc);

        TFM_COVERITY_DEVIATE_BLOCK(cert_int36_c, "Converting to a pointer type SAU_Type * is fair.")
        SAU->RNR  = _VAL2FLD(SAU_RNR_REGION, idx); /* Set SAU Region number */
        TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_18_1, "Assignment is correct. As SAU->RBAR and ifx_sau_config.base_addr both have the same uint32_t type")
        SAU->RBAR =  SAU_config[idx].base_addr;    /* Base address of SAU region*/
        /* Limit address must be processed before writing to register:
         * Subtracting 1 is applied to get the last address that belongs to the region
         * Setting last 5 bits to 0 by mask is applied because address must
         * be aligned to multiple of 32 bytes */
        TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_18_1, "Assignment is correct. As all the comparison members have the same uint32_t type")
        SAU->RLAR = ((SAU_config[idx].base_addr + SAU_config[idx].size - 1U) & SAU_RLAR_LADDR_Msk) | /* Limit address of SAU region */
                    _VAL2FLD(SAU_RLAR_ENABLE, 1); /* 1 - Enable SAU region */
        TFM_COVERITY_BLOCK_END(cert_int36_c)
    }

#if CONFIG_TFM_USE_TRUSTZONE
    /* Configures VENEER region as Non-Secure Callable (NSC) */
    /* Check are addresses aligned to 32 bytes */
    SPM_ASSERT(((uint32_t)&REGION_NAME(Image$$, ER_VENEER, $$Base)     & ~SAU_RBAR_BADDR_Msk) == 0U);
    SPM_ASSERT(((uint32_t)&REGION_NAME(Image$$, VENEER_ALIGN, $$Limit) & ~SAU_RLAR_LADDR_Msk) == 0U);

    TFM_COVERITY_DEVIATE_BLOCK(cert_int36_c, "converting to a pointer type SAU_Type * is fair.")
    SAU->RNR  = _VAL2FLD(SAU_RNR_REGION, idx); /* Set SAU Region number */
    SAU->RBAR = (uint32_t)&REGION_NAME(Image$$, ER_VENEER, $$Base); /* Base address of Veneer SAU region */
    SAU->RLAR = (((uint32_t)&REGION_NAME(Image$$, VENEER_ALIGN, $$Limit) - 1U) & SAU_RLAR_LADDR_Msk) | /* Limit address of Veneer SAU region */
                              _VAL2FLD(SAU_RLAR_NSC, 1) |    /* 1 - Veneer SAU region is Non-Secure Callable */
                              _VAL2FLD(SAU_RLAR_ENABLE, 1);  /* 1 - Enable Veneer SAU region */
    TFM_COVERITY_BLOCK_END(cert_int36_c)
    idx++;
#endif

    /* Disable unused SAU regions */
    TFM_COVERITY_DEVIATE_BLOCK(deadcode, "User can define variable number of SAU regions, thus this code can be unreachable in some cases")
    TFM_COVERITY_DEVIATE_BLOCK(MISRA_C_2023_Rule_14_3, "User can define variable number of SAU regions, thus this code can be unreachable in some cases")
    for(; idx < CPUSS_CM33_0_SAU_REGION_NR; idx++) {
        /* Set SAU Region number */
        SAU->RNR = _VAL2FLD(SAU_RNR_REGION, idx);
        /* Clear Enable bit (and limit) */
        SAU->RLAR = 0;
    }
    TFM_COVERITY_BLOCK_END(MISRA_C_2023_Rule_14_3)
    TFM_COVERITY_BLOCK_END(deadcode)

    /* Enable SAU */
    TZ_SAU_Enable();

    /* Add barriers to assure the SAU configuration is done before continue
     * the execution.
     */
    __DSB();
    __ISB();

    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}
TFM_COVERITY_BLOCK_END(cert_str31_c)
TFM_COVERITY_BLOCK_END(cert_int30_c)
TFM_COVERITY_BLOCK_END(cert_arr30_c)
TFM_COVERITY_BLOCK_END(overrun)

#ifdef TFM_FIH_PROFILE_ON
/**
 * \brief Validates enabled SAU region configuration.
 *
 * \param[in] idx       SAU region index
 * \param[in] address   SAU region base address
 * \param[in] size      SAU region size
 * \param[in] nsc       SAU NSC callable flag
 *
 * \return  TFM_HAL_SUCCESS       - validation passed
 *          TFM_HAL_ERROR_GENERIC - validation failed
 */
static FIH_RET_TYPE(enum tfm_hal_status_t) ifx_sau_verify_region(uint32_t idx,
                                                                 uint32_t address,
                                                                 uint32_t size,
                                                                 bool nsc)
{
    TFM_COVERITY_DEVIATE_BLOCK(cert_int36_c, "Converting to a pointer type SAU_Type * is fair.")
    SAU->RNR = _VAL2FLD(SAU_RNR_REGION, idx);                          /* Set SAU Region number */

    /* Base address of SAU region */
    if ((SAU->RBAR != address) ||
        (SAU->RLAR != (((address + size - 1U) & SAU_RLAR_LADDR_Msk) |   /* Limit address of SAU region */
                       _VAL2FLD(SAU_RLAR_NSC, nsc ? 1 : 0) |
                       _VAL2FLD(SAU_RLAR_ENABLE, 1)))) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }
    TFM_COVERITY_BLOCK_END(cert_int36_c)

    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}

FIH_RET_TYPE(enum tfm_hal_status_t) ifx_sau_verify_static_boundaries(void)
{
    FIH_RET_TYPE(enum tfm_hal_status_t) fih_rc = fih_int_encode(TFM_HAL_ERROR_GENERIC);
    uint32_t idx;

    /* Validate SAU regions against configuration */
    for(idx = 0UL; idx < CY_SAU_REGION_CNT; idx++) {
        FIH_CALL(ifx_sau_verify_region, fih_rc,
                 idx, SAU_config[idx].base_addr, SAU_config[idx].size,
                 false);
        if (fih_not_eq(fih_rc, fih_int_encode(TFM_HAL_SUCCESS))) {
            FIH_RET(fih_rc);
        }
    }

#if CONFIG_TFM_USE_TRUSTZONE
    FIH_CALL(ifx_sau_verify_region, fih_rc, idx,
            (uint32_t)&REGION_NAME(Image$$, ER_VENEER, $$Base),
            (uint32_t)&REGION_NAME(Image$$, VENEER_ALIGN, $$Limit) - (uint32_t)&REGION_NAME(Image$$, ER_VENEER, $$Base),
            true);
    if (fih_not_eq(fih_rc, fih_int_encode(TFM_HAL_SUCCESS))) {
        FIH_RET(fih_rc);
    }
    idx++;
#endif

    /* Check unused SAU regions */
    TFM_COVERITY_DEVIATE_BLOCK(deadcode, "User can define variable number of SAU regions, thus this code can be unreachable in some cases")
    TFM_COVERITY_DEVIATE_BLOCK(MISRA_C_2023_Rule_14_3, "User can define variable number of SAU regions, thus this code can be unreachable in some cases")
    for(; idx < CPUSS_CM33_0_SAU_REGION_NR; idx++) {
        /* Set SAU Region number */
        SAU->RNR = _VAL2FLD(SAU_RNR_REGION, idx);

        /* Unused SAU region must be disabled */
        if ((SAU->RLAR & _VAL2FLD(SAU_RLAR_ENABLE, 1)) != 0) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
        }
    }
    TFM_COVERITY_BLOCK_END(MISRA_C_2023_Rule_14_3)
    TFM_COVERITY_BLOCK_END(deadcode)

    FIH_RET(fih_rc);
}
#endif /* TFM_FIH_PROFILE_ON */
