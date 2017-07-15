#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "hashmap.h"

#define NUM_BUCKETS 16
#define NUM_STUDENTS 4
#define MAX_NAME_LEN 16

/* The keys need to be hashed with a self-supplied function. */
size_t hash(const void *key) {
    size_t hash = 0;
    for (size_t i = 0; i < strlen(key); i++) {
        hash = 31 * hash + *((char *) (key + i));
    }

    return hash;
}

/* Comparison functions must return true if the keys are equal,
 * false otherwise.
 */
bool compare(const void *key1, const void *key2) {
    return strcmp(key1, key2) == 0;
}

int main(void) {
    /* Create a new hashmap. */
    Hashmap *hashmap = hashmap_new(NUM_BUCKETS, hash, compare);

    char student_names[NUM_STUDENTS][MAX_NAME_LEN] = {
        "Alice",
        "April",
        "Bob",
        "John"
    };
    char student_grades[NUM_STUDENTS] = {'A', 'C', 'F', 'B'};

    /* Put the key-value pairs into the hashmap. */
    for (int i = 0; i < NUM_STUDENTS; i++) {
        hashmap_set(hashmap, student_names[i], &student_grades[i], NULL);
    }

    /* Print the grades of the students. */
    for (int i = 0; i < NUM_STUDENTS; i++) {
        printf("%s has a grade of %c.\n", student_names[i],
               *(char *) hashmap_get(hashmap, student_names[i]));
    }

    /* Delete John from the hashmap. */
    hashmap_delete(hashmap, "John");
    assert(hashmap_get(hashmap, "John") == NULL);

    hashmap_free(hashmap);

    return 0;
}
