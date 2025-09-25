/* Linker script for application firmware (runs at 0x00010000) */

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x00010000, LENGTH = 192K
  SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

_estack = ORIGIN(SRAM) + LENGTH(SRAM);

SECTIONS
{
  .text : {
    KEEP(*(.isr_vector))
    *(.text*) *(.rodata*)
  } > FLASH

  .data : {
    *(.data*)
  } > SRAM AT > FLASH

  .bss : {
    *(.bss*) *(COMMON)
  } > SRAM
}

