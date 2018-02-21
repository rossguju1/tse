/* CS50 TSE - Querier 
 *
 * Ross Guju
 *
 *
 * querier.c - An implementation of querier that takes a crawler directory and index file as input, then asks the user to input words to search in the  converted indexfile and outputs matched words, doc Id and frequency for that doc Id and the Url which is fetched from the corresponding crawler file (which is the doc Id that was found with the word.
 *
 *
 */


#include "file.h"
#include "pagedir.h" 
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

/************** Data types *******************************/
typedef struct ctrsnode {
  int key;                // search key for this item
  int count;               // Counter for number of occurances       
  struct ctrsnode *next;   // next linked list
  } ctrsnode_t;


typedef struct ctrs {
 struct ctrsnode *head;                     // head of the set
} counters_t;

struct AndCounters { 
  counters_t *CurrentCounter;
  counters_t *Intersect;
};
// AndCounters structure is used to pass two counter nodes into
//counters iterate function


/**************** local constants ********************/

static const int NUMSLOTS = 1009;  // hashtable size


/******************* function prototypes **************/
int ArgumentCheck(int argument_count, char **argument_array);

void BuildQuery(hashtable_t *invertedIndex, char *pageDirectory);

bool CheckIfoperator(char* token);

bool validToken(char** Tokens);

char** SearchParser(char* input, int *number);

counters_t *OrCombine(counters_t **counters, int CounterNumber);

void OrCombineHelper(void* arg, int key, int count);

counters_t *AndCombine(counters_t *CtrFirst, counters_t *CtrSecond);

void AndCombineHelper(void* arg, int key, int count);

void RemoveAndSequences(char** token, int *TokenNumber);

void InputCounter(counters_t *Ctrs, counters_t *CtrsCopy);

void InputCounterHelper(void *arg, int key, int count);

void RemoveAnd(char **GetQuery, int index, int length);

bool CheckCharacters(char* word);

hashtable_t *IndexLoad(char *oldIndex);

void Rank(counters_t *counters, char* pageDirectory);

void CounterSize(void *arg, int key, int count);

void itemdelete(void *data);

/**************** main() ******************/


int main(int argc, char *argv[])
{
 	// check arguments
    if (ArgumentCheck(argc, argv) != 0) {
      exit(-1);
    }
   
  char* pageDirectory;
  char* indexFilename;
  pageDirectory = argv[1];
  indexFilename = argv[2];

  // initialize inverted index to the hashtable of the built inverted index
  hashtable_t *Inverted = IndexLoad(indexFilename);
  // construct the query
  BuildQuery(Inverted, pageDirectory);
  // clean up
  hashtable_delete(Inverted, itemdelete);

  return 0;
}

/**************** ArgumentCheck()*******************
 * This function checks that there are only two input arguments;
 * a directory to crawler files and an index file. The function checks
 * that the directory and file exist and can be opened
 * inputs: pageDirectory and indexFilename
 * output: integer status
 * return: negative integer implies error and 0 means success
 */

int ArgumentCheck(int argument_count, char **argument_array)
{
   char *pageDirectory, *indexFilename;
  pageDirectory = argument_array[1];
  
  if (argument_count != 3) {
    fprintf(stderr, "Needs two arguments");
    return -1;
  }
  //check if crawler  page directory exists
  if (IsDir(pageDirectory) ==0)  {
    fprintf(stderr, "Arg1 is a not an existing  directory ");
    return -2;
  }

  indexFilename = argument_array[2];
  // check if index file exists and is readable
  FILE* fp = fopen( indexFilename, "r");
  if (fp == NULL) {
    fprintf(stderr, "file at '%s' is not readable\n", argument_array[2]);
    return -3;
  }
  fclose(fp);
 return 0;
}

/****************** BuildQuery() *******************
 * Builds the query using the inverted index data structure
 *
 *input: filled inverted index hashtable and crawler directory
 *output: None
 *SideEffect: goes through inverted index and builds the query according
 * to the user key words
 */

void BuildQuery(hashtable_t *invertedIndex, char *pageDirectory)
{


  int i;
  //initial flag when reading or operator
  bool AfterOr;
  // counter used after parsing and's
  int CounterCurrent = 0;
  // predetermined size for array of pointer 
  int CounterNumber = 25;

  // read from standard in
  while (!feof(stdin)) {
    fprintf(stdout, "KEY WORDS:> ");
    char* GetQuery = readlinep(stdin);
    // if the readline is empty
    if (GetQuery == NULL) {
        break;
      }
    if (strcmp(GetQuery, "") == 0) {
        // free the query and go to end of this function
        free(GetQuery);
        continue;
      }

    counters_t **WordCounters;
    //initialize array that will hold counters
    WordCounters = calloc(CounterNumber, sizeof(counters_t));

   //make sure characters are legal
   if (CheckCharacters(GetQuery)) {
      // change from upper to lower
      NormalizeWord(GetQuery);

      // string array that stores the initial input for later 
      char QueryCopy[200];
      strcpy(QueryCopy, GetQuery);

      // initialize number of tokens
      int TokenCount = 0;

      // parse the line
      char** WordTokens = SearchParser(GetQuery, &TokenCount);
      //set flag to false used to go to correct else if statement for 
      //and/or sequence
      AfterOr = false;
      // make sure the parsed words are valid
      if (validToken(WordTokens)) {
	//remove and's
        RemoveAndSequences(WordTokens, &TokenCount);

        for (i = 0; i < TokenCount; i++) {
	  // go through rest of tokens and check its sequence of or/words
	  // to determine operation order
          if ( strcmp(WordTokens[i], "or") == 0) {
            AfterOr = false;
	    //we will combine and's so or's will be the
	    //remaining operators
            CounterCurrent++ ;

          }
          else if (strcmp(WordTokens[i], "or") != 0) {
            if (AfterOr) {
              //previous word wasnt or operator and next word isnt or operator
              counters_t *current = WordCounters[CounterCurrent];
	      // find next word in hashtable and set to next counter
              counters_t *NextCounter = hashtable_find(invertedIndex, WordTokens[i]);
              // new counter to combine the and-sequence
              counters_t *new = AndCombine(current, NextCounter);
	      // clean up
              counters_delete(NextCounter);
	      // replace 1st counter with the combined counter
              WordCounters[CounterCurrent] = new;
	      //if two  consecutive words, then or operator 
            } else if ( ((WordTokens[i + 1] == NULL) || (strcmp(WordTokens[i + 1], "or") == 0)) && !AfterOr ) {
	      //get the word from hashtable and then make it a new counter
              counters_t *word = hashtable_find(invertedIndex, WordTokens[i]);
              counters_t *AddWord = counters_new();
              InputCounter(word, AddWord);
	      //set word which is new counter to current position
              WordCounters[CounterCurrent] = AddWord;


            } else if ( (WordTokens[i + 1] != NULL) && !AfterOr && (strcmp(WordTokens[i + 1], "or") != 0) ) {
	      // consecutive and-sequence i.e. consecutive words case
              counters_t *current = hashtable_find(invertedIndex, WordTokens[i]);
              counters_t *NextToken = hashtable_find(invertedIndex, WordTokens[i + 1]);
              counters_t *new = AndCombine(current, NextToken);
	      //combine the consecutive words i.e. and-sequence and assign to 1st position
              WordCounters[CounterCurrent] = new;
	      //flag to check if next word is or operator
              AfterOr = true;
              i++;
            }
          }
        }
        counters_t *QueryResult;
	//combine the remaining counters into a final counter
        QueryResult = OrCombine(WordCounters, CounterCurrent);
        fprintf(stdout, "Query: %s\n", QueryCopy);
	// rank the final counter
        Rank(QueryResult, pageDirectory);
      }
     //clean up
     free(WordTokens);
    } 
    free(WordCounters);
    free(GetQuery);
  }
}

/************ Rank() *******************
 * Goes through combined counter set of the matched 
 * Doc Id's and their counts and prints the Id/count 
 * pairs in descending count order. The doc Id is then 
 * joined with crawler directory to open the Id document 
 * and the URL is read from the first line 
 *
 * input: combined counters set of the matched Id/count pairs
 *        and crawler directory
 * output: None
 *
 * SideEffect: prints matches  
 *
 */

void Rank(counters_t *counters, char* pageDirectory)
{   
  int size = 0;
  int i;
  // gets the number of documents matched
  counters_iterate(counters, &size, CounterSize);
  fprintf(stdout, "Matches %d documents (Ranked) \n", size);
  while (1) {  
      
    int maxindex = 0; // variable for  doc Id
    int maxScore = 0; // this is the counts for the doc Id
    int score = 0; //temporary value to rank the doc Id's according to counts
    for ( i = 1; i < 2*size; i++ ) {
      score = counters_get(counters, i);
      if (score > maxScore) {
        maxScore = score;
        maxindex = i;
      }
    }
    // documents were matched
    if (maxScore > 0) {

      //print matched score and doc Id
      fprintf(stdout, "Score: %d Doc: %d ~>  ", maxScore,  maxindex);
      //gets path to doc Id
      char *file_path = malloc(strlen(pageDirectory) + sizeof(int) + 1);
      sprintf(file_path, "%s/%d", pageDirectory, maxindex);
      FILE* doc = fopen(file_path, "r");
      free(file_path);
      //read 1st line of doc to get Url
      char* URL = readlinep(doc);
      fclose(doc);
      fprintf(stdout, "%s\n", URL);
      free(URL);
    }
    counters_set(counters, maxindex, 0 );
    if (maxScore == 0) {
      break;
    }
  }
  //clean
  counters_delete(counters);
}

/******** CounterSize() ********
 * Is an itemfunction passed through counters
 * iterate and gets the total number of matches 
 * i.e number of documents found by querier
 */


void CounterSize(void *arg, int key, int count)
{
  int *size = arg;
  if (count > 0) {
    (*size)++;
  }
}


/********** validToken() ***************
 * Goes through array of tokens and validates
 * that operator is not first token, no adjacent tokens
 * and the last token is not an operator
 *
 *input: array of tokens
 *output: true if description is satisfied
 */

bool validToken(char** Tokens)
{
  if (CheckIfoperator(Tokens[0])) {
    fprintf(stderr, "Errpr: 1st word cant be an operator\n");
  }
  int j = 0, i;
  // go through token array and check that no two operators are adjacent
  for (i = 1; Tokens[i] != NULL; i++) {
    if (CheckIfoperator(Tokens[i]) && CheckIfoperator(Tokens[j])) {
      fprintf(stderr, "Error: adjacent operators\n");
      return false;
    }
    j++;
  } if (CheckIfoperator(Tokens[j])) { // make sure last token is not operator
    fprintf(stderr, "Error: last word is an operator\n");
    return false;
  }
  return true;
}

/********** CheckIfoperator() ************
 * Used to find and/or operators
 *
 * input: word/parsed token
 *
 * output: true if word/token is operator
 *
 */

bool CheckIfoperator(char* token)
{
  if (strcmp( token, "and") == 0 || strcmp(token, "or") == 0) {
    return true;
  } else {
    return false;
  }
}

/*********** CheckCharacters() **********
 * Goes through the characters of the input line and 
 * makes sure there is only letters and spaces
 *
 *input: string
 *Output: true if only spaces and letters
 *
 */


bool CheckCharacters(char* word)
{
  char *ptr;
  ptr = word;
  while (*ptr != '\0') {
    char ch = *ptr;
    if (ch != ' ' && !isalpha(ch)) {
      fprintf(stderr, "\nNot a valid character: %c\n",  ch);
      return false;
    }
    ptr++;
  }
  return true;
}

/************ OrCombine() *************
 * Takes an array of counters and interates through them and
 * combines them into one whole counter
 *
 * Input: array of counters and number of counters in the array
 * output: sum of all counters according to matching doc Id
 */


counters_t *OrCombine(counters_t **counters, int CounterNumber)
{
  int i;
  //final counter of combined counters
  counters_t *OrCounters = counters_new();
  if (OrCounters == NULL) {
    fprintf(stderr, "Failed allocating memoring for or-sequences\n");
  }
  // CounterNumber is the number of 'or' operators
  for (i = 0; i <= CounterNumber; i++) {
    counters_t *current = counters[i];
    //iterate through all counters and get their union
    counters_iterate(current, OrCounters, OrCombineHelper);

    counters_delete(current);
  }
  return OrCounters;
}

/******** OrCombineHelper() ************
 * Helper function for OrCombine
 */

void OrCombineHelper(void* arg, int key, int count)
{
  counters_t *AllOrCounters = arg;

  int current;
  // combine counts for union of doc Id's
  current = counters_get(AllOrCounters, key);
  current = current + count;
  counters_set(AllOrCounters, key, current);

}

/********** AndCombine ***********
 * For a 'and' sequence or consecutive words, the right 
 * word is compared to the first word and gets the inserection
 * between the two words which is the intersection; This ends finding the 
 * intersection of the doc Id counter nodes and if the doc Id nodes match; 
 * the left counter takes the value of the smallest count
 *
 * Input: Counter 1 and Counter 2
 * Output: intersection of the two which is inserted in counter 1
 *
 */


counters_t *AndCombine(counters_t *CtrFirst, counters_t *CtrSecond)
{
  counters_t *AndIntersection = counters_new();

  if (AndIntersection == NULL) {
    fprintf(stderr, "Failed allocating memory for And-sequence");
  }
  // use AndCounters structure to pass the two words that
  // make up and-sequence
  struct AndCounters *TempIntersection;
  TempIntersection = malloc(sizeof(struct AndCounters));

  if (TempIntersection == NULL) {
    fprintf(stderr, "Could not allocate memory for And sequence intersection");
  }

  // the intersection is the new counter
  TempIntersection->Intersect = AndIntersection;

  TempIntersection->CurrentCounter = CtrSecond;

  // replace first counter with the intersection
  counters_iterate(CtrFirst, TempIntersection, AndCombineHelper);

  return AndIntersection;

}


/************ AndCombineHelper() ***********
 * Helper function for AndCombine
 */


void AndCombineHelper(void* arg, int key, int count)
{
  int Smallest = 0;
  int CountSize1;
  int CountSize2;

  struct AndCounters *TempIntersection = arg;
  // currentcounter is the 2nd counter
  counters_t *Intersect = TempIntersection->Intersect;
  counters_t *CurrentCounter = TempIntersection->CurrentCounter;

  CountSize2 = counters_get(CurrentCounter, key);

  CountSize1 = count;
  // replace 1st counter with intersection between the two counters
  if (CountSize1 > CountSize2) {
    // the interesection is the counter with the smallest count	  
    Smallest = CountSize2;
  } else {
    Smallest = CountSize1;
  }

  counters_set(Intersect, key, Smallest);
}

/********* RemoveAndSequences()*************
 * Function removes 'and' operators from the token array
 * input: token and Token number which is the total number of tokens
 * Output: None
 * SideEffect: reduces TokenNumber for every 'and' operator that
 * is taken out of the array
 */


void RemoveAndSequences(char** token, int *TokenNumber)
{
  int  i;
 // go through the array of tokens and take out 'and' operator
  for (i = 0; i < (*TokenNumber); i++) {
    if (strcmp(token[i], "and") == 0) {
      RemoveAnd(token, i, (*TokenNumber));
      (*TokenNumber)--;
    }
  }
}

/********** InputCounter() ****************
 *Interates through a counter set and essentially just
 * copies the counter into a new counter.
 * input: Ctrs, which is the source counter
 * 	CtrsCopy, which is the destination counter
 * Output: None
 */
void InputCounter(counters_t *Ctrs, counters_t *CtrsCopy)
{
  counters_iterate(Ctrs, CtrsCopy, InputCounterHelper);
}


/*********** InputCounterHelper() ***************
 * Helper function for InputCounter
 */ 


void InputCounterHelper(void *arg, int key, int count)
{
  counters_t *InputCounter = arg;
  counters_set(InputCounter, key, count);
}

/****** IndexLoad() ************
 * reads index file and loads the inverted index data structure
 * and returns inverted index hashtable
 *
 * input: index file
 * Output: inverted index hashtable
 *
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

  }//clean up and close file
  free(line);
  fclose(fp);
 
  return index;
}

/******** SearchParser() *****************
 * Function that parses the standard input from user into
 * word tokens. Uses realloc() to adjust WordTokens array
 *
 * input: a line of from readlinep and a pointer to a number to 
 * keep track of the token count
 * output: Array filled with the parsed tokens
 */



char** SearchParser(char* input, int *number)
{ //initialize token array
  char** WordTokens = NULL;
  char* CurrentToken = strtok(input, " ");
  int TokenNum = 0;
  //parse the line accoring by space delimitor
  while (CurrentToken != NULL) {
    WordTokens = realloc(WordTokens, sizeof(char*) * (++TokenNum));
    if (WordTokens == NULL) {
      fprintf(stderr, "Error parsing\n");
    }
    //save last position for end of string indicator
    WordTokens[TokenNum - 1] = CurrentToken;
    CurrentToken = strtok(NULL, " ");
  }
  WordTokens = realloc(WordTokens, sizeof(char*) * (TokenNum + 1));
 // the last position is 0
  WordTokens[TokenNum] = 0;

  *number = TokenNum;

  return WordTokens;
}

/******** RemoveAnd() **********
 * Helper function for RemoveAndSequence
 */

void RemoveAnd(char **GetQuery, int index, int length)
{
  int i;
  for (i = index; i < length - 1; i++) {
    GetQuery[i] = GetQuery[i + 1];
  } // replace next word with current word
  GetQuery[length - 1] = NULL;
 
}

/******** itemdelete *********
 *
 * Simple function for passing as item function 
 * in hashtable_delete()
 */


void itemdelete(void *data)
{
  counters_t *ctrs = data;
  if (ctrs != NULL) {
    counters_delete(ctrs);
  }

}



