/* Index.h- Header file for index.c
 * Header for index.c which contains functions that are shared amongst query,
 * indexer.c and crawler
 *
 * Ross Guju
 *
 * Cs50 TSE 
 *
 */
/********************** Functions() **********************/

//for loading inverted index data struction from oldIndex file
hashtable_t *IndexLoad(char *oldIndex);

// Builds index data struction from crawler directory
int indexBuild(char *Directory, char *fileName, hashtable_t *ht);

//Saves index data struction to output fileName
void indexSave(hashtable_t *ht, char *fileName);

//Item delete function used for hashtable_delete() and is used to delete counters
void itemdelete(void *data);

// printer function for hashtable interate used for printing index format
void printer(counters_t *ctrs, FILE *fp);
