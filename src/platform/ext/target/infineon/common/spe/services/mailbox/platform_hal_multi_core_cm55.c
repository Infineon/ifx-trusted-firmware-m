/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 * Copyright (c) 2019-2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>

#include "cy_ipc_drv.h"
#include "fih.h"
#include "psa/service.h"
#include "tfm_hal_multi_core.h"
#include "tfm_peripherals_def.h"
#include "tfm_sp_log.h"

void tfm_hal_boot_ns_cpu(uintptr_t start_addr)
{
    LOG_INFFMT("Starting Cortex-M55 at 0x%x\r\n", start_addr);
    Cy_SysEnableCM55(IFX_MXCM55, start_addr, CY_SYS_CORE_WAIT_INFINITE);

#ifdef TFM_FIH_PROFILE_ON
    (void)fih_delay();

    /* Verify that CM55 was started from the correct address.
     * From the time CM55 was started till this check, CM55 might have already
     * changed the vector table address (e.g. to RAM_VECTOR_TABLE), thus verify
     * that address of Reset_Handler from active vector table is same as
     * Reset_Handler address from expected vector table.
     */
    if ((*((uint32_t *)((IFX_MXCM55->CM55_NS_VECTOR_TABLE_BASE) + 4U))) !=
        (*((uint32_t *)((start_addr) + 4U)))) {
        psa_panic();
    }
#endif /* TFM_FIH_PROFILE_ON */
}
