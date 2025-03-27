/*
 * Copyright (c) 2022 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __STATIC_CHECKS_H__
#define __STATIC_CHECKS_H__

/*******************************************************************************
*  Provides the macros for MISRA and CERT C violation documentation in Coverity tool.
*******************************************************************************/
/* 
 * Macros usage examples:
 *    1. For MISRA checker violation specification used misra_c_2012_rule_12_1 message
 *       TFM_COVERITY_DEVIATE_LINE(MISRA_C_2012_Rule_12_1, "violation specified message")
 * 
 *    2. For CERT C checker violation specification used cert_exp34_c message
 *       TFM_COVERITY_DEVIATE_LINE(cert_exp34_c, "violation specified message")
 * 
 *    3. If TFM_COVERITY_BLOCK macro is used then you must use Multiple Checkers
 *       (TFM_COVERITY_DEVIATE, TFM_COVERITY_DEVIATE_COUNT, TFM_COVERITY_DEVIATE_COUNT, TFM_COVERITY_FP_COUNT)
 *       as arguments to specified type of actions on violation.
 *
 *       Example: 
 *       TFM_COVERITY_BLOCK(TFM_COVERITY_DEVIATE(cert_int31_c, "type cast is safe because SystemCoreClock limits are checked") \
 *                          TFM_COVERITY_FP(cert_int30_c, "SystemCoreClock limits are checked, wrap is impossible"))
 *       *****deviated block of code*****
 *       TFM_COVERITY_BLOCK_END(cert_int31_c cert_int30_c)
 */
#ifdef TFM_COVERITY_CHECK /* Check MISRA-C:2012 and CERT C with Coverity tool */
#define _TFM_COVERITY_STR(a) #a

#define TFM_COVERITY_DEVIATE_LINE(checker, message) \
_Pragma(_TFM_COVERITY_STR(coverity compliance deviate checker message))

#define TFM_COVERITY_FP_LINE(checker, message) \
_Pragma(_TFM_COVERITY_STR(coverity compliance fp checker message))

#define TFM_COVERITY_DEVIATE_BLOCK(checker, message) \
_Pragma(_TFM_COVERITY_STR(coverity compliance block deviate checker message))

#define TFM_COVERITY_DEVIATE_BLOCK_COUNT(checker, count, message) \
_Pragma(_TFM_COVERITY_STR(coverity compliance block (deviate:count checker message)))

#define TFM_COVERITY_FP_BLOCK(checker, message) \
_Pragma(_TFM_COVERITY_STR(coverity compliance block fp checker message))

#define TFM_COVERITY_FP_BLOCK_COUNT(checker, count, message) \
_Pragma(_TFM_COVERITY_STR(coverity compliance block (fp:count checker message)))

#define TFM_COVERITY_BLOCK(checkers) \
_Pragma(_TFM_COVERITY_STR(coverity compliance block checkers))

#define TFM_COVERITY_BLOCK_END(checker) \
_Pragma(_TFM_COVERITY_STR(coverity compliance end_block checker))

/* Multiple Checkers */
#define TFM_COVERITY_DEVIATE(checker, message)                 (deviate checker message)
#define TFM_COVERITY_DEVIATE_COUNT(checker, count, message)    (deviate:count checker message)
#define TFM_COVERITY_FP(checker, message)                      (fp checker message)
#define TFM_COVERITY_FP_COUNT(checker, count, message)         (fp:count checker message)

#else /* General usage */

#define TFM_COVERITY_DEVIATE_LINE(checker, message)
#define TFM_COVERITY_FP_LINE(checker, message)
#define TFM_COVERITY_DEVIATE_BLOCK(checker, message)
#define TFM_COVERITY_DEVIATE_BLOCK_COUNT(checker, count, message)
#define TFM_COVERITY_FP_BLOCK(checker, message)
#define TFM_COVERITY_FP_BLOCK_COUNT(checker, count, message)
#define TFM_COVERITY_BLOCK(checkers)
#define TFM_COVERITY_BLOCK_END(checker)

/* Multiple Checkers */
#define TFM_COVERITY_DEVIATE(checker, message)
#define TFM_COVERITY_DEVIATE_COUNT(checker, count, message)
#define TFM_COVERITY_FP(checker, message)
#define TFM_COVERITY_FP_COUNT(checker, count, message)

#endif /* TFM_COVERITY_CHECK */

#endif /* __STATIC_CHECKS_H__ */
