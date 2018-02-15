## CS50 Lab 5

## TESTING- indexer 


To test my *indexer* program, [indexer](indexer.c) YOU MUST go to the indexer directory, then type `make all` into the command line. This makes the indexer and indexerTest programs.

Once compiled do the following for tests:

pwd to get your current directory.

./indexer /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/crawler-output-small  /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/SMALL_TEST

where */net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/* is your pwd

This will make the file *SMALL_TEST*: which is the index file for 
*crawler-output-small* file.

You should also do the test:
./indexer /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/crawler-output  /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/BIG_TEST

which makes the BIG_TEST file with the index of the *crawler-output* file

## TESTING EXAMPLES:
format: `input`->`output`

TEST 1:

You must be in the indexer directory:

`make all`

`pwd`-> /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer

`./indexer /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/crawler-output-small  /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/SMALL_TEST`->`SMALL_TEST`

`cat SMALL_TEST`-> *shows the index for crawler-output-small*

TEST 2:

After compiling as directed above:

`./indexer /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/crawler-output  /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/TEST_BIG`->`TEST_BIG`

`cat TEST_BIG`-> *shows the index for crawler-output*

## TESTING indexertest program

From the indexer directory go to *indexTest* directory by `cd indexTester` 

I have moved the *SMALL_TEST* and *BIG_TEST* to this directory.

now do the following:

`cd indexTester`

then in the command line:

`./indexertest /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/indexTester/SMALL_TEST /net/ifs-users/rguju/cs50/labs/lab4/tse/indexer/indexTester/invertSMALL_TEST`

outputs the file-> invertSMALL_TEST

then

`diff SMALL_TEST invertSMALL_TEST `

outputs->

if the two files are the same *diff* outputs nothing to the command line.

follow the same process but instead of SMALL_TEST, use BIG_TEST.












