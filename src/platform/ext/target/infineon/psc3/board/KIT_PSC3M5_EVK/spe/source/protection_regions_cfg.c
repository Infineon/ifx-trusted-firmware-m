/*
 * Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "array.h"
#include "cy_device_headers.h"
#include "ifx_regions.h"
#include "project_memory_layout.h"
#include "protection_regions_cfg.h"
#ifdef TEST_S_FPU
#include "tfm_peripherals_def.h"
#endif /* TEST_S_FPU */

/* Array of pointers to PPC_Type structures for available PPC controllers.
 * The index in the array corresponds to the PPC controller ID.*/
PPC_Type* const ifx_ppcx_region_ptrs[] = {
    PPC
};

/* Number of items in \ref ifx_ppcx_region_ptrs */
const size_t ifx_ppcx_region_ptrs_count = ARRAY_SIZE(ifx_ppcx_region_ptrs);

/* Following peripherals are reserved and are not configured by TFM:
 * PROT_PERI0_GR0_BOOT
 * PROT_PERI0_GR1_BOOT
 * PROT_PERI0_GR2_BOOT
 * PROT_PERI0_GR3_BOOT
 * PROT_PERI0_GR4_BOOT
 * PROT_PERI0_GR5_BOOT
 * PROT_PPC0_PPC_PPC_SECURE
 * PROT_PPC0_PPC_PPC_NONSECURE
 * PROT_RAMC0_BOOT
 * PROT_RAMC0_MPC0_PPC_MPC_MAIN
 * PROT_RAMC0_MPC0_PPC_MPC_PC
 * PROT_RAMC0_MPC0_PPC_MPC_ROT
 * PROT_PROMC_MPC0_PPC_MPC_MAIN
 * PROT_PROMC_MPC0_PPC_MPC_PC
 * PROT_PROMC_MPC0_PPC_MPC_ROT
 * PROT_FLASHC_MPC0_PPC_MPC_MAIN
 * PROT_FLASHC_MPC0_PPC_MPC_PC
 * PROT_FLASHC_MPC0_PPC_MPC_ROT
 * PROT_FLASHC_DFT
 * PROT_FLASHC_FM_CTL_FM_DFT
 * PROT_FLASHC_FM_CTL_FM_BOOT
 * PROT_MXCM33_BOOT_PC0
 * PROT_MXCM33_BOOT_PC1
 * PROT_MXCM33_BOOT_PC2
 * PROT_MXCM33_BOOT_PC3
 * PROT_MXCM33_BOOT
 * PROT_CPUSS_DDFT
 * PROT_CPUSS_AP
 * PROT_CPUSS_BOOT
 * PROT_MS0_MAIN
 * PROT_MS4_MAIN
 * PROT_MS5_MAIN
 * PROT_MS7_MAIN
 * PROT_MS31_MAIN
 * PROT_MS_PC0_PRIV
 * PROT_BACKUP_BACKUP_SECURE
 * PROT_DFT
 * PROT_EFUSE_DATA_BOOT1
 */

static const cy_en_prot_region_fix_t ppc_spm_regions[] = {
    /* PPC controllers */
    PROT_CPUSS,
    PROT_RAMC0_CM33,
    PROT_RAMC0_RAM_PWR,
    PROT_FLASHC_BOOT,
    PROT_FLASHC_BOOT1,
    PROT_FLASHC_MAIN,
    PROT_FLASHC_ECC,
    PROT_FLASHC_FM_CTL_FM_MAIN,
    PROT_MXCM33_CM33_S,
    PROT_MXCM33_CM33_INT,
    PROT_CPUSS_CM33_S,
    PROT_CPUSS_MSC_INT,
    PROT_MSC_ACG,
    PROT_CPUSS_SL_CTL_GROUP,
    PROT_FAULT_STRUCT0_MAIN,
    PROT_SRSS_SECURE,
    PROT_SRSS_DPLL,
    PROT_PWRMODE_PWRMODE,

#if IFX_UART_ENABLED
    PROT_SCB3,
#endif
    PROT_CRYPTOLITE_MAIN,
    PROT_CRYPTOLITE_TRNG,
    PROT_EFUSE_CTL2
};

/* Configuration of PPC regions used by SPM and Partitions. */
static const cy_en_prot_region_fix_t ppc_spm_partitions_regions[] = {
#if TEST_NS_IFX_ISOLATION_TEST /* IFX isolation tests use WDT so reserve it */
    PROT_SRSS_GENERAL,
    PROT_SRSS_MAIN,
    PROT_SRSS_WDT,
#endif
};

static const cy_en_prot_region_fix_t ppc_ns_regions[] = {
    PROT_PERI0_MAIN,
    PROT_PERI0_GR0_GROUP,
    PROT_PERI0_GR1_GROUP,
    PROT_PERI0_GR2_GROUP,
    PROT_PERI0_GR3_GROUP,
    PROT_PERI0_GR4_GROUP,
    PROT_PERI0_GR5_GROUP,
    PROT_PERI0_TR,
    PROT_PERI_PCLK0_MAIN,
    PROT_PROMC_CM33,
    PROT_MXCM33_CM33,
    PROT_MXCM33_CM33_NS,
    PROT_DW0_DW,
    PROT_DW1_DW,
    PROT_DW0_DW_CRC,
    PROT_DW1_DW_CRC,
    PROT_DW0_CH_STRUCT0_CH,
    PROT_DW0_CH_STRUCT1_CH,
    PROT_DW0_CH_STRUCT2_CH,
    PROT_DW0_CH_STRUCT3_CH,
    PROT_DW0_CH_STRUCT4_CH,
    PROT_DW0_CH_STRUCT5_CH,
    PROT_DW0_CH_STRUCT6_CH,
    PROT_DW0_CH_STRUCT7_CH,
    PROT_DW0_CH_STRUCT8_CH,
    PROT_DW0_CH_STRUCT9_CH,
    PROT_DW0_CH_STRUCT10_CH,
    PROT_DW0_CH_STRUCT11_CH,
    PROT_DW0_CH_STRUCT12_CH,
    PROT_DW0_CH_STRUCT13_CH,
    PROT_DW0_CH_STRUCT14_CH,
    PROT_DW0_CH_STRUCT15_CH,
    PROT_DW1_CH_STRUCT0_CH,
    PROT_DW1_CH_STRUCT1_CH,
    PROT_DW1_CH_STRUCT2_CH,
    PROT_DW1_CH_STRUCT3_CH,
    PROT_DW1_CH_STRUCT4_CH,
    PROT_DW1_CH_STRUCT5_CH,
    PROT_DW1_CH_STRUCT6_CH,
    PROT_DW1_CH_STRUCT7_CH,
    PROT_DW1_CH_STRUCT8_CH,
    PROT_DW1_CH_STRUCT9_CH,
    PROT_DW1_CH_STRUCT10_CH,
    PROT_DW1_CH_STRUCT11_CH,
    PROT_DW1_CH_STRUCT12_CH,
    PROT_DW1_CH_STRUCT13_CH,
    PROT_DW1_CH_STRUCT14_CH,
    PROT_DW1_CH_STRUCT15_CH,
    PROT_CPUSS_ALL_PC,
    PROT_CPUSS_CM33_NS,
    PROT_MS_PC31_PRIV,
    PROT_MS_PC0_PRIV_MIR,
    PROT_MS_PC31_PRIV_MIR,
    PROT_IPC_STRUCT0_IPC,
    PROT_IPC_STRUCT1_IPC,
    PROT_IPC_STRUCT2_IPC,
    PROT_IPC_STRUCT3_IPC,
    PROT_IPC_INTR_STRUCT0_INTR,
    PROT_IPC_INTR_STRUCT1_INTR,
    PROT_SRSS_GENERAL2,
    PROT_SRSS_HIB_DATA,
    PROT_RAM_TRIM_SRSS_SRAM,
#if !TEST_NS_IFX_ISOLATION_TEST /* IFX isolation tests use WDT so reserve it */
    PROT_SRSS_GENERAL,
    PROT_SRSS_MAIN,
    PROT_SRSS_WDT,
#endif
    PROT_MAIN,
    PROT_BACKUP_BACKUP,
    PROT_BACKUP_B_BREG0,
    PROT_BACKUP_B_BREG1,
    PROT_BACKUP_B_BREG2,
    PROT_BACKUP_B_BREG3,
    PROT_MXCORDIC_1_0,
    PROT_DEBUG600_DEBUG600,
    PROT_HSIOM_PRT0_PRT,
    PROT_HSIOM_PRT1_PRT,
    PROT_HSIOM_PRT2_PRT,
    PROT_HSIOM_PRT3_PRT,
    PROT_HSIOM_PRT4_PRT,
    PROT_HSIOM_PRT5_PRT,
    PROT_HSIOM_PRT6_PRT,
    PROT_HSIOM_PRT7_PRT,
    PROT_HSIOM_PRT8_PRT,
    PROT_HSIOM_PRT9_PRT,
    PROT_HSIOM_SECURE_PRT0_SECURE_PRT,
    PROT_HSIOM_SECURE_PRT1_SECURE_PRT,
    PROT_HSIOM_SECURE_PRT2_SECURE_PRT,
    PROT_HSIOM_SECURE_PRT3_SECURE_PRT,
    PROT_HSIOM_SECURE_PRT4_SECURE_PRT,
    PROT_HSIOM_SECURE_PRT5_SECURE_PRT,
    PROT_HSIOM_SECURE_PRT6_SECURE_PRT,
    PROT_HSIOM_SECURE_PRT7_SECURE_PRT,
    PROT_HSIOM_SECURE_PRT8_SECURE_PRT,
    PROT_HSIOM_SECURE_PRT9_SECURE_PRT,
    PROT_HSIOM_AMUX,
    PROT_HSIOM_MON,
    PROT_GPIO_PRT0_PRT,
    PROT_GPIO_PRT1_PRT,
    PROT_GPIO_PRT2_PRT,
    PROT_GPIO_PRT3_PRT,
    PROT_GPIO_PRT4_PRT,
    PROT_GPIO_PRT5_PRT,
    PROT_GPIO_PRT6_PRT,
    PROT_GPIO_PRT7_PRT,
    PROT_GPIO_PRT8_PRT,
    PROT_GPIO_PRT9_PRT,
    PROT_GPIO_PRT0_CFG,
    PROT_GPIO_PRT1_CFG,
    PROT_GPIO_PRT2_CFG,
    PROT_GPIO_PRT3_CFG,
    PROT_GPIO_PRT4_CFG,
    PROT_GPIO_PRT5_CFG,
    PROT_GPIO_PRT6_CFG,
    PROT_GPIO_PRT7_CFG,
    PROT_GPIO_PRT8_CFG,
    PROT_GPIO_PRT9_CFG,
    PROT_GPIO_SEC_GPIO,
    PROT_GPIO_GPIO,
    PROT_GPIO_TEST,
    PROT_SMARTIO_PRT0_PRT,
    PROT_SMARTIO_PRT1_PRT,
    PROT_SMARTIO_PRT2_PRT,
    PROT_SMARTIO_PRT3_PRT,
    PROT_SMARTIO_PRT5_PRT,
    PROT_SMARTIO_PRT6_PRT,
    PROT_SMARTIO_PRT9_PRT,
    PROT_LPCOMP,
    PROT_EFUSE_CTL1,
    PROT_EFUSE_CTL3,
    PROT_CANFD0_CH0_CH,
    PROT_CANFD0_CH1_CH,
    PROT_CANFD0_MAIN,
    PROT_CANFD0_BUF,
    PROT_SCB0,
    PROT_SCB1,
    PROT_SCB2,
#if !IFX_UART_ENABLED
    PROT_SCB3,
#endif
    PROT_SCB4,
    PROT_SCB5,
    PROT_TCPWM0_GRP0_CNT0_CNT,
    PROT_TCPWM0_GRP0_CNT1_CNT,
    PROT_TCPWM0_GRP0_CNT2_CNT,
    PROT_TCPWM0_GRP0_CNT3_CNT,
    PROT_TCPWM0_GRP1_CNT0_CNT,
    PROT_TCPWM0_GRP1_CNT1_CNT,
    PROT_TCPWM0_GRP1_CNT2_CNT,
    PROT_TCPWM0_GRP1_CNT3_CNT,
    PROT_TCPWM0_GRP1_CNT4_CNT,
    PROT_TCPWM0_GRP1_CNT5_CNT,
    PROT_TCPWM0_GRP1_CNT6_CNT,
    PROT_TCPWM0_GRP1_CNT7_CNT,
    PROT_TCPWM0_GRP2_CNT0_CNT,
    PROT_TCPWM0_GRP2_CNT1_CNT,
    PROT_TCPWM0_GRP2_CNT2_CNT,
    PROT_TCPWM0_GRP2_CNT3_CNT,
    PROT_TCPWM0_GRP2_CNT4_CNT,
    PROT_TCPWM0_GRP2_CNT5_CNT,
    PROT_TCPWM0_GRP2_CNT6_CNT,
    PROT_TCPWM0_GRP2_CNT7_CNT,
    PROT_TCPWM0_TR_ALL_GF_TR_ALL_GF,
    PROT_TCPWM0_TR_ALL_SYNC_BYPASS_TR_ALL_SYN_BYPASS,
    PROT_TCPWM0_BOOT,
    PROT_TCPWM0_MOTIF_GRP1_MOTIF0_MOTIF,
    PROT_MCPASS
};

/* List of PPC static configs for different configurations and PPC controllers */
const ifx_ppcx_config_t ifx_ppcx_static_config[] = {
    { /* PPC SPM */
        .sec_attr      = CY_PPC_SECURE,
        .allow_unpriv  = false,
        .pc_mask       = IFX_PC_DEFAULT,
        .regions       = ppc_spm_regions,
        .region_count  = ARRAY_SIZE(ppc_spm_regions),
    },
    { /* SPM+Partitions */
        .sec_attr      = CY_PPC_SECURE,
        .allow_unpriv  = true,
        .pc_mask       = IFX_PC_DEFAULT | IFX_PC_TFM_PROT | IFX_PC_TFM_AROT,
        .regions       = ppc_spm_partitions_regions,
        .region_count  = ARRAY_SIZE(ppc_spm_partitions_regions),
    },
    { /* PPC CM33 NS */
        .sec_attr      = CY_PPC_NON_SECURE,
        .allow_unpriv  = true,
        .pc_mask       = IFX_PC_DEFAULT | IFX_PC_TFM_PROT | IFX_PC_TFM_AROT | IFX_PC_TZ_NSPE | IFX_PC_DEBUGGER,
        .regions       = ppc_ns_regions,
        .region_count  = ARRAY_SIZE(ppc_ns_regions),
    },
};

/* Number of items in \ref ifx_ppcx_static_config */
const size_t ifx_ppcx_static_config_count = ARRAY_SIZE(ifx_ppcx_static_config);

/* MSC (Bus Masters) violation response config */
const ifx_msc_agc_resp_config_t ifx_msc_agc_resp_config[] = {
#if CPUSS_CODE_MS_0_PRESENT
    {.bus_master = CODE_MS0_MSC, .gate_resp = CY_MS_CTL_GATE_RESP_ERR, .sec_resp = CY_MS_CTL_SEC_RESP_ERR,},
#endif /* CPUSS_CODE_MS_0_PRESENT */
#if CPUSS_SYS_MS_0_PRESENT
    {.bus_master = SYS_MS0_MSC,  .gate_resp = CY_MS_CTL_GATE_RESP_ERR, .sec_resp = CY_MS_CTL_SEC_RESP_ERR,},
#endif /* CPUSS_SYS_MS_0_PRESENT */
#if CPUSS_SYS_MS_1_PRESENT
    {.bus_master = SYS_MS1_MSC,  .gate_resp = CY_MS_CTL_GATE_RESP_ERR, .sec_resp = CY_MS_CTL_SEC_RESP_ERR,},
#endif /* CPUSS_SYS_MS_1_PRESENT */
#if CPUSS_EXP_MS_PRESENT
    {.bus_master = EXP_MS_MSC,   .gate_resp = CY_MS_CTL_GATE_RESP_ERR, .sec_resp = CY_MS_CTL_SEC_RESP_ERR,},
#endif /* CPUSS_EXP_MS_PRESENT */
#if CPUSS_DMAC0_PRESENT
    {.bus_master = DMAC0_MSC,    .gate_resp = CY_MS_CTL_GATE_RESP_ERR, .sec_resp = CY_MS_CTL_SEC_RESP_ERR,},
#endif /* CPUSS_DMAC0_PRESENT */
#if CPUSS_DMAC1_PRESENT
    {.bus_master = DMAC1_MSC,    .gate_resp = CY_MS_CTL_GATE_RESP_ERR, .sec_resp = CY_MS_CTL_SEC_RESP_ERR,},
#endif /* CPUSS_DMAC1_PRESENT */
};

/* Number of items in \ref ifx_msc_agc_resp_config */
const size_t ifx_msc_agc_resp_config_count = ARRAY_SIZE(ifx_msc_agc_resp_config);

/* SAU */
const ifx_sau_config_t SAU_config[] = {
    /* This array lists all the areas that should be marked as NS.
     * Note that to save SAU regions, consecutive NS areas are grouped into
     * one SAU region.
     *
     * Additionally Veneers region is configured as NSC during TFM
     * initialization. Veneers region location is not configurable (is
     * determined during linking) so Veneers region is not present in this list.
     */
    { /* ROM BOOT configuration, required for correct work of ROM flash api */
        .base_addr = CY_SFLASH_NS_CBUS_BASE,
        .size      = CY_SFLASH_SIZE,
        .nsc       = false,
    },
    {
        .base_addr = CY_SFLASH_NS_SBUS_BASE,
        .size      = CY_SFLASH_SIZE,
        .nsc       = false,
    },
    { /* Secondary image slot */
        .base_addr = IFX_CM33_NS_SECONDARY_SLOT_ADDR,
        .size      = IFX_CM33_NS_SECONDARY_SLOT_SIZE,
        .nsc       = false,
    },
    { /* NS code */
        .base_addr = IFX_CM33_NS_IMAGE_EXECUTE_ADDR,
        .size      = IFX_CM33_NS_IMAGE_EXECUTE_SIZE,
        .nsc       = false,
    },
    { /* NS data in SRAM */
        .base_addr = IFX_CM33_NS_DATA_ADDR,
        .size      = IFX_CM33_NS_DATA_SIZE,
        .nsc       = false,
    },
    { /* Peripherals regions */
        .base_addr = IFX_PROTECTION_SAU_PERIPHERAL_REGION_START,
        .size      = IFX_PROTECTION_SAU_PERIPHERAL_REGION_SIZE,
        .nsc       = false,
    },
};

/* Number of items in \ref SAU_config */
const size_t CY_SAU_REGION_CNT = ARRAY_SIZE(SAU_config);

/* List of secure interrupts that are not assigned to any Secure Partition
 * (e.g. interrupts used by hardware assigned to SPM) */
const IRQn_Type ifx_secure_interrupts_config[] = {
    cpuss_interrupts_fault_0_IRQn,
    cpuss_interrupt_msc_IRQn,
#ifdef TEST_S_FPU
    TFM_FPU_S_TEST_IRQ
#endif /* TEST_S_FPU */
};

/* Number of items in \ref ifx_secure_interrupts_config */
const size_t ifx_secure_interrupts_config_count = ARRAY_SIZE(ifx_secure_interrupts_config);

/* List of Fault sources for FAULT_STRUCT0 (to handle secure violations) */
const cy_en_SysFault_source_t ifx_fault_sources_fault_struct0[] = {
    PERI_PERI_MS0_PPC_VIO,
    PERI_PERI_MS1_PPC_VIO,
    PERI_PERI_PPC_PC_MASK_VIO,
    PERI_PERI_GP1_TIMEOUT_VIO,
    PERI_PERI_GP2_TIMEOUT_VIO,
    PERI_PERI_GP3_TIMEOUT_VIO,
    PERI_PERI_GP4_TIMEOUT_VIO,
    PERI_PERI_GP5_TIMEOUT_VIO,
    PERI_PERI_GP0_AHB_VIO,
    PERI_PERI_GP1_AHB_VIO,
    PERI_PERI_GP2_AHB_VIO,
    PERI_PERI_GP3_AHB_VIO,
    PERI_PERI_GP4_AHB_VIO,
    PERI_PERI_GP5_AHB_VIO,
    CPUSS_RAMC0_MPC_FAULT_MMIO,
    CPUSS_PROMC_MPC_FAULT_MMIO,
    CPUSS_FLASHC_MPC_FAULT,
};

/* Number of items in \ref ifx_fault_sources_fault_struct0 */
const size_t ifx_fault_sources_fault_struct0_count = ARRAY_SIZE(ifx_fault_sources_fault_struct0);
