/* Index.c 
 *
 * Contains all the logic for saving and loading index
 *
 *Ross Guju
 *
 * CS50
 */

#include "hashtable.h"
#include "set.h"
#include "counters.h"
#include "webpage.h"
#include "file.h"
#include "pagedir.h"
#include "word.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <libgen.h>
#include <errno.h>
#include <unistd.h>


/**************** file-local global variables ****************/
static const int NUMSLOTS = 1009;  // hashtable size

/******************** Data types ***************************/
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

/**************** Function prototypes***********************/

hashtable_t *IndexLoad(char *oldIndex);

int indexBuild(char *Directory, char *fileName, hashtable_t *ht);

void indexSave(hashtable_t *ht, char *fileName);

void itemdelete(void *data);

void printer(counters_t *ctrs, FILE *fp);






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
    strcpy(word, wordToken);
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
        strcpy(docId, wordToken);
        Id = atoi(docId);
        free(docId);
        counters_add(newCounter, Id);
        alternate = false;
      } else {
        // save the frequency count to with the key: doc Id
        char *count = calloc(1 + strlen(wordToken), sizeof(char));
        strcpy(count, wordToken);
        counters = atoi(count);
        free(count);
        counters_set(newCounter, Id, counters);
        //next token will be doc Id
        alternate = true;
      }
      wordToken = strtok(NULL, " []");
    }
    free(line);
    //counters_delete(newCounter);


    //(word);
  }//clean up and close file
  free(line);
  fclose(fp);

  return index;
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

    char *pathcopy = calloc(1 + strlen(Directory), sizeof(char));
    strcpy(pathcopy, Directory);
    //moving to the directory of indexFilename
    chdir(basename(pathcopy));
    // if current directory is read, ignore it
    free(pathcopy);
    //do nothing for hidden files and unwanted files
    if ((in_file->d_name[0] != '.') && (in_file->d_name[strlen(in_file->d_name) - 1] != '~')) {
      ;
    }

    // change file name to integer
    char *docId = calloc(1 + strlen(in_file->d_name), sizeof(char));
    strcpy(docId, in_file->d_name);
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


