#include <stdio.h>
#include <stdint.h>
const uint8_t IP[8] = {1, 5, 2, 0, 3, 7, 4, 6}; 
const uint8_t IP_INV[8] = {3, 0, 2, 4, 6, 1, 7, 5}; 
const uint8_t EP[8] = {3, 0, 1, 2, 1, 2, 3, 0}; 
const uint8_t P4[4] = {1, 3, 2, 0}; 
const uint8_t P10[10] = {2, 4, 1, 6, 3, 9, 0, 8, 7, 5}; 
const uint8_t P8[8] = {5, 2, 6, 3, 7, 4, 9, 8}; 
const uint8_t S0[4][4] = { 
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};

const uint8_t S1[4][4] = { 
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};
uint8_t permute(uint8_t input, const uint8_t *perm, int n) {
    uint8_t output = 0;
    for (int i = 0; i < n; i++) {
        output |= ((input >> perm[i]) & 1) << i;
    }
    return output;
}
uint8_t left_rotate(uint8_t input, int n, int bits) {
    return ((input << n) | (input >> (bits - n))) & ((1 << bits) - 1);
}
uint8_t sbox_lookup(uint8_t input, const uint8_t sbox[4][4]) {
    int row = ((input & 0x8) >> 2) | (input & 0x1);
    int col = (input & 0x6) >> 1;
    return sbox[row][col];
}
uint8_t f_k(uint8_t input, uint8_t key) {
    uint8_t left = input >> 4;
    uint8_t right = input & 0xF;
    uint8_t ep_output = permute(right, EP, 8);
    uint8_t xor_output = ep_output ^ key;
    uint8_t s0_output = sbox_lookup(xor_output >> 4, S0);
    uint8_t s1_output = sbox_lookup(xor_output & 0xF, S1);
    uint8_t s_output = (s0_output << 2) | s1_output;
    s_output = permute(s_output, P4, 4);
    return (left ^ s_output) << 4 | right;
}
void generate_keys(uint8_t key, uint8_t *k1, uint8_t *k2) {
    uint8_t permuted_key = permute(key, P10, 10);
    uint8_t left = permuted_key >> 5;
    uint8_t right = permuted_key & 0x1F;
    left = left_rotate(left, 1, 5);
    right = left_rotate(right, 1, 5);
    uint8_t combined = (left << 5) | right;
    *k1 = permute(combined, P8, 8);
    left = left_rotate(left, 2, 5);
    right = left_rotate(right, 2, 5);
    combined = (left << 5) | right;
    *k2 = permute(combined, P8, 8);
}
uint8_t sdes_encrypt(uint8_t plaintext, uint8_t key) {
    uint8_t ip_output = permute(plaintext, IP, 8);
    uint8_t k1, k2;
    generate_keys(key, &k1, &k2);
    uint8_t fk1_output = f_k(ip_output, k1);
    uint8_t swapped = (fk1_output << 4) | (fk1_output >> 4);
    uint8_t fk2_output = f_k(swapped, k2);
    return permute(fk2_output, IP_INV, 8);
}
uint8_t sdes_decrypt(uint8_t ciphertext, uint8_t key) {
    uint8_t ip_output = permute(ciphertext, IP, 8);
    uint8_t k1, k2;
    generate_keys(key, &k1, &k2);
    uint8_t fk2_output = f_k(ip_output, k2);
    uint8_t swapped = (fk2_output << 4) | (fk2_output >> 4);
    uint8_t fk1_output = f_k(swapped, k1);

    return permute(fk1_output, IP_INV, 8);
}
void ctr_encrypt_decrypt(uint8_t *plaintext, uint8_t *output, uint8_t key, uint8_t counter, int length) {
    for (int i = 0; i < length; i++) {
        uint8_t keystream = sdes_encrypt(counter, key);
        output[i] = plaintext[i] ^ keystream;
        counter++;
    }
}
int main() {
    uint8_t plaintext[] = {0x01, 0x02, 0x04};
    uint8_t key = 0xFD; 
    uint8_t counter = 0x00;
    int length = sizeof(plaintext) / sizeof(plaintext[0]);
    uint8_t encrypted[length];
    uint8_t decrypted[length];
    printf("Plaintext: ");
    for (int i = 0; i < length; i++) {
        printf("%02X ", plaintext[i]);
    }
    printf("\n");
    ctr_encrypt_decrypt(plaintext, encrypted, key, counter, length);

    printf("Encrypted: ");
    for (int i = 0; i < length; i++) {
        printf("%02X ", encrypted[i]);
    }
    printf("\n");
    ctr_encrypt_decrypt(encrypted, decrypted, key, counter, length);

    printf("Decrypted: ");
    for (int i = 0; i < length; i++) {
        printf("%02X ", decrypted[i]);
    }
    printf("\n");
    return 0;
}

