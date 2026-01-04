/** @file
 * Differentiated System Description Table (DSDT) for SM8750
 *
 * Copyright (c) 2024, Project Silicium. All rights reserved.
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 */

DefinitionBlock ("DSDT.aml", "DSDT", 2, "QCOM  ", "SM8750  ", 0x00000001)
{
    Scope (\_SB)
    {
        //
        // Processor Devices
        //
        Device (CPU0) {
            Name (_HID, "ACPI0007")
            Name (_UID, 0x0)
        }

        Device (CPU1) {
            Name (_HID, "ACPI0007")
            Name (_UID, 0x1)
        }

        Device (CPU2) {
            Name (_HID, "ACPI0007")
            Name (_UID, 0x2)
        }

        Device (CPU3) {
            Name (_HID, "ACPI0007")
            Name (_UID, 0x3)
        }

        Device (CPU4) {
            Name (_HID, "ACPI0007")
            Name (_UID, 0x4)
        }

        Device (CPU5) {
            Name (_HID, "ACPI0007")
            Name (_UID, 0x5)
        }

        Device (CPU6) {
            Name (_HID, "ACPI0007")
            Name (_UID, 0x6)
        }

        Device (CPU7) {
            Name (_HID, "ACPI0007")
            Name (_UID, 0x7)
        }

        //
        // GIC (Generic Interrupt Controller)
        //
        Device (GIC0) {
            Name (_HID, "ARMHC000")
            Name (_UID, 0x0)
            Name (_CRS, ResourceTemplate () {
                Memory32Fixed (ReadWrite, 0x17A00000, 0x10000)      // Distributor
                Memory32Fixed (ReadWrite, 0x17A60000, 0x100000)     // Redistributor
            })
        }

        //
        // Generic Timer
        //
        Device (TMR0) {
            Name (_HID, "ARMHC001")
            Name (_UID, 0x0)
        }

        //
        // SMEM (Shared Memory)
        //
        Device (SMEM) {
            Name (_HID, "QCOM0600")
            Name (_UID, 0x0)
            Name (_CRS, ResourceTemplate () {
                Memory32Fixed (ReadWrite, 0x81D00000, 0x200000)
            })
        }

        //
        // IMEM (Internal Memory)
        //
        Device (IMEM) {
            Name (_HID, "QCOM0601")
            Name (_UID, 0x0)
            Name (_CRS, ResourceTemplate () {
                Memory32Fixed (ReadWrite, 0x14680000, 0x2C000)
            })
        }

        //
        // SPMI (System Power Management Interface)
        //
        Device (SPMI) {
            Name (_HID, "QCOM0602")
            Name (_UID, 0x0)
            Name (_CRS, ResourceTemplate () {
                Memory32Fixed (ReadWrite, 0x0C400000, 0x500000)
            })
        }

        //
        // USB Controller
        //
        Device (USB0) {
            Name (_HID, "QCOM0603")
            Name (_UID, 0x0)
            Name (_CRS, ResourceTemplate () {
                Memory32Fixed (ReadWrite, 0x0A600000, 0x200000)
            })
        }

        //
        // GPIO (TLMM)
        //
        Device (GPIO) {
            Name (_HID, "QCOM0604")
            Name (_UID, 0x0)
            Name (_CRS, ResourceTemplate () {
                Memory32Fixed (ReadWrite, 0x0F000000, 0x1000000)
            })
        }
    }
}
