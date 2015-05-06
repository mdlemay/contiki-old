#include <math.h>

#include "helpers.h"
#include "interrupt.h"
#include "nmi.h"
#include "pic.h"

#define RTC_INDEX_REGISTER   0x70
#define RTC_TARGET_REGISTER  0x71
#define RTC_IRQ 8
#define RTC_INT PIC_INT(RTC_IRQ)

void (*user_callback)(void);

static void
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
  pic_eoi(RTC_IRQ);
}
/*---------------------------------------------------------------------------*/
/* Initialize the Real Time Clock.
 * @frequency: Number of ticks per second. RTC has very specific values
 *             for frequency. They are: 2, 4, 8, 16, 32, 64, 128, 256, 512,
 *             1024, 2048, 4096, and 8192 Hz. Make sure you use a valid
 *             value otherwise it will not work properly.
 * @callback:  This callback is called every time the RTC IRQ is raised.
 *             It is executed in interrupt context.
 */
void
rtc_init(unsigned int frequency, void (*callback)(void))
{
  uint8_t rate_select = log2(32768 / frequency) + 1;
  uint8_t reg_a, reg_b;

  user_callback = callback;

  SET_INTERRUPT_HANDLER(RTC_INT, 0, rtc_handler);

  nmi_disable();

  /* Select interrupt period to 7.8125 ms */
  outb(RTC_INDEX_REGISTER, 0x8A);
  reg_a = inb(RTC_TARGET_REGISTER);
  reg_a &= 0xF0;
  reg_a |= rate_select;
  outb(RTC_INDEX_REGISTER, 0x8A);
  outb(RTC_TARGET_REGISTER, reg_a);

  /* Enable periodic interrupt */
  outb(RTC_INDEX_REGISTER, 0x8B);
  reg_b = inb(RTC_TARGET_REGISTER);
  outb(RTC_INDEX_REGISTER, 0x8B);
  outb(RTC_TARGET_REGISTER, reg_b | BIT(6));

  nmi_enable();

  pic_unmask_irq(RTC_IRQ);
}
