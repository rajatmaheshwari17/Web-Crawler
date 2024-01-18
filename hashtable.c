#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "set.h"

hashtable_t *hashtable_new(const int num_slots) {
     (void)hash_function("", num_slots);
    hashtable_t *ht = malloc(sizeof(hashtable_t));
    if (ht == NULL) {
        return NULL;
    }

    ht->num_slots = num_slots;
    ht->slots = malloc(sizeof(set_t*) * num_slots);
    if (ht->slots == NULL) {
        free(ht);
        return NULL;
    }

    for (int i = 0; i < num_slots; i++) {
        ht->slots[i] = set_new();
        if (ht->slots[i] == NULL) {
            for (int j = 0; j < i; j++) {
                set_delete(ht->slots[j], NULL);
            }
            free(ht->slots);
            free(ht);
            return NULL;
        }
    }

    return ht;
}

bool hashtable_insert(hashtable_t *ht, const char *key, void *item) {
    if (ht == NULL || key == NULL || item == NULL) {
        return false;
    }

    int slot = hash_function(key, ht->num_slots);
    void *existing_item = set_find(ht->slots[slot], key);

    char *key_copy = strdup(key);
    if (key_copy == NULL) {
        free(key_copy);
        return false;
    }
    if (existing_item != NULL) {
        free(key_copy);
        return false;
    }

    if (!set_insert(ht->slots[slot], key_copy, item)) {
        free(key_copy);
        return false;
    }
    free(key_copy);
    return true;
}


void *hashtable_find(hashtable_t *ht, const char *key) {
    if (ht == NULL || key == NULL) {
        return NULL;
    }

    int slot = hash_function(key, ht->num_slots);
    return set_find(ht->slots[slot], key);
}

void hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (fp == NULL || ht == NULL) {
        return;
    }

    for (int i = 0; i < ht->num_slots; i++) {
        set_print(ht->slots[i], fp, itemprint);
    }
}

void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (ht == NULL || itemfunc == NULL) {
        return;
    }

    for (int i = 0; i < ht->num_slots; i++) {
        set_iterate(ht->slots[i], arg, itemfunc);
    }
}

void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item)) {
    if (ht == NULL) {
        return;
    }

    for (int i = 0; i < ht->num_slots; i++) {
        set_delete(ht->slots[i], itemdelete);
    }

    free(ht->slots);
    free(ht);
}

int hash_function(const char *key, int num_slots) {
    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;

    return hash % num_slots;
}

bool hashtable_remove(hashtable_t *ht, const char *key, void (*itemdelete)(void *item)) {
    if (ht == NULL || key == NULL) {
        return false;
    }

    int slot = hash_function(key, ht->num_slots);

    return set_remove(ht->slots[slot], key, itemdelete);
}

