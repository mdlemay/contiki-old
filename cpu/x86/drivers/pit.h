#ifndef PIT_H
#define PIT_H

#include <stdint.h>

/**
 * Initializes the 8254 Programmable Interrupt Timer chip (Counter 0 only).
 */
void pit_init(uint32_t ticks_rate);

#endif
