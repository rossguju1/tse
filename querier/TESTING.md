## TESTING- querier 
Ross Guju
CS50


To test my *querier* program, [querier](querier.c) You can run `make` either in the tse directory or in the /tse/querier directory.

Once compiled do the following for tests:

*pwd* to get your current directory.
For Example:
./querier /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/crawler-output-small  /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/SMALL_TEST

where */net/ifs-users/rguju/cs50/labs/lab4/tse/querier/* is your pwd

Also, you must be in the /querier directory to run ./querier since the program is compiled here. 

This will prompt the following in stdin: 
[flume:querier] 126) ./querier /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/crawler-output-small  /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/SMALL_TEST
KEY WORDS:> 

You can now search for words in the crawler-output-small.

For example:
[flume:querier] 126) ./querier /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/crawler-output-small  /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/SMALL_TEST
KEY WORDS:> makes or language and guided or spaces
Query: makes or language and guided or spaces
Matches 6 documents (Ranked) 
Score: 7 Doc: 2 ~>  http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Linked_list.html
Score: 3 Doc: 4 ~>  http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Dartmouth_College.html
Score: 3 Doc: 6 ~>  http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/C_(programming_language).html
Score: 2 Doc: 3 ~>  http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Hash_table.html
Score: 2 Doc: 5 ~>  http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Unix.html
Score: 1 Doc: 7 ~>  http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Computer_science.html
KEY WORDS:> 


You should also do this test for crawler-output:

Command Line input:
./querier /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/crawler-output  /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/BIG_TEST

Output:
[flume:querier] 127) ./querier /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/crawler-output  /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/BIG_TEST
KEY WORDS:> 


You must be in the /querier directory to run ./querier since the program is compiled here. 

## Note:

*RunTests* directory in the querier directory contains the crawler-output directories for small and big sizes and SMALL_TEST and BIG_TEST are the index files when my ./indexer formated the crawler directories.

You can use these to test the functionality of ./querier program.

The directory *TEST* contains output test files when I ran ./querier program myself. 


## TESTING EXAMPLES (cases):

These are specific test cases for verify that `querier` program satisfies specs.

TEST 1:

[flume:querier] 142) ./querier /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/crawler-output  /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/BIG_TEST 
KEY WORDS:> and
Errpr: 1st word cant be an operator
Error: Last word is an operator
KEY WORDS:>

TEST 2:

KEY WORDS:> and and
Errpr: 1st word cant be an operator
Error: adjacent operators
KEY WORDS:> 

TEST 3:

KEY WORDS:> or and
Errpr: 1st word cant be an operator
Error: adjacent operators
KEY WORDS:> 

TEST 4:

KEY WORDS:> or or
Errpr: 1st word cant be an operator
Error: adjacent operators
KEY WORDS:> 

TEST 5:

KEY WORDS:> work or college -
Not a valid character: -
KEY WORDS:> 

TEST 6:

KEY WORDS:> word or
Error: Last word is an operator
KEY WORDS:> 

TEST 7:

KEY WORDS:> college or word and
Error: Last word is an operator
KEY WORDS:> 

`cat SMALL_TEST`-> *shows the index for crawler-output-small*

TEST 8: 

[flume:querier] 144) ./querier /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/  /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/BIG
file at '/net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/BIG' is not readable

TEST 9: 

[flume:querier] 145) ./querier /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunT  /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/BIG_TEST 
Arg1 is a not an existing  directory 
After compiling as directed above:

TEST 10:

[flume:querier] 174) ./querier /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/ crawler-output-small  /net/ifs-users/rguju/cs50/labs/lab4/tse/querier/RunTests/SMALL_TEST
Needs two arguments

