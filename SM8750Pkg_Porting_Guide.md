# SM8750Pkg (SunPkg/PakalaPkg) Porting Guide
## Mu-Silicium / Project Silicium

---

## 📁 Directory Structure to Create

```
Silicon/Qualcomm/SM8750Pkg/
├── SM8750Pkg.dsc.inc          # DSC include file
├── SM8750Pkg.dec              # Package declaration
├── AcpiTables/
│   ├── AcpiTables.inf
│   ├── DSDT.asl               # Main ACPI table
│   ├── FACP.aslc
│   ├── GTDT.aslc
│   ├── MADT.aslc
│   ├── DBG2.aslc
│   └── PPTT.aslc
├── Drivers/
│   └── SmBiosTableDxe/
│       ├── SmBiosTableDxe.c
│       └── SmBiosTableDxe.inf
├── Include/
│   ├── Configuration/
│   │   └── DeviceMemoryMap.h
│   ├── ACPI.h
│   └── Protocol/
├── Library/
│   ├── PlatformMemoryMapLib/
│   │   ├── PlatformMemoryMapLib.c
│   │   └── PlatformMemoryMapLib.inf
│   ├── PlatformPeiLib/
│   │   ├── PlatformPeiLib.c
│   │   └── PlatformPeiLib.inf
│   └── AcpiPlatformUpdateLib/
│       ├── AcpiPlatformUpdateLib.c
│       └── AcpiPlatformUpdateLib.inf
└── GPLLibrary/                # GPL-licensed code
```

---

## 🔧 Step 1: Create SM8750Pkg.dec

```ini
# Silicon/Qualcomm/SM8750Pkg/SM8750Pkg.dec

[Defines]
  DEC_SPECIFICATION           = 0x00010005
  PACKAGE_NAME                = SM8750Pkg
  PACKAGE_GUID                = XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX  # Generate new GUID
  PACKAGE_VERSION             = 0.1

[Includes]
  Include

[Guids]
  gSM8750PkgTokenSpaceGuid = { 0xXXXXXXXX, 0xXXXX, 0xXXXX, { 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX } }

[Protocols]

[PcdsFixedAtBuild]
  # SoC Identification
  gSM8750PkgTokenSpaceGuid.PcdSocId|0x00000209|UINT32|0x0000001
```

---

## 🔧 Step 2: Create SM8750Pkg.dsc.inc

```ini
# Silicon/Qualcomm/SM8750Pkg/SM8750Pkg.dsc.inc

[Defines]
  USE_PHYSICAL_TIMER = 1

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -DSOC_SM8750 -DENABLE_SIMPLE_INIT

[PcdsFixedAtBuild]
  #
  # System Memory Configuration (from analysis)
  #
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x200000000        # 8GB typical

  #
  # UEFI FD Configuration
  #
  gArmTokenSpaceGuid.PcdFdBaseAddress|0xA7000000
  gArmTokenSpaceGuid.PcdFdSize|0x00400000                   # 4MB

  #
  # CPU Stack Configuration
  #
  gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0xA760D000
  gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0x40000

  #
  # Timer Configuration
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
  # Qualcomm Platform Configuration
  #
  gQcomPkgTokenSpaceGuid.PcdSmemBaseAddress|0x81D00000
  gQcomPkgTokenSpaceGuid.PcdSmemSize|0x200000
  
  gQcomPkgTokenSpaceGuid.PcdUefiMemPoolBase|0xA7000000
  gQcomPkgTokenSpaceGuid.PcdUefiMemPoolSize|0x01000000

  #
  # IMEM Configuration
  #
  gQcomPkgTokenSpaceGuid.PcdIMEMBase|0x14680000
  gQcomPkgTokenSpaceGuid.PcdIMEMSize|0x2C000

  #
  # Peripheral Configuration
  #
  gQcomPkgTokenSpaceGuid.PcdSpmiBaseAddress|0x0C400000

  #
  # SoC Specific
  #
  gQcomPkgTokenSpaceGuid.PcdSocChipId|0x00000209             # SM8750 ChipID
  gQcomPkgTokenSpaceGuid.PcdSocCodeName|"pakala"

  #
  # Display Configuration
  #
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferAddress|0xFC800000
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|1260        # Adjust per device
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|2800       # Adjust per device

[LibraryClasses]
  PlatformMemoryMapLib|Silicon/Qualcomm/SM8750Pkg/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.inf
  PlatformPeiLib|Silicon/Qualcomm/SM8750Pkg/Library/PlatformPeiLib/PlatformPeiLib.inf
  AcpiPlatformUpdateLib|Silicon/Qualcomm/SM8750Pkg/Library/AcpiPlatformUpdateLib/AcpiPlatformUpdateLib.inf

[Components]
  Silicon/Qualcomm/SM8750Pkg/Drivers/SmBiosTableDxe/SmBiosTableDxe.inf
```

---

## 🔧 Step 3: Create DeviceMemoryMap.h

```c
// Silicon/Qualcomm/SM8750Pkg/Include/Configuration/DeviceMemoryMap.h

#ifndef _DEVICE_MEMORY_MAP_H_
#define _DEVICE_MEMORY_MAP_H_

#include <Library/ArmLib.h>

#define MAX_ARM_MEMORY_REGION_DESCRIPTOR_COUNT 128

// DDR Start Address
#define DDR_START_ADDRESS 0x80000000

// Memory Map Definitions
#define SYSTEM_MEMORY_SIZE   0x200000000  // 8GB

// Critical Memory Regions (from analysis)
#define UEFI_FD_BASE         0xA7000000
#define UEFI_FD_SIZE         0x00400000

#define SMEM_BASE            0x81D00000
#define SMEM_SIZE            0x00200000

#define IMEM_BASE            0x14680000
#define IMEM_SIZE            0x0002C000

#define DXE_HEAP_BASE        0xB90C0000
#define DXE_HEAP_SIZE        0x1D500000

// Hardware Register Regions
#define PERIPH_SS_BASE       0x08800000
#define PERIPH_SS_SIZE       0x00100000

#define USB_BASE             0x0A600000
#define USB_SIZE             0x00200000

#define AOSS_BASE            0x0B000000
#define AOSS_SIZE            0x04000000

#define SPMI_BUS_BASE        0x0C400000
#define SPMI_BUS_SIZE        0x00500000

#define TLMM_BASE            0x0F000000
#define TLMM_SIZE            0x01000000

#define GIC_BASE             0x17A00000
#define GIC_SIZE             0x00100000

#define APSS_HM_BASE         0x16000000
#define APSS_HM_SIZE         0x06000000

// Display
#define FRAMEBUFFER_BASE     0xFC800000
#define FRAMEBUFFER_SIZE     0x02B00000

// Memory Type Definitions
static ARM_MEMORY_REGION_DESCRIPTOR_EX gDeviceMemoryDescriptor[] = {
  // Name, Address, Length, HobOption, ResourceAttribute, ArmAttributes, ResourceType, MemoryType

  // DDR Reserved Regions
  {"NOMAP",              0x80000000, 0x01960000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"SMEM",               0x81D00000, 0x00200000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},
  {"PIL_Reserved",       0x89E00000, 0x19700000, AddMem, MEM_RES, UNCACHEABLE, Reserv, UNCACHED_UNBUFFERED_XN},

  // UEFI Regions
  {"UEFI_FD",            0xA7000000, 0x00400000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},
  {"UEFI_FD_Reserved",   0xA7400000, 0x00200000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},
  {"CPU_Vectors",        0xA7600000, 0x00001000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK},
  {"Info_Blk",           0xA7601000, 0x00001000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},
  {"MMU_PageTables",     0xA7602000, 0x00003000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},
  {"Log_Buffer",         0xA7605000, 0x00008000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},
  {"UEFI_Stack",         0xA760D000, 0x00040000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
  {"SEC_Heap",           0xA764D000, 0x0008C000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
  {"Sched_Heap",         0xA76D9000, 0x00400000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},
  {"FV_Region",          0xA7AD9000, 0x00400000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN},

  // Kernel Region
  {"Kernel",             0xA8000000, 0x10000000, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN},

  // DXE Heap
  {"DXE_Heap",           0xB90C0000, 0x1D500000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK_XN},

  // Display
  {"Display_Reserved",   0xFC800000, 0x02B00000, AddMem, MEM_RES, WRITE_COMBINEABLE, Reserv, WRITE_COMBINEABLE},

  // Hardware Registers (MMIO)
  {"QUP",                0x00800000, 0x00300000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"PRNG",               0x010C0000, 0x0000C000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"CRYPTO",             0x01DC0000, 0x00040000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"TCSR",               0x01F00000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"PERIPH_SS",          0x08800000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"USB",                0x0A600000, 0x00200000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"AOSS",               0x0B000000, 0x04000000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"SPMI_BUS",           0x0C400000, 0x00500000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"TLMM",               0x0F000000, 0x01000000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"IMEM",               0x14680000, 0x0002C000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"SMMU",               0x15000000, 0x00200000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"APSS_HM",            0x16000000, 0x06000000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"GIC",                0x17A00000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},

  // Terminator
  {"Terminator", 0, 0, 0, 0, 0, 0, 0}
};

#endif /* _DEVICE_MEMORY_MAP_H_ */
```

---

## 🔧 Step 4: Create PlatformMemoryMapLib

```c
// Silicon/Qualcomm/SM8750Pkg/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.c

#include <Library/BaseLib.h>
#include <Library/PlatformMemoryMapLib.h>
#include <Configuration/DeviceMemoryMap.h>

ARM_MEMORY_REGION_DESCRIPTOR_EX*
GetPlatformMemoryMap(
  VOID
)
{
  return gDeviceMemoryDescriptor;
}
```

```ini
# Silicon/Qualcomm/SM8750Pkg/Library/PlatformMemoryMapLib/PlatformMemoryMapLib.inf

[Defines]
  INF_VERSION                    = 0x00010005
  BASE_NAME                      = PlatformMemoryMapLib
  FILE_GUID                      = XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
  MODULE_TYPE                    = BASE
  VERSION_STRING                 = 1.0
  LIBRARY_CLASS                  = PlatformMemoryMapLib

[Sources]
  PlatformMemoryMapLib.c

[Packages]
  ArmPkg/ArmPkg.dec
  ArmPlatformPkg/ArmPlatformPkg.dec
  MdePkg/MdePkg.dec
  MdeModulePkg/MdeModulePkg.dec
  Silicon/Qualcomm/QcomPkg/QcomPkg.dec
  Silicon/Qualcomm/SM8750Pkg/SM8750Pkg.dec

[LibraryClasses]
  BaseLib
```

---

## 🔧 Step 5: Create Device Platform (Nubia Z70)

```
Platforms/Nubia/z70Pkg/
├── z70.dsc
├── z70.fdf
├── Include/
│   ├── Configuration/
│   │   └── DeviceConfigurationMap.h
│   └── ACPI.h
├── Library/
│   └── DeviceConfigurationMapLib/
│       ├── DeviceConfigurationMapLib.c
│       └── DeviceConfigurationMapLib.inf
├── PlatformBuild.py
└── z70.cfg
```

### z70.dsc

```ini
# Platforms/Nubia/z70Pkg/z70.dsc

[Defines]
  PLATFORM_NAME                  = z70
  PLATFORM_GUID                  = XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/z70Pkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = Platforms/Nubia/z70Pkg/z70.fdf
  DEVICE_DT_IMAGE                = TRUE

!include Silicon/Qualcomm/SM8750Pkg/SM8750Pkg.dsc.inc

[PcdsFixedAtBuild]
  # Device Specific
  gQcomPkgTokenSpaceGuid.PcdDeviceVendor|"Nubia"
  gQcomPkgTokenSpaceGuid.PcdDeviceName|"Z70"
  gQcomPkgTokenSpaceGuid.PcdDeviceCodeName|"z70"

  # Display Configuration (adjust for actual panel)
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferWidth|1260
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferHeight|2800
  gQcomPkgTokenSpaceGuid.PcdMipiFrameBufferPixelBpp|32

[LibraryClasses]
  DeviceConfigurationMapLib|Platforms/Nubia/z70Pkg/Library/DeviceConfigurationMapLib/DeviceConfigurationMapLib.inf
```

### z70.cfg (Build Configuration)

```ini
# Platforms/Nubia/z70Pkg/z70.cfg

[Defines]
  TARGET_DEVICE_VENDOR           = Nubia
  TARGET_SOC                     = SM8750
  TARGET_ARCH                    = AARCH64
  TARGET_FD_BASE                 = 0xA7000000
  TARGET_FD_SIZE                 = 0x00400000
  TARGET_CREATE_POINTER          = TRUE
  TARGET_POINTER_ADDRESS         = 0x81A00000
```

---

## 🔧 Step 6: Binary Drivers

### Location
```
Binaries/Nubia/z70/
├── ClockDxe.inf
├── ClockDxe.efi
├── PlatformInfoDxeDriver.inf
├── PlatformInfoDxeDriver.efi
├── ChipInfo.inf
├── ChipInfo.efi
├── DALSys.inf
├── DALSys.efi
├── DisplayDxe.inf
├── DisplayDxe.efi
├── TzDxeLA.inf
├── TzDxeLA.efi
├── UFSDxe.inf
├── UFSDxe.efi
└── ... (other extracted drivers)
```

### Driver INF Template

```ini
# Binaries/Nubia/z70/ClockDxe.inf

[Defines]
  INF_VERSION                    = 0x00010005
  BASE_NAME                      = ClockDxe
  FILE_GUID                      = XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
  MODULE_TYPE                    = DXE_DRIVER
  VERSION_STRING                 = 1.0

[Binaries.AARCH64]
  PE32|ClockDxe.efi|*

[Depex]
  TRUE
```

---

## 📋 Step 7: Extract Binary Drivers

### จาก Stock UEFI

```bash
# ใช้ uefi-firmware-parser extract
cd fw_analysis
uefi-firmware-parser --extract uefi_payload.bin

# จะได้โฟลเดอร์ที่มี .efi files
# Copy drivers ที่ต้องการไปยัง Binaries/
```

### Drivers สำคัญที่ต้อง Extract

| Priority | Driver | GUID | Description |
|----------|--------|------|-------------|
| 1 | ClockDxe | - | Clock controller |
| 2 | PlatformInfoDxeDriver | - | Platform info |
| 3 | DALSys | - | DAL system |
| 4 | HALIOMMU | - | IOMMU |
| 5 | ChipInfo | - | SoC info |
| 6 | TzDxeLA | - | TrustZone |
| 7 | DisplayDxe | - | Display |
| 8 | UFSDxe | - | Storage |
| 9 | PmicDxe | - | PMIC |
| 10 | SmemDxe | - | Shared memory |

---

## 🔧 Step 8: Build & Test

### Build Command

```bash
# Clone repo
git clone https://github.com/Project-Silicium/Mu-Silicium.git --recursive
cd Mu-Silicium

# Setup environment
./setup_env.sh -p apt

# Build
./build_uefi.sh -d z70 -r DEBUG
```

### Flash

```bash
# Output will be: Mu-z70.img
fastboot flash boot Mu-z70.img
# OR
fastboot boot Mu-z70.img
```

---

## 🐛 Debugging Tips

### Common Issues

| Issue | Solution |
|-------|----------|
| DXE Phase Crash | Check memory map overlaps |
| No display | Verify framebuffer address |
| USB not working | Check USB region in memory map |
| Storage fail | Extract UFSDxe from stock |

### Debug Serial

```ini
# Enable in DSC
gEfiMdeModulePkgTokenSpaceGuid.PcdSerialUseHardwareFlowControl|FALSE
gEfiMdeModulePkgTokenSpaceGuid.PcdSerialBaudRate|115200
```

---

## 📊 SM8750 vs SM8650 Key Differences

| Parameter | SM8750 (Pakala) | SM8650 (Pineapple) |
|-----------|-----------------|---------------------|
| UEFI Entry | 0xA7000000 | 0x9FC00000 |
| GIC Base | 0x17A00000 | 0x17100000 |
| DXE Heap | 469 MB | ~200 MB |
| CPU | Oryon | Kryo |
| PMIC | PM8750BH | PM8550 |

---

## ✅ Checklist

- [ ] Create SM8750Pkg directory structure
- [ ] Create SM8750Pkg.dec with GUIDs
- [ ] Create SM8750Pkg.dsc.inc with PCDs
- [ ] Create DeviceMemoryMap.h
- [ ] Create PlatformMemoryMapLib
- [ ] Create device platform (z70Pkg)
- [ ] Extract binary drivers from stock UEFI
- [ ] Create INF files for binaries
- [ ] Create z70.cfg build config
- [ ] Test build
- [ ] Flash and debug

---

*Generated for Project Silicium SM8750/Pakala porting*
