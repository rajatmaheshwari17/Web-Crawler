#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <unistd.h>
#include "crawler.h"
#include "curl.h"
#include "url.h"
#include "hashtable.h"
#include "bag.h"
#include "pagedir.h"

static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen);

char *nextURL(const char *html, int *position) {
    if (!html || !position) {
        return NULL;
    }

    const char *tagStart, *hrefStart, *hrefEnd;
    char *url = NULL;
    char quoteChar;

    while ((tagStart = strstr(html + *position, "<a ")) != NULL) {
        hrefStart = strstr(tagStart, "href=");
        if (!hrefStart) {
            *position = tagStart - html + 1;
            continue;
        }

        hrefStart += 5;
        while (isspace(*hrefStart)) {
            hrefStart++;
        }

        if (*hrefStart == '\"' || *hrefStart == '\'') {
            quoteChar = *hrefStart;
            hrefStart++;
        } else {
            *position = hrefStart - html;
            continue;
        }

        hrefEnd = strchr(hrefStart, quoteChar);
        if (!hrefEnd) {
            break;
        }

        if (*hrefStart == '#') {
            *position = hrefEnd - html + 1;
            continue;
        }

        size_t urlLength = hrefEnd - hrefStart;
        url = malloc(urlLength + 1);
        if (url) {
            strncpy(url, hrefStart, urlLength);
            url[urlLength] = '\0';
            *position = hrefEnd - html + 1;
            return url;
        }

        *position = hrefStart - html;
    }
    return url;
}

/**
 * Parses command-line arguments, placing the corresponding values into the pointer arguments seedURL,
 * pageDirectory and maxDepth. argc and argv should be passed in from the main function.
 */
static void parseArgs(const int argc, char *argv[], char **seedURL, char **pageDirectory, int *maxDepth) {
    if (argc != 4) {
        fprintf(stderr, "Usage: ./crawler seedURL pageDirectory maxDepth\n");
        exit(1);
    }

    *seedURL = strdup(argv[1]);
    *pageDirectory = strdup(argv[2]);

    *maxDepth = atoi(argv[3]);
    if (*maxDepth < 0 || *maxDepth > 10) {
        fprintf(stderr, "Error: maxDepth must be in the range [0..10].\n");
        free(*seedURL);
        free(*pageDirectory);
        exit(2);
    }
}

/**
 * Crawls webpages given a seed URL, a page directory and a max depth.
 */
 static void crawl(char *seedURL, char *pageDirectory, const int maxDepth) {
    hashtable_t *pagesSeen = hashtable_new(100);
    if (!pagesSeen) {
        fprintf(stderr, "Error: Failed to create hashtable.\n");
        exit(4);
    }

    bag_t *pagesToCrawl = bag_new();
    if (!pagesToCrawl) {
        fprintf(stderr, "Error: Failed to create bag.\n");
        hashtable_delete(pagesSeen, NULL);
        exit(5);
    }

    if (!pagedir_init(pageDirectory)) {
        fprintf(stderr, "Error: Failed to initialize page directory.\n");
        bag_delete(pagesToCrawl, NULL);
        hashtable_delete(pagesSeen, NULL);
        exit(3);
    }

    webpage_t *seedPage = malloc(sizeof(webpage_t));
    if (!seedPage) {
        fprintf(stderr, "Error: Failed to allocate memory for seed page.\n");
        bag_delete(pagesToCrawl, NULL);
        hashtable_delete(pagesSeen, NULL);
        exit(6);
    }

    seedPage->url = strdup(seedURL);
    seedPage->depth = 0;
    seedPage->html = NULL;
    bag_insert(pagesToCrawl, seedPage);
    hashtable_insert(pagesSeen, seedURL, seedPage);

    int documentID = 0;

    while (!bag_is_empty(pagesToCrawl)) {
        //sleep(1);
        webpage_t *currentPage = bag_extract(pagesToCrawl);
        char *html = download(currentPage->url, &(currentPage->length));

        if (html != NULL && currentPage->length > 0) {
            currentPage->html = html;
            printf("%d Fetched: %s\n", currentPage->depth, currentPage->url);
            pagedir_save(currentPage, pageDirectory, ++documentID);
            if (currentPage->depth < maxDepth) {
                pageScan(currentPage, pagesToCrawl, pagesSeen);
            }
        } else {
            fprintf(stderr, "Error: Failed to download page at URL: %s\n", currentPage->url);
        }
        
        free(currentPage->url);
        free(currentPage->html);
        free(currentPage);
        sleep(1);
    }

    free(seedURL);
    free(pageDirectory);

    bag_delete(pagesToCrawl, NULL);
    hashtable_delete(pagesSeen, NULL);
}

/**
 * Scans a webpage for URLs.
 */

static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen) {
    printf("%d Scanning: %s\n", page->depth, page->url);
    printf("%s\n",page->url);
    const char *htmlContent = page->html;
    int pos = 0;
    char *url = nextURL(htmlContent, &pos);

    while (url != NULL) {
        char *normalizedUrl = normalizeURL(page->url, url);

        if (normalizedUrl != NULL) {
            if (isInternalURL(page->url, normalizedUrl)) {
                if (!hashtable_find(pagesSeen, normalizedUrl)) {
                    printf("%d Found: %s\n", page->depth, normalizedUrl);
                    printf("%d Added: %s\n", page->depth, normalizedUrl);
                    //hashtable_insert(pagesSeen, normalizedUrl, normalizedUrl);
                    if (!hashtable_insert(pagesSeen, normalizedUrl, normalizedUrl)) {
                        free(normalizedUrl);
                    } else{
                        //printf("%d Added: %s\n", page->depth, normalizedUrl);
                        webpage_t *newPage = malloc(sizeof(webpage_t));
                        newPage->url = normalizedUrl;
                        newPage->depth = page->depth + 1;
                        newPage->html = NULL;
                        bag_insert(pagesToCrawl, newPage);
                }
                } else {
                    printf("%d IgnDupl: %s\n", page->depth, normalizedUrl);
                    free(normalizedUrl);
                }
            } else {
                printf("%d IgnExtrn: %s\n", page->depth, normalizedUrl);
                free(normalizedUrl);
            }
        }
        free(url);
        url = nextURL(htmlContent, &pos);
    }
    
}

int main(const int argc, char *argv[]) {
    char *seedURL, *pageDirectory;
    int maxDepth;

   
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    pagedir_init(pageDirectory);
    crawl(seedURL, pageDirectory, maxDepth);

    return 0;
}


