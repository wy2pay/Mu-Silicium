/** @file
 * SMBIOS Table DXE Driver for SM8750 (Snapdragon 8 Elite)
 *
 * Creates SMBIOS tables for Windows/Linux compatibility
 *
 * Copyright (c) 2024, Project Silicium. All rights reserved.
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>

/* SMBIOS Table Handle Counter */
STATIC UINT16 mSmbiosTableHandle = 0x100;

/* Get next available SMBIOS handle */
STATIC
UINT16
GetNextSmbiosHandle(
    VOID
)
{
    return mSmbiosTableHandle++;
}

/*****************************************************************************
 * Type 0 - BIOS Information
 *****************************************************************************/
STATIC
EFI_STATUS
InstallSmbiosType0(
    IN EFI_SMBIOS_PROTOCOL *Smbios
)
{
    EFI_STATUS          Status;
    EFI_SMBIOS_HANDLE   Handle;
    SMBIOS_TABLE_TYPE0  *Type0;
    CHAR8               *StringPtr;
    UINTN               TableSize;
    
    /* Strings for Type 0 */
    CHAR8 *Vendor       = "Project Silicium";
    CHAR8 *BiosVersion  = "Mu-Silicium UEFI";
    CHAR8 *BiosDate     = __DATE__;
    
    /* Calculate table size with strings */
    TableSize = sizeof(SMBIOS_TABLE_TYPE0) + 
                AsciiStrLen(Vendor) + 1 +
                AsciiStrLen(BiosVersion) + 1 +
                AsciiStrLen(BiosDate) + 1 + 1;
    
    Type0 = AllocateZeroPool(TableSize);
    if (Type0 == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }
    
    /* Fill in Type 0 structure */
    Type0->Hdr.Type                         = EFI_SMBIOS_TYPE_BIOS_INFORMATION;
    Type0->Hdr.Length                       = sizeof(SMBIOS_TABLE_TYPE0);
    Type0->Hdr.Handle                       = GetNextSmbiosHandle();
    Type0->Vendor                           = 1;  /* String 1 */
    Type0->BiosVersion                      = 2;  /* String 2 */
    Type0->BiosSegment                      = 0;
    Type0->BiosReleaseDate                  = 3;  /* String 3 */
    Type0->BiosSize                         = 0xFF;  /* 16MB+ */
    Type0->BiosCharacteristics.AcpiIsSupported              = 1;
    Type0->BiosCharacteristics.UefiSpecificationSupported   = 1;
    Type0->BiosCharacteristics.TargetContentDistributionEnabled = 1;
    Type0->BIOSCharacteristicsExtensionBytes[0]             = 0x01;
    Type0->BIOSCharacteristicsExtensionBytes[1]             = 0x0C;
    Type0->SystemBiosMajorRelease           = 1;
    Type0->SystemBiosMinorRelease           = 0;
    Type0->EmbeddedControllerFirmwareMajorRelease = 0xFF;
    Type0->EmbeddedControllerFirmwareMinorRelease = 0xFF;
    Type0->ExtendedBiosSize.Size            = 4;  /* 4MB */
    Type0->ExtendedBiosSize.Unit            = 0;  /* MB */
    
    /* Add strings */
    StringPtr = (CHAR8 *)Type0 + sizeof(SMBIOS_TABLE_TYPE0);
    AsciiStrCpyS(StringPtr, AsciiStrLen(Vendor) + 1, Vendor);
    StringPtr += AsciiStrLen(Vendor) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(BiosVersion) + 1, BiosVersion);
    StringPtr += AsciiStrLen(BiosVersion) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(BiosDate) + 1, BiosDate);
    StringPtr += AsciiStrLen(BiosDate) + 1;
    *StringPtr = 0;  /* Double null terminator */
    
    Handle = SMBIOS_HANDLE_PI_RESERVED;
    Status = Smbios->Add(Smbios, NULL, &Handle, (EFI_SMBIOS_TABLE_HEADER *)Type0);
    
    FreePool(Type0);
    
    DEBUG((DEBUG_INFO, "SMBIOS Type 0 installed: %r\n", Status));
    return Status;
}

/*****************************************************************************
 * Type 1 - System Information
 *****************************************************************************/
STATIC
EFI_STATUS
InstallSmbiosType1(
    IN EFI_SMBIOS_PROTOCOL *Smbios,
    IN CHAR8               *DeviceVendor,
    IN CHAR8               *DeviceName,
    IN CHAR8               *DeviceCodename
)
{
    EFI_STATUS          Status;
    EFI_SMBIOS_HANDLE   Handle;
    SMBIOS_TABLE_TYPE1  *Type1;
    CHAR8               *StringPtr;
    UINTN               TableSize;
    
    CHAR8 *Version      = "1.0";
    CHAR8 *SerialNumber = "123456789";
    CHAR8 *SkuNumber    = DeviceCodename;
    CHAR8 *Family       = "Snapdragon 8 Elite";
    
    TableSize = sizeof(SMBIOS_TABLE_TYPE1) +
                AsciiStrLen(DeviceVendor) + 1 +
                AsciiStrLen(DeviceName) + 1 +
                AsciiStrLen(Version) + 1 +
                AsciiStrLen(SerialNumber) + 1 +
                AsciiStrLen(SkuNumber) + 1 +
                AsciiStrLen(Family) + 1 + 1;
    
    Type1 = AllocateZeroPool(TableSize);
    if (Type1 == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }
    
    Type1->Hdr.Type     = EFI_SMBIOS_TYPE_SYSTEM_INFORMATION;
    Type1->Hdr.Length   = sizeof(SMBIOS_TABLE_TYPE1);
    Type1->Hdr.Handle   = GetNextSmbiosHandle();
    Type1->Manufacturer = 1;
    Type1->ProductName  = 2;
    Type1->Version      = 3;
    Type1->SerialNumber = 4;
    
    /* UUID - generate a pseudo-random one based on device name */
    Type1->Uuid.Data1 = 0x12345678;
    Type1->Uuid.Data2 = 0x9ABC;
    Type1->Uuid.Data3 = 0xDEF0;
    Type1->Uuid.Data4[0] = 'S';
    Type1->Uuid.Data4[1] = 'M';
    Type1->Uuid.Data4[2] = '8';
    Type1->Uuid.Data4[3] = '7';
    Type1->Uuid.Data4[4] = '5';
    Type1->Uuid.Data4[5] = '0';
    Type1->Uuid.Data4[6] = 0x00;
    Type1->Uuid.Data4[7] = 0x01;
    
    Type1->WakeUpType   = SystemWakeupTypePowerSwitch;
    Type1->SKUNumber    = 5;
    Type1->Family       = 6;
    
    StringPtr = (CHAR8 *)Type1 + sizeof(SMBIOS_TABLE_TYPE1);
    AsciiStrCpyS(StringPtr, AsciiStrLen(DeviceVendor) + 1, DeviceVendor);
    StringPtr += AsciiStrLen(DeviceVendor) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(DeviceName) + 1, DeviceName);
    StringPtr += AsciiStrLen(DeviceName) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(Version) + 1, Version);
    StringPtr += AsciiStrLen(Version) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(SerialNumber) + 1, SerialNumber);
    StringPtr += AsciiStrLen(SerialNumber) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(SkuNumber) + 1, SkuNumber);
    StringPtr += AsciiStrLen(SkuNumber) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(Family) + 1, Family);
    StringPtr += AsciiStrLen(Family) + 1;
    *StringPtr = 0;
    
    Handle = SMBIOS_HANDLE_PI_RESERVED;
    Status = Smbios->Add(Smbios, NULL, &Handle, (EFI_SMBIOS_TABLE_HEADER *)Type1);
    
    FreePool(Type1);
    
    DEBUG((DEBUG_INFO, "SMBIOS Type 1 installed: %r\n", Status));
    return Status;
}

/*****************************************************************************
 * Type 2 - Baseboard Information
 *****************************************************************************/
STATIC
EFI_STATUS
InstallSmbiosType2(
    IN EFI_SMBIOS_PROTOCOL *Smbios,
    IN CHAR8               *DeviceVendor,
    IN CHAR8               *DeviceName,
    IN CHAR8               *DeviceCodename
)
{
    EFI_STATUS          Status;
    EFI_SMBIOS_HANDLE   Handle;
    SMBIOS_TABLE_TYPE2  *Type2;
    CHAR8               *StringPtr;
    UINTN               TableSize;
    
    CHAR8 *Version      = "1.0";
    CHAR8 *SerialNumber = "BSN123456";
    CHAR8 *AssetTag     = DeviceCodename;
    CHAR8 *Location     = "Main Board";
    
    TableSize = sizeof(SMBIOS_TABLE_TYPE2) +
                AsciiStrLen(DeviceVendor) + 1 +
                AsciiStrLen(DeviceName) + 1 +
                AsciiStrLen(Version) + 1 +
                AsciiStrLen(SerialNumber) + 1 +
                AsciiStrLen(AssetTag) + 1 +
                AsciiStrLen(Location) + 1 + 1;
    
    Type2 = AllocateZeroPool(TableSize);
    if (Type2 == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }
    
    Type2->Hdr.Type             = EFI_SMBIOS_TYPE_BASEBOARD_INFORMATION;
    Type2->Hdr.Length           = sizeof(SMBIOS_TABLE_TYPE2);
    Type2->Hdr.Handle           = GetNextSmbiosHandle();
    Type2->Manufacturer         = 1;
    Type2->ProductName          = 2;
    Type2->Version              = 3;
    Type2->SerialNumber         = 4;
    Type2->AssetTag             = 5;
    Type2->FeatureFlag.Motherboard           = 1;
    Type2->FeatureFlag.Replaceable           = 0;
    Type2->LocationInChassis    = 6;
    Type2->ChassisHandle        = 0;
    Type2->BoardType            = BaseBoardTypeMotherBoard;
    
    StringPtr = (CHAR8 *)Type2 + sizeof(SMBIOS_TABLE_TYPE2);
    AsciiStrCpyS(StringPtr, AsciiStrLen(DeviceVendor) + 1, DeviceVendor);
    StringPtr += AsciiStrLen(DeviceVendor) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(DeviceName) + 1, DeviceName);
    StringPtr += AsciiStrLen(DeviceName) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(Version) + 1, Version);
    StringPtr += AsciiStrLen(Version) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(SerialNumber) + 1, SerialNumber);
    StringPtr += AsciiStrLen(SerialNumber) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(AssetTag) + 1, AssetTag);
    StringPtr += AsciiStrLen(AssetTag) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(Location) + 1, Location);
    StringPtr += AsciiStrLen(Location) + 1;
    *StringPtr = 0;
    
    Handle = SMBIOS_HANDLE_PI_RESERVED;
    Status = Smbios->Add(Smbios, NULL, &Handle, (EFI_SMBIOS_TABLE_HEADER *)Type2);
    
    FreePool(Type2);
    
    DEBUG((DEBUG_INFO, "SMBIOS Type 2 installed: %r\n", Status));
    return Status;
}

/*****************************************************************************
 * Type 3 - Chassis Information
 *****************************************************************************/
STATIC
EFI_STATUS
InstallSmbiosType3(
    IN EFI_SMBIOS_PROTOCOL *Smbios,
    IN CHAR8               *DeviceVendor
)
{
    EFI_STATUS          Status;
    EFI_SMBIOS_HANDLE   Handle;
    SMBIOS_TABLE_TYPE3  *Type3;
    CHAR8               *StringPtr;
    UINTN               TableSize;
    
    CHAR8 *Version      = "1.0";
    CHAR8 *SerialNumber = "CSN123456";
    CHAR8 *AssetTag     = "SM8750";
    CHAR8 *SkuNumber    = "Pakala";
    
    TableSize = sizeof(SMBIOS_TABLE_TYPE3) +
                AsciiStrLen(DeviceVendor) + 1 +
                AsciiStrLen(Version) + 1 +
                AsciiStrLen(SerialNumber) + 1 +
                AsciiStrLen(AssetTag) + 1 +
                AsciiStrLen(SkuNumber) + 1 + 1;
    
    Type3 = AllocateZeroPool(TableSize);
    if (Type3 == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }
    
    Type3->Hdr.Type                 = EFI_SMBIOS_TYPE_SYSTEM_ENCLOSURE;
    Type3->Hdr.Length               = sizeof(SMBIOS_TABLE_TYPE3);
    Type3->Hdr.Handle               = GetNextSmbiosHandle();
    Type3->Manufacturer             = 1;
    Type3->Type                     = MiscChassisTypeHandHeld;
    Type3->Version                  = 2;
    Type3->SerialNumber             = 3;
    Type3->AssetTag                 = 4;
    Type3->BootupState              = ChassisStateSafe;
    Type3->PowerSupplyState         = ChassisStateSafe;
    Type3->ThermalState             = ChassisStateSafe;
    Type3->SecurityStatus           = ChassisSecurityStatusNone;
    Type3->OemDefined               = 0;
    Type3->Height                   = 0;
    Type3->NumberofPowerCords       = 0;
    Type3->ContainedElementCount    = 0;
    Type3->ContainedElementRecordLength = 0;
    Type3->SKUNumber                = 5;
    
    StringPtr = (CHAR8 *)Type3 + sizeof(SMBIOS_TABLE_TYPE3);
    AsciiStrCpyS(StringPtr, AsciiStrLen(DeviceVendor) + 1, DeviceVendor);
    StringPtr += AsciiStrLen(DeviceVendor) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(Version) + 1, Version);
    StringPtr += AsciiStrLen(Version) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(SerialNumber) + 1, SerialNumber);
    StringPtr += AsciiStrLen(SerialNumber) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(AssetTag) + 1, AssetTag);
    StringPtr += AsciiStrLen(AssetTag) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(SkuNumber) + 1, SkuNumber);
    StringPtr += AsciiStrLen(SkuNumber) + 1;
    *StringPtr = 0;
    
    Handle = SMBIOS_HANDLE_PI_RESERVED;
    Status = Smbios->Add(Smbios, NULL, &Handle, (EFI_SMBIOS_TABLE_HEADER *)Type3);
    
    FreePool(Type3);
    
    DEBUG((DEBUG_INFO, "SMBIOS Type 3 installed: %r\n", Status));
    return Status;
}

/*****************************************************************************
 * Type 7 - Cache Information
 *****************************************************************************/
STATIC UINT16 mCacheHandleL1 = 0;
STATIC UINT16 mCacheHandleL2 = 0;
STATIC UINT16 mCacheHandleL3 = 0;

STATIC
EFI_STATUS
InstallSmbiosType7(
    IN EFI_SMBIOS_PROTOCOL *Smbios
)
{
    EFI_STATUS          Status;
    EFI_SMBIOS_HANDLE   Handle;
    SMBIOS_TABLE_TYPE7  *Type7;
    CHAR8               *StringPtr;
    UINTN               TableSize;
    UINTN               i;
    
    /* SM8750 Oryon Cache Configuration:
     * L1I: 192KB per core (8 cores)
     * L1D: 128KB per core (8 cores)
     * L2: 12MB shared (per cluster)
     * L3: 12MB system cache
     */
    
    struct {
        CHAR8   *Name;
        UINT16  MaxSize;    /* KB */
        UINT16  InstallSize;
        UINT8   CacheLevel;
        UINT8   Associativity;
        UINT16  *HandlePtr;
    } CacheInfo[] = {
        { "L1 Cache",    320,  320,  1, CacheAssociativity8Way,  &mCacheHandleL1 },
        { "L2 Cache",   12288, 12288, 2, CacheAssociativity16Way, &mCacheHandleL2 },
        { "L3 Cache",   12288, 12288, 3, CacheAssociativity16Way, &mCacheHandleL3 },
    };
    
    for (i = 0; i < sizeof(CacheInfo)/sizeof(CacheInfo[0]); i++) {
        TableSize = sizeof(SMBIOS_TABLE_TYPE7) + AsciiStrLen(CacheInfo[i].Name) + 1 + 1;
        
        Type7 = AllocateZeroPool(TableSize);
        if (Type7 == NULL) {
            return EFI_OUT_OF_RESOURCES;
        }
        
        Type7->Hdr.Type                         = EFI_SMBIOS_TYPE_CACHE_INFORMATION;
        Type7->Hdr.Length                       = sizeof(SMBIOS_TABLE_TYPE7);
        Type7->Hdr.Handle                       = GetNextSmbiosHandle();
        *(CacheInfo[i].HandlePtr)               = Type7->Hdr.Handle;
        Type7->SocketDesignation                = 1;
        Type7->CacheConfiguration               = (CacheInfo[i].CacheLevel - 1) | (1 << 7);
        Type7->MaximumCacheSize                 = CacheInfo[i].MaxSize;
        Type7->InstalledSize                    = CacheInfo[i].InstallSize;
        Type7->SupportedSRAMType.Synchronous    = 1;
        Type7->CurrentSRAMType.Synchronous      = 1;
        Type7->CacheSpeed                       = 0;
        Type7->ErrorCorrectionType              = CacheErrorSingleBit;
        Type7->SystemCacheType                  = CacheTypeUnified;
        Type7->Associativity                    = CacheInfo[i].Associativity;
        Type7->MaximumCacheSize2                = CacheInfo[i].MaxSize;
        Type7->InstalledSize2                   = CacheInfo[i].InstallSize;
        
        StringPtr = (CHAR8 *)Type7 + sizeof(SMBIOS_TABLE_TYPE7);
        AsciiStrCpyS(StringPtr, AsciiStrLen(CacheInfo[i].Name) + 1, CacheInfo[i].Name);
        StringPtr += AsciiStrLen(CacheInfo[i].Name) + 1;
        *StringPtr = 0;
        
        Handle = SMBIOS_HANDLE_PI_RESERVED;
        Status = Smbios->Add(Smbios, NULL, &Handle, (EFI_SMBIOS_TABLE_HEADER *)Type7);
        
        FreePool(Type7);
        
        DEBUG((DEBUG_INFO, "SMBIOS Type 7 (%a) installed: %r\n", CacheInfo[i].Name, Status));
    }
    
    return EFI_SUCCESS;
}

/*****************************************************************************
 * Type 4 - Processor Information
 *****************************************************************************/
STATIC
EFI_STATUS
InstallSmbiosType4(
    IN EFI_SMBIOS_PROTOCOL *Smbios
)
{
    EFI_STATUS          Status;
    EFI_SMBIOS_HANDLE   Handle;
    SMBIOS_TABLE_TYPE4  *Type4;
    CHAR8               *StringPtr;
    UINTN               TableSize;
    
    CHAR8 *SocketDesignation     = "SM8750";
    CHAR8 *ProcessorManufacturer = "Qualcomm";
    CHAR8 *ProcessorVersion      = "Snapdragon 8 Elite (Oryon)";
    CHAR8 *SerialNumber          = "N/A";
    CHAR8 *AssetTag              = "Pakala";
    CHAR8 *PartNumber            = "SM8750-AB";
    
    TableSize = sizeof(SMBIOS_TABLE_TYPE4) +
                AsciiStrLen(SocketDesignation) + 1 +
                AsciiStrLen(ProcessorManufacturer) + 1 +
                AsciiStrLen(ProcessorVersion) + 1 +
                AsciiStrLen(SerialNumber) + 1 +
                AsciiStrLen(AssetTag) + 1 +
                AsciiStrLen(PartNumber) + 1 + 1;
    
    Type4 = AllocateZeroPool(TableSize);
    if (Type4 == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }
    
    Type4->Hdr.Type                 = EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION;
    Type4->Hdr.Length               = sizeof(SMBIOS_TABLE_TYPE4);
    Type4->Hdr.Handle               = GetNextSmbiosHandle();
    Type4->Socket                   = 1;
    Type4->ProcessorType            = CentralProcessor;
    Type4->ProcessorFamily          = ProcessorFamilyARMv8;
    Type4->ProcessorManufacturer    = 2;
    
    /* Processor ID - ARM MIDR equivalent */
    Type4->ProcessorId.Signature.ProcessorFamily    = 0x8;
    Type4->ProcessorId.Signature.ProcessorModel     = 0x750;
    Type4->ProcessorId.Signature.ProcessorStepping  = 0x1;
    Type4->ProcessorId.FeatureFlags                 = 0;
    
    Type4->ProcessorVersion         = 3;
    Type4->Voltage.ProcessorVoltageCapability3_3V = 0;
    Type4->Voltage.ProcessorVoltageCapability2_9V = 0;
    Type4->Voltage.ProcessorVoltage               = 8;  /* 0.8V */
    Type4->ExternalClock            = 19;   /* 19.2 MHz */
    Type4->MaxSpeed                 = 4320; /* 4.32 GHz (Prime cores) */
    Type4->CurrentSpeed             = 4320;
    Type4->Status                   = (1 << 6) | 1;  /* CPU Enabled, Populated */
    Type4->ProcessorUpgrade         = ProcessorUpgradeNone;
    Type4->L1CacheHandle            = mCacheHandleL1;
    Type4->L2CacheHandle            = mCacheHandleL2;
    Type4->L3CacheHandle            = mCacheHandleL3;
    Type4->SerialNumber             = 4;
    Type4->AssetTag                 = 5;
    Type4->PartNumber               = 6;
    Type4->CoreCount                = 8;
    Type4->EnabledCoreCount         = 8;
    Type4->ThreadCount              = 8;
    Type4->ProcessorCharacteristics = (1 << 2) | (1 << 3) | (1 << 5);
    Type4->ProcessorFamily2         = ProcessorFamilyARMv8;
    Type4->CoreCount2               = 8;
    Type4->EnabledCoreCount2        = 8;
    Type4->ThreadCount2             = 8;
    
    StringPtr = (CHAR8 *)Type4 + sizeof(SMBIOS_TABLE_TYPE4);
    AsciiStrCpyS(StringPtr, AsciiStrLen(SocketDesignation) + 1, SocketDesignation);
    StringPtr += AsciiStrLen(SocketDesignation) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(ProcessorManufacturer) + 1, ProcessorManufacturer);
    StringPtr += AsciiStrLen(ProcessorManufacturer) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(ProcessorVersion) + 1, ProcessorVersion);
    StringPtr += AsciiStrLen(ProcessorVersion) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(SerialNumber) + 1, SerialNumber);
    StringPtr += AsciiStrLen(SerialNumber) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(AssetTag) + 1, AssetTag);
    StringPtr += AsciiStrLen(AssetTag) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(PartNumber) + 1, PartNumber);
    StringPtr += AsciiStrLen(PartNumber) + 1;
    *StringPtr = 0;
    
    Handle = SMBIOS_HANDLE_PI_RESERVED;
    Status = Smbios->Add(Smbios, NULL, &Handle, (EFI_SMBIOS_TABLE_HEADER *)Type4);
    
    FreePool(Type4);
    
    DEBUG((DEBUG_INFO, "SMBIOS Type 4 installed: %r\n", Status));
    return Status;
}

/*****************************************************************************
 * Type 16 - Physical Memory Array
 *****************************************************************************/
STATIC UINT16 mMemArrayHandle = 0;

STATIC
EFI_STATUS
InstallSmbiosType16(
    IN EFI_SMBIOS_PROTOCOL *Smbios,
    IN UINT64              MemorySize
)
{
    EFI_STATUS           Status;
    EFI_SMBIOS_HANDLE    Handle;
    SMBIOS_TABLE_TYPE16  *Type16;
    UINTN                TableSize;
    
    TableSize = sizeof(SMBIOS_TABLE_TYPE16) + 2;
    
    Type16 = AllocateZeroPool(TableSize);
    if (Type16 == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }
    
    Type16->Hdr.Type                    = EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY;
    Type16->Hdr.Length                  = sizeof(SMBIOS_TABLE_TYPE16);
    Type16->Hdr.Handle                  = GetNextSmbiosHandle();
    mMemArrayHandle                     = Type16->Hdr.Handle;
    Type16->Location                    = MemoryArrayLocationSystemBoard;
    Type16->Use                         = MemoryArrayUseSystemMemory;
    Type16->MemoryErrorCorrection       = MemoryErrorCorrectionNone;
    Type16->MaximumCapacity             = (UINT32)(MemorySize / 1024);
    Type16->MemoryErrorInformationHandle = 0xFFFE;
    Type16->NumberOfMemoryDevices       = 1;
    Type16->ExtendedMaximumCapacity     = MemorySize;
    
    *((CHAR8 *)Type16 + sizeof(SMBIOS_TABLE_TYPE16)) = 0;
    *((CHAR8 *)Type16 + sizeof(SMBIOS_TABLE_TYPE16) + 1) = 0;
    
    Handle = SMBIOS_HANDLE_PI_RESERVED;
    Status = Smbios->Add(Smbios, NULL, &Handle, (EFI_SMBIOS_TABLE_HEADER *)Type16);
    
    FreePool(Type16);
    
    DEBUG((DEBUG_INFO, "SMBIOS Type 16 installed: %r\n", Status));
    return Status;
}

/*****************************************************************************
 * Type 17 - Memory Device
 *****************************************************************************/
STATIC
EFI_STATUS
InstallSmbiosType17(
    IN EFI_SMBIOS_PROTOCOL *Smbios,
    IN UINT64              MemorySize
)
{
    EFI_STATUS           Status;
    EFI_SMBIOS_HANDLE    Handle;
    SMBIOS_TABLE_TYPE17  *Type17;
    CHAR8                *StringPtr;
    UINTN                TableSize;
    
    CHAR8 *DeviceLocator   = "On-Board";
    CHAR8 *BankLocator     = "BANK 0";
    CHAR8 *Manufacturer    = "Samsung";
    CHAR8 *SerialNumber    = "00000000";
    CHAR8 *AssetTag        = "LPDDR5X";
    CHAR8 *PartNumber      = "K3LK7K70BM-BGCP";
    CHAR8 *FirmwareVersion = "1.0";
    
    TableSize = sizeof(SMBIOS_TABLE_TYPE17) +
                AsciiStrLen(DeviceLocator) + 1 +
                AsciiStrLen(BankLocator) + 1 +
                AsciiStrLen(Manufacturer) + 1 +
                AsciiStrLen(SerialNumber) + 1 +
                AsciiStrLen(AssetTag) + 1 +
                AsciiStrLen(PartNumber) + 1 +
                AsciiStrLen(FirmwareVersion) + 1 + 1;
    
    Type17 = AllocateZeroPool(TableSize);
    if (Type17 == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }
    
    Type17->Hdr.Type                        = EFI_SMBIOS_TYPE_MEMORY_DEVICE;
    Type17->Hdr.Length                      = sizeof(SMBIOS_TABLE_TYPE17);
    Type17->Hdr.Handle                      = GetNextSmbiosHandle();
    Type17->MemoryArrayHandle               = mMemArrayHandle;
    Type17->MemoryErrorInformationHandle    = 0xFFFE;
    Type17->TotalWidth                      = 64;
    Type17->DataWidth                       = 64;
    Type17->Size                            = (UINT16)(MemorySize / (1024 * 1024));
    Type17->FormFactor                      = MemoryFormFactorChip;
    Type17->DeviceSet                       = 0;
    Type17->DeviceLocator                   = 1;
    Type17->BankLocator                     = 2;
    Type17->MemoryType                      = MemoryTypeLpddr5;
    Type17->TypeDetail.Synchronous          = 1;
    Type17->Speed                           = 8533;
    Type17->Manufacturer                    = 3;
    Type17->SerialNumber                    = 4;
    Type17->AssetTag                        = 5;
    Type17->PartNumber                      = 6;
    Type17->Attributes                      = 2;
    Type17->ExtendedSize                    = (UINT32)(MemorySize / (1024 * 1024));
    Type17->ConfiguredMemoryClockSpeed      = 8533;
    Type17->MinimumVoltage                  = 500;
    Type17->MaximumVoltage                  = 1100;
    Type17->ConfiguredVoltage               = 1050;
    Type17->MemoryTechnology                = MemoryTechnologyDram;
    Type17->MemoryOperatingModeCapability.VolatileMemory = 1;
    Type17->FirmwareVersion                 = 7;
    Type17->ModuleManufacturerID            = 0xCE00;
    Type17->ModuleProductID                 = 0x0000;
    Type17->VolatileSize                    = MemorySize;
    Type17->ExtendedSpeed                   = 8533;
    Type17->ExtendedConfiguredMemorySpeed   = 8533;
    
    StringPtr = (CHAR8 *)Type17 + sizeof(SMBIOS_TABLE_TYPE17);
    AsciiStrCpyS(StringPtr, AsciiStrLen(DeviceLocator) + 1, DeviceLocator);
    StringPtr += AsciiStrLen(DeviceLocator) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(BankLocator) + 1, BankLocator);
    StringPtr += AsciiStrLen(BankLocator) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(Manufacturer) + 1, Manufacturer);
    StringPtr += AsciiStrLen(Manufacturer) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(SerialNumber) + 1, SerialNumber);
    StringPtr += AsciiStrLen(SerialNumber) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(AssetTag) + 1, AssetTag);
    StringPtr += AsciiStrLen(AssetTag) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(PartNumber) + 1, PartNumber);
    StringPtr += AsciiStrLen(PartNumber) + 1;
    AsciiStrCpyS(StringPtr, AsciiStrLen(FirmwareVersion) + 1, FirmwareVersion);
    StringPtr += AsciiStrLen(FirmwareVersion) + 1;
    *StringPtr = 0;
    
    Handle = SMBIOS_HANDLE_PI_RESERVED;
    Status = Smbios->Add(Smbios, NULL, &Handle, (EFI_SMBIOS_TABLE_HEADER *)Type17);
    
    FreePool(Type17);
    
    DEBUG((DEBUG_INFO, "SMBIOS Type 17 installed: %r\n", Status));
    return Status;
}

/*****************************************************************************
 * Type 19 - Memory Array Mapped Address
 *****************************************************************************/
STATIC
EFI_STATUS
InstallSmbiosType19(
    IN EFI_SMBIOS_PROTOCOL *Smbios,
    IN UINT64              MemoryBase,
    IN UINT64              MemorySize
)
{
    EFI_STATUS           Status;
    EFI_SMBIOS_HANDLE    Handle;
    SMBIOS_TABLE_TYPE19  *Type19;
    UINTN                TableSize;
    
    TableSize = sizeof(SMBIOS_TABLE_TYPE19) + 2;
    
    Type19 = AllocateZeroPool(TableSize);
    if (Type19 == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }
    
    Type19->Hdr.Type                = EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS;
    Type19->Hdr.Length              = sizeof(SMBIOS_TABLE_TYPE19);
    Type19->Hdr.Handle              = GetNextSmbiosHandle();
    Type19->StartingAddress         = 0xFFFFFFFF;
    Type19->EndingAddress           = 0xFFFFFFFF;
    Type19->MemoryArrayHandle       = mMemArrayHandle;
    Type19->PartitionWidth          = 1;
    Type19->ExtendedStartingAddress = MemoryBase;
    Type19->ExtendedEndingAddress   = MemoryBase + MemorySize - 1;
    
    *((CHAR8 *)Type19 + sizeof(SMBIOS_TABLE_TYPE19)) = 0;
    *((CHAR8 *)Type19 + sizeof(SMBIOS_TABLE_TYPE19) + 1) = 0;
    
    Handle = SMBIOS_HANDLE_PI_RESERVED;
    Status = Smbios->Add(Smbios, NULL, &Handle, (EFI_SMBIOS_TABLE_HEADER *)Type19);
    
    FreePool(Type19);
    
    DEBUG((DEBUG_INFO, "SMBIOS Type 19 installed: %r\n", Status));
    return Status;
}

/*****************************************************************************
 * Type 32 - System Boot Information
 *****************************************************************************/
STATIC
EFI_STATUS
InstallSmbiosType32(
    IN EFI_SMBIOS_PROTOCOL *Smbios
)
{
    EFI_STATUS           Status;
    EFI_SMBIOS_HANDLE    Handle;
    SMBIOS_TABLE_TYPE32  *Type32;
    UINTN                TableSize;
    
    TableSize = sizeof(SMBIOS_TABLE_TYPE32) + 2;
    
    Type32 = AllocateZeroPool(TableSize);
    if (Type32 == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }
    
    Type32->Hdr.Type    = EFI_SMBIOS_TYPE_SYSTEM_BOOT_INFORMATION;
    Type32->Hdr.Length  = sizeof(SMBIOS_TABLE_TYPE32);
    Type32->Hdr.Handle  = GetNextSmbiosHandle();
    ZeroMem(Type32->Reserved, 6);
    Type32->BootStatus  = BootInformationStatusNoError;
    
    *((CHAR8 *)Type32 + sizeof(SMBIOS_TABLE_TYPE32)) = 0;
    *((CHAR8 *)Type32 + sizeof(SMBIOS_TABLE_TYPE32) + 1) = 0;
    
    Handle = SMBIOS_HANDLE_PI_RESERVED;
    Status = Smbios->Add(Smbios, NULL, &Handle, (EFI_SMBIOS_TABLE_HEADER *)Type32);
    
    FreePool(Type32);
    
    DEBUG((DEBUG_INFO, "SMBIOS Type 32 installed: %r\n", Status));
    return Status;
}

/*****************************************************************************
 * Driver Entry Point
 *****************************************************************************/
EFI_STATUS
EFIAPI
SmBiosTableDxeInitialize(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
{
    EFI_STATUS          Status;
    EFI_SMBIOS_PROTOCOL *Smbios;
    
    /* Device information - customize per device */
    CHAR8 *DeviceVendor     = "Nubia";
    CHAR8 *DeviceName       = "Z70";
    CHAR8 *DeviceCodename   = "z70";
    UINT64 MemoryBase       = 0x80000000;
    UINT64 MemorySize       = 0x200000000;  /* 8GB */
    
    DEBUG((DEBUG_INFO, "\n"));
    DEBUG((DEBUG_INFO, "============================================\n"));
    DEBUG((DEBUG_INFO, "SM8750 SMBIOS Table DXE Driver\n"));
    DEBUG((DEBUG_INFO, "============================================\n"));
    
    Status = gBS->LocateProtocol(&gEfiSmbiosProtocolGuid, NULL, (VOID **)&Smbios);
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "Failed to locate SMBIOS Protocol: %r\n", Status));
        return Status;
    }
    
    /* Install SMBIOS tables in order */
    InstallSmbiosType0(Smbios);
    InstallSmbiosType1(Smbios, DeviceVendor, DeviceName, DeviceCodename);
    InstallSmbiosType2(Smbios, DeviceVendor, DeviceName, DeviceCodename);
    InstallSmbiosType3(Smbios, DeviceVendor);
    InstallSmbiosType7(Smbios);   /* Cache before processor */
    InstallSmbiosType4(Smbios);
    InstallSmbiosType16(Smbios, MemorySize);  /* Memory array before device */
    InstallSmbiosType17(Smbios, MemorySize);
    InstallSmbiosType19(Smbios, MemoryBase, MemorySize);
    InstallSmbiosType32(Smbios);
    
    DEBUG((DEBUG_INFO, "============================================\n"));
    DEBUG((DEBUG_INFO, "SMBIOS Tables Installation Complete\n"));
    DEBUG((DEBUG_INFO, "============================================\n\n"));
    
    return EFI_SUCCESS;
}
