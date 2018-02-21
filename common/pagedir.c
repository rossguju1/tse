/*
 *CS50- pageDirectory.c
 *
 * Functions used by cralwer and indexer
 *
 *Ross Guju
 *
 *CS50 TSE

 */



#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>                     
#include <string.h>                          
#include <dirent.h>                          
#include <sys/stat.h>
#include "pagedir.h"

/*********************** Function prototypes ******************/

int IsDir(const char *path);

int IsFile(const char *path);

bool isCrawlerDirectory(char *dir);

/*****************************************************************/

/**************** IsDir()*****************************/
 

int IsDir(const char *path)
{
    struct stat statbuf;                 
    if(!path) return 0;

    // return non-negative
    return (!stat(path, &statbuf) && S_ISDIR(statbuf.st_mode));
}

/******************* IsFile() *************************/

int IsFile(const char *path)
{
    struct stat statbuf;              
    if(!path) return 0;

    // if its a regular file
    return (!stat(path, &statbuf) && S_ISREG(statbuf.st_mode));
}

/************************* isCrawlerDirectory() ***************/
 bool isCrawlerDirectory(char *dir)
{
	char filename[] = ".crawler";

	
	char *file_path = malloc(strlen(dir) + sizeof(char) + 1);
        sprintf(file_path, "%s/%s", dir, filename);
	//try to read file given by path
	FILE *fp = fopen(file_path, "r");
	if (fp == NULL) {
		free(file_path);
		return false;
	}
	fclose(fp);
	free(file_path);
	return true;
}





