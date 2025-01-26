#include "vga.h"

typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint32_t bpp;
    uint32_t addr;
} __attribute__((packed)) multiboot_tag_framebuffer_t;

// Global framebuffer information structure
framebuffer_info_t fb_info;

void vga_init(void *multiboot_info) {
    uint32_t *tags = (uint32_t *)multiboot_info;

    while (*tags != 0) {
        uint32_t tag_type = *tags;

        if (tag_type == 6) {
            multiboot_tag_framebuffer_t *fb_tag = (multiboot_tag_framebuffer_t *)(tags + 1);
            
            fb_info.addr = fb_tag->addr;
            fb_info.width = fb_tag->width;
            fb_info.height = fb_tag->height;
            fb_info.pitch = fb_tag->pitch;
            fb_info.bpp = fb_tag->bpp;

            break;
        }

        tags = (uint32_t *)((char *)tags + *((uint32_t *)(tags + 1)));
    }
}

void put_pixel(uint32_t x, uint32_t y, uint16_t color) {
    volatile uint16_t *framebuffer = (uint16_t *)fb_info.addr;

    int index = y * fb_info.pitch / 2 + x;

    framebuffer[index] = color;
}

void draw_rectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color) {
    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            put_pixel(x + j, y + i, color);
        }
    }
}