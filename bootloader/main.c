#include <stdint.h>

#define APP_START_ADDR  0x00010000

typedef void (*app_entry_t)(void);

void jump_to_app(uint32_t app_addr) {
    uint32_t *vector_table = (uint32_t *)app_addr;
    uint32_t app_stack = vector_table[0];
    uint32_t app_reset = vector_table[1];

    __asm volatile ("msr msp, %0" :: "r" (app_stack));
    ((app_entry_t)app_reset)();
}

int main(void) {
    jump_to_app(APP_START_ADDR);
    while (1); // should never return
}
