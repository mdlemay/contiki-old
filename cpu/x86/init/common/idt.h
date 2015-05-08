#ifndef IDT_H
#define IDT_H

#include <stdint.h>

void idt_init(void);
void idt_set_intr_gate_desc(int intr_num, uint32_t offset);

#endif
