.syntax unified
.cpu cortex-m3
.thumb

.section .isr_vector,"a",%progbits
.type g_pfnVectors, %object
.size g_pfnVectors, .-g_pfnVectors

.extern _estack
.extern Reset_Handler

g_pfnVectors:
  .word _estack
  .word Reset_Handler

.text
.thumb
.global Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
  bl main
  b .
