#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

int my_writestr(const char *str) {
    return fwrite(str, 1, strlen(str), stdout);
}

void strrev(char *str) {
    const size_t len = strlen(str);
    for (size_t i = 0, j = len - 1; i < j; ++i, --j) {
        char c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
}

void int_to_str(char *buffer, long long x, int base) {
    const char * const digits = "0123456789abcdef";
    if (x == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    if (x < 0) {
        *buffer = '-';
        ++buffer;
        x = -x;
    }
    
    int i = 0;
    for (; x != 0; x /= base, i++) {
        int digit = x % base;
        buffer[i] = digits[digit];
    }
    buffer[i] = '\0';
    strrev(buffer);
}

int my_printf(char * restrict format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[32];
    char prev = 0;
    for (; *format != '\0'; ++format) {
        if (prev == '%') {
            switch (*format) {
            case 'd': {
                int_to_str(buffer, va_arg(args, int), 10);
                if (my_writestr(buffer) == EOF) return EOF;
                break;
            }
            case 'o': {
                int_to_str(buffer, va_arg(args, int), 8);
                if (my_writestr(buffer) == EOF) return EOF;
                break;
            }
            case 'u': {
                int_to_str(buffer, va_arg(args, unsigned), 10);
                if (my_writestr(buffer) == EOF) return EOF;
                break;
            }
            case 'x': {
                int_to_str(buffer, va_arg(args, unsigned), 16);
                if (my_writestr(buffer) == EOF) return EOF;
                break;
            }
            case 'c': {
                unsigned char c = va_arg(args, int);
                if (fwrite(&c, 1, 1, stdout) != 1) return EOF;
                break;
            }
            case 's': {
                if (my_writestr(va_arg(args, const char*)) == EOF) return EOF;
                break;
            }
            case 'p': {
                const void *p = va_arg(args, const void*);
                int_to_str(buffer, (intptr_t) p, 16);
                if (my_writestr(buffer) == EOF) return EOF;
                break;
            }
            }
        }
        else if (*format != '%') {
            putchar(*format);
        }
        prev = *format;
    }
    va_end(args);
    return 0;
}


  
}