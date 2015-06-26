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

#include <stddef.h>
#include <math.h>

#include "sys/clock.h"
#include "sys/etimer.h"

#include "contiki-conf.h"
#include "drivers/legacy_pc/rtc.h"

static volatile clock_time_t tick_count = 0;

static void
update_ticks(void)
{
  ++tick_count;

  /* Notify etimer library if time has expired */
  clock_time_t expire_time = etimer_next_expiration_time();
  if(expire_time != 0 && tick_count >= expire_time) {
    etimer_request_poll();
  }
}
/*---------------------------------------------------------------------------*/
void
clock_init(void)
{
  rtc_init(CLOCK_CONF_SECOND, update_ticks);
}
clock_time_t
clock_time(void)
{
  return tick_count;
}
/*---------------------------------------------------------------------------*/
unsigned long
clock_seconds(void)
{
  return tick_count / CLOCK_CONF_SECOND;
}
/*---------------------------------------------------------------------------*/
void
clock_wait(clock_time_t t)
{
  clock_time_t initial = clock_time();

  while(clock_time() < t + initial);
}
/*---------------------------------------------------------------------------*/
void
clock_delay_usec(uint16_t t)
{
  /* TODO: Wait for t microseconds. Remember that for achieving such resolution, */
  /* disabling interrupts is needed. */
}
