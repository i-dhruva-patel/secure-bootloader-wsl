#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>

int verify_signature(const char *pubkey_path,
                     const unsigned char *data, size_t data_len,
                     const unsigned char *sig, size_t sig_len) {
    FILE *fp = fopen(pubkey_path, "r");
    if (!fp) {
        perror("fopen");
        return 0;
    }

    EVP_PKEY *pubkey = PEM_read_PUBKEY(fp, NULL, NULL, NULL);
    fclose(fp);

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_PKEY_CTX *pctx = NULL;

    if (!EVP_DigestVerifyInit(ctx, &pctx, EVP_sha256(), NULL, pubkey)) return 0;
    if (!EVP_DigestVerifyUpdate(ctx, data, data_len)) return 0;
    
    int result = EVP_DigestVerifyFinal(ctx, sig, sig_len);

    EVP_MD_CTX_free(ctx);
    EVP_PKEY_free(pubkey);

    return result == 1;
}
