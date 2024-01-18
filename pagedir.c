#define CRAWLER_FILE ".crawler"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "pagedir.h"
#include "crawler.h"

bool pagedir_init(const char *pageDirectory) {
	// Fill in with your implementation
    struct stat st = {0};

    if (stat(pageDirectory, &st) == -1) {
        fprintf(stderr, "Error: Directory %s does not exist\n", pageDirectory);
        return false;
    }

    if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "Error: %s is not a directory\n", pageDirectory);
        return false;
    }

    char filepath[256];
    sprintf(filepath, "%s/%s", pageDirectory, CRAWLER_FILE);
    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to create file %s in directory %s\n", CRAWLER_FILE, pageDirectory);
        return false;
    }

    fclose(file);
    return true;
}

void pagedir_save(const webpage_t *page, const char *pageDirectory, const int documentID) {
	// Fill in with your implementation
   char filepath[256];
    sprintf(filepath, "%s/%d", pageDirectory, documentID);
    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        fprintf(stderr, "Unable to write to file: %s\n", filepath);
        exit(4);
    }

    if (fprintf(file, "%s\n%d\n%s", page->url, page->depth, page->html) < 0) {
        fprintf(stderr, "Error writing to file: %s\n", filepath);
        fclose(file);
        exit(4);
    }

    fclose(file);
}


