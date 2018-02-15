# CS50 TSE Winter

## INDEXER/INDEXERTEST Lab README.md

Ross Guju - winter 2018

README for indexer.c and indexertest.c

### Compiling/Make
You must be in the indexer directory and do the following to compile indexer and indexertest programs:

To build, run 'make all'

To clean up, run 'make clean'

#### Usage:
indexer.c - takes crawler output director and then stores the words into index data structure then prints it to a file.

usage: indexer must take crawler formated files: firstline-URL, secondline-depth and thirdline-HTML content. If the it is of this format, indexer prints an index of the words, document Id and frequency of each word in the document. 

 output: the output file will be the words, docId of the crawler filename and frequency of the words
FORMAT: word [docId count] [docId count] [docId count] [docId count] ..... 

stdin: 
    commandline arguments: ./indexer crawlerPATH indexfilenamePATH

 stdout:
    index status for arguements, ie only will show if there are errors
    

 stderr: prints if arguments failed

 
indexertest.c - takes a old index file that follows the index format then puts it into an inverted index data structure, then prints the new index into a file

 usage: checking that indexer program worked

 output:
 	a new index file that is the same as the input index.
 stdin: 
 	commandline arguments: ./indexertest oldIndexFilenamePATH newIndexFilenamePATH
 stdout:
 	none
 stderr: prints if arguments fail

 ###Example command lines:

TESTING indexer 
TEST 1:
`make all`

`pwd`-> /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer

`./indexer /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/crawler-output-small  /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/SMALL_TEST`->`SMALL_TEST`

`cat SMALL_TEST`-> *shows the index for crawler-output-small*

TEST 2:
`./indexer /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/crawler-output  /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/BIG_TEST`->`BIG_TEST`

`cat BIG_TEST`-> *shows the index for crawler-output*

TESTING USING indexertest program
TEST 1:
`cd indexTester`

then in the command line:


`./indexertest /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/indexTester/SMALL_TEST /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/indexTester/invertSMALL_TEST`

outputs the file-> invertSMALL_TEST

then

`diff SMALL_TEST invertSMALL_TEST `

TEST 2:

`./indexertest /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/indexTester/BIG_TEST /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/indexTester/invertBIG_TEST`

`diff BIG_TEST invertBIG_TEST `


###### Assumptions:

1. crawler directory exists and follows the crawler format
2. indexfilename directory exists so we can make the file for indexertest
3. crawler files are ordered and names such as: 1 2 3 4 5.....

