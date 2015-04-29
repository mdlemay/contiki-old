#include "contiki-conf.h"
#include "drivers/legacy_pc/pit.h"
#include "sys/rtimer.h"

static volatile rtimer_clock_t tick_count = 0;
static rtimer_clock_t trigger = UINT64_MAX;

static void
update_ticks(void)
{
  if (++tick_count >= trigger) {
    /* Disable trigger by assigning it to the maximum value */
    trigger = UINT64_MAX;
    rtimer_run_next();
  }
}

void
rtimer_arch_init(void)
{
  pit_init(RTIMER_ARCH_SECOND, update_ticks);
}

rtimer_clock_t
rtimer_arch_now()
{
  return tick_count;
}

void
rtimer_arch_schedule(rtimer_clock_t t)
{
  trigger = t;
}
