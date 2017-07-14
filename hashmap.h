#ifndef _HASHMAP_H
#define _HASHMAP_H

#include <stdlib.h>
#include <stdbool.h>


typedef size_t (*HashFunction)(const void *key);
typedef bool (*ComparisonFunction)(const void *key1, const void *key2);

typedef struct Pair {
    const void *key;
    void *value;
    struct Pair *next;
} Pair;

typedef struct {
    size_t num_buckets;
    Pair **buckets;
    HashFunction hash;
    ComparisonFunction compare;
} Hashmap;


/* Create a new hashmap.
 * Returns NULL if memory allocation fails.
 */
Hashmap *hashmap_new(size_t num_buckets,
                     HashFunction hash, ComparisonFunction compare);

/* Free all memory used by the hashmap. */
void hashmap_free(Hashmap *hashmap);

/* Set the mapping for a given key.
 * Overwrites the current mapping if it already exists and returns NULL if
 * memory allocation fails.
 */
Pair *hashmap_set(Hashmap *hashmap, const void *key, void *value);

/* Get the mapping for a given key.
 * Returns NULL if no mapping exists for the key.
 */
Pair *hashmap_get(Hashmap *hashmap, const void *key);

/* Delete a mapping for a given key. */
void hashmap_delete(Hashmap *hashmap, const void *key);

#endif
