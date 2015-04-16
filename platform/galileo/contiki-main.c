#include <stdio.h>

#include "contiki.h"
#include "cpu.h"
#include "interrupt.h"

int
main(void)
{
  cpu_init();

  printf("Starting Contiki on Galileo...\n");
  // TODO: Init all the board-related bits (bus, uart, gpio, etc) here.

  // NOTE: We call these functions because all other ports do.
  // They are the reason we have stub implementations on clock.c and rtimer-arch.
  process_init();
  process_start(&etimer_process, NULL);
  ctimer_init();
  rtimer_init();

  ENABLE_IRQ();

  // This is necessary to load the main processes.
  autostart_start(autostart_processes);

  while(1) {
    /* Call run_process() how many time it is needed until there is no
     * pending events.
     * */
    while(process_run());

    /* XXX: We might want to set a power save mode here since there is no
     * events to be processed anymore.
     */
  }

  return 0;
}

