/** @file
 * Memory Map Library for Nubia Z70 (SM8750)
 *
 * Copyright (c) 2024, Project Silicium. All rights reserved.
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#include <Library/BaseLib.h>
#include <Library/MemoryMapLib.h>

// Use SM8750 memory map from SM8750Pkg
#include <Configuration/DeviceMemoryMap.h>

ARM_MEMORY_REGION_DESCRIPTOR_EX*
GetDeviceMemoryMap(
    VOID
)
{
    return gDeviceMemoryDescriptorEx;
}
