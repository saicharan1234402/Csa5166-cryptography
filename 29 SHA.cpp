#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#define NUM_LANES 25
#define LANE_SIZE 64
#define RATE_LANES 16
#define CAPACITY_LANES 9
int allNonZero(uint64_t *capacityLanes) {
    for (int i = 0; i < CAPACITY_LANES; i++) {
        if (capacityLanes[i] == 0)
            return 0;
    }
    return 1;
}
void setBits(uint64_t *capacityLanes) {
    for (int i = 0; i < CAPACITY_LANES; i++) {
        capacityLanes[i] |= ((uint64_t)rand() << 32) | rand();
    }
}
int main() {
    uint64_t state[NUM_LANES] = {0};
    srand(time(NULL));
    for (int i = 0; i < RATE_LANES; i++) {
        state[i] = ((uint64_t)rand() << 32) | rand();
    }
    uint64_t capacityLanes[CAPACITY_LANES] = {0};
    int rounds = 0;
    while (!allNonZero(capacityLanes)) {
        setBits(capacityLanes);
        rounds++;
    }
    printf("Number of rounds required: %d\n", rounds);

    return 0;
}

