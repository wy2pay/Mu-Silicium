/** @file
 * Configuration Map Library for Nubia Z70 (SM8750)
 *
 * Copyright (c) 2024, Project Silicium. All rights reserved.
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#include <Library/BaseLib.h>
#include <Library/ConfigurationMapLib.h>

STATIC CONFIGURATION_DESCRIPTOR_EX gDeviceConfigurationDescriptorEx[] = {
    // Nubia Z70 specific configurations

    // Display Configuration
    {"DisplayPanelType", 0x01},          // Primary panel
    {"DisplayPanelController", 0x00},     // DSI controller 0

    // USB Configuration
    {"EnableUSB", 0x01},
    {"USBRole", 0x02},                    // Dual role (host + device)

    // Storage Configuration
    {"EnableUFS", 0x01},
    {"UFSControllerID", 0x00},

    // Power Configuration
    {"EnableCharging", 0x01},
    {"FastChargingSupport", 0x01},

    // Audio Configuration
    {"EnableAudio", 0x01},
    {"AudioCodec", 0x01},

    // Terminator
    {"Terminator", 0xFFFFFFFF}
};

CONFIGURATION_DESCRIPTOR_EX*
GetDeviceConfigurationMap(
    VOID
)
{
    return gDeviceConfigurationDescriptorEx;
}
