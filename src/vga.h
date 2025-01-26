#ifndef VGA_H
#define VGA_H

// Manually define uint8_t, uint16_t, uint32_t, and uint64_t
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

// Define the framebuffer information structure
typedef struct {
    uint64_t addr;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint8_t bpp;
} framebuffer_info_t;

// Function declarations
void vga_init(void *multiboot_info);
void put_pixel(uint32_t x, uint32_t y, uint16_t color);
void draw_rectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color);

#endif