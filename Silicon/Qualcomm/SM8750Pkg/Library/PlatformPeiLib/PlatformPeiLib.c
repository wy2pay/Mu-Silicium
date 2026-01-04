/** @file
 * Platform PEI Library for SM8750
 *
 * Copyright (c) 2024, Project Silicium. All rights reserved.
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/ArmLib.h>

EFI_STATUS
EFIAPI
PlatformPeim(
    VOID
)
{
    DEBUG((DEBUG_INFO, "SM8750 (Pakala) Platform PEI Init\\n"));
    
    /* Add any early platform initialization here */
    
    return EFI_SUCCESS;
}
