CC = gcc
CFLAGS = -g -Wall
OBJS = crawler.o indexPage.o webQuery.o

all: webSearch

webSearch: webSearch.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $< -o $@ -lm

%.o : %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o webSearch
