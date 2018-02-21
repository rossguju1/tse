# cs50 Querier IMPLEMENTATION.MD

Ross Guju

## Querier.c
My Querier program makes uses a variety of functions that split the querier prog
ram into components. The main components are the checking that the directories and files exist, and verify the words being searched meet the specs. The query building function, and the and/or operating functions are the last major components.
The rest of the funcitons either come from common library or are fairly simple functions such as `Rank()` which sorts the query by printing the next largest word occurrences. 
The functions that come from libcs50 or common libraries are `IndexLoad()`, `intemdelete`, and `hashtable_delete`. 
The `main()` function links the major functions together:
`ArgumentCheck()` ~> checks command line arguments
`IndexLoad()` ~> reads index file and inverts the index into hashtable data structure.
`BuildQuery()` ~> goes through the standard input utilizes the majority of the minor functions to simplify the input for querying.

## Layout of BuildQuery:
`BuildQuery` at a high level, reads from stdin, parses input into tokens. Tokens are then seperated into and/or operators and words. The words are checked for valid characters. Then the parses seperates all of the words and or/and operators, and the *and* operators are taken out. Here I implement a tagging method that after reading an *or* operator, the loop is then directed to a specific case. As the *or* operators are found and their cases are applied, the *and* sequences (where *and* has been removed so that the words are consecutive) are combined into their intersection. Once all of the ands have been combined, there should only remain *or* operators. Then the `OrCombine()` function combines all of the *or's* into a single counter.  

## Testing

I designed the `Querier` to be broken up to the most essential and concise functions that construct the program. `ArgumentsCheck()` can be easly tested through passing different cases as argument inputs in the command line. `BuildQuery()` should be tested by verifying the output of the `Querier` program. The other verifying functions such as `CheckIfoperator()`, `CheckCharacters` and `validToken()` can be tested by different input cases. The cleaing functions such as `hashtable_delete()` and `itemdelete()` can be tested by checking memory leaks using valgrind. 
