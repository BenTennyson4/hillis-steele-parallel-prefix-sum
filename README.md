# hillis-steele-parallel-prefix-sum
This repository contains my implementation of the Hillis and Steele algorithm for concurrent parallel prefix sum calculation.
The program takes an array of integers (may be negative) consisting of n numbers, and computes another array B of size n such that the ith element in B is the sum of the first i elements in A.
The program accepts for arguments:
(1) the number of elements in the input array, denoted by n,
(2) the number of cores, denoted by m,
(3) the input file that contains the elements in A, denoted by A.txt, and
(4) the output file that will contain the elements in B, denoted by B.txt.
