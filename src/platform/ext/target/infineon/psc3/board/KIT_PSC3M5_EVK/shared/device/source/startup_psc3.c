/*
 * Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psc3_core_interrupts.h"
#include "startup_psc3.h"
#include "static_checks.h"

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
TFM_COVERITY_DEVIATE_BLOCK(MISRA_C_2023_Rule_8_5, "Double declaration due to project architecture")
TFM_COVERITY_DEVIATE_BLOCK(MISRA_C_2023_Rule_8_6, "Extern values are defined externally in linker file")
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
extern uint64_t __STACK_SEAL;
#endif
TFM_COVERITY_BLOCK_END(MISRA_C_2023_Rule_8_6)
TFM_COVERITY_BLOCK_END(MISRA_C_2023_Rule_8_5)

TFM_COVERITY_DEVIATE_BLOCK(MISRA_C_2023_Rule_21_2, "__cmsis_start is named this way by cmsis")
TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_8_8, "Static storage modifier is not missing here")
extern __NO_RETURN void __PROGRAM_START(void);
TFM_COVERITY_BLOCK_END(MISRA_C_2023_Rule_21_2)

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler  (void);
__NO_RETURN void Default_Handler(void);

#if defined(__ARMCC_VERSION)
/* __s_vector_table_rw/__ns_vector_table_rw symbols are referred by
 * cy_sysint_v2.c and provided by startup file from BSP.
 * IMPROVEMENT: Rework this solution (DRIVERS-21544). */
#if DOMAIN_S
cy_israddress_cat1b __s_vector_table_rw[VECTORTABLE_SIZE];
#else /* DOMAIN_S */
cy_israddress_cat1b __ns_vector_table_rw[VECTORTABLE_SIZE];
#endif /* DOMAIN_S */
#endif /* defined(__ARMCC_VERSION) */

#ifdef RAM_VECTORS_SUPPORT
    VECTOR_TABLE_Type __vector_table_rw[VECTORTABLE_SIZE] __IFX_RAM_VECTOR_TABLE_ATTRIBUTE;
#endif

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Exceptions */
TFM_COVERITY_DEVIATE_BLOCK(MISRA_C_2023_Rule_8_5, "PendSV_Handler, SVC_Handler, SysTick_Handler are defined in CMSIS libraries")
TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_8_6, "DebugMon_Handler and NMI_Handler are defined in CMSIS libraries")
IFX_CORE_DEFINE_EXCEPTIONS_LIST
TFM_COVERITY_BLOCK_END(MISRA_C_2023_Rule_8_5)

/* Interrupts */
TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_8_6, "IRQn Handlers are defined in MCU libraries")
IFX_CORE_DEFINE_INTERRUPTS_LIST

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
#endif

TFM_COVERITY_DEVIATE_BLOCK(MISRA_C_2023_Rule_11_1, "uint32_t * to VECTOR_TABLE_Type, this cast is acceptable as functions are local.")
TFM_COVERITY_DEVIATE_LINE(MISRA_C_2023_Rule_21_2, "__vector_table is used as is in startup")
const VECTOR_TABLE_Type __vector_table[VECTORTABLE_SIZE] IFX_VECTOR_TABLE_ATTRIBUTE = {
    /* Exceptions */
    IFX_CORE_EXCEPTIONS_LIST
    /* Interrupts */
    IFX_CORE_INTERRUPTS_LIST
};
TFM_COVERITY_BLOCK_END(MISRA_C_2023_Rule_11_1)

#if defined ( __GNUC__ )
    #pragma GCC diagnostic pop
#endif

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler(void)
{
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    __disable_irq();
#endif

#if defined(CONFIG_TFM_ENABLE_CP10CP11)
    /* Compiler may use FPU related assembler commands before FPU is enabled
     * in tfm_arch_config_extensions which will cause a Fault.
     * To avoid this, FPU is additionally enabled for SPE at startup.
     */
    SCB->CPACR |= (3UL << (10U*2U))     /* enable CP10 full access for SPE */
               |  (3UL << (11U*2U));    /* enable CP11 full access for SPE */
#endif

    __set_PSP((uint32_t)(&__INITIAL_SP));

    __set_MSPLIM((uint32_t)(&__STACK_LIMIT));
    __set_PSPLIM((uint32_t)(&__STACK_LIMIT));

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    TFM_COVERITY_DEVIATE_BLOCK(MISRA_C_2023_Rule_11_3, "Intentional pointer type conversion")
    TFM_COVERITY_DEVIATE_LINE(incompatible_cast, "__TZ_set_STACKSEAL_S does necessary type casts, thus suppressing the violation")
    __TZ_set_STACKSEAL_S((uint32_t *)(&__STACK_SEAL));
    TFM_COVERITY_BLOCK_END(MISRA_C_2023_Rule_11_3);
#endif

#ifdef RAM_VECTORS_SUPPORT
    /* GCC copies vector table automatically, but Clang and IAR doesn`t.
     * Copying of vector table is done for all compilers, even tho it is
     * redundant for GCC. This is done to have common way of handling RAM vector
     * table. */
    for (size_t count = 0; count < VECTORTABLE_SIZE; count++)
    {
        __vector_table_rw[count] = __vector_table[count];
    }
    SCB->VTOR = (uint32_t)__vector_table_rw;
#else
    SCB->VTOR = (uint32_t)__vector_table;
#endif

#if DOMAIN_S
    /* SystemInit accesses protected resources, thus run in in Secure side only. */
    SystemInit();                             /* CMSIS System Initialization */
#endif /* DOMAIN_S */

    __PROGRAM_START();                        /* Enter PreMain (C library entry point) */
}

/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
__NO_RETURN void Default_Handler(void)
{
    while(true) {
        ;
    }
}
