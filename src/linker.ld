ENTRY(_start)

SECTIONS {
    . = 1M;

    .multiboot_header : {
        *(.multiboot_header)
    }

    .text : {
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

    . = ALIGN(0x1000);
}