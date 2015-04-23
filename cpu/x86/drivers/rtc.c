#include <math.h>

#include "helpers.h"
#include "interrupt.h"
#include "drivers/pic.h"

#define RTC_INDEX_REGISTER   0x70
#define RTC_TARGET_REGISTER  0x71

void (*user_callback)(void);

static void __attribute__((__used__))
rtc_handler()
{
  user_callback();

  /* Clear Register C otherwise interrupts will not happen again.
   * Register C is automatically cleared when it is read so we do
   * a dummy read to clear it.
   */
  outb(RTC_INDEX_REGISTER, 0x0C);
  inb(RTC_TARGET_REGISTER);

  /* Issue the End of Interrupt to PIC */
  outb(0xA0, 0x20);
  outb(0x20, 0x20);
}

/* Initialize the Real Time Clock.
 * @frequency: Number of ticks per second. RTC has very specific values
 *             for frequency. They are: 2, 4, 8, 16, 32, 64, 128, 256, 512,
 *             1024, 2048, 4096, and 8192 Hz. Make sure you use a valid
 *             value otherwise it will not work properly.
 * @callback:  This callback is called every time the RTC IRQ is raised.
 *             It is executed in interrupt context.
 */
void
rtc_init(unsigned int frequency, void(*callback)(void))
{
  uint8_t rate_select = log2(32768 / frequency) + 1;
  uint8_t reg_a, reg_b;

  user_callback = callback;

  /* FIXME: Once we have a proper API to ask PIC what is the IRQ offset, we
   * should consider using it here.
   */
  SET_INTERRUPT_HANDLER(40, 0, rtc_handler);

  /* Select interrupt period to 7.8125 ms */
  outb(RTC_INDEX_REGISTER, 0x0A);
  reg_a = inb(RTC_TARGET_REGISTER);
  reg_a &= 0xF0;
  reg_a |= rate_select;
  outb(RTC_INDEX_REGISTER, 0x0A);
  outb(RTC_TARGET_REGISTER, reg_a);

  /* Enable periodic interrupt */
  outb(RTC_INDEX_REGISTER, 0x0B);
  reg_b = inb(RTC_TARGET_REGISTER);
  outb(RTC_INDEX_REGISTER, 0x0B);
  outb(RTC_TARGET_REGISTER, reg_b | BIT(6));

  pic_unmask_irq(8);
}
