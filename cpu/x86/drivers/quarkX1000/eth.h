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

#ifndef CPU_X86_DRIVERS_QUARKX1000_ETH_H_
#define CPU_X86_DRIVERS_QUARKX1000_ETH_H_

#include <stdint.h>
#include "net/ip/uip.h"
#include "pci.h"

typedef pci_driver_t quarkX1000_eth_driver_t;

/* Refer to Intel Quark SoC X1000 Datasheet, Chapter 15 for more details on
 * Ethernet device operation.
 *
 * This driver puts the Ethernet device into a very simple and space-efficient
 * mode of operation.  It only allocates a single packet descriptor for each of
 * the transmit and receive directions, computes checksums on the CPU, and
 * enables store-and-forward mode for both transmit and receive directions.
 */

/** Transmit descriptor */
typedef struct quarkX1000_eth_tx_desc {
  /** First word of transmit descriptor */
  union {
    struct {
      /** Only valid in half-duplex mode. */
      uint32_t deferred_bit      : 1;
      uint32_t err_underflow     : 1;
      uint32_t err_excess_defer  : 1;
      uint32_t coll_cnt_slot_num : 4;
      uint32_t vlan_frm          : 1;
      uint32_t err_excess_coll   : 1;
      uint32_t err_late_coll     : 1;
      uint32_t err_no_carrier    : 1;
      uint32_t err_carrier_loss  : 1;
      uint32_t err_ip_payload    : 1;
      uint32_t err_frm_flushed   : 1;
      uint32_t err_jabber_tout   : 1;
      /** OR of all other error bits. */
      uint32_t err_summary       : 1;
      uint32_t err_ip_hdr        : 1;
      uint32_t tx_timestamp_stat : 1;
      uint32_t vlan_ins_ctrl     : 2;
      uint32_t addr2_chained     : 1;
      uint32_t tx_end_of_ring    : 1;
      uint32_t chksum_ins_ctrl   : 2;
      uint32_t replace_crc       : 1;
      uint32_t tx_timestamp_en   : 1;
      uint32_t dis_pad           : 1;
      uint32_t dis_crc           : 1;
      uint32_t first_seg_in_frm  : 1;
      uint32_t last_seg_in_frm   : 1;
      uint32_t intr_on_complete  : 1;
      /** When set, descriptor is owned by DMA. */
      uint32_t own               : 1;
    };
    uint32_t tdes0;
  };
  /** Second word of transmit descriptor */
  union {
    struct {
      uint32_t tx_buf1_sz        : 13;
      uint32_t                   : 3;
      uint32_t tx_buf2_sz        : 13;
      uint32_t src_addr_ins_ctrl : 3;
    };
    uint32_t tdes1;
  };
  /** Pointer to frame data buffer */
  uint8_t *buf1_ptr;
  /**
   * Unused, since this driver initializes only a single descriptor for each
   * direction.
   */
  uint8_t *buf2_ptr;
} quarkX1000_eth_tx_desc_t;

/** Transmit descriptor */
typedef struct quarkX1000_eth_rx_desc {
  /** First word of receive descriptor */
  union {
    struct {
      uint32_t ext_stat          : 1;
      uint32_t err_crc           : 1;
      uint32_t err_dribble_bit   : 1;
      uint32_t err_rx_mii        : 1;
      uint32_t err_rx_wdt        : 1;
      uint32_t frm_type          : 1;
      uint32_t err_late_coll     : 1;
      uint32_t giant_frm         : 1;
      uint32_t last_desc         : 1;
      uint32_t first_desc        : 1;
      uint32_t vlan_tag          : 1;
      uint32_t err_overflow      : 1;
      uint32_t length_err        : 1;
      uint32_t s_addr_filt_fail  : 1;
      uint32_t err_desc          : 1;
      uint32_t err_summary       : 1;
      uint32_t frm_len           : 14;
      uint32_t d_addr_filt_fail  : 1;
      uint32_t own               : 1;
    };
    uint32_t rdes0;
  };
  /** Second word of receive descriptor */
  union {
    struct {
      uint32_t rx_buf1_sz        : 13;
      uint32_t                   : 1;
      uint32_t addr2_chained     : 1;
      uint32_t rx_end_of_ring    : 1;
      uint32_t rx_buf2_sz        : 13;
      uint32_t                   : 2;
      uint32_t dis_int_compl     : 1;
    };
    uint32_t rdes1;
  };
  /** Pointer to frame data buffer */
  uint8_t *buf1_ptr;
  /**
   * Unused, since this driver initializes only a single descriptor for each
   * direction.
   */
  uint8_t *buf2_ptr;
} quarkX1000_eth_rx_desc_t;

/** Driver metadata associated with each Ethernet device */
typedef struct quarkX1000_eth_meta {
  /** Transmit descriptor */
  volatile quarkX1000_eth_tx_desc_t tx_desc;
  /** Transmit DMA packet buffer */
  volatile uint8_t tx_buf[UIP_BUFSIZE];
  /** Receive descriptor */
  volatile quarkX1000_eth_rx_desc_t rx_desc;
  /** Receive DMA packet buffer */
  volatile uint8_t rx_buf[UIP_BUFSIZE];
} quarkX1000_eth_meta_t;

void quarkX1000_eth_init(quarkX1000_eth_driver_t *c_this,
                         quarkX1000_eth_meta_t *meta,
                         pci_config_addr_t pci_addr);
void quarkX1000_eth_poll(quarkX1000_eth_driver_t c_this, uint16_t *frame_len);
void quarkX1000_eth_send(quarkX1000_eth_driver_t c_this);

#endif /* CPU_X86_DRIVERS_QUARKX1000_ETH_H_ */
