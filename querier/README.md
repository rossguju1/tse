# CS50 TSE Winter

## Querier Lab README.md

Ross Guju - winter 2018

README for querier.c

### Compiling/Make
To build, run 'make' in parent or querier directory

To clean up, run 'make clean'


#### Usage:
querier.c - program to match words using crawler directory and an index file

usage: loading the program from commandline with the 1st argument being the path to crawler directory and the 2nd argument being the path to index filename, you can then input words for the querier to match. 

 output: Martched documents and their scores of frequencies ranked.

FORMAT: 
Matches [number] documents (Ranked) 
Score: [number] Doc: [number] ~>  *Document Url*

stdin: 
    commandline arguments: ./querier crawlerPATH indexfilenamePATH

 stdout:
Matches [number] documents (Ranked) 
Score: [number] Doc: [number] ~>  *Document Url*
   

 stderr: Mainly if memory allocation fails or validing tokens/characters/arguments fails.


 ##### Example command lines:

run `make` in tse directory or querier directory 

`pwd`-> /net/ifs-users/rguju/cs50/labs/lab4/tse/querier

`./querier /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/crawler-output-small  /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/SMALL_TEST`

Output:
[flume:querier] 152) ./querier /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/crawler-output-small  /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/SMALL_TEST
KEY WORDS:> makes or spaces
Query: makes or spaces
Matches 5 documents (Ranked) 
Score: 6 Doc: 2 ~>  http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Linked_list.html
Score: 2 Doc: 4 ~>  http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Dartmouth_College.html
Score: 2 Doc: 6 ~>  http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/C_(programming_language).html
Score: 1 Doc: 3 ~>  http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Hash_table.html
Score: 1 Doc: 5 ~>  http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Unix.html
KEY WORDS:> 

##### Note:
*RunTests* directory in the querier directory contains the crawler-output directories for small and big sizes and SMALL_TEST and BIG_TEST are the index files when my ./indexer formated the crawler directories.

You can use these to test the functionality of ./querier program.

The directory *TEST* contains output test files when I ran ./querier program myself. 

###### Assumptions:

1. crawler directory exists and follows the crawler format
2. indexfilename directory exists so we can make the file for indexertest
3. crawler files are ordered and names such as: 1 2 3 4 5.....

