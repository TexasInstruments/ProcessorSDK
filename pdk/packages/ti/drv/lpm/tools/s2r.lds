MEMORY
{
  sram      (RWX) : ORIGIN = 0x41011000, LENGTH = 0x00008000
}

STACK_SIZE = 0x1000; /* 4 kB */

ENTRY(Lpm_enterRetention)

SECTIONS
{
    .text : {
        *(.text.Lpm_enterRetention)
        *(.text)
    } > sram

    .stack (NOLOAD) :
    {
        . = ALIGN(8);
        . = . + STACK_SIZE;
        . = ALIGN(8);
    } > sram
}
