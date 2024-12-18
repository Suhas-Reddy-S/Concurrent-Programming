# Lab 0 WRITE-UP

Submitted by- Suhas Srinivasa Reddy
Date: 27th September 2024

## Description of two parallelization strategies
### Merge Sort
- Merge sort uses a fork join implementation where the input array is segmented based on number of threads provided. Uses barrier for synchronization where all the threads start only when all the threads arrive at the barrier and exist when all the threads arrive at the second barrier. This allow merging of subarray properly withoout any overlap.
- Note: I tried decomposing the mergsort algorithm and fell into a situation where it stalls indefinetly, or termination exception. Handling these increased complexity to debug giving up on this I resorted to implementing a simpler strategy.

### Bucket Sort
- Bucket Sort uses a task segmentation based strategy. It creates tasks and pushes them into the queue. As there are fixed number of threads each thread pulls the a task from the queue. Mutex locking and condition_variable is used for thread synchronization to prevent data races. Buckets are created using map data structures and lock gaurd is used to allow only one thread to modify a bucket while inserting. Once all the elements in the in_arr are added to the buckets they are concatenated to provide a fully sorted array. 

## Code Organization

The code is written in C++ is organised as follows:

lab0-Suhas-Reddy-S|
<br>|--mergesort.cpp
<br>|--bucketsort.cpp 
<br>|--mysort.h
<br>|--mysort.cpp
<br>|--Makefile 
<br>|--custom.sh 

## File Description

### mysort.h
The "mysort.h" header contains function prototypes for merge and bucket sort along with common include files.

### mysort.cpp
The "mysort.cpp" conatins the source code which taken a text file conatining unsorted intgeres as an input and 
sorts it using either parallelized merge or bucket sort algorithms.

Based on user input the command parser sets the boolean to the appropriate algorithm, by default it is
set to merge sort. 

### mergesort.cpp
Merge sort has four functions associated with it: merge, merge_sort, merge_sort_recursive

merge_sort_recurse: Is a recurrsive function that performs merge sort same as the one implemented in lab 0.
merge: Merges the smaller sorted arrays into a large one.
merge_sort: Is a paralleized implemenation of merge sort by segmenting the input array based on num_threads and sorting them parallely which are then merged sequentially.
sorting_thread: This is a thread function that calls merge_sort_recurse to sort the provided subarray. 

### bucketsort.cpp
Bucket sort has three functions associated with it: insert, concat, bucket_sort

insert: Is an api which uses to queues data structure to insert elements into buckets.
concat: Concatenates the sorted buckets to provide a fully sorted array.
bucket_sort: This is the main function the creates buckets using map, number of buckets is sqrt of total elements. The buckets are given indiviual min values. The tasks are segementted and assigned to threads. Each task is added to the queue which is then referenced in the insert function. This demands locking using mutexes to avoid data races and provide synchronization while working parallely.

### custom.sh 
Tests the function against custom test cases

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

mysort.h: Contains function prototypes

mergesort.cpp: Contains Parallelized merge sort

bucketsort.cpp: Conatins Parallelized bucket sort

Makefile: Builds the mysort.cpp file

WRITEUP.md: This file contains the write up for lab 1.

custom.sh: Tests the function against custom test cases

output.txt: Sample output text file containing sorted integers

## Extant Bugs
All the test cases provided have passed.

My inital implemenation of merge sort which I tried decomposing completely was failing due to idle thread and bad barrier synchronization. Due to this I reworked my recursive merge sort implementation and avoided using barriers.

I have tried to cover most edge cases, I may have missed/overlooked many.

The current code doesn't handle strings in the text.


## Performance Analysis - what algorithm is faster? on what cases? why?
- It was observed that merge sort using forkjoin method performs pretty consistently even for larger valued arrays, the execution was approximately proportional to the number of elements. Whereas, bucket sort performs better when the range of values are samll i.e. if max - min is small. Its perfomance degrades drastically with increase in elements and their ranges. 
- Using Maps and queue requires more memory especially for  large datasets as comprared to merge sort.
- Merge Sort is better for general-purpose sorting where the data is varied and not uniform.
- Bucket Sort is best when the values are uniform i.e. when range is small. Skewed values degrades the performance significantly in comparission to merge sort.

```
Bucket Sort Algorithm lkbucket Selected
Bucket Sort Selected
Elapsed (ns): 676512
Elapsed (s): 0.000677
./autograde_tests/10in0-20 lkbucket ..... Pass
Bucket Sort Algorithm lkbucket Selected
Bucket Sort Selected
Elapsed (ns): 391833
Elapsed (s): 0.000392
./autograde_tests/10in1-10 lkbucket ..... Pass
Bucket Sort Algorithm lkbucket Selected
Bucket Sort Selected
Elapsed (ns): 525072
Elapsed (s): 0.000525
./autograde_tests/20in0-100 lkbucket ..... Pass
Bucket Sort Algorithm lkbucket Selected
Bucket Sort Selected
Elapsed (ns): 745151
Elapsed (s): 0.000745
./autograde_tests/256in1-10000 lkbucket ..... Pass
Bucket Sort Algorithm lkbucket Selected
Bucket Sort Selected
Elapsed (ns): 876644569
Elapsed (s): 0.876645
./autograde_tests/300000in1-300000 lkbucket ..... Pass
Bucket Sort Algorithm lkbucket Selected
Bucket Sort Selected
Elapsed (ns): 1586474422
Elapsed (s): 1.586474
./autograde_tests/550000in1-1000000skew lkbucket ..... Pass
Bucket Sort Algorithm lkbucket Selected
Bucket Sort Selected
Elapsed (ns): 44667939754
Elapsed (s): 44.667940
./autograde_tests/6200000in1-10000000 lkbucket ..... Pass
Merge Sort Algorithm fork/join Selected
Merge Sort Selected
Elapsed (ns): 448239
Elapsed (s): 0.000448
./autograde_tests/10in0-20 forkjoin ..... Pass
Merge Sort Algorithm fork/join Selected
Merge Sort Selected
Elapsed (ns): 734608
Elapsed (s): 0.000735
./autograde_tests/10in1-10 forkjoin ..... Pass
Merge Sort Algorithm fork/join Selected
Merge Sort Selected
Elapsed (ns): 579185
Elapsed (s): 0.000579
./autograde_tests/20in0-100 forkjoin ..... Pass
Merge Sort Algorithm fork/join Selected
Merge Sort Selected
Elapsed (ns): 482546
Elapsed (s): 0.000483
./autograde_tests/256in1-10000 forkjoin ..... Pass
Merge Sort Algorithm fork/join Selected
Merge Sort Selected
Elapsed (ns): 22859782
Elapsed (s): 0.022860
./autograde_tests/300000in1-300000 forkjoin ..... Pass
Merge Sort Algorithm fork/join Selected
Merge Sort Selected
Elapsed (ns): 51772140
Elapsed (s): 0.051772
./autograde_tests/550000in1-1000000skew forkjoin ..... Pass
Merge Sort Algorithm fork/join Selected
Merge Sort Selected
Elapsed (ns): 850240833
Elapsed (s): 0.850241
./autograde_tests/6200000in1-10000000 forkjoin ..... Pass
Score: 35.0 
```