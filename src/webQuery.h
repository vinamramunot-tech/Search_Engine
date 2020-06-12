#ifndef WEB_QUERY_H_
#define WEB_QUERY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "crawler.h"

#define MAX_USER_INPUT 1000

struct listNode;
void webQuery(struct listNode* urlHead);
void getUserInput(char* userInput);
int isUserInputEmpty(char* userInput);
int tokenizeUserInput(char* userInput, char** queryTerms);
int isUserInputValid(char** queryTerms, int numOfWords);
void printQueryTerms(char** queryTerms, int numOfWords);
double calcTermFrequency(int termTotal, int documentTermTotal);
double calcInverseDocumentFrequency(int documentCount, int atLeastOneOccurence);

#endif