#!/usr/bin/env python3
"""
Build script for Nubia Z70 (SM8750)

Copyright (c) 2024, Project Silicium. All rights reserved.
SPDX-License-Identifier: BSD-2-Clause-Patent
"""

import sys
import os
from pathlib import Path

# Device Information
DEVICE_NAME = "z70"
DEVICE_MANUFACTURER = "Nubia"
DEVICE_MODEL = "Z70 Ultra"
SOC_PLATFORM = "SM8750"

# Build Configuration
DSC_PATH = "z70Pkg/z70.dsc"
FDF_PATH = "z70Pkg/z70.fdf"

def main():
    """Main build entry point"""
    print(f"Building UEFI for {DEVICE_MANUFACTURER} {DEVICE_MODEL}")
    print(f"Platform: {SOC_PLATFORM}")
    print(f"Device: {DEVICE_NAME}")

    # Check if running from correct directory
    if not Path("z70Pkg").exists():
        print("Error: Must be run from Platforms/Nubia directory")
        return 1

    print("\nBuild configuration:")
    print(f"  DSC: {DSC_PATH}")
    print(f"  FDF: {FDF_PATH}")
    print(f"  SOC: {SOC_PLATFORM}")

    return 0

if __name__ == "__main__":
    sys.exit(main())
