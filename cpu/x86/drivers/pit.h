#ifndef PIT_H
#define PIT_H

#include <stdint.h>

typedef void (* pit_int_callback)(void);

/**
 * Initializes the 8254 Programmable Interrupt Timer chip (Counter 0 only).
 * The PIT Interrupt callback is implemented by the driver's users. It is
 * called from interrupt context, so it has to return as soon as possible.
 */
void pit_init(uint32_t ticks_rate, pit_int_callback c);

#endif
