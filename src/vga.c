#include "vga.h"
#include "font.h"

extern int snprintf(char *str, size_t size, const char *format, ...);
extern int vsnprintf(char *str, size_t size, const char *format, unsigned int *args);

framebuffer_info_t fb_info = {0};

void int_to_hex(uint32_t num, char *buffer) {
    const char *hex_chars = "0123456789ABCDEF";
    for (int i = 7; i >= 0; i--) {
        buffer[i] = hex_chars[num & 0xF];
        num >>= 4;
    }
    buffer[8] = '\0';
}

typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t addr;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t bpp;
    uint8_t red_mask_size;
    uint8_t red_field_position;
    uint8_t green_mask_size;
    uint8_t green_field_position;
    uint8_t blue_mask_size;
    uint8_t blue_field_position;
    uint8_t reserved_mask_size;
    uint8_t reserved_field_position;
    uint8_t reserved;
} __attribute__((packed)) multiboot_tag_framebuffer_t;

// Global framebuffer information structure
framebuffer_info_t fb_info;

void print_string(const char *str, uint8_t row, uint8_t col) {
    volatile char *video = (volatile char*)0xB8000;
    while (*str) {
        video[(row * 80 + col) * 2] = *str++;
        video[(row * 80 + col) * 2 + 1] = 0x07; // Light grey on black
        col++;
    }
}

void vga_init(void *multiboot_info) {
    uint32_t *tags = (uint32_t *)multiboot_info;

    // Initialize framebuffer info to zero
    fb_info.addr = 0;
    fb_info.width = 0;
    fb_info.height = 0;
    fb_info.pitch = 0;
    fb_info.bpp = 0;

    if (!tags) {
        draw_string(10, 10, "No multiboot info", COLOR_WHITE);
        return;
    }

    uint32_t total_size = tags[0];
    uint32_t offset = 8;  // Skip header

    while (offset < total_size) {
        uint32_t *tag = (uint32_t*)((char*)tags + offset);
        uint32_t type = tag[0];
        uint32_t size = tag[1];

        if (type == 8) {  // Framebuffer tag
            multiboot_tag_framebuffer_t *fb_tag = (multiboot_tag_framebuffer_t*)tag;
            fb_info.addr = fb_tag->addr;
            fb_info.width = fb_tag->width;
            fb_info.height = fb_tag->height;
            fb_info.pitch = fb_tag->pitch;
            fb_info.bpp = fb_tag->bpp;

            // Print framebuffer information for debugging
            char buffer[50];
            int_to_hex(fb_info.addr, buffer);
            draw_string(10, 10, "Framebuffer Addr:", COLOR_WHITE);
            draw_string(10, 20, buffer, COLOR_WHITE);

            snprintf(buffer, sizeof(buffer), "Width: %d", fb_info.width);
            draw_string(10, 30, buffer, COLOR_WHITE);
            snprintf(buffer, sizeof(buffer), "Height: %d", fb_info.height);
            draw_string(10, 40, buffer, COLOR_WHITE);
            snprintf(buffer, sizeof(buffer), "Pitch: %d", fb_info.pitch);
            draw_string(10, 50, buffer, COLOR_WHITE);

            break;
        }

        offset += (size + 7) & ~7;  // Next aligned tag
    }
}

void put_pixel(uint32_t x, uint32_t y, uint16_t color) {
    if (x >= fb_info.width || y >= fb_info.height) {
        return;
    }
    volatile uint16_t *framebuffer = (uint16_t *)fb_info.addr;
    int index = y * (fb_info.pitch / 2) + x;
    framebuffer[index] = color;
}

void draw_char(uint32_t x, uint32_t y, char c, uint16_t color) {
    const uint8_t *bitmap = font[(unsigned char)c];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (bitmap[i] & (1 << (7 - j))) {
                put_pixel(x + j, y + i, color);
            }
        }
    }
}

void draw_string(uint32_t x, uint32_t y, const char *str, uint16_t color) {
    while (*str) {
        if (*str == '\n') {
            x = 10;
            y += 10;
        } else {
            draw_char(x, y, *str, color);
            x += 8;
        }
        str++;
    }
}

void draw_rectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color) {
    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            put_pixel(x + j, y + i, color);
        }
    }
}