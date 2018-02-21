#Ross Guju

#Makefile For Common Libray

# CS50 TSE


L = ../libcs50

# object files, and the target library
OBJS = word.o pagedir.o index.o

LIB = common.a
LLIBS = $L/libcs50.a
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L
CC = gcc
MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h
index.o: index.h $L/hashtable.h $L/set.h $L/counters.h $L/webpage.h  pagedir.h word.h 
word.o: word.h

.PHONY: clean sourcelist

# list all the sources and docs in this directory.
sourcelist:
	@echo Makefile *.md $(OBJS:.o=.[ch])

# clean up after our compilation
clean:
	rm -f $(LIB) *~ *.o
