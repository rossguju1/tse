# cs50 Crawler IMPLEMENTATION.MD

Ross Guju

## crawler.c layout
1. `main()` tests arguments from the command line
2. the argument checking goes: no more than 3 arguments; checks if pageDirectory is a directory; checks if depth is a integer; and that the depth is non-negative.
3. Negative values imply that there were errors and a message is sent to stderr.
4. the arguments are passedf to crawler.
5. `crawler()` does most of the work; it initializes the `bag` and `hashtable` and creates the seedURL page
6. `pagefetcher()` is essentially `webpage_fetch()`
7. `pagesaver()` makes the documents and puts them in the passed argument directory. 
8. `pagescanner()` is essentiallu `webpage_getNextUrl()` 
9. The `crawler()` first checks that seedURL is internal and then fetches the page.
10. There are two loops that implement the `crawler()`; checking the current page called `page_to_crawl` and then checks if there are any internal links. If there are internal links, another loops checks and saves the links in the *bag*. 
11. *hashtable* is used to store the already seen webpages.
