#include <common/print.h>

#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 25

static struct VideoMemory *VideoMemory = (struct VideoMemory *)0xb8000;
static uint8_t x = 0, y = 0;
static char hex_upper[] = "0123456789ABCDEF";
static char hex_lower[] = "0123456789abcdef";

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

static void print_string(char *str)
{
	for (int i = 0; str[i] != '\0'; i++) {
		putchar(str[i]);
	}
}

static inline void print_udecimal(uint32_t number)
{
	char sym[10];
	int i = 0;
	while (number > 0) {
		sym[i++] = number % 10 + '0';
		number /= 10;
	}
	for (i--; i >= 0; i--) {
		putchar(sym[i]);
	}
}

static inline void print_decimal(uint32_t number)
{
	print_string("print_decimal is not implemented");
}

static inline void print_uhexadecimal(uint32_t number, int min_num, char *hex)
{
	for (int i = 7; i >= 0; i--) {
		int num = (number & (0xF << i * 4)) >> (i * 4);
		if (num > 0) {
			for (min_num-- ;min_num > i; min_num--) {
				putchar('0');
			}
			putchar(hex[num]);
			for (i--; i >= 0; i--) {
				num = (number & (0xF << i * 4)) >> (i * 4);
				putchar(hex[num]);
			}
			return;
		}
	}
	putchar('0');
}

static inline void print_uhexadecimal64(uint64_t number, char *hex)
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
	putchar('0');
}

static inline void print_bitmask(uint32_t number)
{
	int i = 0;
	for (; number > 0; i++, number /= 2) {
		putchar(number % 2 + '0');
	}
	for (; i < sizeof(number) * 8; i++) {
		putchar('0');
	}
}

void printf(const char *format, ...)
{
	uint32_t param_ptr = (uint32_t)&format;
	uint32_t offset = sizeof(char *);

	for (int i = 0; format[i] != '\0'; i++) {
		if (format[i] == '%') {
			int min_num = 0;
			for (int num = format[++i] - '0'; num >= 0 && num <= 9; num = format[++i] - '0') {
				min_num = min_num * 10 + num;
			}
			switch (format[i]) {
				case '%': putchar('%'); break;
				case 'd': case 'i': print_udecimal(*((uint32_t*)(param_ptr + offset))); offset += sizeof(uint32_t); break;
				case 'x': print_uhexadecimal(*((uint32_t*)(param_ptr + offset)), min_num, hex_lower); offset += sizeof(uint32_t); break;
				case 'X': print_uhexadecimal(*((uint32_t*)(param_ptr + offset)), min_num, hex_upper); offset += sizeof(uint32_t); break;
				case 'r': print_uhexadecimal64(*((uint64_t*)(param_ptr + offset)), hex_lower); offset += sizeof(uint64_t); break;
				case 'R': print_uhexadecimal64(*((uint64_t*)(param_ptr + offset)), hex_upper); offset += sizeof(uint64_t); break;
				case 'u': print_udecimal(*((uint32_t*)(param_ptr + offset))); offset += sizeof(uint32_t); break;
				case 'c': putchar(*((char*)(param_ptr + offset))); offset += sizeof(uint32_t); break;
				case 's': print_string(*((char**)(param_ptr + offset))); offset += sizeof(char *); break;
				case 'b': print_bitmask(*((uint32_t*)(param_ptr + offset))); offset += sizeof(uint32_t); break;
				case '\0': return;
				default: print_string("unknown command: "); putchar(format[i]); putchar('\n');
			}
		} else {
			putchar(format[i]);
		}
	}
}