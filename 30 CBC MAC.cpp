#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
void printHex(const unsigned char *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}
void xorBlocks(unsigned char *out, const unsigned char *a, const unsigned char *b, int len) {
    for (int i = 0; i < len; i++) {
        out[i] = a[i] ^ b[i];
    }
}
int main() {
    AES_KEY encKey;
    const unsigned char key[16] = "thisisakey123456"; 
    const unsigned char X[16] = "thisisamessagebl";
    unsigned char T[16]; 
    unsigned char X_xor_T[16];
    unsigned char Y[32]; 
    unsigned char T2[16];
    AES_set_encrypt_key(key, 128, &encKey);
    AES_encrypt(X, T, &encKey);
    printf("MAC of one-block message X: ");
    printHex(T, 16);
    xorBlocks(X_xor_T, X, T, 16);
    memcpy(Y, X, 16);
    memcpy(Y + 16, X_xor_T, 16);
    unsigned char temp[16];
    AES_encrypt(Y, temp, &encKey); 
    xorBlocks(temp, temp, Y + 16, 16);
    AES_encrypt(temp, T2, &encKey);
    printf("MAC of two-block message X || (X ? T): ");
    printHex(T2, 16);
    return 0;
}

