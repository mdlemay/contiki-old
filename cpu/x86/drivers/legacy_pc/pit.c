#include <math.h>

#include "pit.h"
#include "pic.h"
#include "helpers.h"
#include "interrupt.h"
#include "pic.h"

/* PCs usually provide an 8254 PIT chip with maximum clock of 1.193182 MHz. */
#define PIT_CONTROL_PORT    0x43
#define PIT_COUNTER0_PORT   0x40
#define PIT_CLOCK_FREQUENCY 1193182
#define PIT_IRQ 0
#define PIT_INT PIC_INT(PIT_IRQ)

static pit_int_callback interrupt_cb;

static void
pit_int_handler(void)
{
  interrupt_cb();

  pic_eoi(PIT_IRQ);
}
/*---------------------------------------------------------------------------*/
void
pit_init(uint32_t ticks_rate, pit_int_callback cb)
{
  SET_INTERRUPT_HANDLER(PIT_INT, 0, pit_int_handler);

  interrupt_cb = cb;

  /*
   * Calculate the 16bit divisor that can provide the chosen clock tick rate
   * (CLOCK_CONF_SECOND in contiki-conf.h). For reference --> tick rate = clock frequency / divisor.
   * If we provide an odd divisor to the Square Wave generator (Mode 3) of
   * the Counter0, the duty cycle won't be exactly 50%, so we always round
   * it to nearest even integer.
   */
  uint16_t divisor = rint(PIT_CLOCK_FREQUENCY / ticks_rate);

  /* Setup Control register flags in a didactic way. */
  uint8_t flags = 0x30; /* Set bits 7:6 to select Counter0 and 5:4 to select "write 7:0 bits first". */
  flags |= 0x6;         /* Set bits 3:1 to Mode 3 and bit 0 to BCD off. */

  outb(PIT_CONTROL_PORT, flags);

  outb(PIT_COUNTER0_PORT, divisor & 0xFF); /* Write least significant bytes first. */
  outb(PIT_COUNTER0_PORT, (divisor >> 8) & 0xFF);

  pic_unmask_irq(PIT_IRQ);
}
