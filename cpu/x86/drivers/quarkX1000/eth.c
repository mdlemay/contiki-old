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

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "contiki-net.h"
#include "eth.h"
#include "helpers.h"

#define LOG_PFX "quarkX1000_eth: "

#define MMIO_SZ 0x2000

#define PCI_CMD_2_BUS_MST_EN           BIT(2)
#define PCI_CMD_1_MEM_SPACE_EN         BIT(1)

#define MAC_CONF_14_RMII_100M          BIT(14)
#define MAC_CONF_11_DUPLEX             BIT(11)
#define MAC_CONF_3_TX_EN               BIT(3)
#define MAC_CONF_2_RX_EN               BIT(2)

#define BUS_MODE_0_SW_RESET            BIT(0)

#define OP_MODE_25_RX_STORE_N_FORWARD  BIT(25)
#define OP_MODE_21_TX_STORE_N_FORWARD  BIT(21)
#define OP_MODE_13_START_TX            BIT(13)
#define OP_MODE_1_START_RX             BIT(1)

#define REG_ADDR_MAC_CONF              0x0000
#define REG_ADDR_MACADDR_HI            0x0040
#define REG_ADDR_MACADDR_LO            0x0044
#define REG_ADDR_TX_POLL_DEMAND        0x1004
#define REG_ADDR_RX_POLL_DEMAND        0x1008
#define REG_ADDR_RX_DESC_LIST          0x100C
#define REG_ADDR_TX_DESC_LIST          0x1010
#define REG_ADDR_DMA_OPERATION         0x1018

/*---------------------------------------------------------------------------*/
/**
 * \brief          Initialize a Quark X1000 Ethernet device.
 * \param c_this   Structure that will be initialized to represent the device.
 * \param meta     Driver metadata structure.
 * \param pci_addr PCI address of device.
 *
 *                 This procedure assumes that an MMIO range for the device was
 *                 previously assigned, e.g. by firmware.
 */
void
quarkX1000_eth_init(quarkX1000_eth_driver_t *c_this,
                    quarkX1000_eth_meta_t *meta,
                    pci_config_addr_t pci_addr)
{
  uip_eth_addr mac_addr;
  uint32_t mac_tmp1, mac_tmp2;
  uintptr_t mmio;

  /* Activate MMIO and DMA access. */
  pci_addr.reg_off = PCI_CONFIG_REG_CMD;
  pci_config_write(pci_addr, PCI_CMD_2_BUS_MST_EN | PCI_CMD_1_MEM_SPACE_EN);

  printf(LOG_PFX "Activated MMIO and DMA access.\n");

  /* Configure the device MMIO range and initialize the driver structure. */
  pci_init_bar0(c_this, pci_addr, (uintptr_t)meta);

  mmio = c_this->mmio;

  /* Read the MAC address from the device. */
  mac_tmp1 = *(uint32_t *)(mmio + REG_ADDR_MACADDR_HI);
  mac_tmp2 = *(uint32_t *)(mmio + REG_ADDR_MACADDR_LO);

  /* Convert the data read from the device into the format expected by
   * Contiki.
   */
  mac_addr.addr[5] = (uint8_t)(mac_tmp1 >> 8);
  mac_addr.addr[4] = (uint8_t)mac_tmp1;
  mac_addr.addr[3] = (uint8_t)(mac_tmp2 >> 24);
  mac_addr.addr[2] = (uint8_t)(mac_tmp2 >> 16);
  mac_addr.addr[1] = (uint8_t)(mac_tmp2 >> 8);
  mac_addr.addr[0] = (uint8_t)mac_tmp2;

  printf(LOG_PFX "MAC address = %02x:%02x:%02x:%02x:%02x:%02x.\n",
         mac_addr.addr[0],
         mac_addr.addr[1],
         mac_addr.addr[2],
         mac_addr.addr[3],
         mac_addr.addr[4],
         mac_addr.addr[5]
         );

  uip_setethaddr(mac_addr);

  /* Initialize transmit descriptor. */
  meta->tx_desc.tdes0 = 0;
  meta->tx_desc.tdes1 = 0;

  meta->tx_desc.buf1_ptr = (uint8_t *)meta->tx_buf;
  meta->tx_desc.tx_end_of_ring = 1;
  meta->tx_desc.first_seg_in_frm = 1;
  meta->tx_desc.last_seg_in_frm = 1;
  meta->tx_desc.tx_end_of_ring = 1;

  /* Initialize receive descriptor. */
  meta->rx_desc.rdes0 = 0;
  meta->rx_desc.rdes1 = 0;

  meta->rx_desc.buf1_ptr = (uint8_t *)meta->rx_buf;
  meta->rx_desc.own = 1;
  meta->rx_desc.first_desc = 1;
  meta->rx_desc.last_desc = 1;
  meta->rx_desc.rx_buf1_sz = UIP_BUFSIZE;
  meta->rx_desc.rx_end_of_ring = 1;

  /* Install transmit and receive descriptors. */
  *((uint32_t *)(mmio + REG_ADDR_RX_DESC_LIST)) = (uint32_t)&meta->rx_desc;
  *((uint32_t *)(mmio + REG_ADDR_TX_DESC_LIST)) = (uint32_t)&meta->tx_desc;

  *((uint32_t *)(mmio + REG_ADDR_MAC_CONF)) =
    /* Set the RMII speed to 100Mbps */
    MAC_CONF_14_RMII_100M |
    /* Enable full-duplex mode */
    MAC_CONF_11_DUPLEX |
    /* Enable transmitter */
    MAC_CONF_3_TX_EN |
    /* Enable receiver */
    MAC_CONF_2_RX_EN;

  *((uint32_t *)(mmio + REG_ADDR_DMA_OPERATION)) =
    /* Enable receive store-and-forward mode for simplicity. */
    OP_MODE_25_RX_STORE_N_FORWARD |
    /* Enable transmit store-and-forward mode for simplicity. */
    OP_MODE_21_TX_STORE_N_FORWARD |
    /* Place the transmitter state machine in the Running state. */
    OP_MODE_13_START_TX |
    /* Place the receiver state machine in the Running state. */
    OP_MODE_1_START_RX;

  printf(LOG_PFX "Enabled 100M full-duplex mode.\n");
}
/*---------------------------------------------------------------------------*/
/**
 * \brief           Poll for a received Ethernet frame.
 * \param c_this    Initialized structure representing the device.
 * \param frame_len Will be set to the size of the received Ethernet frame or
 *                  zero if no frame is available.
 *
 *                  If a frame is received, this procedure copies it into the
 *                  global uip_buf buffer.
 */
void
quarkX1000_eth_poll(quarkX1000_eth_driver_t c_this, uint16_t *frame_len)
{
  uint16_t frm_len = 0;
  quarkX1000_eth_meta_t *meta = (quarkX1000_eth_meta_t *)c_this.meta;
  uintptr_t mmio = c_this.mmio;

  /* Check whether the RX descriptor is still owned by the device.  If not,
   * process the received frame or an error that may have occurred.
   */
  if(meta->rx_desc.own == 0) {
    if(meta->rx_desc.err_summary) {
      fprintf(stderr,
              LOG_PFX "Error receiving frame: RDES0 = %08x, RDES1 = %08x.\n",
              meta->rx_desc.rdes0, meta->rx_desc.rdes1);
      assert(0);
    }

    frm_len = meta->rx_desc.frm_len;
    assert(frm_len <= UIP_BUFSIZE);
    memcpy(uip_buf, (void *)meta->rx_buf, frm_len);

    /* Return ownership of the RX descriptor to the device. */
    meta->rx_desc.own = 1;

    /* Request that the device check for an available RX descriptor, since
     * ownership of the descriptor was just transferred to the device.
     */
    *((uint32_t *)(mmio + REG_ADDR_RX_POLL_DEMAND)) = 1;
  }

  *frame_len = frm_len;
}
/*---------------------------------------------------------------------------*/
/**
 * \brief        Transmit the current Ethernet frame.
 * \param c_this Initialized structure representing the device.
 *
 *               This procedure will block indefinitely until the Ethernet
 *               device is ready to accept a new outgoing frame.  It then
 *               copies the current Ethernet frame from the global uip_buf
 *               buffer to the device DMA buffer and signals to the device that
 *               a new frame is available to be transmitted.
 */
void
quarkX1000_eth_send(quarkX1000_eth_driver_t c_this)
{
  quarkX1000_eth_meta_t *meta = (quarkX1000_eth_meta_t *)c_this.meta;
  uintptr_t mmio = c_this.mmio;

  /* Wait until the TX descriptor is no longer owned by the device. */
  while(meta->tx_desc.own == 1);

  /* Check whether an error occurred transmitting the previous frame. */
  if(meta->tx_desc.err_summary) {
    fprintf(stderr,
            LOG_PFX "Error transmitting frame: TDES0 = %08x, TDES1 = %08x.\n",
            meta->tx_desc.tdes0, meta->tx_desc.tdes1);
    assert(0);
  }

  /* Transmit the next frame. */
  assert(uip_len <= sizeof(meta->tx_buf));
  memcpy((void *)meta->tx_buf, uip_buf, uip_len);

  meta->tx_desc.tx_buf1_sz = uip_len;

  meta->tx_desc.own = 1;

  /* Request that the device check for an available TX descriptor, since
   * ownership of the descriptor was just transferred to the device.
   */
  *((uint32_t *)(mmio + REG_ADDR_TX_POLL_DEMAND)) = 1;
}
/*---------------------------------------------------------------------------*/
