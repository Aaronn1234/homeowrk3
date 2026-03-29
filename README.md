# Homework 3
## Execution
The program consists of one .c file titled hmwk3.c  

To compile:  
gcc hmwk3.c -o hmwk3

To execute:  
./hmwk3
## Description
The program generates data with random integers in batches of 30, 1000, 6000, and 10000 and stores these numbers in separate text files. The time to generate each batch is then recorded as Part 1. The rest of the data is timed by batch size, ordered smallest to largest. The data is first sorted from low to high and stored in new text files for each size. The data is then searched using both linear search and binary search, with the time to execute recorded. This is followed by executing and recording the times to create both random and sorted binary search trees, to search said trees, and to perform both iterative and recursive insertion. All times are recorded in seconds in the console.
