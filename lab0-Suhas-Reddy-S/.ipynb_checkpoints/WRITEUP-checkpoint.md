# Lab 0 WRITE-UP

Submitted by- Suhas Srinivasa Reddy
Date: 04th September 2024

## Code Organization

The code is written in C++ is organised as follows:

lab0-Suhas-Reddy-S|
|--mysort.cpp
|--Makefile 

### mysort.cpp
The "mysort.cpp" conatins the source code which taken a text file conatining unsorted intgeres as an input and 
sorts it using either merger or quick sort algorithms.

Based on user input the command parser sets the funtion pointer to the appropriate algorithm, by default it is
set to merge sort. 

It opens the input file and computes the number of lines to dynamically create an array to hold than many 32-bit 
integer values. The cursor is moved to the begining of the file to and reads every line which is converted to 
integer and added to the integer array. This array it provided as an input to the sorting algorithm. 

Quick Sort has three funtions associated with it: swap, partition, quick_sort

quick_sort: Is a recurrsive function which partitions the array about the pivot and calls itself until the entire is sorted.
partition: This function computes the pivot and returns the next pivot point.
swap: Swaps to 32-bit integers

Merger sort has two functions associated with it: merge, merge_sort

merge_sort: Is a recurrsive function which divides the large array into smaller ones and merges the induvidually sorted 
small arrays.
merge: Merges the smaller sorted arrays into a large one.

### Makefile
Uses g++ compiler to compile the mysort.cpp and provide an executable mysort.

### Error Handling
- Handles cases when there are not arguements or if there are too many arguements.
- Command parser takes care of the multiple algorithm cases, miss or wrong selection.
- Check if both input and output files are provided.
- Performs checks if the input file exists or if it is empty.


## Files Submitted

mysort.cpp: Conatins the source code which taken a text file conatining unsorted intgeres as an input and 
sorts it using either merger or quick sort algorithms.

Makefile: Builds the mysort.cpp file

WRITEUP.md: This file contains the write up for lab 0.

output.txt: Sample output text file containing sorted integers

## Extant Bugs
All the test cases provided have passed.

I have tried to cover most edge cases, I may have missed/overlooked many.

The current code doesn't handle strings in the text.
