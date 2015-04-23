#ifndef HELPERS_H
#define HELPERS_H

#include <stdint.h>


#define BIT(n) (1 << (n))

void halt(void) __attribute__((__noreturn__));

/**
 * Wrapper for the assembly 'out' instruction.
 *
 */
void outb(uint16_t port, uint8_t byte);

/* Wrapper for the assembly 'in' instruction */
uint8_t inb(uint16_t port);

#endif
