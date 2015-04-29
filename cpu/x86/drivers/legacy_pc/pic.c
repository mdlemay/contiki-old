#include "pic.h"

#define PIC_ACK 0x20

void
pic_unmask_irq(unsigned int num)
{
  uint16_t port;
  uint8_t bitmap;

  if(num <= 7) {
    port = PIC1_DATA_PORT;
  } else {
    port = PIC2_DATA_PORT;
    num -= 8;
  }

  bitmap = inb(port);
  outb(port, bitmap & ~BIT(num));
}
/*---------------------------------------------------------------------------*/
void
pic_eoi(unsigned int irq)
{
  if(irq >= 8) {
    outb(PIC2_CMD_PORT, PIC_ACK);
  }

  outb(PIC1_CMD_PORT, PIC_ACK);
}
/*---------------------------------------------------------------------------*/
void
pic_init(void)
{
  /* ICW1: Initialization. */
  outb(PIC1_CMD_PORT, 0x11);
  outb(PIC2_CMD_PORT, 0x11);

  /* ICW2: Remap IRQs by setting an IDT Offset for each PIC. */
  outb(PIC1_DATA_PORT, PIC1_OFFSET);
  outb(PIC2_DATA_PORT, PIC2_OFFSET);

  /* ICW3: Setup Slave to Master's IRQ2. */
  outb(PIC1_DATA_PORT, 0x04);
  outb(PIC2_DATA_PORT, 0x02);

  /* ICW4: Environment setup. Set PIC1 as master and PIC2 as slave. */
  outb(PIC1_DATA_PORT, 0x01);
  outb(PIC2_DATA_PORT, 0x01);

  /* Set the IMR register, masking all hardware interrupts but IRQ 2.
   * We will have to unmask each IRQ when registering them. */
  outb(PIC1_DATA_PORT, 0xfb);
  outb(PIC2_DATA_PORT, 0xff);
}
