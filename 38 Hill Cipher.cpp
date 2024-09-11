#include <stdio.h>
#include <stdlib.h>

#define MATRIX_SIZE 2
#define ALPHABET_SIZE 26
#define TEXT_SIZE (MATRIX_SIZE * MATRIX_SIZE) // Size for plaintext and ciphertext strings

// Helper function to convert a character to its numeric value (0-25)
int char_to_index(char c) {
    return c - 'A';
}

// Helper function to convert a numeric value (0-25) to its character
char index_to_char(int i) {
    return i + 'A';
}

// Calculate the determinant of a 2x2 matrix
int determinant(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

// Calculate the inverse of a 2x2 matrix modulo 26
void inverse(int matrix[MATRIX_SIZE][MATRIX_SIZE], int inv_matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int det = determinant(matrix);
    int det_inv = -1;

    // Find modular inverse of determinant
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if ((det * i) % ALPHABET_SIZE == 1) {
            det_inv = i;
            break;
        }
    }

    if (det_inv == -1) {
        printf("Matrix is not invertible modulo 26.\n");
        exit(1);
    }

    inv_matrix[0][0] = (matrix[1][1] * det_inv) % ALPHABET_SIZE;
    inv_matrix[0][1] = (-matrix[0][1] * det_inv) % ALPHABET_SIZE;
    inv_matrix[1][0] = (-matrix[1][0] * det_inv) % ALPHABET_SIZE;
    inv_matrix[1][1] = (matrix[0][0] * det_inv) % ALPHABET_SIZE;

    // Ensure values are positive
    if (inv_matrix[0][0] < 0) inv_matrix[0][0] += ALPHABET_SIZE;
    if (inv_matrix[0][1] < 0) inv_matrix[0][1] += ALPHABET_SIZE;
    if (inv_matrix[1][0] < 0) inv_matrix[1][0] += ALPHABET_SIZE;
    if (inv_matrix[1][1] < 0) inv_matrix[1][1] += ALPHABET_SIZE;
}

// Matrix multiplication modulo 26
void matrix_mult_mod26(int A[MATRIX_SIZE][MATRIX_SIZE], int B[MATRIX_SIZE][MATRIX_SIZE], int result[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                result[i][j] = (result[i][j] + A[i][k] * B[k][j]) % ALPHABET_SIZE;
            }
        }
    }
}

// Solve for the key matrix using known plaintext and ciphertext pairs
void find_key_matrix(const char *plaintext, const char *ciphertext) {
    int plaintext_matrix[MATRIX_SIZE][MATRIX_SIZE];
    int ciphertext_matrix[MATRIX_SIZE][MATRIX_SIZE];
    int key_matrix[MATRIX_SIZE][MATRIX_SIZE];
    int plaintext_inv[MATRIX_SIZE][MATRIX_SIZE];

    // Form the matrix from the plaintext and ciphertext
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            plaintext_matrix[i][j] = char_to_index(plaintext[i * MATRIX_SIZE + j]);
            ciphertext_matrix[i][j] = char_to_index(ciphertext[i * MATRIX_SIZE + j]);
        }
    }

    // Compute the inverse of the plaintext matrix
    inverse(plaintext_matrix, plaintext_inv);
    
    // Compute the key matrix as: key_matrix = ciphertext_matrix * plaintext_matrix_inv
    matrix_mult_mod26(ciphertext_matrix, plaintext_inv, key_matrix);
    
    printf("Key matrix:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%c ", index_to_char(key_matrix[i][j]));
        }
        printf("\n");
    }
}

int main() {
    // Example plaintext and ciphertext pairs
    char plaintext[TEXT_SIZE + 1] = "GHIJ";  // Ensure the size matches exactly
    char ciphertext[TEXT_SIZE + 1] = "WXYZ"; // Ensure the size matches exactly

    // Null-terminate the strings
    plaintext[TEXT_SIZE] = '\0';
    ciphertext[TEXT_SIZE] = '\0';

    find_key_matrix(plaintext, ciphertext);

    return 0;
}

