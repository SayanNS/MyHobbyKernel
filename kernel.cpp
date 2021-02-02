struct VideoMemory
{
	char symbol;
	char color;
};

void printf(char *str)
{
	struct VideoMemory *VideoMemory = (struct VideoMemory *)0xb8000;

	for (int i = 0; str[i] != '\0'; i++) {
		VideoMemory[i].symbol = str[i];
	}
}

extern "C" void kernelMain(void *multiboot_structure, unsigned int magicnumber)
{
	printf("Hello World!");

	while(true);
}
