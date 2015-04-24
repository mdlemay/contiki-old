#include "drivers/pic.h"

#define PIC_ACK 0x20


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

void
pic_eoi(unsigned int irq)
{
  if (irq >= 8)
    outb(PIC2_CMD_PORT, PIC_ACK);

  outb(PIC1_CMD_PORT, PIC_ACK);
}
