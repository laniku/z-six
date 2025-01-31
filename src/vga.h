#ifndef VGA_H
#define VGA_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned int uint64_t;
typedef unsigned int size_t;

// RGB565 color format (16-bit)
#define RGB565(r,g,b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))

// Standard 16-bit colors
#define COLOR_BLACK   0x0000
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_WHITE   0xFFFF

typedef struct {
    uint32_t addr;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint8_t bpp;
} framebuffer_info_t;

extern framebuffer_info_t fb_info;

void vga_init(void *multiboot_info);
void put_pixel(uint32_t x, uint32_t y, uint16_t color);
void draw_rectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color);
void print_string(const char *str, uint8_t row, uint8_t col);
void draw_char(uint32_t x, uint32_t y, char c, uint16_t color);
void draw_string(uint32_t x, uint32_t y, const char *str, uint16_t color);
void int_to_hex(uint32_t num, char *buffer);

#endif