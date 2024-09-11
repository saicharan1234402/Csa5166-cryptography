#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define CONST_64  0x1B
#define CONST_128 0x87
void left_shift(uint8_t *key, int len) {
    int carry = 0;
    for (int i = len - 1; i >= 0; i--) {
        int next_carry = key[i] & 0x80 ? 1 : 0;
        key[i] = (key[i] << 1) | carry;
        carry = next_carry;
    }
}
void generate_subkeys(uint8_t *L, uint8_t *K1, uint8_t *K2, int block_size) {
    int len = block_size / 8;
    uint8_t C;
    if (block_size == 64) {
        C = CONST_64;
    } else if (block_size == 128) {
        C = CONST_128;
    } else {
        printf("Unsupported block size!\n");
        return;
    }
    memcpy(K1, L, len);
    left_shift(K1, len);
    if (L[0] & 0x80) {
        K1[len - 1] ^= C;
    }
    memcpy(K2, K1, len);
    left_shift(K2, len);
    if (K1[0] & 0x80) {
        K2[len - 1] ^= C;
    }
}
int main() {
    uint8_t L_64[8] = {0};
    uint8_t K1_64[8], K2_64[8];
    uint8_t L_128[16] = {0}; 
    uint8_t K1_128[16], K2_128[16];
    printf("Generating subkeys for 64-bit block size:\n");
    generate_subkeys(L_64, K1_64, K2_64, 64);
    printf("K1: ");
    for (int i = 0; i < 8; i++) {
        printf("%02x ", K1_64[i]);
    }
    printf("\nK2: ");
    for (int i = 0; i < 8; i++) {
        printf("%02x ", K2_64[i]);
    }
    printf("\n");
    printf("Generating subkeys for 128-bit block size:\n");
    generate_subkeys(L_128, K1_128, K2_128, 128);
    printf("K1: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x ", K1_128[i]);
    }
    printf("\nK2: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x ", K2_128[i]);
    }
    printf("\n");
    return 0;
}

