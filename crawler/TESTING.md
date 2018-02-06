# CS50 Lab 3
## Ross Guju

### Testing 'crawler'

To test my *crawler* I wrote a test program, (crawler.c), to explore the capabilities of the *crawler* and to demonstrate the use of the *bag* *set* *hashtable* modules.

This test program assumes its stdin is from the commandline, and the testing cases include unit and integrating testing.

I first start out by testing *crawler* with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

Next I test the crawler with a `seedURL` that points to a non-existent server,  non-internal server, and a valid server but non-existent page.

Then I crawl a simple, closed set of cross-linked web pages. Ensuring that some page/s are mentioned multiple times within a page, and multiple times across the set of pages. Ensure there is a loop (a cycle in the graph of pages). 

I point the crawler at a page in that site, and explore at depths 0, 1, 2, 3, 4, 5. Verify that the files created match expectations.

Repeated with a different seed page in that same site. 

I pointed the crawler at our Wikipedia playground. Exploring at depths 0, 1, 2. 


### Test run

An example test run is in , built with two testing cases: stderr's and stdout.

	make
	make clean
	./crawler http://old-www.cs.dartmouth.edu/index.html ./data/ 2 &> ./crawlertest/crawlertest.err1.out


Then open the contents of crawlertest directory is were the stderr files printed and the command stdout for successful cases are in test cases header.

### Test Cases


./crawler http://old-www.cs.dartmouth.edu/index.html ./data r &> ./crawlertest/crawlertest.err1.out

./crawler https://www.google.com ./data 2 &> ./crawlertest/crawlertest.err2.out

./crawler http://old-www.cs.dartmouth.edu/index.html ./folder 2 &> ./crawlertest/crawlertest.err3.out

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./data -3 &> ./crawlertest/crawlertest.err4.out

./crawler rossguju ./data -3 &> ./crawlertest/crawlertest.err5.out

[flume:crawler] 313) ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./data 0
Page crawling: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
Crawler is done crawling

[flume:crawler] 322) ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./data 2
Page crawling: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
Not an internal page: https://en.wikipedia.org/wiki/Algorithm
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
Crawler is done crawling




[flume:crawler] 314) ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./data 2 
Page crawling: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
Not an internal page: https://en.wikipedia.org/wiki/Algorithm
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
Crawler is done crawling



[flume:crawler] 321) ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./data 3
Page crawling: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
Not an internal page: https://en.wikipedia.org/wiki/Algorithm
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
Not an internal page: https://en.wikipedia.org/wiki/Breadth-first_search
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/C.html
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/C.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/C.html
Crawler is done crawling





[flume:crawler] 318) ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./data 4
Page crawling: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
Not an internal page: https://en.wikipedia.org/wiki/Algorithm
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
Not an internal page: https://en.wikipedia.org/wiki/Breadth-first_search
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/C.html
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html
Not an internal page: https://en.wikipedia.org/wiki/ENIAC
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/F.html
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/G.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/G.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/G.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/F.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/F.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html
Not an internal page: https://en.wikipedia.org/wiki/Depth-first_search
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/C.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/C.html
Not an internal page: https://en.wikipedia.org/wiki/Computational_biology
Crawler is done crawling


[flume:crawler] 320) ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./data 5
Page crawling: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html
Not an internal page: https://en.wikipedia.org/wiki/Algorithm
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html
Not an internal page: https://en.wikipedia.org/wiki/Breadth-first_search
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/C.html
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html
Not an internal page: https://en.wikipedia.org/wiki/ENIAC
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/F.html
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/G.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/G.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/G.html
Not an internal page: https://en.wikipedia.org/wiki/Graph_traversal
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/F.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/F.html
Not an internal page: https://en.wikipedia.org/wiki/Fast_Fourier_transform
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html
Not an internal page: https://en.wikipedia.org/wiki/Depth-first_search
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/C.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/C.html
Not an internal page: https://en.wikipedia.org/wiki/Computational_biology
Crawler is done crawling



[flume:crawler] 323) ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ ./data 0
Page crawling: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
Crawler is done crawling

[flume:crawler] 324) ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ ./data 1 
Page crawling: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Computer_science.html
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/C_(programming_language).html
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Unix.html
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Dartmouth_College.html
new url added
http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Hash_table.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Hash_table.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Hash_table.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Dartmouth_College.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Dartmouth_College.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Unix.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Unix.html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/C_(programming_language).html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/C_(programming_language).html
Fetching contents: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Computer_science.html
Page Saving: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/wiki/Computer_science.html
Crawler is done crawling



