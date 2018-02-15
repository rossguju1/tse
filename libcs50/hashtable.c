/* cs50 Lab3 
 *
 * Ross Guju
 *
 * hashtable.c module
 *
 * hashtable module that makes use of the set module. Essential this module just has an array of pointers that correspond to an index that point to a set. 
 *
 * for more detailed information hashtable.h and README discuss usage
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"
#include "set.h"
#include "jhash.h"
#include "memory.h"


/*****************************************************
 *local types
 ******************************************************/
typedef struct setnode {
  char *key;                       // search key for this item
  void *item;                      // pointer to data for this item
  struct setnode *next;            // next node
} setnode_t;                           //type casting

typedef struct set {
 struct setnode *head;       // head of the set
} set_t;


/*********************************************************
 * Global types
 * ******************************************************/

 typedef struct ht {
 int num_slots; // length of pointer table
 struct set **table; // hashtable
} hashtable_t;


/**************** global functions ****************/
/* that is, visible outside this file */
/* see hashtable.h for  exported functions */

/**************** local functions ****************/

hashtable_t *hashtable_new(const int num_slots);

/************************ hashtable_new() *****************/
/* Create a new (empty) hashtable; return NULL if error. */

 hashtable_t *hashtable_new(const int num_slots)
 {
        int i; // counter for loop
        hashtable_t *ht = count_malloc( sizeof ( hashtable_t )); // actual hashtable
        if (!ht){
                return NULL;
        }

        ht->num_slots = num_slots; // set number of pointers to num_slots

        ht->table = count_calloc(ht->num_slots, sizeof( set_t ) ); //pointers to the headnodes

        for (i = 0; i < num_slots; i++) {
                //create an empty set for all indexes in table
                ht->table[i] = set_new();
        }

        return ht;
 }

 /************************ hashtable_insert() ************************/
/* Insert item, identified by key (string), into the given hashtable.
 * The key string is copied for use by the hashtable.
 * Return false if key exists in ht, any parameter is NULL, or error;
 * return true iff new item was inserted.
 */

bool 
hashtable_insert(hashtable_t *ht, const char *key, void *item)
{
  if (ht != NULL && item != NULL && key != NULL) {
    unsigned long hash = JenkinsHash(key,ht->num_slots);

    return (set_insert( ht->table[hash], key, item));


   } else {
      return false;
   }
   
}


/*********************** hashtable_find() *********************/
/* Return the item associated with the given key;
 * return NULL if hashtable is NULL, key is NULL, key is not found.
 */

void 
*hashtable_find(hashtable_t *ht, const char *key)
{
  if (ht != NULL && key != NULL) {
    unsigned long hash = JenkinsHash(key,ht->num_slots);
    return set_find(ht->table[hash], key);

  } else {
    return NULL;
  }

}

/************************* hashtable_print() **************************/
/* Print the whole table; provide the output file and func to print each item.
 * Ignore if NULL fp. Print (null) if NULL ht.
 * Print a table with no items if NULL itemprint.
 */

void hashtable_print(hashtable_t *ht, FILE *fp,
                     void (*itemprint)(FILE *fp, const char *key, void *item))
  {
    int i;
    // ignore cases when fp/ctrs == NULL
        if ( fp != NULL) {
          if (ht != NULL) {
                fputs("~~~\n", fp);
                // print hashtable sets
                for (i = 0; i < ht->num_slots; i++) {
                if (itemprint != NULL) { // ignore itemprint == NULL
                        set_print( ht->table[i], fp, itemprint);
                }
                fputs("\n", fp);  // newline to space each set from each other
                }
                fputs("~~~", fp);
                }
                } else {
                fputs("(null)", fp);
                }
  }

/************************ hashtable_iterate() *****************/
/* Iterate over all items in the table; in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If ht==NULL or itemfunc==NULL, do nothing.
 */

void hashtable_iterate(hashtable_t *ht, void *arg,
               void (*itemfunc)(void *arg, const char *key, void *item) )
{
    int i;

    if ( ht == NULL || itemfunc == NULL) {
            // do nothing when ht == NULL or itemfunc == NULL
      return;
    }
    else {
      for (i = 0; i < ht->num_slots; i++) {
              //apply itemfunc to all nodes in the set
        set_iterate(ht->table[i], arg, itemfunc );
      }
    }
  }

/************************** hashtable_delete() ***********************/
/* Delete the whole hashtable; ignore NULL ht.
 * Provide a function that will delete each item (may be NULL).
 */

void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item))
{
    int i;
    if (ht != NULL) {
    for (i = 0; i < ht->num_slots; i++) {
      set_delete(ht->table[i], itemdelete);
   }
      // free table of arrays
      count_free(ht->table);
      //free hashtable
      count_free(ht);
    }

}






