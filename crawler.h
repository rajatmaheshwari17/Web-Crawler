#ifndef CRAWLER_H
#define CRAWLER_H
#include <stddef.h>
#include "hashtable.h"
#include "bag.h"

typedef struct {
	char *url;
	char *html;
	size_t length;
	int depth;
} webpage_t;

#endif

