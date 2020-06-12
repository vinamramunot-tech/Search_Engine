#ifndef INDEX_PAGE_H_
#define INDEX_PAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26 //size of alphabet
#define MAX_CHARACTERS 300000 //max amount of characters the program can handle per web page

struct trie {
  char character; //character of a word
  int timesVisited; //how many times a word was visited
  int isEndOfTerm; //boolean

  struct trie *children[ALPHABET_SIZE];
};

struct trie* indexPage(const char* url, int* pTotalNumOfTerms);
void printTermFromBuffer(char* buffer, int startIndex, int endIndex);
int addTermOccurrence(struct trie* root, const char* word);
void printTrieContents(const struct trie* root, int level, char* word);
int destroyTrie(struct trie* root);
int getText(const char* srcAddr, char* buffer, const int bufSize);
struct trie* createNode(void);
int getTermOccurence(struct trie* root, char* word, int level);

#endif