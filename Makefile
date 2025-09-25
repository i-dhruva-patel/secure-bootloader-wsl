# Makefile for Secure Bootloader Project

CC      = arm-none-eabi-gcc
AS      = arm-none-eabi-as
LD      = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

CFLAGS  = -mcpu=cortex-m3 -mthumb -Os -Wall -ffreestanding -fno-builtin -nostdlib \
          -Icommon -Ibootloader -Iapp
LDFLAGS = -Wl,--gc-sections

BUILD   = build

BOOT_SRCS = bootloader/main.c common/startup.s
BOOT_LDS  = common/bootloader.ld
BOOT_ELF  = $(BUILD)/bootloader.elf
BOOT_BIN  = $(BUILD)/bootloader.bin

APP_SRCS  = app/main.c common/startup_app.s
APP_LDS   = common/app.ld
APP_ELF   = $(BUILD)/app.elf
APP_BIN   = $(BUILD)/app.bin

COMBINED_IMAGE = $(BUILD)/flash_image.bin

.PHONY: all clean run qemu

all: $(BOOT_ELF) $(APP_ELF) $(COMBINED_IMAGE)

$(BUILD):
	mkdir -p $(BUILD)

$(BOOT_ELF): $(BOOT_SRCS) $(BOOT_LDS) | $(BUILD)
	$(CC) $(CFLAGS) -T$(BOOT_LDS) $(LDFLAGS) -o $@ $(BOOT_SRCS)
	$(OBJCOPY) -O binary $@ $(BOOT_BIN)

$(APP_ELF): $(APP_SRCS) $(APP_LDS) | $(BUILD)
	$(CC) $(CFLAGS) -T$(APP_LDS) $(LDFLAGS) -o $@ $(APP_SRCS)
	$(OBJCOPY) -O binary $@ $(APP_BIN)

$(COMBINED_IMAGE): $(BOOT_BIN) $(APP_BIN)
	dd if=/dev/zero bs=1 count=0 seek=$$((256*1024)) of=$(COMBINED_IMAGE)
	printf '\xFF%.0s' {1..262144} | dd of=$(COMBINED_IMAGE) bs=1 conv=notrunc
	dd if=$(BOOT_BIN) of=$(COMBINED_IMAGE) bs=1 seek=0 conv=notrunc
	dd if=$(APP_BIN)  of=$(COMBINED_IMAGE) bs=1 seek=65536 conv=notrunc
	@echo "🔧 Combined image ready: $(COMBINED_IMAGE)"

run: $(COMBINED_IMAGE)
	qemu-system-arm -M lm3s6965evb -kernel $(BOOT_ELF) -semihosting -nographic

qemu-gdb:
	qemu-system-arm -M lm3s6965evb -kernel $(BOOT_ELF) -semihosting -nographic -S -gdb tcp::3333

clean:
	rm -rf $(BUILD)

