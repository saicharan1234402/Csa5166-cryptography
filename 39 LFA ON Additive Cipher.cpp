#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define ALPHABET_SIZE 26
#define TOP_N 10

// Frequencies of letters in English language (approximate)
const double english_freq[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 
    0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.317, 9.056, 
    2.758, 0.978, 2.560, 0.150, 1.929, 0.074
};

// Compute letter frequency in text
void compute_freq(const char *text, double freq[ALPHABET_SIZE]) {
    int counts[ALPHABET_SIZE] = {0};
    int total = 0;

    for (const char *ptr = text; *ptr; ptr++) {
        if (isalpha(*ptr)) {
            char ch = tolower(*ptr);
            counts[ch - 'a']++;
            total++;
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freq[i] = (total > 0) ? (counts[i] / (double)total) * 100 : 0.0;
    }
}

// Compute similarity score between two frequency distributions
double compute_score(const double freq1[ALPHABET_SIZE], const double freq2[ALPHABET_SIZE]) {
    double score = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        score -= fabs(freq1[i] - freq2[i]);  // Lower difference = higher similarity
    }
    return score;
}

// Decrypt ciphertext with a given shift
void decrypt_with_shift(const char *ciphertext, int shift, char *plaintext) {
    for (int i = 0; ciphertext[i]; i++) {
        if (isalpha(ciphertext[i])) {
            char base = islower(ciphertext[i]) ? 'a' : 'A';
            plaintext[i] = (ciphertext[i] - base - shift + ALPHABET_SIZE) % ALPHABET_SIZE + base;
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
}

// Perform frequency analysis and output top N plaintexts
void frequency_attack(const char *ciphertext) {
    double ciphertext_freq[ALPHABET_SIZE];
    compute_freq(ciphertext, ciphertext_freq);

    char possible_plaintexts[ALPHABET_SIZE][100];
    double scores[ALPHABET_SIZE];
    int count = 0;

    for (int shift = 0; shift < ALPHABET_SIZE; shift++) {
        char plaintext[100] = {0};
        decrypt_with_shift(ciphertext, shift, plaintext);

        double plaintext_freq[ALPHABET_SIZE];
        compute_freq(plaintext, plaintext_freq);

        scores[count] = compute_score(ciphertext_freq, plaintext_freq);
        sprintf(possible_plaintexts[count], "%s", plaintext);
        count++;
    }

    // Sort plaintexts by score
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (scores[j] > scores[i]) {
                // Swap scores
                double temp_score = scores[i];
                scores[i] = scores[j];
                scores[j] = temp_score;

                // Swap plaintexts
                char temp_text[100];
                strcpy(temp_text, possible_plaintexts[i]);
                strcpy(possible_plaintexts[i], possible_plaintexts[j]);
                strcpy(possible_plaintexts[j], temp_text);
            }
        }
    }

    // Output top N plaintexts
    int top_n = TOP_N;
    printf("Top %d possible plaintexts:\n", top_n);
    for (int i = 0; i < top_n && i < count; i++) {
        printf("%d: %s\n", i + 1, possible_plaintexts[i]);
    }
}

int main() {
    char ciphertext[1000];
    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = 0;  // Remove newline character

    frequency_attack(ciphertext);

    return 0;
}

