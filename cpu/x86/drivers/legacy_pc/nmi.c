#include "helpers.h"

#define NMI_ENABLE_PORT 0x70

void
nmi_enable(void)
{
  uint8_t value = inb(NMI_ENABLE_PORT);
  outb(NMI_ENABLE_PORT, value & ~BIT(8));
}
/*---------------------------------------------------------------------------*/
void
nmi_disable(void)
{
  uint8_t value = inb(NMI_ENABLE_PORT);
  outb(NMI_ENABLE_PORT, value | BIT(8));
}
