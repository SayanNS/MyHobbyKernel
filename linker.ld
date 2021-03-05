OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i386:i386)

SECTIONS
{
	. = 0x0100000;

	.text :
	{
		load_addr = .;
		header_addr = .;
		*(.multiboot*)
		*(.text*)
		*(.rodata)
	}

	.data :
	{
		/* start_ctors = .;
		*(SORT_BY_INIT_PRIORITY(.init_array.*))
		end_ctors = .; */

		*(.data)
		load_end_addr = .;
	}

	.bss :
	{
		*(.bss)
		bss_end_addr = .;
	}

	memory_info = .;

	/DISCARD/ :
	{
		*(.init_array)
		*(.fini_array*)
		*(.comment)
	}
}
