Estimation of time used: 30h
Key concepts practiced / learnt: Mutexes, POSIX threads, Signals, Pipes


Description:

Implemented to use in a project for a university class.

This is not a generalized queue, it was made purposly for project and as such works with "objects" of type "struct request *"

It was implemented to run in unix OS using the pthread and semaphore libraries.

Note: This is not the whole project. The stats_manager makes use of pipes and signals to update statistics for the rest of the project.