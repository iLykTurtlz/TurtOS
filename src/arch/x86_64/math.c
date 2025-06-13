#include "math.h"

uint64_t min(uint64_t x, uint64_t y) {
    return x < y ? x : y;
}

uint64_t max(uint64_t x, uint64_t y) {
    return x > y ? x : y;
}

uint64_t pow_base_2(int exponent) {
    uint64_t res = 1;
    for (int i=0; i<exponent; i++) {
        res *= 2;
    }
    return res;
}
