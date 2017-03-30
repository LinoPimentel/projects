Estimation of time used: 50h
Key concepts practiced / learnt: Java Threads, better code structure, TCP / UDP connections, programming Interfaces, RMI.


Description:

The source code in here was part of a university project between 2 members. The project included having a logic server,
a TCP server, communicating through java RMI.
Only the source code developed by Lino is included in here.

The server works with a 1 thread per client structure. Makes use of java's thread pool instead of explicitly creating threads.
The project required that multiple instances of the TCP servers could be on, and they communicate their load to each other
through UDP multicast.