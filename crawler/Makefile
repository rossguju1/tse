# Makefile for crawler
#   Builds and compiles all these utility crawler
#
# Ross Guju

# object files, and the target library
PROG = crawler

OBJS = crawler.o ../libcs50/bag.o ../libcs50/file.o ../libcs50/hashtable.o ../libcs50/jhash.o ../libcs50/memory.o ../libcs50/set.o ../libcs50/webpage.o

LIB =

# add -DNOSLEEP to disable the automatic sleep after web-page fetches
#               (students, please do not use -DNOSLEEP!)
# add -DMEMTEST for memory tracking report in indexer
# (and run `make clean; make` whenever you change this)
FLAGS = # -DNOSLEEP # -DMEMTEST 

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I../common -I../libcs50
CC = gcc
MAKE = make


# Build the library by archiving object files
$(PROG): $(OBJS)
		$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# Dependencies: object files depend on header files
crawler.o: ../libcs50/webpage.h ../libcs50/hashtable.h ../libcs50/bag.h ../libcs50/memory.h
bag.o: ../libcs50/bag.h ../libcs50/memory.h
file.o: ../libcs50/file.h
hashtable.o: ../libcs50/hashtable.h ../libcs50/set.h ../libcs50/jhash.h ../libcs50/memory.h  
jhash.o: ../libcs50/jhash.h
memory.o: ../libcs50/memory.h
set.o: ../libcs50/set.h ../libcs50/memory.h
webpage.o:  ../libcs50/file.h ../libcs50/webpage.h ../libcs50/memory.h

.PHONY: test clean

test: $(PROG)
		./$(PROG)

# clean up after our compilation
clean:
	rm -f *~ *.o
