ENTRY (start)
OUTPUT_FORMAT(elf32-i386)

SECTIONS {
	. = 0x00100000;

	.header :
	AT (0x00100000)
	{
		*(.header)
	}

	.text ALIGN (0x1000) :
	{
		*(.text)
	}

	.data ALIGN (0x1000) :
	{
		*(.data)
	}

	.rodata ALIGN (0x1000) :
	{
		*(.rodata)
	}

	.bss :
	{
		sbss = .;
		*(.COMMON)
		*(.bss)
		ebss = .;
	}
}
