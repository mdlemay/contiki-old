.set MAGIC_NUMBER, 0x1BADB002
.set FLAGS, 0x0                  # we should probably use 1<<0 | 1<<1 here. Check the multiboot spec.
.set CHECKSUM, -MAGIC_NUMBER

.section .multiboot
    .align 4
    .long MAGIC_NUMBER
    .long FLAGS
    .long CHECKSUM

.section .text
    .extern main
    .global start
    start:
        call main
