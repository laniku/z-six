#include "temp-libc.h"

// Minimal snprintf implementation
int vsnprintf(char *str, size_t size, const char *format, unsigned int *args);

int snprintf(char *str, size_t size, const char *format, ...) {
    // Pointer to the first unnamed argument
    unsigned int *args = (unsigned int *)(&format + 1);
    return vsnprintf(str, size, format, args);
}

int vsnprintf(char *str, size_t size, const char *format, unsigned int *args) {
    size_t i = 0;
    const char *p = format;
    while (*p && i < size - 1) {
        if (*p == '%') {
            p++;
            if (*p == 'x') {
                // Handle hex format
                unsigned int num = *args++;
                char num_str[10];
                size_t num_len = 0;
                if (num == 0) {
                    num_str[num_len++] = '0';
                } else {
                    while (num > 0) {
                        int digit = num & 0xF;
                        num_str[num_len++] = digit < 10 ? '0' + digit : 'a' + (digit - 10);
                        num >>= 4;
                    }
                }
                for (size_t j = num_len; j > 0 && i < size - 1; j--) {
                    str[i++] = num_str[j - 1];
                }
            } else if (*p == 'd') {
                // Handle decimal format
                unsigned int num = *args++;
                char num_str[10];
                size_t num_len = 0;
                if (num == 0) {
                    num_str[num_len++] = '0';
                } else {
                    while (num > 0) {
                        num_str[num_len++] = '0' + (num % 10);
                        num /= 10;
                    }
                }
                for (size_t j = num_len; j > 0 && i < size - 1; j--) {
                    str[i++] = num_str[j - 1];
                }
            }
            p++;
        } else {
            str[i++] = *p++;
        }
    }
    str[i] = '\0';
    return i;
}