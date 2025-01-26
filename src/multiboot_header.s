    .section .multiboot_header, "a"
    .align 8
    .globl multiboot_header
multiboot_header:
    .long 0xe85250d6

    .long 0

    .long multiboot_header_end - multiboot_header

    .long -(0xe85250d6 + 0 + (multiboot_header_end - multiboot_header))

    .long 6
    .long 0
    .long 20
    .long 640
    .long 480
    .word 16
    .word 0


    .long 0
    .long 0
    .long 8

multiboot_header_end:
