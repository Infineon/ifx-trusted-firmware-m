/*
 * Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __FLASH_PROJECT_MEMORY_LAYOUT_H__
#define __FLASH_PROJECT_MEMORY_LAYOUT_H__

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
********************************* Memory areas *********************************
*******************************************************************************/


/*******************************************************************************
************************************* FLASH ************************************
*******************************************************************************/


/************************* TFM image execution region *************************/
#define IFX_TFM_IMAGE_EXECUTE_OFFSET                (IFX_UNSIGNED(0x0)) /* Beginning of FLASH */
#define IFX_TFM_IMAGE_EXECUTE_ADDR                  (IFX_FLASH_OFFSET_TO_S_ALIAS(IFX_TFM_IMAGE_EXECUTE_OFFSET))
#define IFX_TFM_IMAGE_EXECUTE_SIZE                  (IFX_UNSIGNED(0x2E800)) /* 186 KB */


/******************************** TFM ITS area ********************************/
#define IFX_TFM_ITS_OFFSET                          (IFX_UNSIGNED(0x2E800))
#define IFX_TFM_ITS_ADDR                            (IFX_FLASH_OFFSET_TO_S_ALIAS(IFX_TFM_ITS_OFFSET))
#define IFX_TFM_ITS_SIZE                            (IFX_UNSIGNED(0x1000)) /* 4 KB */


/****************************** NV counters area ******************************/
#define IFX_FLASH_NV_COUNTERS_AREA_OFFSET           (IFX_UNSIGNED(0x2F800))
#define IFX_FLASH_NV_COUNTERS_AREA_ADDR             (IFX_FLASH_OFFSET_TO_S_ALIAS(IFX_FLASH_NV_COUNTERS_AREA_OFFSET))
#define IFX_FLASH_NV_COUNTERS_AREA_SIZE             (IFX_UNSIGNED(0x800)) /* 2 KB */


/*********************** CM33 NS image execution region ***********************/
#define IFX_CM33_NS_IMAGE_EXECUTE_OFFSET            (IFX_UNSIGNED(0x30000))
#define IFX_CM33_NS_IMAGE_EXECUTE_ADDR              (IFX_FLASH_OFFSET_TO_NS_ALIAS(IFX_CM33_NS_IMAGE_EXECUTE_OFFSET))
#define IFX_CM33_NS_IMAGE_EXECUTE_SIZE              (IFX_UNSIGNED(0x8000)) /* 32 KB */


/************************ CM33 user app secondary slot ************************/
#define IFX_CM33_NS_SECONDARY_SLOT_OFFSET           (IFX_UNSIGNED(0x38000))
#define IFX_CM33_NS_SECONDARY_SLOT_ADDR             (IFX_FLASH_OFFSET_TO_NS_ALIAS(IFX_CM33_NS_SECONDARY_SLOT_OFFSET))
#define IFX_CM33_NS_SECONDARY_SLOT_SIZE             (IFX_UNSIGNED(0x8000)) /* 32 KB */


/*******************************************************************************
************************************ SRAM0 *************************************
*******************************************************************************/


/******************************* TFM data area ********************************/
#define IFX_TFM_DATA_OFFSET                         (IFX_UNSIGNED(0x0)) /* Beginning of SRAM0 */
#define IFX_TFM_DATA_ADDR                           (IFX_SRAM0_OFFSET_TO_S_ALIAS(IFX_TFM_DATA_OFFSET))
#define IFX_TFM_DATA_SIZE                           (IFX_UNSIGNED(0xC000)) /* 48 KB */


/***************************** CM33 NS data area ******************************/
#define IFX_CM33_NS_DATA_OFFSET                     (IFX_UNSIGNED(0xC000))
#define IFX_CM33_NS_DATA_ADDR                       (IFX_SRAM0_OFFSET_TO_NS_ALIAS(IFX_CM33_NS_DATA_OFFSET))
#define IFX_CM33_NS_DATA_SIZE                       (IFX_UNSIGNED(0x4000)) /* 16 KB */


#ifdef __cplusplus
}
#endif

#endif /* __FLASH_PROJECT_MEMORY_LAYOUT_H__ */
