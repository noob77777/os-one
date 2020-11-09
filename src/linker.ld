ENTRY(loader)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

PHYSICAL_BASE = 0x00100000;
VIRTUAL_BASE = 0x00000000;

SECTIONS
{
  . = PHYSICAL_BASE + VIRTUAL_BASE;

  .text ALIGN(0x1000) : AT(ADDR(.text) - VIRTUAL_BASE)
  {
    *(.multiboot)
    *(.text*)
    *(.rodata)
  }

  .data ALIGN(0x1000) : AT(ADDR(.data) - VIRTUAL_BASE)
  {
    start_ctors = .;
    KEEP(*( .init_array ));
    KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
    end_ctors = .;

    *(.data)
  }

  .bss ALIGN(0x1000) : AT(ADDR(.bss) - VIRTUAL_BASE)
  {
    *(.bss)
  }

  /DISCARD/ : { *(.fini_array*) *(.comment) }
}
