#include <common/print.h>

struct VideoMemory *VideoMemory = (struct VideoMemory *)0xb8000;
uint8_t x = 0, y = 0;
char hex_upper[] = "0123456789ABCDEF";
char hex_lower[] = "0123456789abcdef";

static void putchar(char ch)
{
	if (ch == '\n') {
		x = 0;
		if (++y == CONSOLE_HEIGHT)
			y = 0;
	} else {
		// VideoMemory[CONSOLE_WIDTH * y + x].color = Color::SILVER;
		VideoMemory[CONSOLE_WIDTH * y + x].symbol = ch;
		if (++x == CONSOLE_WIDTH) {
			x = 0;
			if (++y == CONSOLE_HEIGHT)
				y = 0;
		}
	}
}

void print_str(char *str)
{
	for (int i = 0; str[i] != '\0'; i++) {
		putchar(str[i]);
	}
}

void print_udecimal(uint32_t number)
{
	print_str("print_udecimal is not implemented");
}

void print_decimal(uint32_t number)
{
	print_str("print_udecimal is not implemented");
}

void print_uhexadecimal(uint32_t number, char *hex)
{
	for (int i = 7; i >= 0; i--) {
		uint32_t num = (number & (0xF << i * 4)) >> (i * 4);
		if (num > 0) {
			putchar(hex[num]);
			for (i--; i >= 0; i--) {
				num = (number & (0xF << i * 4)) >> (i * 4);
				putchar(hex[num]);
			}
			return;
		}
	}
	print_str("0");
}

void print_uhexadecimal64(uint64_t number, char *hex)
{
	uint64_t mask = 0xF;
	for (int i = 15; i >= 0; i--) {
		uint32_t num = (number & (mask << i * 4)) >> (i * 4);
		if (num > 0) {
			putchar(hex[num]);
			for (i--; i >= 0; i--) {
				num = (number & (mask << i * 4)) >> (i * 4);
				putchar(hex[num]);
			}
			return;
		}
	}
	print_str("0");
}

void printf(const char *format, ...)
{
	uint32_t param_ptr = (uint32_t)&format;
	uint32_t offset = sizeof(char *);

	for (int i = 0; format[i] != '\0'; i++) {
		if (format[i] == '%') {
			switch (format[++i]) {
				case '%': putchar('%'); break;
				// case 'd': case 'i': print_udecimal(*(*(uint32_t**)(param_ptr + i++))); break;
				case 'x': print_uhexadecimal(*((uint32_t*)(param_ptr + offset)), hex_lower); offset += sizeof(uint32_t); break;
				case 'X': print_uhexadecimal(*((uint32_t*)(param_ptr + offset)), hex_upper); offset += sizeof(uint32_t); break;
				case 'r': print_uhexadecimal64(*((uint64_t*)(param_ptr + offset)), hex_lower); offset += sizeof(uint64_t); break;
				case 'R': print_uhexadecimal64(*((uint64_t*)(param_ptr + offset)), hex_upper); offset += sizeof(uint64_t); break;
				case 'u': print_udecimal(*((uint32_t*)(param_ptr + offset))); offset += sizeof(uint32_t); break;
				// case 'c': putchar(format[i]); break;
				// case 's': print_str(*(*(uint32_t**)(param_ptr + i++)))
				case '\0': return;
			}
		} else {
			putchar(format[i]);
		}
	}
}