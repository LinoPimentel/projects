Estimation of time used for this: 35 hours
Key concepts practiced / learnt: C++ templates and lambdas


Description:

This was for a university class project.
The objective was to develop 3 different ways of storing the data under "dados.csv" with data structures.
Any structure between: linked lists, trees, queues or stacks would have been acceptable. Everything else we got to decide how to implement it.

So I used that chance to practice my C++, mainly templates and lambdas.

I implemented linked lists, AVL trees and Binary trees with templates, taking any time through <typename T> and recieving a reference to an object,
in the constructor implementing a preset interface to know how to de-alloc, compare, and edit data of type T.

It was also required to implement a big set of operations when searching or removing elements. So I also took the oportunity and instead of 
implementing a function for each of them in every data structure, I implemented 1 method that would run an action std::function passed by argument
in every node. Allowing me to do anything I'd want in the data structure by declaring a lambda when required.


Since it was the first time working with templates, i wasn't aware that usually the implementation for a template method is done in the .h due to linker problems,
so instead of that, i had to include the .cpp whenever i wanted to use a class<T>.

The full code is here and it can be compiled using visual studio 2015 (other compilers weren't tested). It provides a command line interface to try out the features.


