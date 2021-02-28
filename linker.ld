OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i386:i386)

SECTIONS
{
	. = 0x0100000;

	.multiboot :
	{
		header_addr = .;
		KEEP(*(.multiboot*))
	}

	.text :
	{
		load_addr = .;
		*(.text*)
		*(.rodata)
	}

	.data :
	{
		/* start_ctors = .;
		*(.init_array)
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

	/DISCARD/ :
	{
		*(.fini_array*)
		*(.comment)
	}
}
