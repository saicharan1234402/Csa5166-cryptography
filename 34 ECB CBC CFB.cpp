#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BLOCK_SIZE 8
void xor_blocks(uint8_t *a, uint8_t *b, uint8_t *out, int len) {
    for (int i = 0; i < len; i++) {
        out[i] = a[i] ^ b[i];
    }
}
void block_cipher(uint8_t *input, uint8_t *output, uint8_t *key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        output[i] = input[i] ^ key[i % 8];
    }
}
void ecb_encrypt(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *key, int length) {
    for (int i = 0; i < length; i += BLOCK_SIZE) {
        block_cipher(plaintext + i, ciphertext + i, key);
    }
}
void cbc_encrypt(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *key, uint8_t *iv, int length) {
    uint8_t block[BLOCK_SIZE];
    for (int i = 0; i < length; i += BLOCK_SIZE) {
        xor_blocks(plaintext + i, iv, block, BLOCK_SIZE);
        block_cipher(block, ciphertext + i, key);
        memcpy(iv, ciphertext + i, BLOCK_SIZE);
    }
}
void cfb_encrypt(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *key, uint8_t *iv, int length) {
    uint8_t block[BLOCK_SIZE];
    for (int i = 0; i < length; i += BLOCK_SIZE) {
        block_cipher(iv, block, key);
        xor_blocks(plaintext + i, block, ciphertext + i, BLOCK_SIZE);
        memcpy(iv, ciphertext + i, BLOCK_SIZE);
    }
}
void pad_buffer(uint8_t *buffer, int length) {
    buffer[length] = 0x80;
    for (int i = length + 1; i < BLOCK_SIZE; i++) {
        buffer[i] = 0x00;
    }
}
int main() {
    uint8_t key[BLOCK_SIZE] = {0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F, 0x7A, 0x8B};
    uint8_t iv[BLOCK_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t plaintext[BLOCK_SIZE] = "HELLO";
    uint8_t padded_plaintext[BLOCK_SIZE];
    memcpy(padded_plaintext, plaintext, 5);
    pad_buffer(padded_plaintext, 5);
    uint8_t ciphertext[BLOCK_SIZE];
        ecb_encrypt(padded_plaintext, ciphertext, key, BLOCK_SIZE);
    printf("ECB Ciphertext: ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");
    memcpy(iv, (uint8_t[BLOCK_SIZE]){0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, BLOCK_SIZE);
    cbc_encrypt(padded_plaintext, ciphertext, key, iv, BLOCK_SIZE);
    printf("CBC Ciphertext: ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");
    memcpy(iv, (uint8_t[BLOCK_SIZE]){0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, BLOCK_SIZE);
    cfb_encrypt(padded_plaintext, ciphertext, key, iv, BLOCK_SIZE);
    printf("CFB Ciphertext: ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");

    return 0;
}

