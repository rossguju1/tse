# cs50 Indexer IMPLEMENTATION.MD

Ross Guju

## Indexer.c
My indexer program makes use of 4 functions: `VerifyArguments()`
`indexBuild()` `indexSave()` and `CleanUp()`. 
The `main()` function links all of these functions together. 
First `VerifyArguments()`  checks the commandline arguments. The arguements check whether the pageDirectory exists and is readable. Next the *indexFilename* is checked if its directory exists. The *indexFilename* will be created and if it already exists, the original file will be overwritten. 
Next the `indexBuild()` goes into the crawler file directpry and reads all of the files. We assume that the format is uniform to the crawler specs. The html content is then parsed and filtered and inputed into the hashtable/counter data structure. 
The `indexSave()` function now takes the updated hashtable data structure and prints the output to *indexFilename* 
Finally the `CleanUp()` function goes deletes the counters data structure and then deteletes the hashtable structure. 

## Testing

I designed the indexer to be broken up to the most essential and concise functions that construct the program. `VerifyArguments()` can be easly tested through unit testing. `indexBuild()` and `indexSave()` need to be tested back to back to show that the data structure was indeed built. Valgrind will show that `CleanUp()` was successful.

## Compiling
In the indexer directory is indexTest/ directory where everything for indexertest program is.

In the indexer directory, `make all` and `make clean` compiles both the indexer and indexTest directories


