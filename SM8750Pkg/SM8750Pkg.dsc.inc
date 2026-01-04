## @file
# SM8750 SoC DSC Include File
#
# Copyright (c) 2024, Project Silicium. All rights reserved.
# SPDX-License-Identifier: BSD-2-Clause-Patent
##

[Defines]
  USE_PHYSICAL_TIMER = 1

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -DSOC_SM8750 -DENABLE_SIMPLE_INIT

[PcdsFixedAtBuild]
  #
  # System Memory Configuration
  # Extracted from Nubia Z70 UEFI firmware
  #
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x200000000

  #
  # UEFI FD Configuration
  #
  gArmTokenSpaceGuid.PcdFdBaseAddress|0xA7000000
  gArmTokenSpaceGuid.PcdFdSize|0x00400000

  #
  # CPU Stack Configuration
  #
  gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0xA760D000
  gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0x40000

  #
  # Timer Configuration (19.2 MHz)
  #
  gArmTokenSpaceGuid.PcdArmArchTimerFreqInHz|19200000
  gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|29
  gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|30
  gArmTokenSpaceGuid.PcdArmArchTimerVirtIntrNum|27
  gArmTokenSpaceGuid.PcdArmArchTimerHypIntrNum|26

  #
  # GIC Configuration (GICv3)
  #
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x17A00000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x17A60000
  gArmTokenSpaceGuid.PcdGicInterruptInterfaceBase|0x17A00000

  #
  # Qualcomm SMEM Configuration
  #
  gQcomPkgTokenSpaceGuid.PcdSmemBaseAddress|0x81D00000
  gQcomPkgTokenSpaceGuid.PcdSmemSize|0x200000

  #
  # UEFI Memory Pool
  #
  gQcomPkgTokenSpaceGuid.PcdUefiMemPoolBase|0xA7000000
  gQcomPkgTokenSpaceGuid.PcdUefiMemPoolSize|0x01000000

  #
  # IMEM Configuration
  #
  gQcomPkgTokenSpaceGuid.PcdIMEMBase|0x14680000
  gQcomPkgTokenSpaceGuid.PcdIMEMSize|0x2C000

  #
  # SPMI Configuration
  #
  gQcomPkgTokenSpaceGuid.PcdSpmiBaseAddress|0x0C400000

  #
  # SoC Specific
  #
  gQcomPkgTokenSpaceGuid.PcdSocCodeName|"pakala"

  #
  # Display Configuration (placeholder - adjust per device)
  #
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferAddress|0xFC800000

  #
  # USB Configuration
  #
  gQcomPkgTokenSpaceGuid.PcdUsbBaseAddress|0x0A600000

[LibraryClasses]
  PlatformMemoryMapLib|Silicon/Qualcomm/SM8750Pkg/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.inf
  PlatformPeiLib|Silicon/Qualcomm/SM8750Pkg/Library/PlatformPeiLib/PlatformPeiLib.inf
  AcpiPlatformUpdateLib|Silicon/Qualcomm/SM8750Pkg/Library/AcpiPlatformUpdateLib/AcpiPlatformUpdateLib.inf

[Components]
  Silicon/Qualcomm/SM8750Pkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf
