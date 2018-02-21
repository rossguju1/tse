/* header file for pagedir.h
 *
 *Ross Guju
 *
 *
 *
 *
 */

#include <stdbool.h>


#ifndef __PAGEDIR_H
#define __PAGEDIR_H



/******************** functions *********************/

int IsDir(const char *path);

int IsFile(const char *path);

bool isCrawlerDirectory(char *dir);
 
#endif // __PAGEDIR_H
