/*
 * Copyright (C) 2015, Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CPU_X86_DRIVERS_LEGACY_PC_PCI_H_
#define CPU_X86_DRIVERS_LEGACY_PC_PCI_H_

#include <stdint.h>

/** PCI configuration register identifier for Command Register */
#define PCI_CONFIG_REG_CMD  0x04
/** PCI configuration register identifier for Base Address Register 0 (BAR0) */
#define PCI_CONFIG_REG_BAR0 0x10

/**
 * PCI configuration address
 *
 * Refer to Intel Quark SoC X1000 Datasheet, Section 5.5 for more details on
 * PCI configuration register access.
 */
typedef union pci_config_addr {
  struct {
    /** Register/offset number.  Least-significant two bits should be zero. */
    uint32_t reg_off     : 8;
    uint32_t func        : 3; /**< Function number */
    uint32_t dev         : 5; /**< Device number */
    uint32_t bus         : 8; /**< Bus number */
    uint32_t             : 7;
    /** Must be set to perform PCI configuration access. */
    uint32_t en_mapping  : 1;
  };
  uint32_t raw;
} pci_config_addr_t;

uint32_t pci_config_read(pci_config_addr_t addr);
void pci_config_write(pci_config_addr_t addr, uint32_t data);

/**
 * PCI device driver instance with an optional single MMIO range and optional
 * metadata.
 */
typedef struct pci_driver {
  uintptr_t mmio; /**< MMIO range base address */
  uintptr_t meta; /**< Driver-defined metadata base address */
} pci_driver_t;

void pci_init_bar0(pci_driver_t *c_this,
                   pci_config_addr_t pci_addr,
                   uintptr_t meta);

#endif /* CPU_X86_DRIVERS_LEGACY_PC_PCI_H_ */