#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const float english_freq[26] = {
    0.082, 0.015, 0.028, 0.043, 0.127, 0.022, 0.020, 0.061, 0.070,
    0.002, 0.008, 0.040, 0.024, 0.067, 0.075, 0.019, 0.001, 0.060,
    0.063, 0.091, 0.028, 0.010, 0.023, 0.001, 0.020, 0.001
};

void calculate_frequency(const char *text, float *freq) {
    int count[26] = {0};
    int total = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            count[text[i] - 'A']++;
            total++;
        } else if (text[i] >= 'a' && text[i] <= 'z') {
            count[text[i] - 'a']++;
            total++;
        }
    }
    
    for (int i = 0; i < 26; i++) {
        freq[i] = (float)count[i] / total;
    }
}

void sort_indices(const float *freq, int *indices) {
    for (int i = 0; i < 26; i++) {
        indices[i] = i;
    }
    
    for (int i = 0; i < 25; i++) {
        for (int j = i + 1; j < 26; j++) {
            if (freq[indices[i]] < freq[indices[j]]) {
                int temp = indices[i];
                indices[i] = indices[j];
                indices[j] = temp;
            }
        }
    }
}

void generate_possible_plaintexts(const char *ciphertext, const int *sorted_indices, const int *english_sorted_indices, char *plaintext) {
    int map[26];
    
    for (int i = 0; i < 26; i++) {
        map[sorted_indices[i]] = english_sorted_indices[i];
    }
    
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            plaintext[i] = map[ciphertext[i] - 'A'] + 'A';
        } else if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') {
            plaintext[i] = map[ciphertext[i] - 'a'] + 'a';
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[strlen(ciphertext)] = '\0';
}

int main() {
    char ciphertext[256];
    float freq[26];
    int sorted_indices[26];
    int english_sorted_indices[26];
    int top_n;
    
    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    if (ciphertext[strlen(ciphertext) - 1] == '\n') {
        ciphertext[strlen(ciphertext) - 1] = '\0';
    }

    printf("Enter the number of top possible plaintexts to generate: ");
    scanf("%d", &top_n);
    
    calculate_frequency(ciphertext, freq);
    
    sort_indices(freq, sorted_indices);
    
    sort_indices(english_freq, english_sorted_indices);
    
    for (int i = 0; i < top_n; i++) {
        char plaintext[256];
        generate_possible_plaintexts(ciphertext, sorted_indices, english_sorted_indices, plaintext);
        printf("Possible plaintext %d: %s\n", i + 1, plaintext);
        int temp = english_sorted_indices[25];
        for (int j = 25; j > 0; j--) {
            english_sorted_indices[j] = english_sorted_indices[j - 1];
        }
        english_sorted_indices[0] = temp;
    }
    
    return 0;
}

