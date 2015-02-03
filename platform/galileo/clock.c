#include <stddef.h>

#include "sys/clock.h"

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
  return 0;
}

void
clock_delay(unsigned int d)
{
}
