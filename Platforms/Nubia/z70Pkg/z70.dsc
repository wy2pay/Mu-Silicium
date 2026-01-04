##
#  Copyright (c) 2011 - 2022, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2020, Intel Corporation. All rights reserved.
#  Copyright (c) 2018, Bingxing Wang. All rights reserved.
#  Copyright (c) Microsoft Corporation.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = z70
  PLATFORM_GUID                  = 4B2E9E32-7D42-4F8B-A9C1-3D6F8E2B4A5C
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/z70Pkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = z70Pkg/z70.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

  #
  # SM8750 SOC Type
  # 0 = SM8750-AB (standard)
  # 1 = SM8750-3-AB (7-core variant)
  # 2 = SM8750-AC (overclocked variant)
  #
  SOC_TYPE                       = 0

  # Firmware Volume Configuration
  FD_BASE                        = 0xA7000000
  FD_SIZE                        = 0x00400000
  FD_BLOCKS                      = 0x400

[PcdsFixedAtBuild]
  # DDR Start Address
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  # UEFI Stack Addresses (from SM8750 memory map)
  gEmbeddedTokenSpaceGuid.PcdPrePiStackBase|0xA760D000
  gEmbeddedTokenSpaceGuid.PcdPrePiStackSize|0x00040000

  # Device GUID
  gSiliciumPkgTokenSpaceGuid.PcdDeviceGuid|{ 0x32, 0x9E, 0x2E, 0x4B, 0x42, 0x7D, 0x8B, 0x4F, 0xA9, 0xC1, 0x3D, 0x6F, 0x8E, 0x2B, 0x4A, 0x5C }

  # SmBios
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Nubia"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Z70 Ultra"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"z70"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Nubia_Z70_Ultra_z70"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Z70 Ultra"

  # Simple Frame Buffer (adjust for actual panel)
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1260
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2800
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  # Platform Pei
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"
  gQcomPkgTokenSpaceGuid.PcdScheduleInterfaceAddr|0xA703A930
  gQcomPkgTokenSpaceGuid.PcdDtbExtensionAddr|0xA703A0C8

  # Dynamic RAM Start Address
  gSiliciumPkgTokenSpaceGuid.PcdRamPartitionBase|0xB90C0000

  # SD Card Slot
  gQcomPkgTokenSpaceGuid.PcdInitCardSlot|FALSE

  # Display Panel
  # Panel: zte_bf369_rm692k0_6p534_dsc_cmd
  gQcomPkgTokenSpaceGuid.PcdPanelType|"zte_bf369_rm692k0"

[LibraryClasses]
  MemoryMapLib|z70Pkg/Library/MemoryMapLib/MemoryMapLib.inf
  ConfigurationMapLib|z70Pkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf
  AcpiDeviceUpdateLib|SiliciumPkg/Library/AcpiDeviceUpdateLibNull/AcpiDeviceUpdateLibNull.inf

# Include SM8750Pkg configuration
!include SM8750Pkg/SM8750Pkg.dsc.inc
