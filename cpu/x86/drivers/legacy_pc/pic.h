#ifndef PIC_H
#define PIC_H

#include "helpers.h"

#define PIC1_CMD_PORT   0x20
#define PIC1_DATA_PORT  0x21
#define PIC2_CMD_PORT   0xA0
#define PIC2_DATA_PORT  0xA1
#define PIC1_OFFSET     0x20
#define PIC2_OFFSET     PIC1_OFFSET + 8

/*
 * Returns the actual interrupt number of a given IRQ,
 * no matter which PIC it is part of.
 */
#define PIC_INT(a)      (a + PIC1_OFFSET)

void pic_unmask_irq(unsigned int num);

/*
 * This function initializes the daisy-chained Master and Slave 8259 PICs.
 * For more information about the ICWs, please refer to http://stanislavs.org/helppc/8259.html.
 */
void pic_init(void);

/*
 * This function sends an end-of-interrupt (EOI) to the correct PIC according
 * to the IRQ line number.
 */
void pic_eoi(unsigned int irq);

#endif /* PIC_H */
