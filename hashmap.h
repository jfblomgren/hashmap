#ifndef _HASHMAP_H
#define _HASHMAP_H

#include <stdlib.h>
#include <stdbool.h>

typedef size_t (*HashFunction)(const void *key);
typedef bool (*ComparisonFunction)(const void *key1, const void *key2);
typedef struct Hashmap Hashmap;

/* Create a new hashmap.
 * Returns NULL if memory allocation fails.
 */
Hashmap *hashmap_new(size_t num_buckets,
                     HashFunction hash, ComparisonFunction compare);

/* Free the memory used by the hashmap.
 * NOTE: This does NOT free the memory used by the keys and values.
 */
void hashmap_free(Hashmap *hashmap);

/* Set the mapping for a given key.
 * Returns non-zero if memory allocation fails.
 * If old_value is not NULL, the old value of the mapping will be stored there.
 */
int hashmap_set(Hashmap *hashmap, const void *key,
                void *value, void **old_value);

/* Get the value for a given key.
 * Returns the value or NULL if no mapping exists for the key.
 */
void *hashmap_get(Hashmap *hashmap, const void *key);

/* Delete a mapping for a given key.
 * Returns the value of the deleted mapping or NULL if it doesn't exist.
 */
void *hashmap_delete(Hashmap *hashmap, const void *key);

#endif
