#define NULL ((void *)0)
#include "vga.h"

extern void vga_init(void *multiboot_info);
extern void put_pixel(unsigned int x, unsigned int y, unsigned short color);
extern void draw_rectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned short color);

void kernel_main(void *multiboot_info);

void _start() {
    kernel_main(NULL);  // Duct-tape fix for GCC
}

void kernel_main(void *multiboot_info) {
    vga_init(multiboot_info);

    // Draw a blue rectangle at 50,50
    draw_rectangle(50, 50, 100, 50, 0x1F);

    while (1) {
        __asm__ __volatile__("hlt");
    }
}
