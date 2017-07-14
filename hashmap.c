#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"


static Pair *pair_new(const void *key, void *value);
static Pair **hashmap_get_bucket(Hashmap *hashmap, const void *key);
static Pair **hashmap_get_pair(Hashmap *hashmap, Pair **prev_pair,
                               const void *key);


Hashmap *hashmap_new(size_t num_buckets,
                     HashFunction hash, ComparisonFunction compare) {
    Hashmap *hashmap = malloc(sizeof(Hashmap));
    if (hashmap == NULL) {
        goto error_hashmap_alloc;
    }

    /* Initializing the buckets to NULL makes it easy to check if a bucket
     * is already in use.
     */
    hashmap->buckets = calloc(num_buckets, sizeof(Pair *));
    if (hashmap->buckets == NULL) {
        goto error_buckets_alloc;
    }

    hashmap->num_buckets = num_buckets;
    hashmap->hash = hash;
    hashmap->compare = compare;

    return hashmap;

error_buckets_alloc:
    free(hashmap);
error_hashmap_alloc:
    return NULL;
}

void hashmap_free(Hashmap *hashmap) {
    for (size_t i = 0; i < hashmap->num_buckets; i++) {
        Pair *pair = hashmap->buckets[i];
        Pair *prev_pair = NULL;
        while (pair != NULL) {
            prev_pair = pair;
            pair = pair->next;
            free(prev_pair);
        }
    }

    free(hashmap->buckets);
    free(hashmap);
}

Pair *hashmap_set(Hashmap *hashmap, const void *key, void *value) {
    Pair *prev_pair = NULL;
    Pair **pair = hashmap_get_pair(hashmap, &prev_pair, key);
    if (*pair == NULL) {
        *pair = pair_new(key, value);
        if (prev_pair != NULL) {
            prev_pair->next = *pair;
        }
    } else {
        (*pair)->value = value;
    }

    return *pair;
}

Pair *hashmap_get(Hashmap *hashmap, const void *key) {
    return *hashmap_get_pair(hashmap, NULL, key);
}

void hashmap_delete(Hashmap *hashmap, const void *key) {
    Pair *prev_pair = NULL;
    Pair **pair = hashmap_get_pair(hashmap, &prev_pair, key);
    if (*pair != NULL) {
        Pair *next_pair = (*pair)->next;
        free(*pair);
        if (prev_pair != NULL) {
            prev_pair->next = next_pair;
        } else {
             /* This was the first pair in this particular bucket, so the
              * pointer needs to be updated to the next pair (may be NULL).
             */
            *pair = next_pair;
        }
    }
}

static Pair **hashmap_get_bucket(Hashmap *hashmap, const void *key) {
    return hashmap->buckets + hashmap->hash(key) % hashmap->num_buckets;
}

static Pair **hashmap_get_pair(Hashmap *hashmap, Pair **prev_pair,
                               const void *key) {
    Pair **pair = hashmap_get_bucket(hashmap, key);
    while (*pair != NULL) {
        if (hashmap->compare((*pair)->key, key)) {
            break;
        } else if (prev_pair != NULL) {
            *prev_pair = *pair;
        }
        pair = &(*pair)->next;
    }

    return pair;
}

static Pair *pair_new(const void *key, void *value) {
    /* Since key is const, it'll have to be copied over from a struct that's
     * already initialized.
     */
    Pair init = {.key = key, .value = value, .next = NULL};
    Pair *pair = malloc(sizeof(Pair));
    if (pair != NULL) {
        memcpy(pair, &init, sizeof(Pair));
    }

    return pair;
}
