#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "indexPage.h"
#include "crawler.h"

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Invalid number of command line arguments");
        return -1;
    }

    char* fileName = argv[1];
    int MAX_N = atoi(argv[2]);
    long int seed = atol(argv[3]);
    srand(seed);

    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not create file (%s).\n", fileName);
        return -2;
    }

    printf("Indexing...\n");
    struct listNode* urlHead = malloc(sizeof(struct listNode));
    if (urlHead == NULL) {
        fprintf(stderr, "Could not allocate space.\n");
        return -3;
    }

    webCrawl(file, urlHead, MAX_N);

    webQuery(urlHead);

    destroyList(urlHead);
    fclose(file);

    printf("Exiting the program\n");

    return 0;
}