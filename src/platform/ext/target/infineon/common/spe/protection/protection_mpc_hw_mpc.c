/*
 * Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "config_impl.h"
#include "config_tfm.h"
#include "cy_mpc.h"
#include "fih.h"
#include "region_defs.h"
#include "protection_types.h"
#include "protection_utils.h"
#include "protection_regions_cfg.h"
#include "protection_mpc_api.h"
#include "region.h"
#include "ifx_regions.h"
#include "ifx_utils.h"
#include "tfm_hal_isolation.h"
#if IFX_SE_IPC_SERVICE_FULL || IFX_SE_IPC_SERVICE_BASE
#include "ifx_se_platform.h"
#include "ifx_se_tfm_utils.h"
#endif
#include "utilities.h"

#define IFX_MPC_BLOCK_SIZE_TO_BYTES(mpc_size)   (1UL << ((uint32_t)(mpc_size) + 5UL))

#define IFX_MAX_FIXED_CONFIGS 5

/* Additional configs that are internal to TFM.
 * The sizes can't be determined by the compiler as constants.
 * This array is populated by ifx_mpc_fill_fixed_config().
 */
static ifx_mpc_region_config_t ifx_fixed_mpc_static_config[IFX_MAX_FIXED_CONFIGS];

/* The number of used entries in \ref ifx_fixed_mpc_static_config */
static uint32_t ifx_fixed_mpc_static_config_count = 0;

#define IFX_MPC_TFM_PROT_AUTO_INC_DISABLE    (0UL)

#if IFX_SE_IPC_SERVICE_FULL || IFX_SE_IPC_SERVICE_BASE
/* The size of RRAM memory controlled by SE RT Services */
#define IFX_SE_RT_RRAM_SIZE                                 IFX_RRAM_SIZE

/* The number of RRAM MPC blocks controlled by SE RT Services */
#define IFX_SE_RT_RRAM_BLOCK_COUNT                          (IFX_SE_RT_RRAM_SIZE / IFX_SE_RT_RRAM_BLOCK_SIZE)

/* MPC attribute cache used to implement ifx_mpc_memory_check for RRAM MPC controlled
 * by SE RT Services in following format:
 *   2:0  - PC0 W/R/NS
 *   6:4  - PC1 W/R/NS
 *  10:8  - PC2 W/R/NS
 *  14:12 - PC3 W/R/NS
 *  18:16 - PC4 W/R/NS
 *  22:20 - PC5 W/R/NS
 *  26:24 - PC6 W/R/NS
 *  30:28 - PC7 W/R/NS
 *
 * For each nibble:
 * - bit[0] : NS (0 indicates secure, 1 indicates non-secure)
 * - bit[1] : R (0 indicates read access not allowed. 1 indicates read access allowed)
 * - bit[2] : W (0 indicates write access not allowed, 1 indicates write access allowed)
 */
static uint32_t ifx_mpc_se_rt_rram_attr[IFX_SE_RT_RRAM_BLOCK_COUNT];
#endif /* IFX_SE_IPC_SERVICE_FULL || IFX_SE_IPC_SERVICE_BASE */

#if IFX_SE_IPC_SERVICE_FULL || IFX_SE_IPC_SERVICE_BASE
/* Converted information provided by ifx_mpc_region_config_t to attributes for external MPC cache */
typedef struct {
    /*! Bit field with MPC cached attributes */
    uint32_t attr;
    /*! Mask for MPC cached attributes */
    uint32_t mask;
    /*! First block index in cached attributes */
    uint32_t first_block_idx;
    /*! Last block index in cached attributes */
    uint32_t last_block_idx;
} ifx_mpc_ext_cache_cfg_info_t;

/**
 * \brief Returns attributes for MPC attributes cache
 *
 * \param[in] mpc_reg_cfg   MPC configuration structure which will be applied.
 *                          See \ref ifx_mpc_region_config_t for more details.
 * \param[in] memory_config Information about asset's memory that is protected,
 *                          see \ref ifx_memory_config_t
 * \param[out] info         Information generated for requested configuration.
 *
 * \return    TFM_HAL_SUCCESS             - MPC configuration applied successfully.
 *            TFM_HAL_ERROR_INVALID_INPUT - invalid input (e.g. invalid arguments or
 *                                          configuration).
 */
static enum tfm_hal_status_t ifx_mpc_external_mpc_config_to_cache_info(
                                        const ifx_mpc_region_config_t* mpc_reg_cfg,
                                        const ifx_memory_config_t* memory_config,
                                        ifx_mpc_ext_cache_cfg_info_t *info)
{
    if (info == NULL) {
        return TFM_HAL_ERROR_INVALID_INPUT;
    }

    /* Default values to improve FIH resistance */
    info->attr = 0x00000000U;
    info->mask = 0xFFFFFFFFU;

    uint32_t block_size = IFX_MPC_BLOCK_SIZE_TO_BYTES(memory_config->mpc_block_size);
    if (IFX_SE_RT_RRAM_BLOCK_SIZE != block_size) {
        return TFM_HAL_ERROR_INVALID_INPUT;
    }

    uint32_t offset = IFX_S_ADDRESS_ALIAS(mpc_reg_cfg->address) - memory_config->s_address;
    info->first_block_idx = offset / IFX_SE_RT_RRAM_BLOCK_SIZE;
    info->last_block_idx  = IFX_ROUND_UP_TO_MULTIPLE(offset + mpc_reg_cfg->size, IFX_SE_RT_RRAM_BLOCK_SIZE) / IFX_SE_RT_RRAM_BLOCK_SIZE;
    if ((info->first_block_idx >= IFX_SE_RT_RRAM_BLOCK_COUNT) ||
        (info->last_block_idx > IFX_SE_RT_RRAM_BLOCK_COUNT)) {
        return TFM_HAL_ERROR_INVALID_INPUT;
    }

    info->mask = 0U;
    for (uint32_t pc = IFX_PC_TFM_SPM_ID; pc < MPC_PC_NR; pc++) {
        if (IFX_GET_PC(mpc_reg_cfg->pc_mask, pc) != 0U) {
            info->mask |= (7U << (pc * 4U));

            if (IFX_GET_PC(mpc_reg_cfg->ns_mask, pc) != 0U) {
                info->attr |= (1U << (pc * 4U + 0U));
            }

            if (IFX_GET_PC(mpc_reg_cfg->r_mask, pc) != 0U) {
                info->attr |= (1U << (pc * 4U + 1U));
            }

            if (IFX_GET_PC(mpc_reg_cfg->w_mask, pc) != 0U) {
                info->attr |= (1U << (pc * 4U + 2U));
            }
        }
    }

    return TFM_HAL_SUCCESS;
}
#endif /* IFX_SE_IPC_SERVICE_FULL || IFX_SE_IPC_SERVICE_BASE */

/**
 * \brief Apply MPC configuration for MPC controller that is handled by optional
 * SE RT Service or other non-TF-M service/hardware.
 *
 * \param[in] mpc_reg_cfg   MPC configuration structure which will be applied.
 *                          See \ref ifx_mpc_region_config_t for more details.
 * \param[in] memory_config Information about asset's memory that is protected,
 *                          see \ref ifx_memory_config_t
 *
 * \return    TFM_HAL_SUCCESS             - MPC configuration applied successfully.
 *            TFM_HAL_ERROR_GENERIC       - failed to apply MPC configuration.
 *            TFM_HAL_ERROR_NOT_SUPPORTED - configuration can not be applied.
 *                                          Can occur when MPC that should
 *                                          protect the memory is not controlled
 *                                          by TFM.
 *            TFM_HAL_ERROR_INVALID_INPUT - invalid input (e.g. provided memory
 *                                          region is not supported by external
 *                                          service).
 */
static enum tfm_hal_status_t ifx_mpc_apply_configuration_with_external_mpc(
                                        const ifx_mpc_region_config_t* mpc_reg_cfg,
                                        const ifx_memory_config_t* memory_config)
{
#if IFX_SE_IPC_SERVICE_FULL || IFX_SE_IPC_SERVICE_BASE
#if TFM_ISOLATION_LEVEL == 3
    /* SE RT Services are not available for SPM when TF-M scheduler is started.
     * It's expected to return success on dynamic switching. */
    if (ifx_asset_protection_type == IFX_ASSET_PROTECTION_PARTITION_DYNAMIC) {
        return TFM_HAL_SUCCESS;
    }
#endif /* TFM_ISOLATION_LEVEL == 3 */

    ifx_se_mpc_rot_cfg_crc_t config;
    ifx_se_status_t se_status;
    config.mpc_config.addr_offset = IFX_S_ADDRESS_ALIAS(mpc_reg_cfg->address) - memory_config->s_address;
    config.mpc_config.size = mpc_reg_cfg->size;
    if (memory_config->mpc_block_size == CY_MPC_SIZE_4KB) {
        config.mpc_config.region_size = IFX_SE_MPC_SIZE_4KB;
    } else {
        return TFM_HAL_ERROR_INVALID_INPUT;
    }
    for (uint32_t pc = IFX_PC_TFM_SPM_ID; pc < MPC_PC_NR; pc++) {
        config.mpc_config.pc = (ifx_se_mpc_prot_context_t)pc;
        config.mpc_config.secure = IFX_SE_MPC_SECURE;
        config.mpc_config.access = IFX_SE_MPC_ACCESS_DISABLED;

        if (IFX_GET_PC(mpc_reg_cfg->pc_mask, pc) != 0U) {
            if (IFX_GET_PC(mpc_reg_cfg->ns_mask, pc) != 0U) {
                config.mpc_config.secure = IFX_SE_MPC_NON_SECURE;
            } else {
                config.mpc_config.secure = IFX_SE_MPC_SECURE;
            }

            uint8_t r_mask = IFX_GET_PC(mpc_reg_cfg->r_mask, pc);
            uint8_t w_mask = IFX_GET_PC(mpc_reg_cfg->w_mask, pc);
            if ((r_mask != 0U) && (w_mask != 0U)) {
                config.mpc_config.access = IFX_SE_MPC_ACCESS_RW;
            } else if (r_mask != 0U) {
                config.mpc_config.access = IFX_SE_MPC_ACCESS_R;
            } else if (w_mask != 0U) {
                config.mpc_config.access = IFX_SE_MPC_ACCESS_W;
            } else {
                config.mpc_config.access = IFX_SE_MPC_ACCESS_DISABLED;
            }
        }

        config.crc = IFX_CRC32_CALC((uint8_t*)&config.mpc_config, sizeof(config.mpc_config));
        se_status = ifx_se_mpc_config_rot_mpc_struct(&config, IFX_SE_TFM_SYSCALL_CONTEXT);
        if (!IFX_SE_IS_STATUS_SUCCESS(se_status)) {
            return TFM_HAL_ERROR_GENERIC;
        }
    }

    /* Update cache */
    ifx_mpc_ext_cache_cfg_info_t info;
    if (ifx_mpc_external_mpc_config_to_cache_info(mpc_reg_cfg, memory_config, &info) != TFM_HAL_SUCCESS) {
        FIH_RET(TFM_HAL_ERROR_GENERIC);
    }

    for (uint32_t block_id = info.first_block_idx; block_id < info.last_block_idx; block_id++) {
        ifx_mpc_se_rt_rram_attr[block_id] = (ifx_mpc_se_rt_rram_attr[block_id] & ~info.mask) | info.attr;
    }

    return TFM_HAL_SUCCESS;
#else
    return TFM_HAL_ERROR_NOT_SUPPORTED;
#endif /* IFX_SE_IPC_SERVICE_FULL || IFX_SE_IPC_SERVICE_BASE */
}

static cy_en_mpc_sec_attr_t ifx_secure_cfg(const ifx_mpc_region_config_t* mpc_reg_cfg,
                                           cy_en_mpc_prot_context_t pc)
{
    cy_en_mpc_sec_attr_t ret = CY_MPC_SECURE;

    if (IFX_GET_PC(mpc_reg_cfg->pc_mask, pc) != 0U) {
        if (IFX_GET_PC(mpc_reg_cfg->ns_mask, pc) != 0U) {
            ret = CY_MPC_NON_SECURE;
        } else {
            ret = CY_MPC_SECURE;
        }
    }

    return ret;
}

static cy_en_mpc_access_attr_t ifx_access_cfg(const ifx_mpc_region_config_t* mpc_reg_cfg,
                                              cy_en_mpc_prot_context_t pc)
{
    cy_en_mpc_access_attr_t ret = CY_MPC_ACCESS_DISABLED;

    if (IFX_GET_PC(mpc_reg_cfg->pc_mask, pc) != 0U) {
        uint8_t r_mask = IFX_GET_PC(mpc_reg_cfg->r_mask, pc);
        uint8_t w_mask = IFX_GET_PC(mpc_reg_cfg->w_mask, pc);

        if ((r_mask != 0U) && (w_mask != 0U)) {
            ret = CY_MPC_ACCESS_RW;
        } else if (r_mask != 0U) {
            ret = CY_MPC_ACCESS_R;
        } else if (w_mask != 0U) {
            ret = CY_MPC_ACCESS_W;
        } else {
            ret = CY_MPC_ACCESS_DISABLED;
        }
    }

    return ret;
}

/**
 * \brief Apply MPC configuration for MPC controller handled by TF-M.
 *
 * \param[in] mpc_reg_cfg   MPC configuration structure which will be applied.
 *                          See \ref ifx_mpc_region_config_t for more details.
 * \param[in] memory_config Information about asset's memory that is protected,
 *                          see \ref ifx_memory_config_t
 *
 * \return    TFM_HAL_SUCCESS             - MPC configuration applied successfully.
 *            TFM_HAL_ERROR_GENERIC       - failed to apply MPC configuration.
 */
static enum tfm_hal_status_t ifx_mpc_apply_configuration_with_mpc(
                                        const ifx_mpc_region_config_t* mpc_reg_cfg,
                                        const ifx_memory_config_t* memory_config)
{
    cy_stc_mpc_rot_cfg_t       mpc_rot_cfg;

    mpc_rot_cfg.addrOffset = IFX_S_ADDRESS_ALIAS(mpc_reg_cfg->address) - memory_config->s_address;
    mpc_rot_cfg.size       = mpc_reg_cfg->size;
    mpc_rot_cfg.regionSize = memory_config->mpc_block_size;

    for (uint32_t pc = 0UL; pc < MPC_PC_NR; pc++) {
        mpc_rot_cfg.pc     = (cy_en_mpc_prot_context_t)pc;
        mpc_rot_cfg.secure = ifx_secure_cfg(mpc_reg_cfg, (cy_en_mpc_prot_context_t)pc);
        mpc_rot_cfg.access = ifx_access_cfg(mpc_reg_cfg, (cy_en_mpc_prot_context_t)pc);

        if (Cy_Mpc_ConfigRotMpcStruct(memory_config->mpc, &mpc_rot_cfg) != CY_MPC_SUCCESS) {
            return TFM_HAL_ERROR_GENERIC;
        }
    }

    return TFM_HAL_SUCCESS;
}

#ifdef TFM_FIH_PROFILE_ON
static FIH_RET_TYPE(enum tfm_hal_status_t) ifx_mpc_verify_configuration_with_mpc(
                                        const ifx_mpc_region_config_t* mpc_reg_cfg,
                                        const ifx_memory_config_t* memory_config)
{
    /* Verify that the MPC is configured as specified */
    for (uint32_t pc = 0U; pc < MPC_PC_NR; pc++) {
        cy_en_mpc_sec_attr_t secure = ifx_secure_cfg(mpc_reg_cfg,
                                                     (cy_en_mpc_prot_context_t)pc);
        cy_en_mpc_access_attr_t access = ifx_access_cfg(mpc_reg_cfg,
                                                        (cy_en_mpc_prot_context_t)pc);
        uint32_t block_size      = IFX_MPC_BLOCK_SIZE_TO_BYTES(memory_config->mpc_block_size);
        uint32_t offset          = IFX_S_ADDRESS_ALIAS(mpc_reg_cfg->address) - memory_config->s_address;
        uint32_t first_block_idx = offset / block_size;
        uint32_t last_block_idx  = IFX_ROUND_UP_TO_MULTIPLE(offset + mpc_reg_cfg->size, block_size) / block_size;

        for (uint32_t idx = first_block_idx; idx < last_block_idx; idx++) {
            cy_stc_mpc_rot_block_attr_t mpc_rot_cfg;

            /* Read the block config */
            cy_en_mpc_status_t ret = Cy_Mpc_GetRotBlockAttr(memory_config->mpc,
                                                            (cy_en_mpc_prot_context_t)pc,
                                                            idx,
                                                            &mpc_rot_cfg);

            /* Check that it is as expected */
            if ((ret != CY_MPC_SUCCESS)  ||
                (mpc_rot_cfg.secure != secure)  ||
                (mpc_rot_cfg.access != access)) {

                FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
            }
        }
    }

    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}

static FIH_RET_TYPE(enum tfm_hal_status_t) ifx_mpc_verify_configuration_with_external_mpc(
                                        const ifx_mpc_region_config_t* mpc_reg_cfg,
                                        const ifx_memory_config_t* memory_config)
{
    /*
     * We can't read the MPC directly because the PPCs restrict access to PC0/1 only
     * and the SE RT doesn't provide a way to read it
     */
#if IFX_SE_IPC_SERVICE_FULL || IFX_SE_IPC_SERVICE_BASE
    /* But it's possible to validate RRAM MPC attribute cache */
    ifx_mpc_ext_cache_cfg_info_t info;
    if (ifx_mpc_external_mpc_config_to_cache_info(mpc_reg_cfg, memory_config, &info) != TFM_HAL_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }

    for (uint32_t block_id = info.first_block_idx; block_id < info.last_block_idx; block_id++) {
        if ((ifx_mpc_se_rt_rram_attr[block_id] & info.mask) != info.attr) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
        }
    }
#endif /* IFX_SE_IPC_SERVICE_FULL || IFX_SE_IPC_SERVICE_BASE */
    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}
#endif /* TFM_FIH_PROFILE_ON */

/**
 * \brief Apply MPC configuration.
 *
 * \param[in] mpc_reg_cfg   MPC configuration structure which will be applied.
 *                          See \ref ifx_mpc_region_config_t for more details.
 *
 * \return    TFM_HAL_SUCCESS             - MPC configuration applied successfully.
 *            TFM_HAL_ERROR_NOT_SUPPORTED - configuration can not be applied.
 *                                          Can occur when MPC that should
 *                                          protect the memory is not controlled
 *                                          by TFM.
 *            TFM_HAL_ERROR_GENERIC       - failed to apply MPC configuration.
 *            TFM_HAL_ERROR_INVALID_INPUT - invalid input (e.g. provided memory
 *                                          region is not fully located in any
 *                                          known memory).
 */
static FIH_RET_TYPE(enum tfm_hal_status_t) ifx_mpc_apply_configuration(
                                                         const ifx_mpc_region_config_t* mpc_reg_cfg)
{
    const ifx_memory_config_t* memory_configs[IFX_REGION_MAX_MPC_COUNT];
    uint32_t                   mpc_cnt = sizeof(memory_configs)/sizeof(memory_configs[0]);
    enum tfm_hal_status_t      mem_cfg_res;

    mem_cfg_res = ifx_get_all_memory_configs(memory_configs,
                                             &mpc_cnt,
                                             mpc_reg_cfg->address,
                                             mpc_reg_cfg->size);

    if (mem_cfg_res != TFM_HAL_SUCCESS) {
        FIH_RET(fih_int_encode(mem_cfg_res));
    }

    for (uint32_t mem_idx = 0; mem_idx < mpc_cnt; mem_idx++) {
        /* MPC pointer is set to NULL for MPCs that are not configurable by TFM. */
        if (memory_configs[mem_idx]->mpc == IFX_MPC_NOT_CONTROLLED_BY_TFM) {
            /* Use external service to protect memory */
            mem_cfg_res = ifx_mpc_apply_configuration_with_external_mpc(mpc_reg_cfg,
                                                                        memory_configs[mem_idx]);
            if (mem_cfg_res != TFM_HAL_SUCCESS) {
                /* Corresponding memory configuration was not found */
                FIH_RET(fih_int_encode(mem_cfg_res));
            }
        } else {
            /* Use MPC peripheral to protect */
            mem_cfg_res = ifx_mpc_apply_configuration_with_mpc(mpc_reg_cfg,
                                                               memory_configs[mem_idx]);
            if (mem_cfg_res != TFM_HAL_SUCCESS) {
                /* Corresponding memory configuration was not found */
                FIH_RET(fih_int_encode(mem_cfg_res));
            }
        }
    }

    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}

#ifdef TFM_FIH_PROFILE_ON
static FIH_RET_TYPE(enum tfm_hal_status_t) ifx_mpc_verify_configuration(
                                                         const ifx_mpc_region_config_t* mpc_reg_cfg)
{
    const ifx_memory_config_t* memory_configs[IFX_REGION_MAX_MPC_COUNT];
    uint32_t                   mpc_cnt = sizeof(memory_configs)/sizeof(memory_configs[0]);
    enum tfm_hal_status_t mem_cfg_res;

    mem_cfg_res = ifx_get_all_memory_configs(memory_configs,
                                             &mpc_cnt,
                                             mpc_reg_cfg->address,
                                             mpc_reg_cfg->size);

    if (mem_cfg_res != TFM_HAL_SUCCESS) {
        FIH_RET(fih_int_encode(mem_cfg_res));
    }

    for (uint32_t mem_idx = 0; mem_idx < mpc_cnt; mem_idx++) {
        FIH_RET_TYPE(enum tfm_hal_status_t) mem_cfg_res_2;

        /* MPC pointer is set to NULL for MPCs that are not configurable by TFM. */
        if (memory_configs[mem_idx]->mpc == IFX_MPC_NOT_CONTROLLED_BY_TFM) {
            /* Use external service to verify protection */
            FIH_CALL(ifx_mpc_verify_configuration_with_external_mpc,
                     mem_cfg_res_2, mpc_reg_cfg, memory_configs[mem_idx]);
            if (fih_not_eq(mem_cfg_res_2, fih_int_encode(TFM_HAL_SUCCESS))) {
                FIH_RET(mem_cfg_res_2);
            }
        } else {
            /* Read MPC directly */
            FIH_CALL(ifx_mpc_verify_configuration_with_mpc,
                     mem_cfg_res_2, mpc_reg_cfg, memory_configs[mem_idx]);
            if (fih_not_eq(mem_cfg_res_2, fih_int_encode(TFM_HAL_SUCCESS))) {
                FIH_RET(mem_cfg_res_2);
            }
        }
    }

    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}
#endif

FIH_RET_TYPE(enum tfm_hal_status_t) ifx_mpc_isolate_numbered_mmio(
                                                    const ifx_mpc_numbered_mmio_config_t* mpc_cfg,
                                                    const struct asset_desc_t* asset)
{
    FIH_RET_TYPE(enum tfm_hal_status_t) fih_rc = fih_int_encode(TFM_HAL_ERROR_GENERIC);
    ifx_mpc_region_config_t mpc_reg_cfg;

    mpc_reg_cfg.address = asset->mem.start;
    mpc_reg_cfg.size    = asset->mem.limit - asset->mem.start;

    mpc_reg_cfg.pc_mask = mpc_cfg->pc_mask;
    mpc_reg_cfg.ns_mask = mpc_cfg->ns_mask;

    mpc_reg_cfg.r_mask  = mpc_cfg->pc_mask;
    mpc_reg_cfg.w_mask  = ((asset->attr & ASSET_ATTR_READ_WRITE) != 0U) ? mpc_cfg->pc_mask : 0U;

    FIH_CALL(ifx_mpc_apply_configuration, fih_rc, &mpc_reg_cfg);
    FIH_RET(fih_rc);
}

/* Fill ifx_fixed_mpc_static_config[] and set ifx_fixed_mpc_static_config_count */
static void ifx_mpc_fill_fixed_config(void)
{
    uint32_t idx = 0;

    /* Following MPC configs only change the PC mask thus if
     * IFX_ISOLATION_PC_SWITCHING is OFF, then PC mask will be the same, so no
     * need to reconfigure. */
#if IFX_ISOLATION_PC_SWITCHING
#if (TFM_ISOLATION_LEVEL == 2) && (CONFIG_TFM_AROT_PRESENT == 1)
    /* RW, ZI and stack as one region */
    ifx_fixed_mpc_static_config[idx++] =
    (ifx_mpc_region_config_t){
        .address    = (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_START, $$Base),
        .size       = (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base) -
                      (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_START, $$Base),
        .pc_mask    = IFX_PC_TFM_SPM | IFX_PC_TFM_PROT | IFX_PC_TFM_AROT,
        .ns_mask    = IFX_PC_NONE, /**< All secure */
        .r_mask     = IFX_PC_TFM_SPM | IFX_PC_TFM_PROT | IFX_PC_TFM_AROT,
        .w_mask     = IFX_PC_TFM_SPM | IFX_PC_TFM_PROT | IFX_PC_TFM_AROT,
    };
#endif /* (TFM_ISOLATION_LEVEL == 2) && (CONFIG_TFM_AROT_PRESENT == 1) */

#ifdef CONFIG_TFM_PARTITION_META
    /* RW, ZI and stack as one region */
    ifx_fixed_mpc_static_config[idx++] =
    (ifx_mpc_region_config_t){
            .address    = (uint32_t)&REGION_NAME(Image$$, TFM_SPM_RW_OTHER_RO_START, $$Base),
            .size       = (uint32_t)&REGION_NAME(Image$$, TFM_SPM_RW_OTHER_RO_END, $$Base) -
                          (uint32_t)&REGION_NAME(Image$$, TFM_SPM_RW_OTHER_RO_START, $$Base),
            .pc_mask    = IFX_PC_TFM_SPM | IFX_PC_TFM_PROT | IFX_PC_TFM_AROT | IFX_PC_TZ_NSPE,
            .ns_mask    = IFX_PC_NONE, /**< All secure */
            .r_mask     = IFX_PC_TFM_SPM | IFX_PC_TFM_PROT | IFX_PC_TFM_AROT | IFX_PC_TZ_NSPE,
            .w_mask     = IFX_PC_TFM_SPM,
    };
#endif /* CONFIG_TFM_PARTITION_META */

#if TFM_ISOLATION_LEVEL == 3
#if TEST_NS_IFX_CODE_COVERAGE
    /* coverage linker region - secure PCs have RW access */
    ifx_fixed_mpc_static_config[idx++] =
    (ifx_mpc_region_config_t){
            .address    = (uint32_t)&REGION_NAME(Image$$, IFX_CODE_COVERAGE_START, $$Base),
            .size       = (uint32_t)&REGION_NAME(Image$$, IFX_CODE_COVERAGE_END, $$Base) -
                          (uint32_t)&REGION_NAME(Image$$, IFX_CODE_COVERAGE_START, $$Base),
            .pc_mask    = IFX_PC_TFM_SPM | IFX_PC_TFM_PROT | IFX_PC_TFM_AROT | IFX_PC_TZ_NSPE,
            .ns_mask    = IFX_PC_NONE, /**< All secure */
            .r_mask     = IFX_PC_TFM_SPM | IFX_PC_TFM_PROT | IFX_PC_TFM_AROT | IFX_PC_TZ_NSPE,
            .w_mask     = IFX_PC_TFM_SPM | IFX_PC_TFM_PROT | IFX_PC_TFM_AROT | IFX_PC_TZ_NSPE,
    };
#endif /* TEST_NS_IFX_CODE_COVERAGE */
#endif /* TFM_ISOLATION_LEVEL == 3 */

#ifdef CONFIG_TFM_USE_TRUSTZONE
    ifx_fixed_mpc_static_config[idx++] =
    (ifx_mpc_region_config_t){
        .address    = (uint32_t)&REGION_NAME(Image$$, ER_VENEER, $$Base),
        .size       = (uint32_t)&REGION_NAME(Image$$, VENEER_ALIGN, $$Limit) -
                      (uint32_t)&REGION_NAME(Image$$, ER_VENEER, $$Base),
        .pc_mask    = IFX_PC_TFM_SPM | IFX_PC_TFM_PROT | IFX_PC_TFM_AROT | IFX_PC_TZ_NSPE,
        .ns_mask    = IFX_PC_NONE, /**< All secure */
        .r_mask     = IFX_PC_TFM_SPM | IFX_PC_TFM_PROT | IFX_PC_TFM_AROT | IFX_PC_TZ_NSPE,
        .w_mask     = IFX_PC_NONE,
    };

    /* Unprivileged NS Agent TZ code region for TFM image with enabled Default PC (with SPM) + PRoT + ARoT  */
    ifx_fixed_mpc_static_config[idx++] =
    (ifx_mpc_region_config_t){
        .address    = (uint32_t)&REGION_NAME(Image$$, TFM_NS_AGENT_TZ_CODE_START, $$RO$$Base),
        .size       = (uint32_t)&REGION_NAME(Image$$, TFM_NS_AGENT_TZ_CODE_END, $$RO$$Limit) -
                      (uint32_t)&REGION_NAME(Image$$, TFM_NS_AGENT_TZ_CODE_START, $$RO$$Base),
        .pc_mask    = IFX_PC_TFM_SPM | IFX_PC_TFM_PROT | IFX_PC_TFM_AROT | IFX_PC_TZ_NSPE,
        .ns_mask    = IFX_PC_NONE, /**< All secure */
        .r_mask     = IFX_PC_TFM_SPM | IFX_PC_TFM_PROT | IFX_PC_TFM_AROT | IFX_PC_TZ_NSPE,
        .w_mask     = IFX_PC_NONE, /**< No write to code, R/O data */
    };

    /* Unprivileged Secure code region for TFM image with enabled Default PC (with SPM) + PRoT + ARoT  */
    ifx_fixed_mpc_static_config[idx++] =
    (ifx_mpc_region_config_t){
        .address    = (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_BASE_CODE_START, $$RO$$Base),
        .size       = (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_BASE_CODE_END, $$RO$$Limit) -
                      (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_BASE_CODE_START, $$RO$$Base),
        .pc_mask    = IFX_PC_TFM_SPM | IFX_PC_TFM_PROT | IFX_PC_TFM_AROT,
        .ns_mask    = IFX_PC_NONE, /**< All secure */
        .r_mask     = IFX_PC_TFM_SPM | IFX_PC_TFM_PROT | IFX_PC_TFM_AROT,
        .w_mask     = IFX_PC_NONE, /**< No write to code, R/O data */
    };
#endif /* CONFIG_TFM_USE_TRUSTZONE */
#endif /* IFX_ISOLATION_PC_SWITCHING */

    /* finally, set the count */
    if (idx > IFX_MAX_FIXED_CONFIGS) {
        tfm_core_panic();
    }
    ifx_fixed_mpc_static_config_count = idx;
}

FIH_RET_TYPE(enum tfm_hal_status_t) ifx_mpc_init_cfg(void)
{
    /* Populate ifx_fixed_mpc_static_config[] */
    ifx_mpc_fill_fixed_config();

#if IFX_SE_IPC_SERVICE_FULL || IFX_SE_IPC_SERVICE_BASE
    /* Setup RRAM MPC MPC attribute cache used to implement ifx_mpc_memory_check */
    for (uint32_t block_id = 0; block_id < IFX_SE_RT_RRAM_BLOCK_COUNT; block_id++) {
        ifx_mpc_se_rt_rram_attr[block_id] = IFX_SE_RT_RRAM_MPC_DEFAULT_ATTRIBUTES;
    }

#if defined(TFM_FIH_PROFILE_ON)
    /* Validate ifx_mpc_memory_check */
    (void)fih_delay();
    for (uint32_t block_id = 0; block_id < IFX_SE_RT_RRAM_BLOCK_COUNT; block_id++) {
        if (ifx_mpc_se_rt_rram_attr[block_id] != IFX_SE_RT_RRAM_MPC_DEFAULT_ATTRIBUTES) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
        }
    }
#endif
#endif /* IFX_SE_IPC_SERVICE_FULL || IFX_SE_IPC_SERVICE_BASE */

    /*
    * Starts changing actual configuration so issue DMB to ensure every
    * transaction has completed by now
    */
    __DMB();

    /* Disable Auto Increment of BLK ID and set violation response for all used memory types */
    for (uint32_t idx = 0UL; idx < ifx_memory_cm33_config_count; idx++) {
        /* Some MPCs are not controlled by TFM so the MPC pointer is set to NULL */
        if (ifx_memory_cm33_config[idx]->mpc != IFX_MPC_NOT_CONTROLLED_BY_TFM) {
            Cy_Mpc_RotAutoInc(ifx_memory_cm33_config[idx]->mpc, IFX_MPC_TFM_PROT_AUTO_INC_DISABLE);
            /* Set violation response as Bus Error instead RZWI */
            ifx_memory_cm33_config[idx]->mpc->CFG = _VAL2FLD(RAMC_MPC_CFG_RESPONSE, 1);
        }
    }
    for (uint32_t idx = 0UL; idx < ifx_memory_cm55_config_count; idx++) {
        /* Some MPCs are not controlled by TFM so the MPC pointer is set to NULL */
        if (ifx_memory_cm55_config[idx]->mpc != IFX_MPC_NOT_CONTROLLED_BY_TFM) {
            Cy_Mpc_RotAutoInc(ifx_memory_cm55_config[idx]->mpc, IFX_MPC_TFM_PROT_AUTO_INC_DISABLE);
            /* Set violation response as Bus Error instead RZWI */
            ifx_memory_cm55_config[idx]->mpc->CFG = _VAL2FLD(RAMC_MPC_CFG_RESPONSE, 1);
        }
    }

    FIH_RET_TYPE(enum tfm_hal_status_t) mpc_result = fih_int_encode(TFM_HAL_ERROR_BAD_STATE);
    /* Apply predefined static MPC configuration for memory regions generated by TZ Configurator */
    for (uint32_t idx = 0UL; idx < ifx_mpc_static_config_count; idx++) {
        FIH_CALL(ifx_mpc_apply_configuration, mpc_result,
                 &ifx_mpc_static_config[idx]);
        if (fih_not_eq(mpc_result, fih_int_encode(TFM_HAL_SUCCESS))) {
            FIH_RET(mpc_result);
        }
    }

#if defined(TFM_FIH_PROFILE_ON)
    /*
     * Some of the configs in ifx_mpc_static_config[] will be partially overridden
     * by configs in ifx_fixed_mpc_static_config[], which means that verify
     * of these partially-overwritten configs will fail if done after we
     * apply those in ifx_fixed_mpc_static_config[]. So verify these ones now
     */

    for (uint32_t idx = 0UL; idx < ifx_mpc_static_config_count; idx++) {
        FIH_RET_TYPE(enum tfm_hal_status_t) ret;

        FIH_CALL(ifx_mpc_verify_configuration, ret, &ifx_mpc_static_config[idx]);
        if (fih_not_eq(ret, fih_int_encode(TFM_HAL_SUCCESS))) {
            FIH_RET(ret);
        }
    }
#endif

    for (uint32_t idx = 0UL; idx < ifx_fixed_mpc_static_config_count; idx++) {
        FIH_CALL(ifx_mpc_apply_configuration, mpc_result,
                 &ifx_fixed_mpc_static_config[idx]);
        if (fih_not_eq(mpc_result, fih_int_encode(TFM_HAL_SUCCESS))) {
            FIH_RET(mpc_result);
        }
    }

    /* Add barriers to assure the MPC configuration is done before continue
     * the execution.
     */
    __DSB();
    __ISB();

    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}

#ifdef TFM_FIH_PROFILE_ON
FIH_RET_TYPE(enum tfm_hal_status_t) ifx_mpc_verify_static_boundaries(void)
{
    FIH_RET_TYPE(enum tfm_hal_status_t) ret = fih_int_encode(TFM_HAL_ERROR_GENERIC);

    /* Populate ifx_fixed_mpc_static_config[] again, in case of fault injection */
    ifx_mpc_fill_fixed_config();

    /* Check Auto Increment of BLK ID and violation response for all used memory types */
    for (uint32_t idx = 0UL; idx < ifx_memory_cm33_config_count; idx++) {
        /* Some MPCs are not controlled by TFM so the MPC pointer is set to NULL */
        if (ifx_memory_cm33_config[idx]->mpc != IFX_MPC_NOT_CONTROLLED_BY_TFM) {
            /* Read back auto increment - should be off */
            if (_FLD2VAL(RAMC_MPC_ROT_CTRL_AUTO_INC, ifx_memory_cm33_config[idx]->mpc->ROT_CTRL) != IFX_MPC_TFM_PROT_AUTO_INC_DISABLE) {
                FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
            }
            /* and violation response should be Bus Error */
            if (_FLD2VAL(RAMC_MPC_CFG_RESPONSE, ifx_memory_cm33_config[idx]->mpc->CFG) != 1) {
                FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
            }
        }
    }
    for (uint32_t idx = 0UL; idx < ifx_memory_cm55_config_count; idx++) {
        /* Some MPCs are not controlled by TFM so the MPC pointer is set to NULL */
        if (ifx_memory_cm55_config[idx]->mpc != IFX_MPC_NOT_CONTROLLED_BY_TFM) {
            /* Read back auto increment - should be off */
            if (_FLD2VAL(RAMC_MPC_ROT_CTRL_AUTO_INC, ifx_memory_cm55_config[idx]->mpc->ROT_CTRL) != IFX_MPC_TFM_PROT_AUTO_INC_DISABLE) {
                FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
            }
            /* and violation response should be Bus Error */
            if (_FLD2VAL(RAMC_MPC_CFG_RESPONSE, ifx_memory_cm55_config[idx]->mpc->CFG) != 1) {
                FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
            }
        }
    }

    /*
     * Some of the configs in ifx_mpc_static_config[] are partially overridden
     * by configs in ifx_fixed_mpc_static_config[], which means that verify
     * of these partially-overwritten configs will fail if done after we
     * apply those in ifx_fixed_mpc_static_config[]. So we verified those ones
     * immediately after applying them instead.
     */

    for (uint32_t idx = 0UL; idx < ifx_fixed_mpc_static_config_count; idx++) {
        FIH_CALL(ifx_mpc_verify_configuration, ret, &ifx_fixed_mpc_static_config[idx]);
        if (fih_not_eq(ret, fih_int_encode(TFM_HAL_SUCCESS))) {
            FIH_RET(ret);
        }
    }

    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}
#endif /* TFM_FIH_PROFILE_ON */

#if IFX_SE_IPC_SERVICE_FULL || IFX_SE_IPC_SERVICE_BASE
static FIH_RET_TYPE(enum tfm_hal_status_t) ifx_mpc_memory_check_with_external_mpc(
                                           const struct ifx_partition_info_t *p_info,
                                           const ifx_memory_config_t* memory_config,
                                           uintptr_t base,
                                           size_t size,
                                           uint32_t access_type)
{
    if (memory_config == NULL) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
    }

    uint32_t block_size = IFX_MPC_BLOCK_SIZE_TO_BYTES(memory_config->mpc_block_size);
    if (IFX_SE_RT_RRAM_BLOCK_SIZE != block_size) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
    }

    uint32_t offset          = IFX_S_ADDRESS_ALIAS(base) - memory_config->s_address;
    uint32_t first_block_idx = offset / IFX_SE_RT_RRAM_BLOCK_SIZE;
    uint32_t last_block_idx  = IFX_ROUND_UP_TO_MULTIPLE(offset + size, IFX_SE_RT_RRAM_BLOCK_SIZE) / IFX_SE_RT_RRAM_BLOCK_SIZE;
    if ((first_block_idx >= IFX_SE_RT_RRAM_BLOCK_COUNT) ||
        (last_block_idx > IFX_SE_RT_RRAM_BLOCK_COUNT)) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
    }

    bool is_secure = (access_type & TFM_HAL_ACCESS_NS) == 0;
    TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_10_1, "external macro IS_NS_AGENT interprets non-boolean type as a boolean")
    uint32_t pc = (uint32_t)fih_int_decode(IFX_GET_PARTITION_PC(p_info, is_secure));

    uint32_t mask = 1u << (pc * 4u + 0);
    uint32_t attr = is_secure ? 0 : 1u << (pc * 4u + 0u);

    if ((access_type & TFM_HAL_ACCESS_READABLE) != 0U) {
        mask |= 1u << (pc * 4u + 1);
        attr |= 1u << (pc * 4u + 1);
    }

    if ((access_type & TFM_HAL_ACCESS_WRITABLE) != 0U) {
        mask |= 1u << (pc * 4u + 2);
        attr |= 1u << (pc * 4u + 2);
    }

    for (uint32_t block_id = first_block_idx; block_id < last_block_idx; block_id++) {
        if ((ifx_mpc_se_rt_rram_attr[block_id] & mask) != attr) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
        }
    }

    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}
#endif /* IFX_SE_IPC_SERVICE_FULL || IFX_SE_IPC_SERVICE_BASE */

FIH_RET_TYPE(enum tfm_hal_status_t) ifx_mpc_memory_check(const struct ifx_partition_info_t *p_info,
                                                         uintptr_t base,
                                                         size_t size,
                                                         uint32_t access_type)
{
    const ifx_memory_config_t* mem_region_cfg;
#if defined(TFM_FIH_PROFILE_ON) && !defined(TFM_FIH_PROFILE_LOW)
    const ifx_memory_config_t* mem_region_cfg2;
#endif

    /* CM33 memory list is a comprehensive list of memory regions for the
     * device, protections applied to other memory lists are also applied to CM33
     * memory list so checking only CM 33 list is enough. */
    mem_region_cfg = ifx_find_memory_config(base, size,
                                            ifx_memory_cm33_config,
                                            ifx_memory_cm33_config_count);

#if defined(TFM_FIH_PROFILE_ON) && !defined(TFM_FIH_PROFILE_LOW)
    (void)fih_delay();
    mem_region_cfg2 = ifx_find_memory_config(base, size,
                                            ifx_memory_cm33_config,
                                            ifx_memory_cm33_config_count);
    if (mem_region_cfg != mem_region_cfg2) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
    }
#endif

    if (mem_region_cfg == NULL) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_INVALID_INPUT));
    }

    uint32_t block_size      = IFX_MPC_BLOCK_SIZE_TO_BYTES(mem_region_cfg->mpc_block_size);
    uint32_t offset          = IFX_S_ADDRESS_ALIAS(base) - mem_region_cfg->s_address;
    uint32_t first_block_idx = offset / block_size;
    uint32_t last_block_idx  = IFX_ROUND_UP_TO_MULTIPLE(offset + size, block_size) / block_size;
    fih_int is_secure = (access_type & TFM_HAL_ACCESS_NS) == 0 ? FIH_SUCCESS : FIH_FAILURE;
    TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_10_1, "external macro IS_NS_AGENT interprets non-boolean type as a boolean")
    fih_int pc               = FIH_INVALID;

    pc = IFX_GET_PARTITION_PC(p_info, fih_eq(is_secure, FIH_SUCCESS));
    if (!fih_int_validate(pc)) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
    }

    TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_10_1, "external macro IS_NS_AGENT interprets non-boolean type as a boolean")
    cy_en_mpc_sec_attr_t expected_sec_attr = (IS_NS_AGENT(p_info->p_ldinfo) && fih_eq(is_secure, FIH_FAILURE)) ? CY_MPC_NON_SECURE : CY_MPC_SECURE;

#if defined(TFM_FIH_PROFILE_ON) && !defined(TFM_FIH_PROFILE_LOW)
    (void)fih_delay();
    volatile uint32_t access_type_2 = access_type;
    fih_int is_secure2 = (access_type_2 & TFM_HAL_ACCESS_NS) == 0 ? FIH_SUCCESS : FIH_FAILURE;
    if (!fih_eq(is_secure, is_secure2)) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
    }

    volatile uint32_t validated_block_count = 0;
#endif

    if (mem_region_cfg->mpc == IFX_MPC_NOT_CONTROLLED_BY_TFM) {
#if IFX_SE_IPC_SERVICE_FULL || IFX_SE_IPC_SERVICE_BASE
        FIH_RET_TYPE(enum tfm_hal_status_t) ret;
        FIH_CALL(ifx_mpc_memory_check_with_external_mpc,
                ret, p_info, mem_region_cfg, base, size, access_type);
        FIH_RET(ret);
#else /* IFX_SE_IPC_SERVICE_FULL || IFX_SE_IPC_SERVICE_BASE */
        /* If MPC is not controlled by TFM neither by SE RT Services
         * then the assumption is that partition has no access to it */
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
#endif /* IFX_SE_IPC_SERVICE_FULL || IFX_SE_IPC_SERVICE_BASE */
    }

    for (uint32_t idx = first_block_idx; idx < last_block_idx; idx++) {
        cy_stc_mpc_rot_block_attr_t mpc_settings;

        if (Cy_Mpc_GetRotBlockAttr(mem_region_cfg->mpc, (cy_en_mpc_prot_context_t)fih_int_decode(pc), idx, &mpc_settings) != CY_MPC_SUCCESS) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
        }

#if defined(TFM_FIH_PROFILE_ON) && !defined(TFM_FIH_PROFILE_LOW)
        cy_stc_mpc_rot_block_attr_t mpc_settings2;

        (void)fih_delay();
        if (Cy_Mpc_GetRotBlockAttr(mem_region_cfg->mpc, (cy_en_mpc_prot_context_t)fih_int_decode(pc), idx, &mpc_settings2) != CY_MPC_SUCCESS) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
        }

        (void)fih_delay();
        if ((mpc_settings.access != mpc_settings2.access) ||
            (mpc_settings.secure != mpc_settings2.secure)) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
        }

        (void)fih_delay();
        validated_block_count++;
#endif

        if (mpc_settings.secure != expected_sec_attr) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
        }

        /* If MPC has RW access then any (R, W, RW) access is allowed, so no
         * need for additional checks. */
        if (mpc_settings.access == CY_MPC_ACCESS_RW) {
            continue;
        }

        if (((access_type & TFM_HAL_ACCESS_READABLE) != 0U) &&
            (mpc_settings.access != CY_MPC_ACCESS_R)) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
        }

        if (((access_type & TFM_HAL_ACCESS_WRITABLE) != 0U) &&
            (mpc_settings.access != CY_MPC_ACCESS_W)) {
            FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
        }
    }

#if defined(TFM_FIH_PROFILE_ON) && !defined(TFM_FIH_PROFILE_LOW)
    (void)fih_delay();
    if (validated_block_count != last_block_idx - first_block_idx) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_MEM_FAULT));
    }
#endif

    FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
}
