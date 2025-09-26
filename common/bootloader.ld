MEMORY
{
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 64K
  SRAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 64K
}

_estack = ORIGIN(SRAM) + LENGTH(SRAM);

ENTRY(Reset_Handler)

SECTIONS
{
    /* Vector table MUST be at the very beginning */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    /* Code section */
    .text :
    {
        . = ALIGN(4);
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    /* Initialized data */
    _sidata = .;
    .data : AT(_sidata)
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        _edata = .;
        . = ALIGN(4);
    } > SRAM

    /* Uninitialized data */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
        . = ALIGN(4);
    } > SRAM

}
