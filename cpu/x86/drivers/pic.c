#include "drivers/pic.h"

void
pic_unmask_irq(unsigned int num)
{
  uint16_t port;
  uint8_t bitmap;

  if (num <= 7) {
    port = PIC1_DATA_PORT;
  } else {
    port = PIC2_DATA_PORT;
    num -= 8;
  }

  bitmap = inb(port);
  outb(port, bitmap & ~BIT(num));
}
