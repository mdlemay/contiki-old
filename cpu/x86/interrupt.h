#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

#include "idt.h"

struct interrupt_context {
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t esp;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;
  uint32_t error_code;
  uint32_t eip;
};

/* Helper macro to register interrupt handler function.
 *
 * num:             Interrupt number (0-255)
 * has_error_code:  0 if interrupt doesn't push error code onto the
 *                  stack. Otherwise, set this argument to 1.
 * handler:         Pointer to function that should be called once the
 *                  interrupt is raised. In case has_error_code == 0
 *                  the function prototype should be the following:
 *                  void handler(void)
 *                  Otherwise, it should be:
 *                  void handler(struct interrupt_context context)
 *
 * Since there is no easy way to write an Interrupt Service Routines
 * (ISR) in C (for further information on this, see [1]), we provide
 * this helper macro. It basically provides an assembly trampoline
 * to a C function (handler parameter) which, indeed, handles the
 * interrupt.
 *
 * [1] http://wiki.osdev.org/Interrupt_Service_Routines
 */
#define SET_INTERRUPT_HANDLER(num, has_error_code, handler)      \
  do {                                                           \
    void __attribute__((__used__)) _dummy##num(void)             \
    {                                                            \
      __asm__ __volatile__ (                                     \
        ".global isr" #num "\n\t"                                \
        ".align 4\n\t"                                           \
        "isr" #num ":\n\t"                                       \
        "         pushal\n\t"                                    \
        "         call " #handler "\n\t"                         \
        "         popal\n\t"                                     \
        "         .if " #has_error_code "\n\t"                   \
        "         add $4, %esp\n\t"                              \
        "         .endif\n\t"                                    \
        "         iret\n\t"                                      \
      );                                                         \
   }                                                             \
                                                                 \
   void isr##num(void);                                          \
   idt_set_intr_gate_desc(num, (uint32_t) isr##num);             \
  } while (0)                                                    \

/* Disable maskable hardware interrupts */
#define DISABLE_IRQ()                                            \
  do {                                                           \
    __asm__ ("cli");                                             \
  } while (0)                                                    \

/* Enable maskable hardware interrupts */
#define ENABLE_IRQ()                                             \
  do {                                                           \
    __asm__ ("sti");                                             \
  } while (0)                                                    \

#endif /* INTERRUPT_H */
