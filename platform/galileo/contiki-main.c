#include "contiki.h"

int
main(void)
{
  printf("Starting Contiki on Galileo...\n");
  // TODO: Init all the board-related bits (bus, uart, gpio, etc) here.

  // NOTE: We call these functions because all other ports do.
  // They are the reason we have stub implementations on clock.c and rtimer-arch.
  process_init();
  process_start(&etimer_process, NULL);
  ctimer_init();
  rtimer_init();

  // This is necessary to load the main processes.
  autostart_start(autostart_processes);

  while(1) {
    // MAIN LOOP !!
    // here we should iterate through the running processes/jobs
  }

  return 0;
}

