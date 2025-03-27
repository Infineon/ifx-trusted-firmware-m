/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2025 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "crt_impl_private.h"
#include "static_checks.h"

void *memcpy(void *dest, const void *src, size_t n)
{
    union composite_addr_t p_dst, p_src;

    TFM_COVERITY_DEVIATE_BLOCK(MISRA_C_2023_Rule_11_6, "Intentional pointer cast, addresses will fit into uintptr_t")
    p_dst.uint_addr = (uintptr_t)dest;
    p_src.uint_addr = (uintptr_t)src;
    TFM_COVERITY_BLOCK_END(MISRA_C_2023_Rule_11_6)

    /* Byte copy for unaligned address. check the last bit of address. */
    while (n && (ADDR_WORD_UNALIGNED(p_dst.uint_addr) ||
                 ADDR_WORD_UNALIGNED(p_src.uint_addr))) {
        *p_dst.p_byte++ = *p_src.p_byte++;
        n--;
    }

    /* Quad byte copy for aligned address. */
    while (n >= sizeof(uint32_t)) {
        *(p_dst.p_word)++ = *(p_src.p_word)++;
        n -= sizeof(uint32_t);
    }

    /* Byte copy for the remaining bytes. */
    while (n--) {
        *p_dst.p_byte++ = *p_src.p_byte++;
    }

    return dest;
}
