#include "aes.h"
#include <openssl/evp.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int aes_decrypt_cbc(const unsigned char *ciphertext, int ciphertext_len,
                    const unsigned char *key, const unsigned char *iv,
                    unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;
    int len = 0, plaintext_len = 0;

    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return -1;

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) return -1;
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) return -1;
    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) return -1;
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}
