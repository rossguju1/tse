/* 
 * crawler -  crawls a website and retrieves webpages starting with a specified URL. 
 * It parses the initial webpage, extracts any embedded URLs and retrieves those pages, and crawls the pages found at those URLs, 
 * but limits itself to maxDepth hops from the seed URL and to URLs that are ‘internal’ to the designated CS50 server.
 *
 * Ross Guju
 *
 */


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include "file.h"
#include "memory.h"
#include "hashtable.h"
#include "bag.h"
#include "webpage.h"

/* ***************************************** */
/* Private types */
/*
None
*/
/* *********************************************************************** */
/* Private function prototypes */

void crawler(char *seedURL, int depth, char *file_dir);
void pagesaver(webpage_t *webpage, int file_id, char *file_dir);
bool pagefetcher(webpage_t *webpage);
int pagescanner(webpage_t *page, int pos, char **result);
static void myFree(void* str);

/**************** file-local global variables ****************/
static const int NUM_SLOTS = 37; // hashtable size


/********************** main() *************************

 * [main takes commandline arguements, checks them for errors and then passes
 * the arguments to the rest of the functions, more specifically to crawler ]
 * @param  argc [argument count]
 * @param  argv [arg[0]: crawler 
 *              arg[1]: seedURL
 *              arg[2]: pageDirectory
 *              arg[3]: maxDepth]
 * @return      [returns status of prorgam; non-negative implies sucess]
 *
 * Assumptions: 
 * 1. No more or no less than 4 arguements
 * 2. seedURL is internal to the cs50 server
 * 3 pageDirectory must be readable and writable
 * 4. depth must be a number
 * 5. depth cannot be negative
 */
int main(int argc, char *argv[])
{
	int maxDepth = atoi(argv[3]);
	DIR *pageDir;
	pageDir = opendir(argv[2]);

	//Checking arguments
	// case 1: no more than 3 arguments
	if (argc != 4) {
		fprintf(stderr, "Too many or too few arguments: \n");
		return -1;
	}
//case 2: seedURL is internal
	else if (!IsInternalURL(argv[1]) || !NormalizeURL(argv[1])) {
		fprintf(stderr, "SeedURL is not an Internal URL: \n");
		return -2;
	}
//case 3: pageDirectory DNE

	else if ( pageDir == NULL ) {
		perror("directory can not be opened");
		return -3;
	}
// case 4: depth is not a number
	char *ptr = argv[3];
	while (*ptr != '\0') {
		if (*ptr > '9' || *ptr < '0') {
			fprintf(stderr, "maxDepth needs to be an integer: %s\n", argv[3]);
			return -4;
		} ptr++;
	}

	// case 5: Depth is not zero & is less than or equal to maxDepth
	if ( 0 > maxDepth ) {
		fprintf(stderr, "Depth is negative\n");
		return -5;
	}

// arguments check out. initialize seed and pass it through crawler


	else {		
		closedir(pageDir);
		char* seed; // initial seed

		seed = (char*)calloc(strlen(argv[1]) + 1, sizeof(char));
		strcpy(seed, argv[1]);

		// after allocating memory pass seed to crawler
		crawler(seed, maxDepth, argv[2]);

		printf("Crawler is done crawling\n");
	}
	return 0;

}

/******************** crawler() ******************************

[crawler loops over pages to explore, until the list is exhausted]
 *
 * @param seedURL  [pointer to the seed URL]
 * @param depth    [the maxDepth that user wants to explore]
 * @param file_dir [the directory that the user wants to store the documents]
 *
 * Assumptions: 
 * 	1. seedURL has been allocated and is a pointer 
 * 	2. directory for the files is readable and writable and exists
 * 	3. page is valid, contains html and base_url
 * 
 *
 * Returns: Nothing
 */

void crawler(char *seedURL, int depth, char *file_dir)
{
	
	int file_number = 1; // file_id

	bag_t *webpages_to_crawl = bag_new(); //make bag of webpages to crawl
	hashtable_t *urls_crawled; 

	urls_crawled = hashtable_new(NUM_SLOTS); // initialize hashtable for urls seen

	webpage_t *seed_page;
	seed_page = webpage_new(seedURL, 0, NULL); // make seed_page  webpage


	bag_insert(webpages_to_crawl, seed_page); //add seedURL to the bag
	hashtable_insert(urls_crawled, seedURL, seedURL); // add url to hashtable

	webpage_t *page_to_crawl;
	page_to_crawl = bag_extract(webpages_to_crawl); // page_to_crawl is pops webpage out of the bag


	fprintf(stdout, "Page crawling: %s\n", seedURL);

	while ( page_to_crawl != NULL) {
		// loop through the bag of urls to crawl
        if (pagefetcher(page_to_crawl)) {
        	//fetches webpage, url and html and if successful save page contents
            pagesaver(page_to_crawl, file_number, file_dir);
            file_number++;
            //increase file_number which is the file identification by one
                if (webpage_getDepth(page_to_crawl) < depth) {
                    int pos = 0; //initialize position of page_to_crawl linked urls to 0
                    char *new_url; // pointer to the next url
                    while ((pos = pagescanner(page_to_crawl, pos, &new_url)) > 0) {
                             // loop and get the new urls to start crawling to
                            if (!IsInternalURL(new_url)) {    
                            //url is not an internal page to cs50 server
                                fprintf(stderr, "Not an internal page: %s\n",  new_url);
                            } else if (!NormalizeURL(new_url)) {
                            //could not normalize url
                            	fprintf(stderr, "Not Normalizable url: %s\n",  new_url);
                           	//insert new_url into hashtable
                            } else if (hashtable_insert(urls_crawled, new_url, new_url)) {
                                printf("%s%s\n","new url added\n", new_url );
                                // make new_url into webpagetype and have new_webpage point to it
                                webpage_t *new_webpage = webpage_new(new_url, webpage_getDepth(page_to_crawl) + 1, NULL);
			       	//insert new_webpage to be popped off bag for later
                                bag_insert(webpages_to_crawl, new_webpage);
				
			    } else {
				    count_free(new_url);}
			}
                    }
                }
                //pop off the next webpage in bag to process
		webpage_delete(page_to_crawl);
                page_to_crawl = bag_extract(webpages_to_crawl);

        }
   hashtable_delete(urls_crawled, myFree);
   count_free(webpages_to_crawl);
    return;
}

/********************* myFree() ******************
 * used to delete string in hashtable_delete() function
 */

static void myFree(void *str){
	if (str) {
    free((char*)str);
  }
}


/********************* pagefetcher() *****************
 * fetches a page from a URL
 * @param  webpage [the webpage struct containing the url to fetch]
 * @return         [True: success page is fetched; 
 *                  False: some error when fetching page.]
 *  Assumptions:
 *     1. page has been allocated by caller
 *     2. page->url contains the url to fetch
 *     3. page->html is NULL at call time
 *     
 */
bool pagefetcher(webpage_t *webpage)
{
	if (webpage_fetch(webpage)) {
		fprintf(stdout, "Fetching contents: %s\n", webpage_getURL(webpage));
		return true;
	}
	else {
		//send error message to stderr
		fprintf(stderr, "Contents could not be fetched: %s\n", webpage_getURL(webpage));
		return false;
	}

}

/*********************** pagescanner() *****************
 * [pagescanner extracts URLS from a page and processes each one]
 * 
 * @param  webpage [pointer to webpage]
 * @param  pos     [current position in html buffer]
 * @param  result  [a pointer to a character pointer]
 * @return         [Returns current position in html or returns > 0 so that the crawler knows
 *                 there are no more links on the page. The pos argument should be 0 on the 
 *                 initial call; the new pos is returned. On successful parse of html,
 *                 result will contain a newly allocated character buffer; may be NULL on failed return. 
 *                 The caller must free the memory.]
 *  Assumptions:
 *     1. page is valid, contains html and base_url
 *     2. pos = 0 on initial call
 *     3. result is NULL and will be allocated internally
 */
int pagescanner(webpage_t *webpage, int pos, char **result)
{	
	return webpage_getNextURL(webpage, pos, result);

}



/************************* pagesaver() *****************
 * [pagesaver outputs a page to the appropriate file directory. The output file is 
 *  ./${directory}/file_id. In the file, the first like is the url, 2nd line is the 
 *  page depth, and the rest is the filtered html content]
 * @param webpage  [pointer to webpage]
 * @param file_id  [file identification number]
 * @param file_dir [directory were the output document will go]
 *
 * Assumptions:
 *  1. file_dir is readable and exists.
 *  2. page is valid, contains html and base_url
 *  3. webpage is internal.
 * 
 */
void pagesaver(webpage_t *webpage, int file_id, char *file_dir) {
	
	char *words; //pointer to a word in the html file
	int pos = 0; // start in the beggining of html[pos]

	// allocate memory to story argument file_directory
	char *file_path = malloc(strlen(file_dir) + sizeof(int) + 1);
	if (!file_path) {
		// check to see if memory got allocated
		count_free(file_path);
		fprintf(stderr, "Memory did not allocate when collecting file path");

	}
	// store the file:./${directory}/file_id
	sprintf(file_path, "%s/%i", file_dir, file_id);
	// open file
	FILE *page_file = fopen(file_path, "w"); 
	//no longer need file_path bc we are writing the file right now
	count_free(file_path);
	// get the html content to put into file
	webpage_getHTML(webpage);
	// first line is the webpage url
	fprintf(page_file, "\n%s\n", webpage_getURL(webpage));
	// 2nd line is the webpage_depth
	fprintf(page_file, "%i\n", webpage_getDepth(webpage));

	while ((pos = webpage_getNextWord(webpage, pos, &words)) > 0) {
		//print filtered html words in the file
		fprintf(page_file, "%s ", words);
		//no need to keep words
		count_free(words);
	}
	//close file
	fclose(page_file);

	printf("Page Saving: %s\n", webpage_getURL(webpage));
}







