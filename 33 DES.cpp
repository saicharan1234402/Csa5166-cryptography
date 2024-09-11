#include <stdio.h>
#include <stdint.h>
#include <string.h>

static const uint8_t S_BOX[4][16] = {
    {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
    {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
    {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
    {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
};
static const uint8_t PERM_TABLE[32] = {
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};
static const uint8_t IP[64] = { 
};
static const uint8_t FP[64] = { 
};

uint32_t feistel(uint32_t half_block, uint64_t round_key) {
    uint32_t substituted = 0;
    for (int i = 0; i < 8; i++) {
        uint8_t row = (half_block >> (28 - i * 4)) & 0xF;
        substituted |= S_BOX[i % 4][row] << (28 - i * 4);
    }
    uint32_t permuted = 0;
    for (int i = 0; i < 32; i++) {
        permuted |= ((substituted >> (31 - i)) & 1) << (31 - PERM_TABLE[i]);
    }
    return permuted ^ (uint32_t)(round_key >> 32);
}
void des(uint64_t input, uint64_t *output, uint64_t key, int encrypt) {
    uint32_t left = input >> 32;
    uint32_t right = input & 0xFFFFFFFF;
    for (int round = 0; round < 16; round++) {
        uint64_t round_key = (key << round) | (key >> (56 - round));
        uint32_t temp = right;
        right = left ^ feistel(right, round_key);
        left = temp;
    }
    *output = ((uint64_t)right << 32) | left;
}
int main() {
    uint64_t plaintext = 0x0123456789ABCDEF;
    uint64_t key = 0x133457799BBCDFF1;
    uint64_t ciphertext, decryptedtext;
    des(plaintext, &ciphertext, key, 1);
    printf("Ciphertext: %016llX\n", ciphertext);
    des(ciphertext, &decryptedtext, key, 0);
    printf("Decrypted text: %016llX\n", decryptedtext);
    return 0;
}

