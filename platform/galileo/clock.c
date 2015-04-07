#include <stddef.h>
#include <math.h>

#include "sys/clock.h"

#include "contiki-conf.h"
#include "drivers/pit.h"
#include "helpers.h"

void
clock_init(void)
{
  pit_init(CLOCK_CONF_SECOND);
}

clock_time_t
clock_time(void)
{
  // TODO: Return # of clock ticks since boot, whereas clock ticks rate is
  // defined by CLOCK_CONF_SECOND.
  return (clock_time_t) NULL;
}

unsigned long
clock_seconds(void)
{
  // TODO: Return # of seconds since boot.
  return 0;
}

void
clock_wait(clock_time_t t)
{
  // TODO: Wait for t ticks.
}

void
clock_delay_usec(uint16_t t)
{
  // TODO: Wait for t microseconds. Remember that for achieving such resolution,
  // disabling interrupts is needed.
}
