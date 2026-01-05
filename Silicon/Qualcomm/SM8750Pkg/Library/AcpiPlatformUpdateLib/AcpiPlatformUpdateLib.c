/** @file
 * ACPI Platform Update Library for SM8750
 *
 * Copyright (c) 2024, Project Silicium. All rights reserved.
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/AcpiPlatformUpdateLib.h>

VOID
PlatformUpdateAcpiTables(
    VOID
)
{
    DEBUG((DEBUG_INFO, "SM8750 ACPI Tables Update\\n"));
    
    /* Add platform-specific ACPI table updates here */
}
