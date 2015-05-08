#include "gdt.h"
#include "helpers.h"
#include "idt.h"
#include "interrupt.h"
#include "irq.h"

static void
double_fault_handler(struct interrupt_context context)
{
  halt();
}
/*---------------------------------------------------------------------------*/
void
cpu_init(void)
{
  gdt_init();
  idt_init();

  /* Set an interrupt handler for Double Fault exception. This way, we avoid
   * the system to triple fault, leaving no trace about what happened.
   */
  SET_INTERRUPT_HANDLER(8, 1, double_fault_handler);

  irq_init();
}
