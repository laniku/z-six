#define NULL ((void *)0)
#include "vga.h"
#include "font.h"

extern framebuffer_info_t fb_info;
extern void vga_init(void *multiboot_info);
extern void put_pixel(unsigned int x, unsigned int y, unsigned short color);
extern void draw_rectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned short color);
extern void draw_line(uint32_t x, uint32_t y, uint32_t length, uint16_t color);
extern int snprintf(char *str, size_t size, const char *format, ...);
extern int vsnprintf(char *str, size_t size, const char *format, unsigned int *args);
extern void print_string(const char *str, uint8_t row, uint8_t col);
extern void draw_char(uint32_t x, uint32_t y, char c, uint16_t color);
extern void draw_string(uint32_t x, uint32_t y, const char *str, uint16_t color);


void kernel_main(void *multiboot_info);

void _start(void) {
    register uint32_t magic asm ("eax");
    register uint32_t mbi asm ("ebx");
    
    // Check GRUB2 magic number
    if (magic != 0x36d76289) {
        volatile char *video = (volatile char*)0xB8000;
        video[0] = 'M'; // Bad Magic
        video[1] = 0x4F; // Red on black
        while(1) __asm__ __volatile__("hlt");
    }
    
    kernel_main((void*)mbi);
}


void kernel_main(void *multiboot_info) {
    vga_init(multiboot_info);

    if (fb_info.addr == 0) {
        draw_string(10, 10, "Framebuffer not initialized", COLOR_WHITE);
        while (1) __asm__ __volatile__("hlt");
    }

    // Clear the screen
    draw_rectangle(0, 0, fb_info.width, fb_info.height, COLOR_BLACK);

    // Draw a blue rectangle
    draw_rectangle(50, 50, 100, 50, COLOR_BLUE);

    // Draw a white string
    draw_string(10, 150, "Test Line", COLOR_WHITE);

    // Print register values
    register uint32_t eax asm("eax");
    register uint32_t ebx asm("ebx");
    char buffer[9];

    int_to_hex(eax, buffer);
    draw_string(10, 200, "EAX: ", COLOR_WHITE);
    draw_string(50, 200, buffer, COLOR_WHITE);

    int_to_hex(ebx, buffer);
    draw_string(10, 220, "EBX: ", COLOR_WHITE);
    draw_string(50, 220, buffer, COLOR_WHITE);

    while (1) {
        __asm__ __volatile__("hlt");
    }
}