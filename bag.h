#ifndef BAG_H
#define BAG_H
#include <stdbool.h>

typedef struct bag bag_t;

bag_t *bag_new(void);

void bag_insert(bag_t *bag, void *item);

void *bag_extract(bag_t *bag);

bool bag_is_empty(const bag_t *bag);

void bag_delete(bag_t *bag, void (*itemdelete)(void *item));

bool bag_remove(bag_t *bag, void *item, void (*itemdelete)(void *item), int (*itemcompare)(const void *item1, const void *item2));

#endif // BAG_H

