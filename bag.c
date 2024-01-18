#include <stdlib.h>
#include "bag.h"

typedef struct bagnode {
    void *item;
    struct bagnode *next;
} bagnode_t;

struct bag {
    bagnode_t *head;
    bagnode_t *tail;
};

bag_t *bag_new(void) {
    bag_t *bag = malloc(sizeof(bag_t));
    if (bag == NULL) {
        return NULL;
    }
    bag->head = NULL;
    bag->tail = NULL;
    return bag;
}

void bag_insert(bag_t *bag, void *item) {
    if (bag == NULL || item == NULL) {
        return;
    }
    bagnode_t *newnode = malloc(sizeof(bagnode_t));
    newnode->item = item;
    newnode->next = NULL;

    if (bag->head == NULL) {
        bag->head = newnode;
        bag->tail = newnode;
    } else {
        bag->tail->next = newnode;
        bag->tail = newnode;
    }
}

void *bag_extract(bag_t *bag) {
    if (bag == NULL || bag->head == NULL) {
        return NULL;
    }
    bagnode_t *node = bag->head;
    void *item = node->item;

    if (bag->head == bag->tail) {
        bag->head = NULL;
        bag->tail = NULL;
    } else {
        bag->head = bag->head->next;
    }

    free(node);
    return item;
}

bool bag_is_empty(const bag_t *bag) {
    return bag == NULL || bag->head == NULL;
}

void bag_delete(bag_t *bag, void (*itemdelete)(void *item)) {
    if (bag == NULL) {
        return;
    }
    while (!bag_is_empty(bag)) {
        void *item = bag_extract(bag);
        if (itemdelete != NULL) {
            (*itemdelete)(item);
        }
    }
    free(bag);
}

bool bag_remove(bag_t *bag, void *item, void (*itemdelete)(void *item), int (*itemcompare)(const void *item1, const void *item2)) {
    if (bag == NULL || item == NULL) {
        return false;
    }

    bagnode_t *current = bag->head;
    bagnode_t *previous = NULL;

    while (current != NULL) {
        if (itemcompare(current->item, item) == 0) {
            if (previous == NULL) {
                bag->head = current->next;
            } else {
                previous->next = current->next;
            }

            if (itemdelete != NULL) {
                (*itemdelete)(current->item);
            }
            free(current);
            return true;
        }
        previous = current;
        current = current->next;
    }

    return false;
}


