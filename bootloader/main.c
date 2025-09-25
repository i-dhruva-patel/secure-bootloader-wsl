#include <stdint.h>
#include <stdint.h>
#include "boot_image.h"   

#define APP_BASE_ADDR  0x00010000

void jump_to_app(void) {
    uint32_t *app_vector = (uint32_t *)APP_BASE_ADDR;
    uint32_t app_stack = app_vector[0];
    uint32_t app_reset = app_vector[1];

    // 1. Disable interrupts
    __asm volatile ("cpsid i");

    // 2. Set Main Stack Pointer to app's stack
    __asm volatile ("msr msp, %0" :: "r" (app_stack));

    // 3. Set Vector Table Offset Register to app base
    #define SCB_VTOR (*(volatile uint32_t*)0xE000ED08)
    SCB_VTOR = APP_BASE_ADDR;

    // 4. Enable interrupts again
    __asm volatile ("cpsie i");

    // 5. Jump to reset handler
    void (*app_entry)(void) = (void (*)(void))app_reset;
    app_entry();
}

int main(void) {
    image_header_t *hdr = (image_header_t *)APP_BASE_ADDR;

    if (hdr->magic != APP_IMAGE_MAGIC) {
        // Invalid image — stay in bootloader
        while (1) { }
    }

    // Later: check version, verify signature, decrypt payload...

    // If all good → jump to app
    jump_to_app();
    while (1);
}

