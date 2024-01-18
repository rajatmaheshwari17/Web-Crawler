#include "set.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

set_t *set_new(void) {
    set_t *set = malloc(sizeof(set_t));
    if (set == NULL) {
        return NULL;
    }
    set->head = NULL;
    return set;
}

bool set_insert(set_t *set, const char *key, void *item) {
    if (set == NULL || key == NULL || item == NULL) {
        return false;
    }

    for (setnode_t *node = set->head; node != NULL; node = node->next) {
        if (strcmp(node->key, key) == 0) {
            return false;
        }
    }

    setnode_t *new_node = malloc(sizeof(setnode_t));
    if (new_node == NULL) {
        return false;
    }

    new_node->key = strdup(key);
    if (new_node->key == NULL) {
        free(new_node);
        return false;
    }

    new_node->item = item;
    new_node->next = set->head;
    set->head = new_node;

    return true;
}

void *set_find(set_t *set, const char *key) {
    if (set == NULL || key == NULL) {
        return NULL;
    }

    for (setnode_t *node = set->head; node != NULL; node = node->next) {
        if (strcmp(node->key, key) == 0) {
            return node->item;
        }
    }

    return NULL;
}

void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (fp != NULL) {
        if (set == NULL) {
            fputs("(null)\n", fp);
        } else {
            for (setnode_t *node = set->head; node != NULL; node = node->next) {
                if (itemprint != NULL) {
                    (*itemprint)(fp, node->key, node->item);
                }
            }
        }
    }
}

void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (set != NULL && itemfunc != NULL) {
        for (setnode_t *node = set->head; node != NULL; node = node->next) {
            (*itemfunc)(arg, node->key, node->item);
        }
    }
}

void set_delete(set_t *set, void (*itemdelete)(void *item)) {
     if (set != NULL) {
        setnode_t *node = set->head;
        while (node != NULL) {
            setnode_t *next = node->next;

            if (itemdelete != NULL) {
                (*itemdelete)(node->item);
            }

            free(node->key);
            free(node);
            
            node = next;
        }
        free(set);
    }
}


bool set_remove(set_t *set, const char *key, void (*itemdelete)(void *item)) {
    if (set == NULL || key == NULL) {
        return false;
    }

    setnode_t *current = set->head;
    setnode_t *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (previous == NULL) {
                set->head = current->next;
            } else {
                previous->next = current->next;
            }
            if (itemdelete != NULL) {
                (*itemdelete)(current->item);
            }

            free(current->key);
            free(current);

            return true;
        }
        previous = current;
        current = current->next;
    }
    
    return false;
}

