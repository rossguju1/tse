
/* counters.c Lab3
 * 
 * Ross Guju
 *
 * This is my counter module, which is sort of like the set linked list module, however, there is no item to be stored and we are
 * keeping track the 'count' for the key
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"
#include "memory.h"



/**************** file-local global variables ****************/
/* none */
/******************************************************
 *local types
 ******************************************************/

typedef struct ctrsnode {
  int key;                // search key for this item
  int count;               // Counter for number of occurances       
  struct ctrsnode *next;   // next linked list
  } ctrsnode_t;
/*********************************************************
 * Global types
 * ******************************************************/

typedef struct ctrs {
 struct ctrsnode *head;                     // head of the set
} counters_t;


/**************** global functions ****************/
/* that is, visible outside this file */
/* see counters.h for  exported functions */

/**************** local functions ****************/

static ctrsnode_t *ctrsnode_new(const int key, int count);

/********************** counters_new() ***********************/
/* Create a new (empty) counter structure; return NULL if error. */


counters_t *counters_new(void)
{
        counters_t *ctrs = count_malloc(sizeof(counters_t));
        // if ctrs is not allocated
        if ( ctrs == NULL ) {
                return NULL;
        } else {      // let the head be equal to NULL
                ctrs->head = NULL;
                return ctrs;
        }
}

/**************** ctrsnode_new() ****************/
/* Allocate and initialize a ctrsnode */
// not visible outside this file

static ctrsnode_t *ctrsnode_new(const int key, int count)
{
  ctrsnode_t *node = count_malloc(sizeof(ctrsnode_t));

    if (node == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
        // set the new ctrs node to the input parameters  
      node->key = key;
      node->count = count;
      node->next = NULL;
      return node;
    }
  }


/*********************** counters_add()*******************/
/* Increment the counter indicated by key; key must be >= 0.
 * If the key does not yet exist, create a counter for it and initialize to 1.
 * Ignore if ctrs is NULL or key is negative.
*/
void counters_add(counters_t *ctrs, const int key)
{

  if (ctrs != NULL && key >= 0) {  //Ignore if ctrs is NULL or key is negative.
    if (ctrs->head == NULL) {  //If counters has no items
      // allocate a new node to be added to the head
      ctrsnode_t *new = ctrsnode_new(key,1);
      if (new == NULL){
          return; //return if new node is not created
        }
      ctrs->head = new;
    } else {  //If counters has item
      ctrsnode_t *current = ctrs->head;
      ctrsnode_t *tailnode = ctrs->head;  //last item
     for ( ; current != NULL; current = current->next) {  //loop through all counters
        if (current-> key == key ) {  //counter with given key exists
          current->count = current->count + 1;
          return; 
        }
        //update the tail node
        if (current->next == NULL){
          tailnode = current;
        }
      } 
      //if reached here, that means given key does not exist yet
      // allocate a new node to be added to the counters
      ctrsnode_t *new = ctrsnode_new(key,1);
      if (new == NULL){
          return; //return if new node is not created
        }
      tailnode->next = new;
    }
  }


#ifdef MEMTEST
  count_report(stdout, "After counters_add");
#endif
}


/********************** counters_get() ************************/
/* Return current value of counter associated with the given key;
 * return 0 if ctrs is NULL or if key is not found.
 */

int 
counters_get(counters_t *ctrs, const int key)
{
  if (ctrs == NULL) {
    return 0; // bad counters
  } else if (ctrs->head == NULL) {
    return 0; // counters is empty
  } else if (key < 0){
    return 0; // key is negative
  } else {
    ctrsnode_t *current = ctrs->head;
        while ( current != NULL ){  //iterate through all items
          if (key == current->key){ //If key matches, return count
            return current->count;
          }
          current = current->next;
        }
        return 0; //not found
  }
}


/************************** counters_set() ************************/
/* Set the current value of counter associated with the given key;
 * If the key does not yet exist, create a counter for it and initialize to
 * the given value. Ignore if ctrs is NULL, if key < 0, or count < 0.
 */

void 
counters_set(counters_t *ctrs, const int key, int count)
{
  if (ctrs == NULL) {
    return; // bad counters
  } else if (count < 0){
    return; // count is negative
  } else if (key < 0){
    return; // key is negative
  } else if (ctrs->head == NULL) { //counters is empty
    //add the key and set its count
    counters_add(ctrs,key);
    ctrs->head->count = count;
  } else {
    ctrsnode_t *currentnode = ctrs->head;
    ctrsnode_t *tailnode = ctrs->head;
        while ( currentnode != NULL ){  //iterate through all items
          if (key == currentnode->key){ //If key matches, set the count
            currentnode->count = count;
            return;
          }
          if (currentnode->next == NULL){
            tailnode = currentnode;
          }
          currentnode = currentnode->next;
        }
        //If here, means key was not found
        //create a new node and set its count
        //add it to the end of the linkedlist
        ctrsnode_t *node = ctrsnode_new(key,count);
        if (node == NULL) {
          // error allocating memory for node; return error
          return;
        } else {
          tailnode->next = node;
        }
  }
}

/******************** counters_print***********************/

/* Print all counters; provide the output file.
 * Ignore if NULL fp. Print (null) if NULL ctrs.
 */

void counters_print(counters_t *ctrs, FILE *fp)
{
        // ignore cases when fp/ctrs == NULL
        if ( fp != NULL) {
          if (ctrs != NULL) {
                fputc('{', fp);
                // print counter set 
                for (ctrsnode_t *current = ctrs->head; current != NULL; current = current->next) {
                    fputc('(', fp);
                    fprintf(fp, "%d", current->key);
                    fputc(',', fp);
                    fprintf(fp, "%d", current->count);
                    fputc(')', fp);
                    fputc(',', fp);
                }
                fputc('}', fp);
        } else {
           fputs("(null)", fp);
        }
      }
   }


   /*********************** counters_iterate() ********************/
/* Iterate over all counters in the set (in undefined order):
 * call itemfunc for each item, with (arg, key, count).
 * If ctrs==NULL or itemfunc==NULL, do nothing.
 */

void counters_iterate(counters_t *ctrs, void *arg,
                      void (*itemfunc)(void *arg, const int key, int count))
{
        // do nothing case
        if (ctrs == NULL || itemfunc == NULL) {
                return;
        } else {
                // go through whole counter set and apply itemfunc
            for (ctrsnode_t *current = ctrs->head; current != NULL; current = current->next) {
                        (*itemfunc)(arg, current->key, current->count);
                }
            }
        }

/*********************** counters_delete() ************************/

/* Delete the whole counters. ignore NULL ctrs. */

void 
counters_delete(counters_t *ctrs)
{
  if (ctrs != NULL) {
    for (ctrsnode_t *node = ctrs->head; node != NULL;  ) {
      
      ctrsnode_t *next = node->next;     // remember what comes next
      count_free(node);         // free the node
      node = next;          // and move on to next
    }

    count_free(ctrs);
  }

#ifdef MEMTEST
  count_report(stdout, "End of counters_delete");
#endif

}

