OUTPUT_FORMAT("elf32-i386")

ENTRY(start)

SECTIONS {
    /*
       OS-Dev Wiki says it is common for kernels to start at 1M. Addresses before that
       are used by BIOS/EFI, the bootloader and memory-mapped I/O.
    */
    . = 1M;

    _start = .;

    .text ALIGN (4K) :
    {
        _multiboot_header = .;

        *(.multiboot)
        *(.text)
    }

    .rodata ALIGN (4K) :
    {
        *(.rodata*)
    }

    .data ALIGN (4K) :
    {
        *(.data)
    }

    _edata = .;

    .bss ALIGN (4K) :
    {
        *(COMMON)
        *(.bss)
    }

    _end = .;
}
