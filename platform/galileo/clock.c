#include <stddef.h>
#include <math.h>

#include "sys/clock.h"
#include "sys/etimer.h"

#include "contiki-conf.h"
#include "drivers/pit.h"
#include "helpers.h"


static volatile clock_time_t tick_count = 0;

void timer_callback(void)
{
  ++tick_count;

  // The etimer might have expired now that the system clock was updated.
  if(etimer_pending()) {
    etimer_request_poll();
  }
}

void
clock_init(void)
{
  pit_init(CLOCK_CONF_SECOND, timer_callback);
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
