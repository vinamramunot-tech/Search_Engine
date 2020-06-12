#include "webQuery.h"

/* Get user input for the words that will be queried.
    Then print the web pages that are most relevant
    to the queried words. */
void webQuery(struct listNode* urlHead) {
    char userInput[MAX_USER_INPUT];
    char *queryTerms[MAX_USER_INPUT];
    int isInputValid;
    int i, numOfTerms;

    while (1) { //iterate until user enters empty input
        do {
            printf("\n");
            getUserInput(userInput);
            numOfTerms = tokenizeUserInput(userInput, queryTerms);

            if (isUserInputEmpty(userInput))
                return; //return control back to main for clean up

            isInputValid = isUserInputValid(queryTerms, numOfTerms);

            if (!isInputValid) {
                fflush(stdout);
                fprintf(stderr, "Please enter a query containing only lower-case letters.\n");
            }

        } while (!isInputValid);

        //print query information
        printQueryTerms(queryTerms, numOfTerms);

        printf("IDF scores are\n");
        int documentCount = getAddrCount(urlHead, 0);
        int atLeastOneOccurenceInDocumentArr[numOfTerms];

        for (i = 0; i < numOfTerms; i++) {
            int atLeastOneOccurence = getAtLeastOneDocumentOccurrence(urlHead, queryTerms[i], 0);
            atLeastOneOccurenceInDocumentArr[i] = atLeastOneOccurence;
            printf("IDF(%s): %.7f\n", queryTerms[i], calcInverseDocumentFrequency(documentCount, atLeastOneOccurence));
        }

        printf("Web pages:\n");
        calcScore(urlHead, queryTerms, numOfTerms, atLeastOneOccurenceInDocumentArr, documentCount);
        sortList(urlHead);
        printAddressesWithScores(urlHead, 1);
    }
}

/* Reads input from stdin and stores them in
    the parameter char array. */
void getUserInput(char* userInput) {
    printf("Enter a web query: ");
    if(!fgets(userInput, MAX_USER_INPUT, stdin))
        userInput[0] = '\0';

    userInput[strlen(userInput) - 1] = '\0';
}

/* Returns a boolean value based on if user input
    is empty by seeing if the first character of the
    array is a null terminator. */
int isUserInputEmpty(char* userInput) {
    return userInput[0] == '\0';
}

/* Turn the buffer into an array of strings (stored in parameter
    queryTerms) where each element of the string array is
    a query term that was originally delimited with a space.
    Returns the number of strings in the array. */
int tokenizeUserInput(char* userInput, char** queryTerms) {
    char* delim = " \n";
    int numOfTerms = 0;
    int i = 0;
    char* token = strtok(userInput, delim);

    while (token != NULL) {
        queryTerms[i++] = token;
        token = strtok(NULL, delim);
        numOfTerms++;
    }

    return numOfTerms;
}

/* Returns a boolean value for true if the query terms
    are lower case and alphabetical letters, false if
    otherwise. */
int isUserInputValid(char** queryTerms, int numOfTerms) {
    int i, j;

    for (i = 0; i < numOfTerms; i++)
        for (j = 0; j < strlen(queryTerms[i]); j++)
            if (!islower(queryTerms[i][j]) || !isalpha(queryTerms[i][j]))
                return 0; //return false

    return 1; //return true
}

/* Print out all of the query terms. */
void printQueryTerms(char** queryTerms, int numOfTerms) {
    int i;

    printf("Query is \"");
    for (i = 0; i < numOfTerms; i++) {
        if (i == numOfTerms - 1)
            printf("%s", queryTerms[i]); //last word should not have a space after it
        else
            printf("%s ", queryTerms[i]);
    }
    printf("\".\n");
}

/* Calculates term frequency.
    https://en.wikipedia.org/wiki/Tf-idf */
double calcTermFrequency(int termTotal, int documentTermTotal) {
    return (double) termTotal / documentTermTotal;
}

/* Calculates inverse document frequency.
    https://en.wikipedia.org/wiki/Tf-idf */
double calcInverseDocumentFrequency(int documentCount, int atLeastOneOccurence) {
    return log((1.0 + documentCount) / (1.0 + atLeastOneOccurence));
}