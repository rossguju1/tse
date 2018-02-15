/*
 * indexertest.c - This indexer test program takes indexer file as its input and converts the file back into
 * the data structure and then reprints the table out to a new file.
 *
 * cs50
 *
 * Ross Guju
 *
 *
 */


#include "file.h"
#include "word.h"
#include "memory.h"
#include "webpage.h"
#include "hashtable.h"
#include "set.h"
#include "counters.h"
#include "jhash.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>
#include <stddef.h>

/* ***************************************** */
/* Global types */

typedef struct ht {
	int num_slots; // length of pointer table
	struct set **table; // hashtable
} hashtable_t;


typedef struct set {
	struct setnode *head;       // head of the set
} set_t;

typedef struct setnode {
	char *key;                       // search key for this item
	void *item;                      // pointer to data for this item
	struct setnode *next;            // next node
} setnode_t;



typedef struct ctrs {
	struct ctrsnode *head;                     // head of the set
} counters_t;

typedef struct ctrsnode {
	int key;                // search key for this item
	int count;               // Counter for number of occurances
	struct ctrsnode *next;   // next linked list
} ctrsnode_t;

/* *********************************************************************** */
/* Private function prototypes */


hashtable_t *IndexLoad(char *oldIndex);

void IndexSave(char *newIndexfile, hashtable_t *ht);

void printer(counters_t *ctrs, FILE *fp);

void CleanUp(hashtable_t *ht);

void itemdelete(void *data);

/**************** file-local global variables ****************/
static const int NUMSLOTS = 1009;  // hashtable size

/************************ main() ************************
 * [main description]
 * @param  argc [argument count]
 * @param  argv [argument array]
 * @return      [integer status: non-positive integer is failure
 * 		and positive integer is success]
 * Assumptions: 
 * 1. oldIndexFilename is in the index format
 */
int main(int argc, char *argv[])
{
	struct stat statbufArg1;
	struct stat statbufArg2;
	char *oldIndexFilename, *newIndexFilename, *newIndexFilenamePath;

	oldIndexFilename = argv[1];
	// check if for right amount of arguments
	if (argc != 3) {
		fprintf(stderr, "Not the right amount of arguments : %d", argc);
		return -1;
	}
	// if argument 1 is not a file error
	if (!stat(oldIndexFilename, &statbufArg1) && S_ISDIR(statbufArg1.st_mode) && (statbufArg1.st_mode & S_IRUSR)) {
	
	}

	newIndexFilenamePath = argv[2];
	char* indexFile_copy = strdup(newIndexFilenamePath); // ptr to copy string
	//get the directory path
	newIndexFilename = dirname(indexFile_copy);
	//free the duplicates
	free(indexFile_copy);
	free(newIndexFilename);
	//error checking: if newIndexFilename parent directory doesnt exist->failure 
	if (!stat(newIndexFilename, &statbufArg2) && S_ISDIR(statbufArg2.st_mode) && (statbufArg2.st_mode & S_IRUSR)) {
		
	}

	// initialize Newindex variable to the indexload function
	hashtable_t *Newindex = IndexLoad(oldIndexFilename);
	// the new index with is the inverted index and prints to the file
	IndexSave(newIndexFilenamePath, Newindex);
	// clean up data structurei
	hashtable_delete(Newindex, itemdelete);

	return 0;
}

/********************** IndexLoad() *****************************
 * [IndexLoad take an old index file and breaks the formated index into an inverted index
 * data structure and then out puts the inverted data structure for indexSave function]
 * @param  oldIndex [the file that contains the index format]
 * @param  ht       [hashtable which is the inverted index data structure]
 * @return          [return the inverted index hashtable]
 * Assumptions:
 * 1. oldIndex is a formated index file
 */
hashtable_t *IndexLoad(char *oldIndex)
{	
	// initialize hashtable
	hashtable_t *index = hashtable_new(NUMSLOTS);

	FILE *fp = fopen(oldIndex, "r");
	// go through the lines of the index file
	char *line;
	while ((line = readlinep(fp)) != NULL) {
		// parse the lines into tokens
		char *wordToken = strtok(line, " ");
		char *word = calloc(1 + strlen(wordToken), sizeof(char));
		//word is the name of the word parsed by wordToken
		word = strdup(wordToken);
		// initialize counter data structure for later
		counters_t *newCounter = counters_new();
		//insert word and the item: the counter set in hashtable
		hashtable_insert(index, word, newCounter);
		free(word);
		//the next word is the document Id
		wordToken = strtok(NULL, " []");
		int Id = 0, counters;
		//used for interating between doc Id and frequecny of counts
		bool alternate = true;

		while (wordToken != NULL) {
			//token that hits the end of the line
			if (strcmp(wordToken, "\n") == 0) {
				break;
			}
			// save the docId by counters_add 
			if (alternate) {
				char *docId = calloc(1 + strlen(wordToken), sizeof(char));
				docId = strdup(wordToken);
				Id = atoi(docId);
				free(docId);
				counters_add(newCounter, Id);
				alternate = false;
			} else {
				// save the frequency count to with the key: doc Id
				char *count = calloc(1 + strlen(wordToken), sizeof(char));
				count = strdup(wordToken);
				counters = atoi(count);
				free(count);
				counters_set(newCounter, Id, counters);
				//next token will be doc Id
				alternate = true;
			}
			wordToken = strtok(NULL, " []");
		}
		free(line);
	}//clean up and close file
	fclose(fp);
	return index;
}


/**************************** IndexSave() ***************************
 * [IndexSave prints the inverted index to the input file]
 * @param newIndexfile [the new inverted index file to print the formated inverted index]
 * @param ht           [hashtable of the inverted matrix data structure]
 */

void IndexSave(char *newIndexfile, hashtable_t *ht)
{
	char *word;
	int i;
	// save file path to open
	char *file_path = malloc(strlen(newIndexfile) + sizeof(int) + 1);
	if (!file_path) {
		// check to see if memory got allocated
		free(file_path);
		fprintf(stderr, "Memory did not allocate when collecting file path");
		return;
	}
	sprintf(file_path, "%s", newIndexfile);
	//make the file and then open it
	FILE *fp = fopen(file_path, "w");
	free(file_path);
	//go through the inverted index data structure 
	for ( i = 0; i < NUMSLOTS; i++) {
		setnode_t *word_node = ht->table[i]->head;
		if (word_node != NULL) {
			// if node exists, print the key (word)
			word = word_node->key;
			fprintf(fp, "%s", word);
			fprintf(fp, " ");
			//then print the doc Id and counter frequency in counters structure
			counters_t *ctrs = hashtable_find(ht, word);
			printer(ctrs, fp);
			fprintf(fp, "\n");
		}
	}
	fclose(fp);
}

/****************** printer() ***********************
 * [printer goes through the inverted index data structure and prints the 
 * inverted index to the file]
 * @param ctrs [counter data structure]
 * @param fp   [file to print the inverted  index]
 * input: counters and filename
 * output: NONE
 * Side Effect: prints inverted  index format to file
 * Assumptions: 1. Inverted index datastructure is not empty
 * 		2. the file directory exists
 */
void printer(counters_t *ctrs, FILE *fp)
{
	// ignore cases when fp/ctrs == NULL
	if ( fp != NULL) {
		if (ctrs != NULL) {
			
			// print counter set according to the indexer format
			for (ctrsnode_t *current = ctrs->head; current != NULL; current = current->next) {
				fprintf(fp, "[");
				fprintf(fp, "%d", current->key);
				fprintf(fp, " ");
				fprintf(fp, "%d", current->count);
				fprintf(fp, "]");
				fprintf(fp, " ");

			}
		} else {// if error print null
			fputs("(null)", fp);
		}
	}
}

/******************CleanUp()******************
 * uses counter_delete and hastable_delete and itemdelete
 * to clean up the data structure
 * input: indexer datastructure i.e hastable and counter structure
 * output: NONE
 * Side Effect: cleans up data structure
 */
void CleanUp(hashtable_t *ht)
{
	hashtable_delete(ht, itemdelete);
}

/****************** itemdelete() ***********************
 * function is used with hashtable_delete() to delete clean up
 * the memory. This is used to dereference void pointer and let
 * data = counter_t data type.
 * input: counter structure
 * output : NONE
 * side Effect: deletes counters data structure
 * */

void itemdelete(void *data)
{
	counters_t *ctrs = data;
	if (ctrs != NULL) {
		counters_delete(ctrs);
	}
}

