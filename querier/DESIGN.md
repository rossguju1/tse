# cs50 Querier DESIGN.MD

Ross Guju
Feb, 12, 2017

## querier.c Layout
~Continuation on IMPLEMENTATION.md
My querier is split into many functions to enable additional functionality while keeping the code readable. 
The struction of the program groups the functions in certain categories: 
~Verify specs: `ArgumentCheck()`, `CheckIfoperator()`, `validToken()`, `CheckCharacters()`
~Parsing: `SearchParser()`, `RemoveAndSequences()`,`RemoveAnd()`
~*and/or* Operators: `OrCombine()`, `OrComebineHelper()`, `AndCombine()`, `AndCombineHelper()`, 
~Build/Printing Querier: `BuildQuery()`, `InputCounter()`, `InputCounterHelper()`, `IndexLoad()`, `Rank()`

The rest of the functions are from common or libcs50 library. 

I'm going to discuss the constructions `BuildQuery()` and how other functions fit in it.

Once comandline arguments are verified in main, we go into `BuildQuery()` function. The build is has a *while* loop that collects each line that the user inputs. The input line are the keywords the user what to see if and where they exist in the index. 
First the input line checks that the line meets specs. Then the line is parsed and stored into an array of Tokens. We remove the *and* operator tokens from the input because two consecutive words are equvilent to an *and* sequence; also this helps with the next part.
Now that we have an array of tokens that is made up of *or* operators and word tokens to be quequed, we walk through the array, if there is an *or* operator we flag it. This allows the ability to differentiate consecutive words i.e *and* sequences so that we can apply the order operation by intersectioning the two words i.e *and* sequences. Eventually we will reduce the tokens to just *or* sequences, for examples:  *word1* *or* *word2* *or* *word3* *or* ...... 
finishing up the build, I use my `OrCombine()` function to combine all of the *or* sequences into a single counter.

