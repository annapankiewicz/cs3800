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

## Homework 3
The sleeping barbers have fallen so deeply asleep that their program no longer
functions! The good news is the "heavy lifting" is done in that the code is
provided and follows the code trace you worked previously with binary and
general semaphores. The bad news is it is not running to completion. :(

Adjust the code so the 2-barber, 4-customer setup runs to completion as
expected under any interleaving. To help, you are shown an area
-- ONLY MAKE CHANGES BELOW THIS LINE -- which helps show you where we
expect the fix to be happening. (And nothing should be changed above that
line - duh.)

## Homework 4
When implemented, these commands should complement the core functionality of
the file system commands you have already implemented. When users do not have
read permissions, the files/directories should no longer appear. When users
do not have write permissions, they should not be able to rm/rmdir the files.
When users do not have execute permissions they should not be able to ./ the
file. Note: ./ functionality can be as easy as printing “[file_name]
ran/executed.” For each of the RWX permissions, the most permissive
available should be used. In other words, if the group permissions prohibited
reading but the owner permissions gave read permissions, the user would be
allowed to read. Likewise, if a user was part of the group where group
permissions allowed reading but was not the owner and global reading was not
allowed, the files/folders should still be visible (by being in the group
that held the permissions).

You can check Linux Man pages for the usage or see the examples in the
detailed discussion for each command. The required commands to support in
this iteration are:
1. `useradd`
2. `useradd -G`
3. `usermod -a -G`
4. `chown`
4. `chgrp`
5. `./` (execute)
6. `groups`
7. `groupadd`
8. `userdel`
9. `userdel -G`

In addition to the standard Linux-based commands, we need a way to switch
users. Implement a new switchto command which will switch to a different user.

This program is to be coded in C++ and submitted in Canvas before the deadline.
This is an individual assignment – each person should produce and submit their
own code solution.

## Homework 5

This program will mimic the safe interaction of shared resources utilizing the
“dirty/clean fork” algorithm found with the dining philosopher problem.
There will be a given number of programs executing concurrently (the number
will be specified at runtime and could change with each run). We will refer to
each process as Phil (five executing concurrently would be Phil 0, Phil 1, ...
P3, P4). The purpose of each Phil is to generate some text and write it to a
file. Because Phils are overly skeptical, they will only write their data if
they can simultaneously write it out to TWO separate (but specific!) files
(which ensures the data will be safe). Phil 0 will write to File 0 and File 1.
Phil 1 will write to File 1 and File 2. ... Phil N (the last Phil) will write
to File N and File 0. Clearly there is a possibility Phil 0 and Phil 1 could
attempt to write to File 1 at the same time (this is the critical resource
whose safe access we need to ensure). Each Phil will be coded with a sequence
of “generating” (simulating creation of content), followed by the desire to
“write” (use the shared resource to output the generation into the files).
Each Phil needs to keep track of their file’s “status” (i.e., which of the
Phils can write to that file) and can only write when they have access to
*BOTH* of their associated files. To simplify things, each file will be
assigned to a Phil (I’ll call it “available” to that Phil) – with all
files initially assigned to the highest-numbered Phil (so Phil N has File
0 and File N “available”, Phil 0 has File 0 and File 1 both “unavailable”,
etc.).

Additionally, each file’s status will contain a “used” indicator which will
help prevent starvation – all initially set to “used”. After a Phil has
“generated” something to output, it checks to see if its two files are
“available” and will write out when that is the case. If file(s) are
unavailable, messages will be sent requesting the unavailable file(s) and
Phil will have to wait until it receives (a) message(s) back stating the
file(s) are available. When a file’s “availability” changes from one Phil
to another, the receiving Phil will update their local status to “unused”.
When a Phil writes out to the files, the files’ status will be updated to
“used” (indicating it has had a chance to “use” the files it was requesting).
When a message request is received by Phil x, if the file is available and
used, Phil x will mark the file as unavailable and alert the sender the file
is free (available for the sender to write to). If the file is currently marked
as “unused” Phil will wait until their next file write has finished and then
alert the sender the file is free.
