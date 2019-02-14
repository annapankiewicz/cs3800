CS3800 - Operating Systems
==========================

## Homework 1

This assignment will emulate core functionality of a Linux-style shell with
respect to managing a file system. Your primary job is to develop data
structures and algorithms for managing files and folders such that a user
can interact with a limited set of commands via a command-line familiar to
users of Linux. You need NOT actually create files or folders or in any way
manipulate data in a real file system – this assignment focuses on the
management aspect only. Interaction should be fairly close to the standard
Linux behavior but need not be 100% identical. The core functionality
required will be documented below and highlights the creation, deletion,
and modification of files and folders.
You may choose to add additional features such as persistence (storing
the data in some way so the program can start up with the data entered
previously), populating with an initial Linux-like structure, etc. At
this point, things like users and groups can be arbitrarily assigned (you
could own everything if you like). However, the timestamps and permissions
need to be working as expected. That means the chmod command needs to
update permissions and the creation/modification (via touch) should set the
timestamp to the local system time/date.

Required commands to support:
1. `ls`
2. `ls -l`
3. `cd`
4. `pwd`
5. `mkdir`
6. `rmdir`
7. `rm`
8. `chmod`
9. `touch` (to ‘create’ a file or update the timestamp)
10. `exit/quit`

This program is to be coded in C++ and submitted in Canvas before the deadline.

## Homework 2

Your task is to do a code trace through the problem provided. Submit both the
correct code trace (similar to those done in class) and the answers to the
questions regarding the state of the system at the conclusion of your trace.
The operation of the General and Binary Semaphores should be exactly as they
appeared on the concurrency PPT slides.
