/*
 * Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef TEST_PROJECT_MEMORY_LAYOUT_H
#define TEST_PROJECT_MEMORY_LAYOUT_H

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
#define IFX_CM33_NS_IMAGE_EXECUTE_SIZE              (IFX_UNSIGNED(0xF800)) /* 62 KB */


/************************ CM33 user app secondary slot ************************/
#define IFX_CM33_NS_SECONDARY_SLOT_OFFSET           (IFX_UNSIGNED(0x3F800))
#define IFX_CM33_NS_SECONDARY_SLOT_ADDR             (IFX_FLASH_OFFSET_TO_NS_ALIAS(IFX_CM33_NS_SECONDARY_SLOT_OFFSET))
#define IFX_CM33_NS_SECONDARY_SLOT_SIZE             (IFX_UNSIGNED(0x800)) /* 2 KB */


/*******************************************************************************
************************************ SRAM0 *************************************
*******************************************************************************/


/******************************* TFM data area ********************************/
#define IFX_TFM_DATA_OFFSET                         (IFX_UNSIGNED(0x0)) /* Beginning of SRAM0 */
#define IFX_TFM_DATA_ADDR                           (IFX_SRAM0_OFFSET_TO_S_ALIAS(IFX_TFM_DATA_OFFSET))
#define IFX_TFM_DATA_SIZE                           (IFX_UNSIGNED(0xBFC0)) /* 48 KB - 64 B */

/* Reuse same memory regions for IFX tests and PSA Arch tests to save up memory */
/******************** Test memory region for isolation test *******************/
#define IFX_TEST_PROT_PARTITION_MEMORY_OFFSET       (IFX_UNSIGNED(0xBFC0))
#define IFX_TEST_PROT_PARTITION_MEMORY_ADDR         (IFX_SRAM0_OFFSET_TO_S_ALIAS(IFX_TEST_PROT_PARTITION_MEMORY_OFFSET))
#define IFX_TEST_PROT_PARTITION_MEMORY_SIZE         (IFX_UNSIGNED(0x20)) /* 32 B */


/******************** Test memory region for isolation test *******************/
#define IFX_TEST_AROT_PARTITION_MEMORY_OFFSET       (IFX_UNSIGNED(0xBFE0))
#define IFX_TEST_AROT_PARTITION_MEMORY_ADDR         (IFX_SRAM0_OFFSET_TO_S_ALIAS(IFX_TEST_AROT_PARTITION_MEMORY_OFFSET))
#define IFX_TEST_AROT_PARTITION_MEMORY_SIZE         (IFX_UNSIGNED(0x20)) /* 32 B */


/***************** PSA arch tests - FF server partition memory ****************/
#define IFX_FF_TEST_SERVER_PARTITION_MMIO_OFFSET    (IFX_UNSIGNED(0xBFC0))
#define IFX_FF_TEST_SERVER_PARTITION_MMIO_ADDR      (IFX_SRAM0_OFFSET_TO_S_ALIAS(IFX_FF_TEST_SERVER_PARTITION_MMIO_OFFSET))
#define IFX_FF_TEST_SERVER_PARTITION_MMIO_SIZE      (IFX_UNSIGNED(0x20)) /* 32 B */


/***************** PSA arch tests - FF driver partition memory ****************/
#define IFX_FF_TEST_DRIVER_PARTITION_MMIO_OFFSET    (IFX_UNSIGNED(0xBFE0))
#define IFX_FF_TEST_DRIVER_PARTITION_MMIO_ADDR      (IFX_SRAM0_OFFSET_TO_S_ALIAS(IFX_FF_TEST_DRIVER_PARTITION_MMIO_OFFSET))
#define IFX_FF_TEST_DRIVER_PARTITION_MMIO_SIZE      (IFX_UNSIGNED(0x20)) /* 32 B */


/***************************** CM33 NS data area ******************************/
#define IFX_CM33_NS_DATA_OFFSET                     (IFX_UNSIGNED(0xC000))
#define IFX_CM33_NS_DATA_ADDR                       (IFX_SRAM0_OFFSET_TO_NS_ALIAS(IFX_CM33_NS_DATA_OFFSET))
#define IFX_CM33_NS_DATA_SIZE                       (IFX_UNSIGNED(0x4000)) /* 16 KB */


#ifdef __cplusplus
}
#endif

#endif /* TEST_PROJECT_MEMORY_LAYOUT_H */
