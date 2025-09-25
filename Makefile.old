CC = gcc
CFLAGS = -Wall -Wextra
LDLIBS = -lssl -lcrypto

BOOTLOADER_SRC = bootloader/bootloader.c bootloader/aes.c bootloader/rsa_verify.c
FIRMWARE_SRC = firmware/main.c

all: bootloader.elf firmware.bin

bootloader.elf: $(BOOTLOADER_SRC) firmware/main.c
	$(CC) $(CFLAGS) $(BOOTLOADER_SRC) firmware/main.c -o $@ $(LDLIBS)

firmware.bin: $(FIRMWARE_SRC)
	$(CC) -c $(FIRMWARE_SRC) -o firmware/main.o
	ld -r firmware/main.o -o $@

clean:
	rm -f bootloader.elf firmware.bin firmware/*.o

