# cs50 Indexer DESIGN.MD

Ross Guju
Feb, 12, 2017

## Indexer.c Layout
My indexer program makes use of 4 functions: `VerifyArguments()`
`indexBuild()` `indexSave()` and `CleanUp()`. The higher level describtion can be found in the Implementation.md 
The data structure that was designed in the indexer was a combination of the hashtable, set, and counter structures. 
There are essention 3 levels to the data structure. The hashtable that creates an unique hash tag that points to a specific location in the table array. Within the table the specific locations point to sets that have only one node; the head node. The hashtable takes a key and item. The key is the word and the item is the counters data structure. Whenever a word shows up in a different file, the a new countersnode will be created that has the document Id as the key and keeps track of the counts for the frequency of the word.

The data structure is almost completely build in the `indexBuild()` function.


