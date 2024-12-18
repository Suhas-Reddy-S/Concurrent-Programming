# Lab 1 WRITE-UP

Submitted by- Suhas Srinivasa Reddy
Date: 30th October 2024

## Description
This Lab implements six primitives using both Sequential and Release consistency. The file lock.cpp contains all the primitive decalrations and the memeory order can be selected using the macro 'REL'. REL 1 makes the primitives release consistent and REL 0 make them sequencitial. The following primitives are implemented:
- Test and Set Lock (TAS): This lock provides a simple and efficient solution but can lead to high contention under heavy loads.
- Test and Test and Set Lock (TTAS): An optimized version of TAS that minimizes contention by checking the lock status before attempting to acquire it.
- Ticket Lock: Ensures a fair queue-based approach, helpful in avoiding starvation but adds minor overhead for managing ticket counters.
- Peterson Lock: A two-thread mutual exclusion lock that’s limited in scalability but efficient for scenarios involving two threads.
- Mellor-Crummey Scott Lock (MCS): A queue-based lock designed for scalability, reducing contention by granting each thread its position in the queue.
- Reverse Sense Barrier: Used for barrier synchronization, this lock synchronizes threads, releasing all when the count is complete.

## Code Organization

The code is written in C++ is organised as follows:

lab0-Suhas-Reddy-S|
<br>|--counter.cpp
<br>|--lock.cpp
<br>|--locks.h
<br>|--bar_bucketsort.cpp 
<br>|--bar_pthread_bucketsort.cpp 
<br>|--pthread_bucketsort.cpp 
<br>|--tas_bucketsort.cpp
<br>|--ttas_bucketsort.cpp
<br>|--tas_bucketsort.cpp
<br>|--tk_bucketsort.cpp
<br>|--mcs_bucketsort.cpp
<br>|--mysort.h
<br>|--mysort.cpp
<br>|--Makefile 

## Files Submitted and Description

- counter.cpp                 : Schedules threads with locks and barrier based on user input
- lock.cpp                    : Definition of locks and barriers using Sequenctial and Release consistency memory orders. (Set REL 1 for Release Consistency and 0 for Sequential Consisyency)
- locks.h                     : Class protoypes for various concurrency primitives
- bar_bucketsort.cpp          : Bubble sort using Reverse Sense Barrier
- bar_pthread_bucketsort.cpp  : Bubble sort using Pthread Barrier
- pthread_bucketsort.cpp      : Bubble sort using Pthread lock
- tas_bucketsort.cpp          : Bubble sort using TAS lock
- ttas_bucketsort.cpp         : Bubble sort using Test and TAS lock
- pts_bucketsort.cpp          : Bubble sort using Peterson lock
- tk_bucketsort.cpp           : Bubble sort using Ticket lock
- mcs_bucketsort.cpp          : Bubble sort using MCS lock
- mysort.h                    : Bubble sort function prototypes
- mysort.cpp                  : Calls bubble sort function with locks specified by the user
- Makefile                    : Build the two files each conating a main and provides two eecutables counter and mysort.

## Extant Bugs
All the test cases provided have passed.
- Handled most bugs but, it was observed that increasing the number of iterations while keeping the number of threads same using pthread barrier takes unusally long time.

## Performance Analysis 

### Counter - Sequential Consistency

#### Case1: 2 Threads 100 Iteration

| Primitive    | Runtime (s) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|-------------|------------------|--------------------------|---------------------|
| Test and Set Lock                   | 0.021796376 | 90.18 | 96.18 | 140 |
| Test and Test and Set Lock          | 0.025203030 | 89.69 | 95.88 | 141 |
| Ticket Lock                         | 0.021451871 | 90.25 | 95.96 | 143 |
| Peterson Lock                       | 0.028325271 | 90.15 | 96.06 | 146 |
| MCS Lock                            | 0.021451871 | 90.25 | 95.96 | 143 |
| Pthread Lock                        | 0.017999201 | 90.21 | 96.16 | 143 |
| Reverse Sense Barrier               | 0.024066290 | 90.88 | 95.64 | 143 |
| Pthread Barrier                     | 0.031498534 | 90.36 | 96.15 | 144 |

#### Case2: 4 Threads 10000 Iteration

| Primitive    | Runtime (s) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|-------------|------------------|--------------------------|---------------------|
| Test and Set Lock                   | 0.027955275 | 90.94 | 98.17 | 144 |
| Test and Test and Set Lock          | 0.021679340 | 97.50 | 99.37 | 148 |
| Ticket Lock                         | 0.045054185 | 99.40 | 99.90 | 150 |
| Peterson Lock(Usesonly 2 Threads)   | 0.023434916 | 97.62 | 99.37 | 145 |
| MCS Lock                            | 0.024649173 | 97.94 | 99.44 | 143 |
| Pthread Lock                        | 0.024443212 | 93.41 | 97.71 | 145 |
| Reverse Sense Barrier               | 0.028474792 | 97.19 | 99.04 | 149 |
| Pthread Barrier                     | 0.771389182 | 91.35 | 97.90 | 150 |

#### Case3: 4 Threads 1000000 Iteration

| Primitive    | Runtime (s) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|-------------|------------------|--------------------------|---------------------|
| Test and Set Lock                   | 0.647248202 | 91.14 | 99.39 | 146 |
| Test and Test and Set Lock          | 0.594222375 | 98.38 | 99.91 | 145 |
| Ticket Lock                         | 0.653787416 | 98.11 | 99.84 | 148 |
| Peterson Lock(Usesonly 2 Threads)   | 0.764406502 | 99.40 | 99.87 | 145 |
| MCS Lock                            | 1.918096285 | 99.73 | 99.95 | 150 |
| Pthread Lock                        | 0.165495974 | 96.94 | 99.23 | 149 |
| Reverse Sense Barrier               | 1.582719800 | 98.82 | 99.80 | 154 |
| Pthread Barrier                     | 86.001094282| 91.37 | 97.95 | 151 |

### Counter - Release Consistency

#### Case1: 2 Threads 100 Iteration

| Primitive    | Runtime (s) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|-------------|------------------|--------------------------|---------------------|
| Test and Set Lock                   | 0.020891244 | 90.60 | 96.17 | 146 |
| Test and Test and Set Lock          | 0.020027938 | 90.32 | 96.18 | 141 |
| Ticket Lock                         | 0.030114846 | 90.37 | 96.32 | 145 |
| Peterson Lock                       | 0.021921786 | 90.31 | 96.28 | 144 |
| MCS Lock                            | 0.029773913 | 89.65 | 96.00 | 143 |
| Reverse Sense Barrier               | 0.031780398 | 91.89 | 97.15 | 145 |

#### Case2: 4 Threads 10000 Iteration

| Primitive    | Runtime (s) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|-------------|------------------|--------------------------|---------------------|
| Test and Set Lock                   | 0.023984088 | 93.37 | 98.57 | 146 |
| Test and Test and Set Lock          | 0.035882578 | 98.96 | 99.79 | 146 |
| Ticket Lock                         | 0.040789911 | 99.33 | 99.86 | 148 |
| Peterson Lock(Usesonly 2 Threads)   | 0.043355112 | 99.07 | 99.64 | 142 |
| MCS Lock                            | 0.029382604 | 98.88 | 99.64 | 148 |
| Reverse Sense Barrier               | 0.042702191 | 98.78 | 99.65 | 157 |

#### Case3: 4 Threads 1000000 Iteration

| Primitive    | Runtime (s) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|-------------|------------------|--------------------------|---------------------|
| Test and Set Lock                   | 0.597234323 | 93.47 | 99.36 | 145 |
| Test and Test and Set Lock          | 0.511292893 | 98.71 | 99.78 | 148 |
| Ticket Lock                         | 0.660941148 | 99.19 | 99.85 | 148 |
| Peterson Lock(Usesonly 2 Threads)   | 0.319497209 | 99.57 | 99.75 | 145 |
| MCS Lock                            | 1.521474174 | 99.41 | 99.89 | 146 |
| Reverse Sense Barrier               | 2.059966525 | 98.98 | 99.79 | 155 |

### Counter Analysis Summary

#### In the Counter benchmarks:

#### Sequential Consistency: 
- For smaller thread and iteration counts, the Pthread Lock performed efficiently with lower runtime, likely due to the internal optimizations of the pthread library. For higher iterations, MCS Lock demonstrated efficiency due to its scalability in handling multiple threads while avoiding contention. Reverse Sense Barrier and Pthread Barrier showed slower performance, likely due to the overhead associated with synchronizing barriers.
#### Release Consistency: 
- Generally, Test and Test and Set Lock and Ticket Lock offered a good balance of runtime and cache performance, especially with increased iteration counts, due to reduced contention. However, Peterson Lock was restricted to two threads, limiting its scalability. The Pthread Barrier and Reverse Sense Barrier had noticeably higher runtimes under heavy workloads due to the increased synchronization load.

- It was observed that if the number of iterations increased while the threads remain same the perfomance would degrade.
- Overall, Ticket Lock and Pthread Lock were consistently effective across tests due to their optimization for multithreading and reduced contention, while Pthread Barrier was notably slower due to synchronization overhead.


### Bubble Sort - Sequential Consistency

#### Case1: 2 Threads

| Primitive    | Runtime (s) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|-------------|------------------|--------------------------|---------------------|
| Test and Set Lock                   | 1.756998999 | 97.32 | 98.11 | 7010 |
| Test and Test and Set Lock          | 1.702708495 | 97.17 | 98.43 | 7010 |
| Ticket Lock                         | 1.681133747 | 98.21 | 98.81 | 7009 |
| Peterson Lock                       | 1.572391187 | 98.41 | 98.82 | 7011 |
| MCS Lock                            | 1.510794031 | 98.55 | 98.75 | 7010 |
| Pthread Lock                        | 2.401148039 | 96.90 | 97.87 | 7010 |
| Reverse Sense Barrier               | 1.861829309 | 97.39 | 98.33 | 7013 |
| Pthread Barrier                     | 2.079592542 | 96.62 | 97.89 | 7012 |

#### Case2: 4 Threads

| Primitive    | Runtime (s) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|-------------|------------------|--------------------------|---------------------|
| Test and Set Lock                   | 1.802220654 | 96.43 | 98.09 | 7015 |
| Test and Test and Set Lock          | 1.748869695 | 98.67 | 99.28 | 7017 |
| Ticket Lock                         | 1.832109021 | 98.76 | 99.45 | 7017 |
| Peterson Lock(Usesonly 2 Threads)   | 1.596905741 | 98.44 | 98.90 | 7011 |
| MCS Lock                            | 1.817964525 | 99.53 | 99.53 | 7018 |
| Pthread Lock                        | 2.170249522 | 95.94 | 97.88 | 7013 |
| Reverse Sense Barrier               | 2.474817010 | 96.75 | 98.88 | 7033 |
| Pthread Barrier                     | 2.105775301 | 96.20 | 97.86 | 7019 |

#### Case3: 8 Threads

| Primitive    | Runtime (s) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|-------------|------------------|--------------------------|---------------------|
| Test and Set Lock                   | 2.182564378 | 96.24 | 98.15 | 7028 |
| Test and Test and Set Lock          | 1.957024273 | 99.34 | 99.63 | 7025 |
| Ticket Lock                         | 2.236121585 | 99.60 | 99.80 | 7028 |
| Peterson Lock(Usesonly 2 Threads)   | 1.644150004 | 98.69 | 99.11 | 7007 |
| MCS Lock                            | 1.890363224 | 99.83 | 99.82 | 7029 |
| Pthread Lock                        | 2.516606643 | 94.45 | 97.81 | 7030 |
| Reverse Sense Barrier               | 2.474817010 | 96.75 | 98.88 | 7033 |
| Pthread Barrier                     | 2.644752337 | 94.08 | 97.85 | 7030 |

### Bubble Sort - Release Consistency

#### Case1: 2 Threads

| Primitive    | Runtime (s) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|-------------|------------------|--------------------------|---------------------|
| Test and Set Lock                   | 1.524065412 | 97.32 | 97.99 | 7011 |
| Test and Test and Set Lock          | 1.481059040 | 98.05 | 98.39 | 7010 |
| Ticket Lock                         | 1.632112275 | 98.16 | 98.75 | 7009 |
| Peterson Lock                       | 1.553743340 | 98.74 | 99.13 | 7013 |
| MCS Lock                            | 1.437840565 | 98.40 | 98.61 | 7011 |
| Reverse Sense Barrier               | 1.586083543 | 97.63 | 98.49 | 7011 |

#### Case2: 4 Threads

| Primitive    | Runtime (s) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|-------------|------------------|--------------------------|---------------------|
| Test and Set Lock                   | 1.777658791 | 97.04 | 98.15 | 7016 |
| Test and Test and Set Lock          | 1.705651814 | 98.46 | 99.08 | 7015 |
| Ticket Lock                         | 1.786323868 | 99.01 | 99.48 | 7019 |
| Peterson Lock(Usesonly 2 Threads)   | 1.511264479 | 98.77 | 99.20 | 7010 |
| MCS Lock                            | 1.904961062 | 99.52 | 99.56 | 7019 |
| Reverse Sense Barrier               | 1.911783407 | 96.67 | 98.30 | 7022 |

#### Case3: 8 Threads

| Primitive    | Runtime (s) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|-------------|------------------|--------------------------|---------------------|
| Test and Set Lock                   | 1.716625132 | 96.19 | 98.09 | 7029 |
| Test and Test and Set Lock          | 1.585044498 | 98.93 | 99.43 | 7028 |
| Ticket Lock                         | 2.051859015 | 99.54 | 99.77 | 7028 |
| Peterson Lock(Usesonly 2 Threads)   | 1.489561980 | 98.74 | 99.15 | 7012 |
| MCS Lock                            | 1.969892998 | 99.83 | 99.83 | 7028 |
| Reverse Sense Barrier               | 2.552132501 | 97.95 | 99.33 | 7038 |

### Bubble Sort Analysis Summary

#### In the Bubble Sort benchmarks:

#### Sequential Consistency: 
- The MCS Lock and Test and Test and Set Lock performed well with lower runtime and high cache hit rates due to efficient contention management and lower waiting time in thread synchronization. Pthread Lock generally performed slower due to high synchronization overhead. For higher thread counts, Peterson Lock was constrained by its two-thread limitation, which impacted its scalability.

#### Release Consistency: 
- MCS Lock and Ticket Lock performed best with higher cache hit rates and branch prediction accuracy, which indicates that these locks handled parallelism more efficiently. Reverse Sense Barrier consistently showed slower performance due to synchronization overhead across threads.

- In summary, MCS Lock and Test and Test and Set Lock were the most efficient for multithreaded Bubble Sort, especially with higher thread counts due to better management of contention. Conversely, Pthread Barrier and Reverse Sense Barrier showed slower performance due to synchronization overhead.

## Brief Discussion of Performance
### Performance: 
- Generally, MCS Lock and Ticket Lock outperformed others under higher thread counts due to their queue-based approaches, which manage contention more efficiently.
### Algorithm Efficiency:
- For Counter: The Pthread Lock was effective for small-scale concurrency, while MCS Lock and Ticket Lock were faster under heavier loads.
- For Bubble Sort: MCS Lock was particularly efficient, handling synchronization with reduced contention and faster completion times, especially as thread counts increased.
- Synchronization Cost: Barrier implementations, especially Pthread Barrier, were notably slower due to higher synchronization requirements, making them less suited for high-thread, high-iteration scenarios.

## Challenges included:

- Managing synchronization overhead while preserving data consistency required testing multiple lock types under various conditions.
- Implementing both SC and RC models using compiler directives and standard libraries required careful testing to ensure correct behavior.
- Ensuring the implementation could handle increasing thread counts without bottlenecking on contention required fine-tuning.
- Integrating Barriers posed a difficult challenge and required re-writing the algorithm several times.
- Handling MCS node was difficult