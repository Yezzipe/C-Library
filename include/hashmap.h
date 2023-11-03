#pragma once

#include <stdbool.h>

typedef struct hashmap_bucket {
    bool tree;
    void* list;
    int size;
} hashmap_bucket_t;

typedef struct hashmap {
    int bucket_nb;
    int size;
    int init_size;
    unsigned long long int key_size;
    unsigned long long int value_size;
    hashmap_bucket_t* buckets;
} hashmap_t;

hashmap_t* consHashMap(unsigned long long int key_size, unsigned long long int value_size, int init_size);

void hashMapClear(hashmap_t* hashmap);

bool hashMapContainsKey(hashmap_t* hashmap, void* key);
bool hashMapContainsValue(hashmap_t* hashmap, void* value);

void* hashMapGet(hashmap_t* hashmap, void* key);

bool hashMapIsEmpty(hashmap_t* hashmap);

void hashMapPut(hashmap_t* hashmap, void* key, void* value);

void hashMapRemove(hashmap_t* hashmap, void* key);

int hashMapSize(hashmap_t* hashmap);
int hashMapBucketSize(hashmap_t* hashmap);

void hashMapFree(hashmap_t** hashmap);