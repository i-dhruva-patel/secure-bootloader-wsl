#ifndef AES_H
#define AES_H

int aes_decrypt_cbc(const unsigned char *ciphertext, int ciphertext_len,
                    const unsigned char *key, const unsigned char *iv,
                    unsigned char *plaintext);

#endif
