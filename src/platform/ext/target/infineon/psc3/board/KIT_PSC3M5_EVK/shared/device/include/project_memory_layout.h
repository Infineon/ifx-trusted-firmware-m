/*
 * Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef PROJECT_MEMORY_LAYOUT_H
#define PROJECT_MEMORY_LAYOUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ifx_utils.h"
#include "mxs22.h"

/*******************************************************************************
****************************** Helper definitions ******************************
*******************************************************************************/


#define IFX_FLASH_BASE                   IFX_UNSIGNED(0x22000000)
#define IFX_FLASH_SIZE                   IFX_UNSIGNED(0x00040000)
#define IFX_SRAM0_BASE                   IFX_UNSIGNED(0x24000000)
#define IFX_SRAM0_SIZE                   IFX_UNSIGNED(0x00010000)


/* Convert FLASH offset to FLASH Secure absolute address */
#define IFX_FLASH_OFFSET_TO_S_ALIAS(x)                (IFX_S_ADDRESS_ALIAS(IFX_FLASH_BASE + (x)))


/* Convert FLASH offset to FLASH Non-Secure absolute address */
#define IFX_FLASH_OFFSET_TO_NS_ALIAS(x)               (IFX_NS_ADDRESS_ALIAS(IFX_FLASH_BASE + (x)))


/* Convert SRAM0 offset to SRAM0 Secure absolute address */
#define IFX_SRAM0_OFFSET_TO_S_ALIAS(x)                (IFX_S_ADDRESS_ALIAS(IFX_SRAM0_BASE + (x)))


/* Convert SRAM0 offset to SRAM0 Non-Secure absolute address */
#define IFX_SRAM0_OFFSET_TO_NS_ALIAS(x)               (IFX_NS_ADDRESS_ALIAS(IFX_SRAM0_BASE + (x)))


/* PSC3 does not have SMIF memory but macro is defined for compatibility with common code */
#define IS_SMIF_MEMORY(mem)                           (0)


/*******************************************************************************
********************************* Memory areas *********************************
*******************************************************************************/

#include IFX_PROJECT_MEMORY_LAYOUT_FILE

#ifdef __cplusplus
}
#endif

#endif /* PROJECT_MEMORY_LAYOUT_H */
