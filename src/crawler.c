#include "crawler.h"

/* Returns a boolean value for true if parameter linked list,
    pNode contains the parameter addr, otherwise false. */
int contains(const struct listNode *pNode, const char *addr) {
    if (pNode == NULL)
        return 0;
    else if (strncmp(pNode->addr, addr, MAX_ADDR_LENGTH) == 0)
        return 1;

    return contains(pNode->next, addr);
}

/* Insert a node with an address, addr at the end of the linked
    list, pNode. */
void insertBack(struct listNode* pNode, const char* addr, struct trie* root, int* pTotalNumOfTerms) {
    if (pNode == NULL)
      return;

    if (pNode->next == NULL) {
        struct listNode *newNode = malloc(sizeof(struct listNode));
        if (newNode == NULL)
            return;

        newNode->next = NULL;
        strncpy(newNode->addr, addr, MAX_ADDR_LENGTH);
        newNode->trieRoot = root;
        newNode->score = 0;
        newNode->termCount = *pTotalNumOfTerms;

        pNode->next = newNode;

        return;
    }

    insertBack(pNode->next, addr, root, pTotalNumOfTerms);
}

/* Print the first 3 addresses and their respective scores to the console. */
void printAddressesWithScores(const struct listNode *pNode, int level) {
    if (pNode == NULL || level > 3)
        return;

    if (fabs(pNode->score) > 0.000001) { //close enough estimate
        printf("%d. %s (score: %.4f)\n", level, pNode->addr, pNode->score);

        printAddressesWithScores(pNode->next, level + 1);
    }
}

/* Returns the number of nodes in the linked list. */
int getAddrCount(struct listNode* pNode, int addrCount) {
    if (pNode == NULL)
        return addrCount;

    return getAddrCount(pNode->next, addrCount + 1);
}

/* Returns the number of times that a term occurs at least once across all web
    pages. */
int getAtLeastOneDocumentOccurrence(struct listNode* pNode, char* term, int count) {
    if (pNode == NULL || pNode->trieRoot == NULL)
        return count;

    int termCount = getTermOccurence(pNode->trieRoot, term, 0);
    if (termCount > 0)
        count++;

    return getAtLeastOneDocumentOccurrence(pNode->next, term, count);
}

/* Calculate the score for a web page. This is done by finding the tf-idf value
    for each term and summing them together, the score for the linked list node
    is then set to this value. */
void calcScore(struct listNode* pNode, char** queryTerms, int numOfTerms, int* atLeastOneOccurenceInDocumentArr, int documentCount) {
    if (pNode == NULL)
        return;

    double score = 0;
    int i;
    for (i = 0; i < numOfTerms; i++) {
        int documentTermTotal = pNode->termCount;
        int termTotal = getTermOccurence(pNode->trieRoot, queryTerms[i], 0);

        double tf = calcTermFrequency(termTotal, documentTermTotal);
        double idf = calcInverseDocumentFrequency(documentCount, atLeastOneOccurenceInDocumentArr[i]);
        double tfIdf = tf * idf;
        score += tfIdf;
    }

    pNode->score = score;

    calcScore(pNode->next, queryTerms, numOfTerms, atLeastOneOccurenceInDocumentArr, documentCount);
}

/* Bubble sort a linked list based on score in descending order. */
void sortList(struct listNode* pNode) {
    if (pNode == NULL)
        return;

    struct listNode* i;
    struct listNode* j;

    for (i = pNode; i->next != NULL; i = i->next)
        for (j = i->next; j != NULL; j = j->next)
            if (i->score < j->score)
                swapNodes(i, j);
}

/* Swap linked list node propeties. */
void swapNodes(struct listNode* pNode1, struct listNode* pNode2) {
    double tempScore;
    int tempTermCount;
    char tempAddr[MAX_ADDR_LENGTH];
    struct trie* tempTrie;

    tempScore = pNode1->score;
    tempTrie = pNode1->trieRoot;
    strncpy(tempAddr, pNode1->addr, MAX_ADDR_LENGTH);
    tempTermCount = pNode1->termCount;

    pNode1->score = pNode2->score;
    pNode1->trieRoot = pNode2->trieRoot;
    strncpy(pNode1->addr, pNode2->addr, MAX_ADDR_LENGTH);
    pNode1->termCount = pNode2->termCount;

    pNode2->score = tempScore;
    pNode2->trieRoot = tempTrie;
    strncpy(pNode2->addr, tempAddr, MAX_ADDR_LENGTH);
    pNode2->termCount = tempTermCount;
}

/* Free each node and trie for a linked list. */
void destroyList(struct listNode* pNode) {
    if (pNode != NULL) {
        destroyList(pNode->next);
        destroyTrie(pNode->trieRoot);
        free(pNode);
    }
}

/* Search the file for a link to a web page and the maximum number of
    links on that page that can also be indexed, then crawl that web page. */
void webCrawl(FILE* file, struct listNode* urlHead, int MAX_N) {
    int n = 0;
    int hopNum = 0;
    int hopsLimit = 0;
    char url[MAX_ADDR_LENGTH];
    //initialize urlHead
    strcpy(urlHead->addr, "");
    urlHead->next = NULL;

    while (fscanf(file, "%s %d", url, &hopsLimit) == 2 && n < MAX_N) {
        hopNum = 0;

        while(1) {
            if (!contains(urlHead, url)) {
                int totalNumOfTerms = 0;
                struct trie* trieRoot = indexPage(url, &totalNumOfTerms);
                n++;

                if (n == 1) { //manually initialize first node
                    urlHead->trieRoot = trieRoot;
                    urlHead->termCount = totalNumOfTerms;
                    urlHead->score = 0;
                    urlHead->next = NULL;
                    strncpy(urlHead->addr, url, MAX_ADDR_LENGTH);
                } else {
                  insertBack(urlHead, url, trieRoot, &totalNumOfTerms);
                }
            }

            hopNum++;

            if (hopNum <= hopsLimit && n < MAX_N) {
                int isNewUrl = getLink(url, url, MAX_ADDR_LENGTH);

                if (!isNewUrl)
                    break;
            } else {
                break;
            }
        }
    }
}

int getLink(const char* srcAddr, char* link, const int maxLinkLength) {
    const int bufSize = 1000;
    char buffer[bufSize];

    int numLinks = 0;

    FILE *pipe;

    snprintf(buffer, bufSize, "curl -s \"%s\" | python getLinks.py", srcAddr);

    pipe = popen(buffer, "r");
    if(pipe == NULL) {
        fprintf(stderr, "ERROR: could not open the pipe for command %s\n", buffer);
        return 0;
    }

    fscanf(pipe, "%d\n", &numLinks);

    if(numLinks > 0) {
        int linkNum;
        double r = (double) rand() / ( (double) RAND_MAX + 1.0);

        for(linkNum = 0; linkNum < numLinks; linkNum++) {
            fgets(buffer, bufSize, pipe);

            if(r < (linkNum + 1.0) / numLinks)
		        break;
        }

        //copy the address from buffer to link
        strncpy(link, buffer, maxLinkLength);
        link[maxLinkLength - 1] = '\0';

        //get rid of the newline
        char* pNewline = strchr(link, '\n');
        if(pNewline != NULL) {
	        *pNewline = '\0';
        }
    }

  pclose(pipe);

  if(numLinks > 0)
    return 1;
  else
    return 0;
}