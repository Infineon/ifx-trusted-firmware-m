/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 * Copyright (c) 2023-2024 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_NS_CTX_H__
#define __TFM_NS_CTX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* Initialize the non-secure context */
bool init_ns_ctx(void);

/*
 * Acquire the non-secure context for a non-secure client thread
 * gid: The group ID of the thread. The threads in one group share one context.
 * idx: Output buffer to retrieve the index of the allocated context.
 * Return: bool type to indicate success of the context allocation.
 */
bool acquire_ns_ctx(uint8_t gid, uint8_t *idx);

/*
 * Release the non-secure context for a non-secure client thread
 * gid: The group ID of the thread. The threads in one group share one context.
 * tid: The thread ID.
 * idx: The context index for that thread.
 * Return: bool type to indicate success of the context release.
 */
bool release_ns_ctx(uint8_t gid, uint8_t tid, uint8_t idx);

/*
 * Load the context for the non-secure client thread
 * gid: The group ID of the thread. The threads in one group share one context.
 * tid: The thread ID.
 * nsid: The non-secure client ID for that thread.
 * idx: The context index for that thread.
 * Return: bool type to indicate success of the context load.
 */
bool load_ns_ctx(uint8_t gid, uint8_t tid, int32_t nsid, uint8_t idx);

/*
 * Save the context for the non-secure client thread
 * gid: The group ID of the thread. The threads in one group share one context.
 * tid: The thread ID.
 * idx: The context index for that thread.
 * Return: bool type to indicate success of the context save.
 */
bool save_ns_ctx(uint8_t gid, uint8_t tid, uint8_t idx);

/*
 * Return non-secure client ID in the active non-secure context.
 * Return invalid NSID if no context is active.
 */
int32_t get_nsid_from_active_ns_ctx(void);

#ifdef __cplusplus
}
#endif

#endif  /* __TFM_NS_CTX_H__ */
