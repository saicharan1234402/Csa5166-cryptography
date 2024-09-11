#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int IP[8] = {1, 5, 2, 0, 3, 7, 4, 6};
int IP_inv[8] = {3, 0, 2, 4, 6, 1, 7, 5};
int EP[8] = {3, 0, 1, 2, 1, 2, 3, 0};
int P4[4] = {1, 3, 2, 0};
int P10[10] = {2, 4, 1, 6, 3, 9, 0, 8, 7, 5};
int P8[8] = {5, 2, 6, 3, 7, 4, 9, 8};
int S0[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};
int S1[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};

void permute(int* input, int* output, int* table, int n) {
    for (int i = 0; i < n; i++) {
        output[i] = input[table[i]];
    }
}

void generate_keys(int* key, int* k1, int* k2) {
    int p10[10], left[5], right[5], temp[5];

    permute(key, p10, P10, 10);

    for (int i = 0; i < 5; i++) {
        left[i] = p10[i];
        right[i] = p10[i + 5];
    }
    for (int i = 0; i < 2; i++) {
        temp[i] = left[i];
        left[i] = left[i + 3];
        left[i + 3] = temp[i];
    }
    for (int i = 0; i < 2; i++) {
        temp[i] = right[i];
        right[i] = right[i + 3];
        right[i + 3] = temp[i];
    }

    for (int i = 0; i < 5; i++) {
        p10[i] = left[i];
        p10[i + 5] = right[i];
    }

    permute(p10, k1, P8, 8);

    for (int i = 0; i < 2; i++) {
        temp[i] = left[i];
        left[i] = left[i + 2];
        left[i + 2] = left[i + 3];
        left[i + 3] = temp[i];
    }
    for (int i = 0; i < 2; i++) {
        temp[i] = right[i];
        right[i] = right[i + 2];
        right[i + 2] = right[i + 3];
        right[i + 3] = temp[i];
    }

    for (int i = 0; i < 5; i++) {
        p10[i] = left[i];
        p10[i + 5] = right[i];
    }

    permute(p10, k2, P8, 8);
}

void sbox(int* input, int* output, int box[4][4]) {
    int row = input[0] * 2 + input[3];
    int col = input[1] * 2 + input[2];
    int val = box[row][col];
    output[0] = val / 2;
    output[1] = val % 2;
}

void f(int* input, int* k, int* output) {
    int ep[8], xored[8], left[4], right[4], sbox_output[4];

    permute(input + 4, ep, EP, 8);

    for (int i = 0; i < 8; i++) {
        xored[i] = ep[i] ^ k[i];
    }

    for (int i = 0; i < 4; i++) {
        left[i] = xored[i];
        right[i] = xored[i + 4];
    }

    sbox(left, sbox_output, S0);
    sbox(right, sbox_output + 2, S1);

    permute(sbox_output, sbox_output, P4, 4);

    for (int i = 0; i < 4; i++) {
        output[i] = input[i] ^ sbox_output[i];
    }
}

void sdes_encrypt(int* plaintext, int* key, int* ciphertext) {
    int ip[8], temp[8], k1[8], k2[8], temp2[8];

    generate_keys(key, k1, k2);
    permute(plaintext, ip, IP, 8);

    f(ip, k1, temp);
    for (int i = 0; i < 4; i++) {
        temp[i + 4] = ip[i];
        temp[i] = ip[i + 4];
    }

    f(temp, k2, temp2);
    for (int i = 0; i < 4; i++) {
        temp2[i + 4] = temp[i];
        temp2[i] = temp[i + 4];
    }

    permute(temp2, ciphertext, IP_inv, 8);
}

void sdes_decrypt(int* ciphertext, int* key, int* plaintext) {
    int ip[8], temp[8], k1[8], k2[8], temp2[8];

    generate_keys(key, k1, k2);
    permute(ciphertext, ip, IP, 8);

    f(ip, k2, temp);
    for (int i = 0; i < 4; i++) {
        temp[i + 4] = ip[i];
        temp[i] = ip[i + 4];
    }

    f(temp, k1, temp2);
    for (int i = 0; i < 4; i++) {
        temp2[i + 4] = temp[i];
        temp2[i] = temp[i + 4];
    }

    permute(temp2, plaintext, IP_inv, 8);
}

void cbc_encrypt(int* plaintext, int* key, int* iv, int* ciphertext, int length) {
    int block[8], prev_block[8];

    memcpy(prev_block, iv, 8 * sizeof(int));

    for (int i = 0; i < length; i += 8) {
        for (int j = 0; j < 8; j++) {
            block[j] = plaintext[i + j] ^ prev_block[j];
        }

        sdes_encrypt(block, key, ciphertext + i);

        memcpy(prev_block, ciphertext + i, 8 * sizeof(int));
    }
}

void cbc_decrypt(int* ciphertext, int* key, int* iv, int* plaintext, int length) {
    int block[8], prev_block[8], decrypted_block[8];

    memcpy(prev_block, iv, 8 * sizeof(int));

    for (int i = 0; i < length; i += 8) {
        sdes_decrypt(ciphertext + i, key, decrypted_block);

        for (int j = 0; j < 8; j++) {
            plaintext[i + j] = decrypted_block[j] ^ prev_block[j];
        }

        memcpy(prev_block, ciphertext + i, 8 * sizeof(int));
    }
}

void print_bits(int* bits, int length) {
    for (int i = 0; i < length; i++) {
        printf("%d", bits[i]);
    }
    printf("\n");
}

int main() {
    int key[10] = {0, 1, 1, 1, 1, 1, 1, 0, 1, 1};
    int iv[8] = {1, 0, 1, 0, 1, 0, 1, 0};
    int plaintext[16] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0};
    int expected_ciphertext[16] = {1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1};
    int ciphertext[16];
    int decryptedtext[16];

    printf("Plaintext: ");
    print_bits(plaintext, 16);

    cbc_encrypt(plaintext, key, iv, ciphertext, 16);

    printf("Ciphertext: ");
    print_bits(ciphertext, 16);

    cbc_decrypt(ciphertext, key, iv, decryptedtext, 16);

    printf("Decrypted text: ");
    print_bits(decryptedtext, 16);

    return 0;
}

