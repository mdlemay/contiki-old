#include <stddef.h>
#include <math.h>

#include "sys/clock.h"
#include "sys/etimer.h"

#include "contiki-conf.h"
#include "drivers/rtc.h"
#include "helpers.h"


static volatile clock_time_t tick_count = 0;

static void
update_ticks(void)
{
  ++tick_count;

  /* Notify etimer library if time has expired */
  clock_time_t expire_time = etimer_next_expiration_time();
  if(expire_time != 0 && tick_count >= expire_time)
    etimer_request_poll();
}

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

unsigned long
clock_seconds(void)
{
  return tick_count / CLOCK_CONF_SECOND;
}

void
clock_wait(clock_time_t t)
{
  clock_time_t initial = clock_time();

  while(clock_time() < t + initial);
}

void
clock_delay_usec(uint16_t t)
{
  // TODO: Wait for t microseconds. Remember that for achieving such resolution,
  // disabling interrupts is needed.
}
