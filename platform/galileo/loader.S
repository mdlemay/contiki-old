# Kernel
.set STACK_SIZE, 8192

# Multiboot
.set MAGIC_NUMBER, 0x1BADB002
.set AOUT_KLUDGE, 0x00010000     # use the address fields in the header
.set FLAGS, AOUT_KLUDGE          # we should probably also use 1<<0 | 1<<1 here. Check the multiboot spec.
.set CHECKSUM, -(MAGIC_NUMBER + FLAGS)

.section .multiboot
.align 4
.long MAGIC_NUMBER
.long FLAGS
.long CHECKSUM
# header_addr
.long _multiboot_header
# load_addr
.long _start
# load_end_addr
.long _edata
# bss_end_addr
.long _end
# entry_addr
.long start

# Reserve space for the C stack.
.lcomm c_stack, STACK_SIZE

.section .text
.extern main
.global start
start:
        cli
        movl $(c_stack + STACK_SIZE), %esp
        call main

        /* We're not expected to return from main(). But if we do we halt */
        cli
halt:   hlt
        jmp halt
