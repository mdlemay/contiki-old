#include <math.h>

#include "pit.h"
#include "helpers.h"
#include "interrupt.h"

// PCs usually provide an 8254 PIT chip with maximum clock of 1.193182 MHz.
#define PIT_CONTROL_PORT    0x43
#define PIT_COUNTER0_PORT   0x40
#define PIT_CLOCK_FREQUENCY 1193182


static pit_int_callback interrupt_cb;

static void __attribute__((__used__)) pit_int_handler(void)
{
  interrupt_cb();

  // FIXME: Add a pic_send_eoi() API or similar and call it here.
  outb(0x20, 0x20); /* master PIC */
}

void
pit_init(uint32_t ticks_rate, pit_int_callback cb)
{
  // FIXME: Call some PIC API to get the current offset and then add to its default IRQ number (0).
  SET_INTERRUPT_HANDLER(32, 0, pit_int_handler);

  interrupt_cb = cb;

  // Calculate the 16bit divisor that can provide the chosen clock tick rate
  // (CLOCK_CONF_SECOND in contiki-conf.h). For reference --> tick rate = clock frequency / divisor.
  // If we provide an odd divisor to the Square Wave generator (Mode 3) of
  // the Counter0, the duty cycle won't be exactly 50%, so we always round
  // it to nearest even integer.
  uint16_t divisor = rint(PIT_CLOCK_FREQUENCY / ticks_rate);

  // Setup Control register flags in a didactic way.
  uint8_t flags = 0x30; // Set bits 7:6 to select Counter0 and 5:4 to select "write 7:0 bits first".
  flags |= 0x6;         // Set bits 3:1 to Mode 3 and bit 0 to BCD off.

  outb(PIT_CONTROL_PORT, flags);

  outb(PIT_COUNTER0_PORT, divisor & 0xFF); // Write least significant bytes first.
  outb(PIT_COUNTER0_PORT, (divisor >> 8) & 0xFF);

  // FIXME: Unmask IRQ0 from Master PIC. Add a pic_set_mask() or similar and call it here.
  outb(0x21, 0xfa);
}

