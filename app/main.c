#include <stdint.h>

#define LED_PORT (*(volatile uint32_t *)0x40025038)  // Just dummy I/O

void delay(volatile uint32_t count) {
    while (count--) { __asm__("nop"); }
}

int main(void) {
    while (1) {
   //     LED_PORT ^= 1;  // Dummy blink
     //   delay(100000);
    }
    return 0;
}

