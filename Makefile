CC = gcc
CFLAGS = -Wall -Wpedantic -ggdb -Werror
LDFLAGS = -lcurl
OBJS = crawler.o pagedir.o hashtable.o set.o bag.o curl.o url.o
EXEC = crawler

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

crawler.o: crawler.c crawler.h pagedir.h hashtable.h set.h bag.h curl.h url.h
pagedir.o: pagedir.c pagedir.h crawler.h
hashtable.o: hashtable.c hashtable.h set.h
set.o: set.c set.h
bag.o: bag.c bag.h
curl.o: curl.c curl.h
url.o: url.c url.h

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(EXEC)

.PHONY: all clean

