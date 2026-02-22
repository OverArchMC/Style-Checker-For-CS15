********************************************************************************
|                         CS15 Style Checker README                            |
********************************************************************************

The Project
-----------

Inpired by the all too farmiliar valgrind output, our team is called, "Invalid
Read @ JCC." Our goal was to design a tool that could make the transition from
CS11 to the rigid CS15 style guide a little easier. We hoped to accomplish this
by making a website to upload files and check their style with many tests
including the 80 character limit for lines, 30 line limit for functions,
spacing around operators, the presence of function and file contracts, inline
loops, and many many more. We accomplished all of our initial goals, and in
implementing the style checks, we discovered many more subtle details in the
style guide that we didn't know about.

The Team
--------

All: Helped other members with editing functions and finding edge cases whenever
help was needed

Bezawit Gessesse:
 * Wrote functions checking for global variables
 * File headers
 * Break statements
 * 80-character line lengths
 * Merge conflict resolving…
 * Lots of luck! 4 bobas and 1 hat from the giveaways.
Joey:
 * Made frontend website
 * Checker functions indentation
 * argumentSpacing
 * operatorSpacing
 * singleLineLoop
 * Suggested taking a break and a very nice walk
Kyra McCandless:
 * Inspiration for project idea
 * Organization in Github
 * File reading and writing
 * Keeping track of requirements, todos
 * Parsing functions from text and checking length
 * Debugging/searching for test cases
 * Merge conflict resolution
Sylvia:
 * Wrote functions checking for std namespace
 * Class members declared public
 * Single line loops
 * Came up with edge cases
 * Fixed resulting errors in functions from that
 * Provided frequent lunch countdown updates

Acknowledgements
----------------

Gemini (Google)
 * Help with navigating Github via command line
 * Help with website requirements
 * Help with website
Claude (Anthropic)
 * Help with regular expressions detecting first lines of functions
 * Help finding a substitute for valgrind on MacOS
 * Line length function editing (help on how to embed inline comments in file)
 * Help with .find function logic

Reflection:
-----------

We all had trouble adjusting to the rigid CS15 style guide compared to the more
flexible CS11 style guide. Having a style checker would make completing
homeworks more efficient and ensure nothing gets missed. Hopefully, we will be
able to add more of the style guide requirements and work on the efficiency of
our program.

We were initially puzzled by recognizing the start and end of functions.
Together, we brainstormed things that made function declaration lines unique,
like the return types, the arguments, the curly brackets, and more. We also
debugged this functionality on function-declaration-lookalikes like if
statements, while loops, and for loops.

We went on a walk together and managed to navigate the area without getting
lost (mostly). We also bonded over boba.


