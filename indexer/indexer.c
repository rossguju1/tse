/*
 * indexer.c - This indexer program takes a crawler directory as its input. Stores it in a hashtable/counter
 * data structure and then prints it to a new file. The format is word [docId frequency] [docId frequency] .... [docId frequency]
 * The indexer is constructed from four functions, VerifyArguments, indexBuild, indexSave and CleanUp.
 * Ross Guju
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
#include <libgen.h> // for basename
#include <sys/stat.h>//stat()
#include <ctype.h>
#include <unistd.h>
#include <stddef.h>

/* ***************************************** */
/* Global types used */

typedef struct webpage {
  char *url;                               // url of the page
  char *html;                              // html code of the page
  size_t html_len;                         // length of html code
  int depth;                               // depth of crawl
} webpage_t;

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

bool VerifyArguments(int argument_count, char **argument_array);


int indexBuild(char *Directory, char *fileName, hashtable_t *ht);

void indexSave(hashtable_t *ht, char *fileName);

void CleanUp(hashtable_t *ht);

void itemdelete(void *data);

void printer(counters_t *ctrs, FILE *fp);


/**************** file-local global variables ****************/
static const int NUMSLOTS = 1009;  // hashtable size


/********************** main() *************************

 * [main takes commandline arguements, checks them for errors and then passes
 * the arguments to the rest of the functions]
 * @param  argc [argument count]
 * @param  argv [arg[0]: indexer
 *              arg[1]: crawler directory with crawler formated docs
 *              arg[2]: indexFilename, where the index will be outputed
 * @return      [returns status of program; non-negative implies sucess]
 *
 * Assumptions:
 * 1. the Crawler directory exists and is readable
 * 2. the crawler directory contains files that are listed from 1, 2, 3....
 * 3. The crawler files are of constant format
 */

int main(int argc, char *argv[])
{
  // verify arguments
  if (!VerifyArguments(argc, argv)) {
    exit(-1);
  }
  // set commandland arguments to variables for later use
  char* pageDirectory;
  char* indexFilename;
  pageDirectory = argv[1];
  indexFilename = argv[2];

  // Initialize the hashtable
  hashtable_t *index = hashtable_new(NUMSLOTS);

  //Build the hashtable
  indexBuild(pageDirectory, indexFilename, index);

  //free the index data structure
  CleanUp(index);

  return 0;
}
/********************* VerifyArguments() ******************************
 * [VerifyArguments checks the commandline arguements and sees if the crawler directory argv[1]
 *  exists and is readable. Argv[2] is the indexfilename where the index will be printed]
 * @param  argument_count [the number of commandline arguments]
 * @param  argument_array [the arguments inputed by the user]
 * @return                [returns true if pageDirectory, which is the crawler directory exists and is readable
 *                          and if there are no more or no less than 3 arguments and checks if the directory where
 *                          the indexFile will be printed exists]
 * intput: argument count and argument array from main function/
 * Output: status if the arguments check: true-Success and false-error
 */
bool VerifyArguments(int argument_count, char **argument_array)
{

  struct stat statbufArg1;
  struct stat statbufArg2;
  //pageDirectory is the crawler directory
  //indexFilenameDir is the directory where the index will be printed
  char *pageDirectory, *indexFilenameDir;
  pageDirectory = argument_array[1];

  // no more than 3 arguments
  if (argument_count != 3) {
    fprintf(stderr, "Not the right amount of arguments : %d", argument_count);
    return false;
  }
  //check if the crawler directory exists
  if (!(!stat(pageDirectory, &statbufArg1) && S_ISDIR(statbufArg1.st_mode) && (statbufArg1.st_mode & S_IRUSR))) {
    fprintf(stderr, "Arg1 is a not an existing file in the directory ");
    return false;
  }
  // check if indexfilename path exists
  char* path_indexFilename = argument_array[2];
  char* indexFile_copy = strdup(path_indexFilename); // ptr to copy string
  indexFilenameDir = dirname(indexFile_copy); // actual indexFilename

  if (!(!stat(indexFilenameDir, &statbufArg2) && S_ISDIR(statbufArg2.st_mode) && (statbufArg2.st_mode & S_IRUSR))) {
        fprintf(stderr, "Arg2 is a not directory");
	return false;
  }
  free(indexFile_copy);


  return true;
}

/**************************** indexBuild() **************************
 * [indexBuild goes through the crawler directory and builds the indexer data structure]
 * @param  Directory [crawler directory holding the html]
 * @param  fileName  [name of the file to print the indexer]
 * @param  ht        [hashtable that will hold the words and counters]
 * @return           [returns a non-negative integer if function was sucessful]
 * intput: crawler output file directory, filename for as input for indexSave, 
 * and hashtable to built the index
 * output: return status of indexBuild
 * Side Effect: takes crawler file and loads index datastructure then uses 
 * indexSave to print the index.  
 */

int indexBuild(char *Directory, char *fileName, hashtable_t *ht)
{
  DIR* FD;
  struct dirent* in_file;
  FILE* entry_file;

  int pos, Id;
  // open directory path
  FD = opendir(Directory);
  // if directory doesnt open, error
  if (NULL == FD) {
    fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
    return (-1);
  }
  // loop through all of the crawler files
  while ((in_file = readdir(FD))) {

    char *pathcopy;
    pathcopy = strdup(Directory);
    //moving to the directory of indexFilename
    chdir(basename(pathcopy));
    // if current directory is read, ignore it
    free(pathcopy);
    //do nothing for hidden files and unwanted files
    if ((in_file->d_name[0] != '.') && (in_file->d_name[strlen(in_file->d_name) - 1] != '~')) {
      ;
    }

    // change file name to integer
    char *docId = strdup(in_file->d_name);
    Id = atoi(docId);
    free(docId);

    entry_file = fopen(in_file->d_name, "r");
    //initialize the weburl and its contents
    char *weburl;
    // read the first line which is the url
    weburl = readlinep(entry_file);
    //ignore 2nd line i.e depth
    char *junk = readlinep(entry_file);
    char *contents = readfilep(entry_file);
    free(junk);

    //initialize the webpage data structure to make use of webpage_getNextWord()
    webpage_t *current_doc = webpage_new(weburl, 0, contents);
  
    //currentWord is the next word were the buffer will save to
    char *currentWord = NULL;

    pos = 0;

    while ((pos = webpage_getNextWord(current_doc, pos, &currentWord)) > 0) {

      currentWord = NormalizeWord(currentWord);
      // if the string length is less then three ignore it
      if (strlen(currentWord) < 3) {
        free(currentWord);
        continue;

      } else if (hashtable_find(ht, currentWord) != NULL) {
        counters_t *current_ctrs = hashtable_find(ht, currentWord);
        counters_add(current_ctrs, Id);
        free(currentWord);
        continue;
      } else {

        // if the word hasn't been inserted into the hashtable
        // make a new counter struct to hold frequency of words
        counters_t *ctrs_new;
        ctrs_new = counters_new();
        //since it is the first word increament by 1

        counters_add(ctrs_new, Id);
        // now insert it into the hashtable, where currentWord is the key
        hashtable_insert(ht, currentWord, ctrs_new);
        free(currentWord);
      }
    }
    //clean up
    free(contents);
    free(weburl);
    free(current_doc);
    fclose(entry_file);
  }
  indexSave(ht, fileName);
  closedir(FD);
  return 0;
}

/***************************** indexSave() *****************************
 * [indexSave goes through the index data structure and prints out the indexer format
 *   where on a line there is a word [docId count] [docId count] [docId count] .....
 *   the indexer is printed to the indexfilename argv[2]]
 * @param ht       [hashtable that is full of the words and [docId count] pairs]
 * @param fileName [the name of the file that the index will be printed to]
 * input: hashtable data strcutre and file path for it to be printed.
 * output: none
 * Side Effect: prints the formated index to the indput file.
 */
void indexSave(hashtable_t *ht, char *fileName)
{
  char *word;
  int i;
  // the path of the indexFilename where we will print the index
  char *file_path = malloc(strlen(fileName) + sizeof(int) + 1);
  if (!file_path) {
    // check to see if memory got allocated
    free(file_path);
    fprintf(stderr, "Memory did not allocate when collecting file path");

    return;
  }
  sprintf(file_path, "%s", fileName);
  // open the indexFilename to write the index
  FILE *indexFile = fopen(file_path, "w");
  free(file_path);

  // got through the hashtable and print the head of the nodes which
  // are the words/keys
  for ( i = 0; i < NUMSLOTS; i++) {
    setnode_t *word_node = ht->table[i]->head;
    if (word_node != NULL) {
      word = word_node->key;
      fprintf(indexFile, "%s", word);
      fprintf(indexFile, " ");
      // now print the counters that are in the hashtable
      counters_t *ctrs;
      ctrs = word_node->item;
      printer(ctrs, indexFile);
      fprintf(indexFile, "\n");
    }
  }
  fclose(indexFile);
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

/****************** printer() ***********************
 * [printer description]
 * @param ctrs [counter data structure]
 * @param fp   [file to print the index]
 * input: counters and filename
 * output: NONE
 * Side Effect: prints index format to file
 */
void printer(counters_t *ctrs, FILE *fp)
{
  // ignore cases when fp/ctrs == NULL
  if ( fp != NULL) {
    if (ctrs != NULL) {
      ;
      // print counter set according to the indexer format
      for (ctrsnode_t *current = ctrs->head; current != NULL; current = current->next) {
        fputc('[', fp);
        fprintf(fp, "%d", current->key);
        fputc(' ', fp);
        fprintf(fp, "%d", current->count);
        fputc(']', fp);
        fputc(' ', fp);
      }
      ;
    } else {
      fputs("(null)", fp);
    }
  }
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
