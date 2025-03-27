/*
 * Copyright (c) 2024-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "ifx_test_arot_master_api.h"
#include "ifx_test_prot_master_api.h"
#include "ifx_test_arot_slave_api.h"
#include "ifx_test_prot_slave_api.h"
#include "ifx_test_service_api.h"
#include "ifx_test_common.h"

const ifx_test_case_t test_cases[] = {
    {ifx_access_to_partition_memory,     ifx_arot_slave_get_partition_data, IFX_HAS_ACCESS_FALSE, "NS has no access to ARoT\r\n"},
    {ifx_access_to_partition_memory,     ifx_prot_slave_get_partition_data, IFX_HAS_ACCESS_FALSE, "NS has no access to PRoT\r\n"},
    {ifx_access_to_partition_memory,     ifx_test_get_spm_assets,           IFX_HAS_ACCESS_FALSE, "NS has no access to SPM\r\n"},
#if TFM_ISOLATION_LEVEL == 1
    {ifx_test_arot_master_access_memory, ifx_test_get_spm_assets,           IFX_HAS_ACCESS_TRUE,  "ARoT has access to SPM\r\n"},
    {ifx_test_arot_master_access_memory, ifx_prot_slave_get_partition_data, IFX_HAS_ACCESS_TRUE,  "ARoT has access to PRoT\r\n"},
    {ifx_test_arot_master_access_memory, ifx_arot_slave_get_partition_data, IFX_HAS_ACCESS_TRUE,  "ARoT has access to ARoT\r\n"},
    {ifx_test_prot_master_access_memory, ifx_arot_slave_get_partition_data, IFX_HAS_ACCESS_TRUE,  "PRoT has access to ARoT\r\n"},
    {ifx_test_prot_master_access_memory, ifx_prot_slave_get_partition_data, IFX_HAS_ACCESS_TRUE,  "PRoT has access to PRoT\r\n"},
    {ifx_test_prot_master_access_memory, ifx_test_get_spm_assets,           IFX_HAS_ACCESS_TRUE,  "PRoT has access to SPM\r\n"},
    {ifx_test_arot_slave_access_memory,  ifx_arot_slave_get_partition_data, IFX_HAS_ACCESS_TRUE,  "ARoT has access to own assets\r\n"},
    {ifx_test_prot_slave_access_memory,  ifx_prot_slave_get_partition_data, IFX_HAS_ACCESS_TRUE,  "PRoT has access to own assets\r\n"},
#elif TFM_ISOLATION_LEVEL == 2
    {ifx_test_arot_master_access_memory, ifx_test_get_spm_assets,           IFX_HAS_ACCESS_FALSE, "ARoT has no access to SPM\r\n"},
    {ifx_test_arot_master_access_memory, ifx_prot_slave_get_partition_data, IFX_HAS_ACCESS_FALSE, "ARoT has no access to PRoT\r\n"},
    {ifx_test_arot_master_access_memory, ifx_arot_slave_get_partition_data, IFX_HAS_ACCESS_TRUE,  "ARoT has access to ARoT\r\n"},
    {ifx_test_prot_master_access_memory, ifx_arot_slave_get_partition_data, IFX_HAS_ACCESS_TRUE,  "PRoT has access to ARoT\r\n"},
    {ifx_test_prot_master_access_memory, ifx_prot_slave_get_partition_data, IFX_HAS_ACCESS_TRUE,  "PRoT has access to PRoT\r\n"},
    {ifx_test_prot_master_access_memory, ifx_test_get_spm_assets,           IFX_HAS_ACCESS_TRUE,  "PRoT has access to SPM\r\n"},
    {ifx_test_arot_slave_access_memory,  ifx_arot_slave_get_partition_data, IFX_HAS_ACCESS_TRUE,  "ARoT has access to own assets\r\n"},
    {ifx_test_prot_slave_access_memory,  ifx_prot_slave_get_partition_data, IFX_HAS_ACCESS_TRUE,  "PRoT has access to own assets\r\n"},
#elif TFM_ISOLATION_LEVEL == 3
#error TFM_ISOLATION_LEVEL == 3 is not supported for PSC3 device
#endif /* TFM_ISOLATION_LEVEL == 1 */
};

const uint32_t num_of_cases = sizeof(test_cases) / sizeof(test_cases[0]);
