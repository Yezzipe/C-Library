#include <stdlib.h>
#include <string.h>
#include "tools.h"

int maxInt(int a, int b) {
    return a >= b ? a : b;
}

int memcmp_fast(const void* p1, const void* p2, size_t size) {
    if (size > 4) {
        int d = *(int*)p1 - *(int*)p2;
        if (d) return d;
    }
    return memcmp(p1, p2, size);
}