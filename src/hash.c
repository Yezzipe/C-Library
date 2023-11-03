#include "hash.h"

int hashCode(void* value, unsigned long long int size) {
    unsigned char* a = value;
    unsigned int hash = 7;
    unsigned int b = 0;
    for (unsigned long long int i = 0; i < size; ++i) {
        b = b | (a[i] << ((i&3)<<3));
        if ((i&3) == 3 || i + 1 == size) {
            hash = (hash << 5) - hash + b;
            b = 0;
        }
    }
    return hash;
}