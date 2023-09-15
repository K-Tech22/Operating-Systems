# Operating-Systems



## Program 1

This project consists of a C or C++ program to serve as a shell interface that accepts user commands and then executes each command in a separate process.  The implementation supports input and output redirection, as well as pipes as a form of IPC between a pair of commands. This project involves the UNIX fork(), exec(), wait(), dup2(), and pipe() system calls and can be completed on any Linux, UNIX, or macOS system.



## Program 2

This assignment implements a user thread library and its round-robin scheduler, say sthread (not pthread). It exercises how to capture and restore the current execution environment with setjmp() and longjmp(), to retrieve stack and base pointers from the CPU register set with asm(), and to copy the current activation record (i.e., the stack area of the current function) into a scheduler's memory space upon a context switch to a next thread. We also use signal() and alarm() to guarantee a minimum time quantum to run the current thread.

The second part of the assignment implements a multi-threaded server over sockets. The server will return the current date to the client in a multi-threaded manner.



## Program 3

This project involves implementing several different process scheduling algorithms. The scheduler will be assigned a predefined set of tasks and will schedule the tasks based on the selected scheduling algorithm.  Each task is assigned a priority and CPU burst. The following scheduling algorithms will be implemented:

 - **First-come, first-served (FCFS**), which schedules tasks in the order in which they request the CPU.
 - **Shortest-job-first (SJF)**, which schedules tasks in order of the length of the tasks' next CPU burst.
 - **Priority scheduling**, which schedules tasks based on priority.
 - **Round-robin (RR)** scheduling, where each task is run for a time quantum (or for the remainder of its CPU burst).
 - **Priority with round-robin**, which schedules tasks in order of priority and uses round-robin scheduling for tasks with equal priority.



## Program 4

A barbershop consists of a waiting room with n waiting chairs and a barber room with one barber chair. If there are no customers to be served, the barber goes to sleep. If a customer enters the barbershop and all the waiting chairs are occupied, then the customer leaves the shop. If the barber is busy but waiting chairs are available, then the customer sits in one of the free chairs. If the barber is asleep, the customer wakes up the barber and gets served.
