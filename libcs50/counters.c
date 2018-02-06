
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
{	// do nothing if ctrs is null or key is neg
        if ( ctrs == NULL || key < 0) {
                return; 
        } else if (ctrs->head == NULL) {
                ctrsnode_t *node = ctrsnode_new(key, 1);
                if (!node) {    //check if node mem was allocated
                        return;
                }
                ctrs->head = node;

        } else {
	       	ctrsnode_t *last_node = ctrs->head; 
		// last_node will be set to current when current->next = NULL
              for (ctrsnode_t *current = ctrs->head; current != NULL; current= current->next) {
                if ( current->key == key ) {
			// if matching key, increament 
                         current->count = current->count + 1;
                         return;
                } else if (current->next == NULL) {
			// current is located at the last node
                        last_node = current;
                }
              }
              ctrsnode_t *node = ctrsnode_new(key, 1);
              if (!node) {    // check if node mem was allocated
                   return;
              }
              last_node->next = node;
        }
}

    

/********************** counters_get() ************************/
/* Return current value of counter associated with the given key;
 * return 0 if ctrs is NULL or if key is not found.
 */

int counters_get(counters_t *ctrs, const int key)
{
        if (ctrs == NULL) { 
	//ctrs set doesnt exist
                return 0;
        } else {
        for (ctrsnode_t *current = ctrs->head; current != NULL; current = current->next) {
                   if ( current->key == key ) {
			// matching key, return count   
                        return current->count;
                     }
                }
                return 0;
        }
}
/************************** counters_set() ************************/
/* Set the current value of counter associated with the given key;
 * If the key does not yet exist, create a counter for it and initialize to
 * the given value. Ignore if ctrs is NULL, if key < 0, or count < 0.
 */

void counters_set(counters_t *ctrs, const int key, int count)
{
        if (key < 0 || count < 0 || ctrs == NULL) {
	// bad cases
                return;
        } else {
        ctrsnode_t *current = ctrs->head;
        while (current->next != NULL) {
		//matching key, set current node's count to input count
                if ( current->key == key) {
                current->count = count;
                return;
                } current = current->next;
        }  //we are at the bottom of set, make new node
         ctrsnode_t *node = ctrsnode_new(key, count);
                   if (!node) {   // check memory allocation
                          return;
                   }
                current->next = node;
                return;
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

void counters_delete(counters_t *ctrs)
{	
	// ctrs set doesn't exist
	// no memory to free
        if ( ctrs == NULL) {
                return;
        } else {
		// go through counter set
                for (ctrsnode_t *current = ctrs->head; current != NULL; ) {
                ctrsnode_t *next = current->next;
		//free current node first
                count_free(current);
                current = next;
                }
		//free ctrs set	
           count_free(ctrs);
        }
    }


