/* 
 *  set.c - set module
 *
 *  Ross Guju Lab3
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"
#include "memory.h"


/**************** file-local global variables ****************/
/* none */
/******************************************************
 *local types
 ******************************************************/
typedef struct setnode {
  char *key;                       // search key for this item
  void *item;                      // pointer to data for this item
  struct setnode *next;            // next node
} setnode_t;

/************************************************i*********
 * Global types
 * ******************************************************/

typedef struct set {
 struct setnode *head;       // head of the set
} set_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static setnode_t *setnode_new(const char *key, void *item);

/********************** set_new() ***********************/

set_t *set_new(void)
{
        set_t *set =count_malloc(sizeof(set_t)); // allocate memory to *set pointer

        if ( set == NULL ) {
                return NULL;       //error allocating memory 
        } else {
                set->head = NULL;  // set first node head to be NULL
                return set;
        }
}

/************** set_insert() **************************/
/* Create a new (empty) set; return NULL if error. */

/* Insert item, identified by a key (string), into the given set.
 * The key string is copied for use by the set.
 * Return false if key exists, any parameter is NULL, or error;
 * return true iff new item was inserted.
 */

bool
set_insert(set_t *set, const char *key, void *item)
{

  if (set != NULL && item != NULL && key != NULL) {
    // allocate a new node to be added to the list
      setnode_t *new = setnode_new(key,item);
      if (new == NULL){
        return false; //return false if new node is not created
      }

      if (set->head == NULL) {  //If set has no items
        set->head = new;  //Make the new Node the head of the set 
      } else {
        setnode_t *currentNode = set->head;
        //compare key for head
        if (strcmp(currentNode->key, key) == 0 ){ //If key exists, return false
            //Also free the node
            count_free(new->key);
            count_free(new);
            return false;
          }
        while ( currentNode->next != NULL ){  //reach the end of the list
          if (strcmp(currentNode->key, key) == 0 ){ //If key exists, return false
            //Also free the node
            count_free(new->key);
            count_free(new);
            return false;
          }
          currentNode = currentNode->next; 
        }
        currentNode->next = new;
      }
      
      return true;

  } else {
    return false;
  }

#ifdef MEMTEST
  count_report(stdout, "After set_insert");
#endif
}


/********************** set_find()*************************/
/* Return the item associated with the given key;
 * return NULL if set is NULL, key is NULL, or key is not found.
 */

void *set_find(set_t *set, const char *key)
{
  if (set == NULL) {
    return NULL; // bad set
  } else if (set->head == NULL) {
    return NULL; // set is empty
  } else if (key == NULL){
    return NULL; // key is NULL
  } else {
       setnode_t *currentNode = set->head;
        while ( currentNode != NULL ){  //iterate through all items
          if (strcmp(currentNode->key, key) == 0 ){ //If key matches, return item
            return currentNode->item;
          }
          currentNode = currentNode->next;
        }
        return NULL; //not found
  }

}

        

/**************** setnode_new ****************/
/* Allocate and initialize a setnode */
// not visible outside this file

static setnode_t *setnode_new(const char *key, void *item)
{
  setnode_t *node = count_malloc(sizeof(setnode_t));

    if (node == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
    node->key = count_malloc(strlen(key)+1);
    if (node->key == NULL) {
      // error allocating memory for key; 
      // cleanup and return error
      count_free(node);
      return NULL;
    } else {
        // copy key string into node->key
      strcpy(node->key, key);
      node->item = item;
      node->next = NULL;
      return node;
    }
  }
}


/************************** set_print()*************************************/

/* Print the whole set; provide the output file and func to print each item.
 * Ignore if NULL fp. Print (null) if NULL set.
 * Print a set with no items if NULL itemprint.
*/

void
set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item) )
{
  if (fp != NULL) {
    if (set != NULL) {
        // these two if cases satisfy ignoring if fp NULL and print null if set NULL
      fputc('{', fp);
      for (setnode_t *node = set->head; node != NULL; node = node->next) {
        // print this node key
     if ( itemprint == NULL) {
        fputs(node->key, fp);
     }  else if (itemprint != NULL) {  // print the node's item 
          (*itemprint)(fp, node->key, node->item);
          fputc(',', fp);
        }
      }
      fputc('}', fp);
    } else {
      fputs("(null)", fp); // set == NULL case
    }
  }
}


/*********************** set_iterate()***********************************/

        /* Iterate over all items in the set, in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If set==NULL or itemfunc==NULL, do nothing.
 */


void set_iterate(set_t *set, void *arg,
                                void (*itemfunc)(void *arg, const char *key, void *item) )
{

        if (set == NULL || (itemfunc) == NULL) {
                // do nothing case
                return;
        } else {
                // go through set and apply itemfunc to inputs
                for (setnode_t *node = set->head; node != NULL; node = node->next) {
                        (*itemfunc)(arg, node->key, node->item);
                }
        }
}

/********************** set_delete() ************************/
/* Delete the whole set; ignore NULL set.
 * Provide a function that will delete each item (may be NULL).
 */


void 
set_delete(set_t *set, void (*itemdelete)(void *item) )
{
  if (set != NULL) {
    for (setnode_t *node = set->head; node != NULL; ) {
      if (itemdelete != NULL) {       // if possible...
        (*itemdelete)(node->item);      // delete node's item
      }
      count_free(node->key);      //delete node's key
      //node->key == NULL;
      setnode_t *next = node->next;     // remember what comes next
      count_free(node);         // free the node
      node = next;          // and move on to next
    }

    count_free(set);
    
  }
}
