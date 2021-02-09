COP4610 Project 1

Group 17:
Noal Gesler
Jacob Mayer
Dylan McClure

Labor divison:
Part 1 - Noal Gesler/Jacob Mayer
Part 2 - Noal Gesler/Jacob Mayer
Part 3 - Noal Gesler/Jacob Mayer
Part 4 - Noal Gesler/Jacob Mayer
Part 5 - Noal Gesler/Jacob Mayer
Part 6 - Noal Gesler/Jacob Mayer
Part 7 - Noal Gesler/Jacob Mayer
Part 8 - Noal Gesler/Jacob Mayer
Part 9 - Noal Gesler/Jacob Mayer
Part 10 - Noal Gesler/Jacob Mayer

The tar file contains this README.txt, a makefile, and main.c
The makefile compiles main.c into an executable command "shell", which runs the entirety of the program

Bugs/Unfinished portions:
Part 9, background processing, is very rough. Currently, it can handle small operations that finish within the next cycle,
however we found it difficult to test larger programs, and are unsure on how that operates.
In part 10, we could not find a suitable way of tracking time for some of the smaller executions (we felt very limited by C),
and thus any program under an execution time of 1 second will print out 1 second as its execution time.
Some commands will sometimes work, other times it will not. We have not found out why (Ex: cd sometimes changes CWD, sometimes doesn't).
Otherwise, however, we have not found any bugs in the rest of the program.
