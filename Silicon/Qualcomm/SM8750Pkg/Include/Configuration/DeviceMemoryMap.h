/** @file
 * Device Memory Map for SM8750 (Snapdragon 8 Elite)
 *
 * Extracted from Nubia Z70 UEFI firmware
 *
 * Copyright (c) 2024, Project Silicium. All rights reserved.
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

#ifndef _DEVICE_MEMORY_MAP_H_
#define _DEVICE_MEMORY_MAP_H_

#include <Library/ArmLib.h>

#define MAX_ARM_MEMORY_REGION_DESCRIPTOR_COUNT 128

/* DDR Start Address */
#define DDR_START_ADDRESS     0x80000000

/* Critical Memory Regions */
#define UEFI_FD_BASE          0xA7000000
#define UEFI_FD_SIZE          0x00400000

#define SMEM_BASE             0x81D00000
#define SMEM_SIZE             0x00200000

#define IMEM_BASE             0x14680000
#define IMEM_SIZE             0x0002C000

#define DXE_HEAP_BASE         0xB90C0000
#define DXE_HEAP_SIZE         0x1D500000

#define FRAMEBUFFER_BASE      0xFC800000
#define FRAMEBUFFER_SIZE      0x02B00000

static ARM_MEMORY_REGION_DESCRIPTOR_EX gDeviceMemoryDescriptorEx[] = {
    /* Name                   Address         Length        HobOption  ResourceAttribute   ArmAttributes            ResourceType  MemoryType */

    /* DDR Reserved Regions */
    {"NOMAP",                0x80000000,     0x01960000,    AddMem,    MEM_RES,    UNCACHEABLE,              Reserv,   UNCACHED_UNBUFFERED_XN},
    {"RSRV0",                0x81960000,     0x000A0000,    AddMem,    MEM_RES,    UNCACHEABLE,              Reserv,   UNCACHED_UNBUFFERED_XN},
    {"XBL_DT",               0x81A00000,     0x00040000,    AddMem,    MEM_RES,    UNCACHEABLE,              Reserv,   UNCACHED_UNBUFFERED_XN},
    {"AOP",                  0x81C00000,     0x000A0000,    AddMem,    MEM_RES,    UNCACHEABLE,              Reserv,   UNCACHED_UNBUFFERED_XN},
    {"UEFI_Log",             0x81CE4000,     0x00010000,    AddMem,    SYS_MEM,    SYS_MEM_CAP,              Reserv,   WRITE_BACK_XN},
    {"SMEM",                 0x81D00000,     0x00200000,    AddMem,    MEM_RES,    UNCACHEABLE,              Reserv,   UNCACHED_UNBUFFERED_XN},
    {"PDP_NS",               0x81F00000,     0x00100000,    AddMem,    MEM_RES,    UNCACHEABLE,              Reserv,   UNCACHED_UNBUFFERED_XN},
    {"PvmFw",                0x824A0000,     0x00100000,    AddMem,    MEM_RES,    UNCACHEABLE,              Reserv,   UNCACHED_UNBUFFERED_XN},
    {"PIL_Reserved",         0x89E00000,     0x19700000,    AddMem,    MEM_RES,    UNCACHEABLE,              Reserv,   UNCACHED_UNBUFFERED_XN},
    {"Display_Demura",       0xA3500000,     0x02C80000,    AddMem,    MEM_RES,    WRITE_COMBINEABLE,        Reserv,   WRITE_COMBINEABLE},

    /* UEFI Regions */
    {"UEFI_FD",              0xA7000000,     0x00400000,    AddMem,    SYS_MEM,    SYS_MEM_CAP,              Reserv,   WRITE_BACK_XN},
    {"UEFI_FD_Reserved",     0xA7400000,     0x00200000,    AddMem,    SYS_MEM,    SYS_MEM_CAP,              Reserv,   WRITE_BACK_XN},
    {"CPU_Vectors",          0xA7600000,     0x00001000,    AddMem,    SYS_MEM,    SYS_MEM_CAP,              Reserv,   WRITE_BACK},
    {"Info_Blk",             0xA7601000,     0x00001000,    AddMem,    SYS_MEM,    SYS_MEM_CAP,              Reserv,   WRITE_BACK_XN},
    {"MMU_PageTables",       0xA7602000,     0x00003000,    AddMem,    SYS_MEM,    SYS_MEM_CAP,              Reserv,   WRITE_BACK_XN},
    {"Log_Buffer",           0xA7605000,     0x00008000,    AddMem,    SYS_MEM,    SYS_MEM_CAP,              Reserv,   WRITE_BACK_XN},
    {"UEFI_Stack",           0xA760D000,     0x00040000,    AddMem,    SYS_MEM,    SYS_MEM_CAP,              BsData,   WRITE_BACK_XN},
    {"SEC_Heap",             0xA764D000,     0x0008C000,    AddMem,    SYS_MEM,    SYS_MEM_CAP,              BsData,   WRITE_BACK_XN},
    {"Sched_Heap",           0xA76D9000,     0x00400000,    AddMem,    SYS_MEM,    SYS_MEM_CAP,              BsData,   WRITE_BACK_XN},
    {"FV_Region",            0xA7AD9000,     0x00400000,    AddMem,    SYS_MEM,    SYS_MEM_CAP,              BsData,   WRITE_BACK_XN},
    {"UEFI_RESV",            0xA7ED9000,     0x00127000,    AddMem,    SYS_MEM,    SYS_MEM_CAP,              Reserv,   WRITE_BACK_XN},

    /* Kernel Region */
    {"Kernel",               0xA8000000,     0x10000000,    AddMem,    SYS_MEM,    SYS_MEM_CAP,              Reserv,   WRITE_BACK_XN},

    /* Ramdump Regions */
    {"XBL_Ramdump",          0xB8000000,     0x001C0000,    AddMem,    MEM_RES,    UNCACHEABLE,              Reserv,   UNCACHED_UNBUFFERED_XN},
    {"DBI_Dump",             0xB81C0000,     0x00F00000,    AddMem,    MEM_RES,    UNCACHEABLE,              Reserv,   UNCACHED_UNBUFFERED_XN},

    /* DXE Heap (469 MB) */
    {"DXE_Heap",             0xB90C0000,     0x1D500000,    AddMem,    SYS_MEM,    SYS_MEM_CAP,              Conv,     WRITE_BACK_XN},

    /* RAM Console */
    {"RamConsole",           0xE4A00000,     0x00200000,    AddMem,    SYS_MEM,    SYS_MEM_CAP,              Reserv,   WRITE_BACK_XN},

    /* Display Framebuffer */
    {"Display_Reserved",     0xFC800000,     0x02B00000,    AddMem,    MEM_RES,    WRITE_COMBINEABLE,        Reserv,   WRITE_COMBINEABLE},

    /* Hardware Registers (MMIO) */
    {"IPC_ROUTER",           0x00400000,     0x00100000,    AddDev,    MMAP_IO,    UNCACHEABLE,              MmIO,     NS_DEVICE},
    {"SECURITY_CONTROL",     0x00780000,     0x00007000,    AddDev,    MMAP_IO,    UNCACHEABLE,              MmIO,     NS_DEVICE},
    {"QUP",                  0x00800000,     0x00300000,    AddDev,    MMAP_IO,    UNCACHEABLE,              MmIO,     NS_DEVICE},
    {"PRNG",                 0x010C0000,     0x0000C000,    AddDev,    MMAP_IO,    UNCACHEABLE,              MmIO,     NS_DEVICE},
    {"CRYPTO",               0x01DC0000,     0x00040000,    AddDev,    MMAP_IO,    UNCACHEABLE,              MmIO,     NS_DEVICE},
    {"TCSR",                 0x01F00000,     0x00100000,    AddDev,    MMAP_IO,    UNCACHEABLE,              MmIO,     NS_DEVICE},
    {"PERIPH_SS",            0x08800000,     0x00100000,    AddDev,    MMAP_IO,    UNCACHEABLE,              MmIO,     NS_DEVICE},
    {"USB",                  0x0A600000,     0x00200000,    AddDev,    MMAP_IO,    UNCACHEABLE,              MmIO,     NS_DEVICE},
    {"AOSS",                 0x0B000000,     0x04000000,    AddDev,    MMAP_IO,    UNCACHEABLE,              MmIO,     NS_DEVICE},
    {"SPMI_BUS",             0x0C400000,     0x00500000,    AddDev,    MMAP_IO,    UNCACHEABLE,              MmIO,     NS_DEVICE},
    {"TLMM",                 0x0F000000,     0x01000000,    AddDev,    MMAP_IO,    UNCACHEABLE,              MmIO,     NS_DEVICE},
    {"IMEM",                 0x14680000,     0x0002C000,    AddDev,    MMAP_IO,    UNCACHEABLE,              MmIO,     NS_DEVICE},
    {"SMMU",                 0x15000000,     0x00200000,    AddDev,    MMAP_IO,    UNCACHEABLE,              MmIO,     NS_DEVICE},
    {"APSS_HM",              0x16000000,     0x06000000,    AddDev,    MMAP_IO,    UNCACHEABLE,              MmIO,     NS_DEVICE},
    {"GIC",                  0x17A00000,     0x00100000,    AddDev,    MMAP_IO,    UNCACHEABLE,              MmIO,     NS_DEVICE},

    /* Terminator */
    {"Terminator", 0, 0, 0, 0, 0, 0, 0}
};

#endif /* _DEVICE_MEMORY_MAP_H_ */
