.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb

.section .isr_vector,"a",%progbits
.type g_pfnVectors, %object
.size g_pfnVectors, .-g_pfnVectors

.extern Reset_Handler
.extern _estack

g_pfnVectors:
  .word _estack
  .word Reset_Handler
  .word 0 /* NMI */
  .word 0 /* HardFault */
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0

.text
.global Reset_Handler
Reset_Handler:
  bl main
  b .
