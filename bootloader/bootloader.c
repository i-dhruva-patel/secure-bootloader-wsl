#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aes.h"

extern void firmware_main();
int verify_signature(const char *pubkey_path, const unsigned char *data, size_t data_len,
                     const unsigned char *sig, size_t sig_len);

#define KEY_LEN 16
#define IV_LEN 16
#define SIG_LEN 256
#define MAX_SIZE 4096

int main() {
    FILE *f = fopen("mock_flash.bin", "rb");
    if (!f) {
        perror("mock_flash.bin");
        return 1;
    }

    // Get file size
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    rewind(f);

    if (file_size <= SIG_LEN) {
        printf("❌ File too small to contain encrypted data + signature\n");
        fclose(f);
        return 1;
    }

    long enc_len = file_size - SIG_LEN;
    unsigned char *enc_data = malloc(enc_len);
    unsigned char sig[SIG_LEN];
    unsigned char plain[MAX_SIZE];
    unsigned char key[KEY_LEN] = "1234567890abcdef";
    unsigned char iv[IV_LEN]  = "fedcba0987654321";

    fread(enc_data, 1, enc_len, f);
    fread(sig, 1, SIG_LEN, f);
    fclose(f);

    printf("🔐 Encrypted data length: %ld bytes\n", enc_len);
    printf("🔏 Signature length: %d bytes\n", SIG_LEN);

    if (!verify_signature("keys/public.pem", enc_data, enc_len, sig, SIG_LEN)) {
        printf("❌ Signature verification failed!\n");
        free(enc_data);
        return 1;
    }

    int plain_len = aes_decrypt_cbc(enc_data, enc_len, key, iv, plain);
    printf("✅ Firmware verified and decrypted!\n\n");

    ((void (*)(void))firmware_main)();  // Simulated jump
    free(enc_data);
    return 0;
}

