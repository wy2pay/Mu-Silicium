# SM8750 (Snapdragon 8 Elite) Analysis for Project Silicium
## Nubia Z70 UEFI Firmware Analysis

### Platform Overview

| Feature | SM8750 (Pakala) | SM8635 (Cliffs) |
|---------|-----------------|-----------------|
| Codename | Pakala | Cliffs |
| CPU | Oryon (Custom ARM) | Kryo (Cortex-A based) |
| Process | 3nm TSMC | 4nm Samsung |
| GPU | Adreno 830 | Adreno 710 |
| UEFI Entry | `0xA7000000` | `0x9FC00000` (typical) |
| Memory Start | `0x80000000` | `0x80000000` |

---

## Memory Map (SM8750 - Nubia Z70)

### Critical UEFI Regions

| Region | Base Address | Size | Description |
|--------|--------------|------|-------------|
| **UEFI_FD** | `0xA7000000` | 4 MB | UEFI Firmware Volume |
| **UEFI_FD_Reserved** | `0xA7400000` | 2 MB | Reserved FD space |
| **CPU_Vectors** | `0xA7600000` | 4 KB | Exception vectors |
| **Info_Blk** | `0xA7601000` | 4 KB | Info block |
| **MMU_PageTables** | `0xA7602000` | 12 KB | MMU tables |
| **Log_Buffer** | `0xA7605000` | 32 KB | Log buffer |
| **UEFI_Stack** | `0xA760D000` | 256 KB | SEC stack |
| **SEC_Heap** | `0xA764D000` | 560 KB | SEC heap |
| **Sched_Heap** | `0xA76D9000` | 4 MB | Scheduler heap |
| **FV_Region** | `0xA7AD9000` | 4 MB | FV region |
| **UEFI_RESV** | `0xA7ED9000` | ~1.2 MB | UEFI reserved |

### Other Important Regions

| Region | Base Address | Size | Description |
|--------|--------------|------|-------------|
| NOMAP | `0x80000000` | 25.4 MB | XBL reserved |
| SMEM | `0x81D00000` | 2 MB | Shared memory |
| PIL_Reserved | `0x89E00000` | 407 MB | Subsystem images |
| Display_Demura | `0xA3500000` | 44.5 MB | Display calibration |
| Kernel | `0xA8000000` | 256 MB | Linux kernel |
| DXE_Heap | `0xB90C0000` | 469 MB | DXE heap |
| Display_Reserved | `0xFC800000` | 43 MB | Framebuffer |

### Hardware Register Regions

| Region | Base Address | Size |
|--------|--------------|------|
| IPC_ROUTER | `0x00400000` | 1 MB |
| SECURITY_CONTROL | `0x00780000` | 28 KB |
| QUP | `0x00800000` | 3 MB |
| PRNG | `0x010C0000` | 48 KB |
| CRYPTO | `0x01DC0000` | 256 KB |
| TCSR | `0x01F00000` | 1 MB |
| PERIPH_SS | `0x08800000` | 1 MB |
| USB | `0x0A600000` | 2 MB |
| AOSS | `0x0B000000` | 64 MB |
| SPMI_BUS0 | `0x0C400000` | 5 MB |
| SPMI_BUS1 | `0x0C431000` | 84 KB |
| TLMM | `0x0F000000` | 16 MB |
| IMEM | `0x14680000` | 168 KB |
| SMMU | `0x15000000` | 2 MB |
| APSS_HM | `0x16000000` | 96 MB |

---

## DXE Drivers Extracted from Stock UEFI

### Core Drivers
- `DxeCore` - DXE Foundation
- `ArmCpuDxe` - ARM CPU services
- `ArmGicDxe` - GIC interrupt controller
- `ArmTimerDxe` - ARM timer

### Qualcomm Platform Drivers
- `ChipInfo` - SoC identification
- `PlatformInfoDxeDriver` - Platform info
- `DALSys` - DAL system services
- `HALIOMMU` - IOMMU abstraction
- `HWIODxeDriver` - Hardware I/O
- `ClockDxe` - Clock controller
- `DALTLMM` - TLMM GPIO

### Power Management
- `PmicDxe` - PMIC driver (PM8750BH, PM8550, PMK8550)
- `PmicGlinkDxe` - PMIC Glink
- `CPRDxe` - Core Power Reduction
- `RpmhDxe` - RPMh driver
- `NpaDxe` - Node Power Architecture
- `CmdDbDxe` - Command DB

### Display
- `DisplayDxe` - Display controller
- Panel configs for Nubia Z70:
  - `Panel_zte_bf369_rm692k0_6p534_dsc_cmd.xml` (primary)
  - `Panel_zte_bf368_nt37900b_8p03_dsc_cmd.xml`

### Storage
- `UFSDxe` - UFS driver
- `SdccDxe` - SD/eMMC
- `PartitionDxe` - GPT partition
- `DiskIoDxe` - Disk I/O
- `FvDxe` - Firmware Volume
- `Fat` - FAT filesystem

### USB
- `UsbConfigDxe` - USB configuration
- `UsbDeviceDxe` - USB device mode
- `UsbfnDwc3Dxe` - DWC3 controller
- `XhciDxe` - xHCI host
- `UsbBusDxe` - USB bus

### Security
- `TzDxeLA` - TrustZone driver
- `ScmDxeCompat` - SCM compatibility
- `ShmBridgeDxeLA` - Shared memory bridge
- `VerifiedBootDxe` - Verified boot
- `SecRSADxe` - RSA operations
- `RngDxe` - Random number generator

### Communication
- `SmemDxe` - Shared memory
- `GlinkDxe` - Glink IPC
- `MailboxDxe` - Mailbox
- `IPCCDxe` - IPCC controller
- `QcomScmiDxe` - SCMI interface

### Nubia-Specific
- `NubiaBoardDxe` - Board-specific init
- `FeatureEnablerDxe` - Feature enabler

---

## SM8750Pkg Configuration for MU-Qcom

### Recommended PCD Values

```ini
# Memory Configuration
gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000
gArmTokenSpaceGuid.PcdSystemMemorySize|0x200000000

# UEFI FD Configuration  
gArmTokenSpaceGuid.PcdFdBaseAddress|0xA7000000
gArmTokenSpaceGuid.PcdFdSize|0x00400000

# Stack Configuration
gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0xA760D000
gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0x40000

# Timer Configuration (GIC v3)
gArmTokenSpaceGuid.PcdArmArchTimerSecIntrNum|29
gArmTokenSpaceGuid.PcdArmArchTimerIntrNum|30
gArmTokenSpaceGuid.PcdArmArchTimerVirtIntrNum|27
gArmTokenSpaceGuid.PcdArmArchTimerHypIntrNum|26

# GIC Configuration
gArmTokenSpaceGuid.PcdGicDistributorBase|0x17A00000
gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x17A60000

# SMEM Configuration
gQcomPkgTokenSpaceGuid.PcdSmemBase|0x81D00000
gQcomPkgTokenSpaceGuid.PcdSmemSize|0x200000
```

### Key Differences from SM8635

1. **Entry Point**: `0xA7000000` vs `0x9FC00000`
2. **DXE Heap**: Larger at 469 MB
3. **CPU**: Oryon custom cores (different cache handling)
4. **Display**: Demura calibration region at `0xA3500000`
5. **PMIC**: PM8750BH (new for SM8750) + PM8550 + PMK8550

---

## Recommended Porting Steps

### Phase 1: Basic Boot
1. Set correct `PcdFdBaseAddress` = `0xA7000000`
2. Configure GIC at `0x17A00000`
3. Extract and use stock `ClockDxe`, `PlatformInfoDxeDriver`
4. Verify SMEM at `0x81D00000`

### Phase 2: Display
1. Extract `DisplayDxe` from stock
2. Use panel XML: `Panel_zte_bf369_rm692k0_6p534_dsc_cmd.xml`
3. Configure framebuffer at `0xFC800000`

### Phase 3: Storage
1. Port `UFSDxe` from stock
2. Configure UFS controller registers
3. Add partition support

### Phase 4: USB
1. Extract `UsbfnDwc3Dxe` drivers
2. Configure USB controller at `0x0A600000`

---

## PMIC Configuration (PMK8550 + PM8550 + PM8750BH)

```
SPMI Bus 0 (0x0C400000):
  - PMK8550 @ SID 0x0
  - PM8550  @ SID 0x1
  - PM8550VE @ SID 0x3
  - Luminous @ SID 0x4
  - PM8550VS @ SID 0x5
  - PM8550VE @ SID 0x6
  - PM8750BH @ SID 0x7 (new for SM8750)
  - PM8550VE @ SID 0x8
  - PM8550VS @ SID 0x9
  - PMR735D @ SID 0xA
  - PM8010 @ SID 0xC
  - PM8010 @ SID 0xD
```

---

## Files Extracted

- `drivers_list.txt` - Complete list of DXE drivers
- `pre-ddr.dtb` - Pre-DDR device tree
- `post-ddr.dts` - Post-DDR device tree (decompiled)
- `uefi_payload.bin` - Raw UEFI FV payload

---

## Notes

1. The SM8750 uses "Pakala" codename internally
2. Display uses Demura calibration (44.5 MB region)
3. Oryon cores may require specific cache handling in ArmCpuDxe
4. Stock UEFI includes `NubiaBoardDxe` for board-specific init
5. DDR boot frequency: 2093824 (0x1FEF00) MHz

---

*Generated from Nubia Z70 (SM8750) firmware analysis*
*For Project Silicium UEFI porting*
