#include "drivers/legacy_pc/pic.h"
#include "gdt.h"
#include "helpers.h"
#include "idt.h"
#include "interrupt.h"

#define IRQ7_INT  PIC_INT(7)

static void
double_fault_handler(struct interrupt_context context)
{
  halt();
}
/*---------------------------------------------------------------------------*/
static void
spurious_irq7_handler(void)
{
  /*
   * NOTE: Originally IRQ7 was used for the parallel port interrupts. Nowadays,
   * though, it is only used if some other IRQ (i.e.: a PCIx interrupt) is
   * mapped to it. In this case we will have to check the PIC ISR register in
   * order to confirm this was a real interrupt.
   *
   * In case of a spurious interrupt, we should NEVER send an EOI here so the PIC
   * doesn't trigger the next queued interrupt.
   */
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

  pic_init();

  /* Set a 'fake' handler for the Spurious IRQ7 interrupts.
   * Refer to http://wiki.osdev.org/PIC .
   */
  SET_INTERRUPT_HANDLER(IRQ7_INT, 0, spurious_irq7_handler);
}
