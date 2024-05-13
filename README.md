# hillis-steele-parallel-prefix-sum
This repository contains my implementation of the Hillis and Steele algorithm for concurrent parallel prefix sum calculation.
The program takes an array of integers (may be negative) consisting of n numbers, and computes another array B of size n such that the ith element in B is the sum of the first i elements in A.
The program accepts for arguments:
(1) the number of elements in the input array, denoted by n,
(2) the number of cores, denoted by m,
(3) the input file that contains the elements in A, denoted by A.txt, and
(4) the output file that will contain the elements in B, denoted by B.txt.

Instructions:
The my-count.cpp program has a Makefile that compiles the program. To compile the program enter the "make" command, this will create an
executable file called "my-count". To run the compiled program, type "./my-count" followed by the arguments that you wish to pass to the
program. For example, "./my-count 8 4 A.txt b.txt" will pass the program a file A.txt as input. This file contains 8 elements, and divides the
work among 4 cores. b.txt is the file that the output will be written to.
