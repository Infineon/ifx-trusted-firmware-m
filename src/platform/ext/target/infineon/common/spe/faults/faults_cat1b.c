/*
 * Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "config_tfm.h"
#include "cmsis.h"
#include "cy_sysfault.h"
#include "exception_info.h"
#include "faults.h"
#include "faults_dump.h"
#include "ifx_utils.h"
#include "tfm_spm_log.h"
#include "utilities.h"
#include "static_checks.h"

TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_8_4, "This function prototype conforms to VECTOR_TABLE_Type, as declared in CMSIS system_Device.h")
void c_cpuss_interrupt_msc_IRQn_Handler(void)
{
    /* Print fault message and block execution */
    SPMLOG_ERRMSG("Platform Exception: MSC fault!!!\r\n");

    /* Clear all causes of MSC interrupt */
    CPUSS->INTR_MSC = 0;

    tfm_core_panic();
}

/* IAR Specific */
#if defined(__ICCARM__)
#pragma required = c_cpuss_interrupt_msc_IRQn_Handler
EXCEPTION_INFO_IAR_REQUIRED
#endif /* defined(__ICCARM__) */
TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_8_4, "This function prototype conforms to VECTOR_TABLE_Type, as declared in CMSIS system_Device.h")
__attribute__((naked)) void cpuss_interrupt_msc_IRQn_Handler(void)
{
    EXCEPTION_INFO();

    __ASM volatile(
        "BL        c_cpuss_interrupt_msc_IRQn_Handler       \n"
        "B         .                                        \n"
    );
}

TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_8_4, "This function prototype conforms to VECTOR_TABLE_Type, as declared in CMSIS system_Device.h")
void c_cpuss_interrupts_fault_0_IRQn_Handler(void)
{
    Cy_SysFault_ClearInterrupt(FAULT_STRUCT0);
    Cy_SysFault_ClearStatus(FAULT_STRUCT0);

    tfm_core_panic();
}

#if defined(__ICCARM__)
#pragma required = c_cpuss_interrupts_fault_0_IRQn_Handler
EXCEPTION_INFO_IAR_REQUIRED
#endif /* defined(__ICCARM__) */
TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_8_4, "This function prototype conforms to VECTOR_TABLE_Type, as declared in CMSIS system_Device.h")
__attribute__((naked)) void cpuss_interrupts_fault_0_IRQn_Handler(void)
{
    EXCEPTION_INFO();

    __ASM volatile(
        "BL        c_cpuss_interrupts_fault_0_IRQn_Handler  \n"
        "B         .                                        \n"
    );
}

FIH_RET_TYPE(enum tfm_plat_err_t) ifx_faults_platform_interrupt_enable(void)
{
    /* Set fault and secure violations priority to less than half of max priority
     * (with AIRCR.PRIS set) to prevent Non-secure from pre-empting faults
     * that may indicate corruption of Secure state. */
    NVIC_SetPriority(cpuss_interrupt_msc_IRQn, 0);
    NVIC_SetPriority(cpuss_interrupts_fault_0_IRQn, 0);

#ifdef TFM_FIH_PROFILE_ON
    (void)fih_delay();

    /* Verify that fault interrupt has the highest priority */
    if (NVIC_GetPriority(cpuss_interrupt_msc_IRQn) != 0U) {
        FIH_RET(fih_int_encode(TFM_PLAT_ERR_SYSTEM_ERR));
    }

    /* Verify that fault interrupt has the highest priority */
    if (NVIC_GetPriority(cpuss_interrupts_fault_0_IRQn) != 0U) {
        FIH_RET(fih_int_encode(TFM_PLAT_ERR_SYSTEM_ERR));
    }
#endif /* TFM_FIH_PROFILE_ON */

    /* Enables needed interrupts */
    NVIC_EnableIRQ(cpuss_interrupt_msc_IRQn);
    NVIC_EnableIRQ(cpuss_interrupts_fault_0_IRQn);

#ifdef TFM_FIH_PROFILE_ON
    (void)fih_delay();

    /* Verify that needed interrupts have been enabled */
    if (!NVIC_GetEnableIRQ(cpuss_interrupt_msc_IRQn) ||
        !NVIC_GetEnableIRQ(cpuss_interrupts_fault_0_IRQn)) {
        FIH_RET(fih_int_encode(TFM_PLAT_ERR_SYSTEM_ERR));
    }
#endif /* TFM_FIH_PROFILE_ON */

    FIH_RET(fih_int_encode(TFM_PLAT_ERR_SUCCESS));
}

#ifdef IFX_FAULTS_INFO_DUMP
void ifx_faults_dump(void)
{
    while (true) {
        cy_en_SysFault_source_t fault_source = Cy_SysFault_GetErrorSource(FAULT_STRUCT0);
        if ((uint8_t)fault_source == CY_SYSFAULT_NO_FAULT) {
            break;
        }

        SPMLOG_ERRMSGVAL("Platform Exception: ", (uint32_t)fault_source);

        switch (fault_source) {
            case PERI_PERI_MS0_PPC_VIO:
            case PERI_PERI_MS1_PPC_VIO: {
                ifx_faults_dump_peri_msx_ppc_vio_fault(FAULT_STRUCT0);
                break;
            }

            case PERI_PERI_PPC_PC_MASK_VIO: {
                ifx_faults_dump_peri_ppc_pc_mask_vio_fault(FAULT_STRUCT0);
                break;
            }

            case PERI_PERI_GP1_TIMEOUT_VIO:
            case PERI_PERI_GP2_TIMEOUT_VIO:
            case PERI_PERI_GP3_TIMEOUT_VIO:
            case PERI_PERI_GP4_TIMEOUT_VIO:
            case PERI_PERI_GP5_TIMEOUT_VIO: {
                ifx_faults_dump_peri_gpx_timeout_vio_fault(FAULT_STRUCT0);
                break;
            }

            case PERI_PERI_GP0_AHB_VIO:
            case PERI_PERI_GP1_AHB_VIO:
            case PERI_PERI_GP2_AHB_VIO:
            case PERI_PERI_GP3_AHB_VIO:
            case PERI_PERI_GP4_AHB_VIO:
            case PERI_PERI_GP5_AHB_VIO: {
                ifx_faults_dump_peri_gpx_ahb_vio_fault(FAULT_STRUCT0);
                break;
            }

            case CPUSS_RAMC0_MPC_FAULT_MMIO:
            case CPUSS_PROMC_MPC_FAULT_MMIO:
            case CPUSS_FLASHC_MPC_FAULT: {
                ifx_faults_dump_mpc_fault(FAULT_STRUCT0);
                break;
            }

            default: {
                ifx_faults_dump_default_fault(FAULT_STRUCT0);
                break;
            }
        }

        Cy_SysFault_ClearStatus(FAULT_STRUCT0);
    }
}
#endif /* IFX_FAULTS_INFO_DUMP */
