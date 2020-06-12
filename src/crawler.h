#ifndef CRAWLER_H_
#define CRAWLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "indexPage.h"
#include "webQuery.h"

#define MAX_ADDR_LENGTH 1000

//Linked list node used to keep track of web page information
struct listNode {
  char addr[MAX_ADDR_LENGTH];
  int termCount;
  double score;

  struct trie* trieRoot;
  struct listNode* next;
};

int getLink(const char* srcAddr, char* link, const int maxLinkLength);
int contains(const struct listNode *pNode, const char *addr);
void insertBack(struct listNode* pNode, const char* addr, struct trie* root, int* pTotalNumOfTerms);
void printAddressesWithScores(const struct listNode *pNode, int level);
int getAddrCount(struct listNode* pNode, int addrCount);
int getAtLeastOneDocumentOccurrence(struct listNode* pNode, char* term, int count);
void calcScore(struct listNode* pNode, char** queryTerms, int numOfTerms, int* atLeastOneOccurenceInDocumentArr, int documentCount);
void sortList(struct listNode* pNode);
void swapNodes(struct listNode* pNode1, struct listNode* pNode2);
void destroyList(struct listNode *pNode);
void webCrawl(FILE* file, struct listNode* urlHead, int MAX_N);

#endif