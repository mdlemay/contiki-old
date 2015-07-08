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

#include "eth.h"
#include "ethernet-drv.h"
#include "contiki-net.h"

static quarkX1000_eth_driver_t driver;
static quarkX1000_eth_meta_t meta;

#define SUBNET_IP       192, 168, 0
#define NETMASK_IP      255, 255, 255, 0
#define HOST_IP         SUBNET_IP, 128
#define GATEWAY_IP      SUBNET_IP, 1
#define NAMESERVER_IP   GATEWAY_IP

void
eth_init(void)
{
  uip_ipaddr_t ip_addr;
  pci_config_addr_t pci_addr;

#define SET_IP_ADDR(x) \
  uip_ipaddr(&ip_addr, x)

  SET_IP_ADDR(HOST_IP);
  uip_sethostaddr(&ip_addr);

  SET_IP_ADDR(NETMASK_IP);
  uip_setnetmask(&ip_addr);

  SET_IP_ADDR(GATEWAY_IP);
  uip_setdraddr(&ip_addr);

#if WITH_DNS
  SET_IP_ADDR(NAMESERVER_IP);
  uip_nameserver_update(&ip_addr, UIP_NAMESERVER_INFINITE_LIFETIME);
#endif

  pci_addr.raw = 0;

  /* PCI address from section 15.4 of Intel Quark SoC X1000 Datasheet. */

  pci_addr.dev = 20;
  pci_addr.func = 6;

  quarkX1000_eth_init(&driver, &meta, pci_addr);

  process_start(&ethernet_process, &driver);
}
