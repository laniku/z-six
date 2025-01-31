.section .multiboot_header, "a"
    .align 8
    .globl multiboot_header
multiboot_header:
    # Header
    .long 0xe85250d6          # Multiboot2 magic
    .long 0                   # i386 arch
    .long header_end - multiboot_header
    .long -(0xe85250d6 + 0 + (header_end - multiboot_header))

    # Framebuffer tag
    .align 8
    .word 5                   # Framebuffer tag type
    .word 0                   # Flags
    .long 20                  # Size
    .long 640                 # Width
    .long 480                 # Height
    .long 16                  # Depth

    # End tag
    .align 8
    .word 0                   # End tag type
    .word 0                   # Flags
    .long 8                   # Size

    .align 8
header_end: