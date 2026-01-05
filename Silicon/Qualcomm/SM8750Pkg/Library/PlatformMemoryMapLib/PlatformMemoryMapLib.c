/** @file
 * Platform Memory Map Library for SM8750
 *
 * Copyright (c) 2024, Project Silicium. All rights reserved.
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#include <Library/BaseLib.h>
#include <Library/PlatformMemoryMapLib.h>
#include <Configuration/DeviceMemoryMap.h>

ARM_MEMORY_REGION_DESCRIPTOR_EX*
GetPlatformMemoryMap(
    VOID
)
{
    return gDeviceMemoryDescriptorEx;
}
