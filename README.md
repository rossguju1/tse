# CS50 Tiny Search Engine

Ross Guju - winter 2018

To build, run `make`.

To clean up, run `make clean`.

The CS50 playground is in:
http://old-www.cs.dartmouth.edu/~cs50/data/tse

There are 3 componenets to the TSE: Crawler, Indexer and Querier.

The `crawler` walks through an initial seed url and goes to the websites linked pages and collects the url, depth number and html contents.

The `indexer` goes through the crawler output page directory and creates an index that lists the words [documentId frequency] ..... in the crawler directory.

The `querier` takes the `crawler` and `indexer` output as arguments and asks for user input for what key words to search. The source, count, doc Id and url are returned to the terminal.

*FOR MORE INFORMATION*
See the README's in the subdirectories
