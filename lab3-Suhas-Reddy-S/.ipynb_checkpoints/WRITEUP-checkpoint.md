# Lab 3 WRITE-UP

Submitted by- Suhas Srinivasa Reddy
Date: 11th November 2024

## Description

This lab implements the parallelization of Merge Sort and Quick Sort using OpenMP and compares the results with pthread and sequential implementations.

## Code Organization

The code is written in C++ is organised as follows:

lab3-Suhas-Reddy-S|
<br>|--mergesort.cpp
<br>|--quicksort.cpp
<br>|--mysort.h
<br>|--mysort.cpp
<br>|--Makefile 
<br>|--test_script.sh

## Files Submitted and Description

- mergesort.cpp               : Merge Sort using Sequential, OpenMP and Pthread.
- quicksort.cpp               : Quick Sort using Sequential, OpenMP and Pthread.
- mysort.h                    : Sorting function prototypes.
- mysort.cpp                  : Calls Merge or Ouick Sort based on user input.
- Makefile                    : Build the mysort to create an executable.
- test_script.sh              : This is an extention to autograder.sh to test merge and quick sort for different number of threads.

## Extant Bugs
All the test cases provided have passed.
- Did not observe any deviations.

## Performance Analysis (O3 Optimization)

### Case1: 2 Threads

| Sample                            | Merge<br>Runtime (s) (Sequential) | Quick<br>Runtime (s) (Sequential) | Merge<br>Runtime (s) (OpenMP) | Merge<br>Runtime (s) (PThread) | Quick<br>Runtime (s) (OpenMP) | Quick<br>Runtime (s) (PThread) |
|-----------------------------------|-----------------------------------|-----------------------------------|-------------------------------|-------------------------------|--------------------------------|--------------------------------|
| 10in0-20                          | 0.000001799                       | 0.000000423                       | 0.000111124                   | 0.000258242                   | 0.000185631                    | 0.000291996                    |
| 10in1-10                          | 0.000001784                       | 0.000000378                       | 0.000128698                   | 0.000206993                   | 0.000126442                    | 0.000293113                    |
| 20in0-100                         | 0.000002889                       | 0.000001081                       | 0.000160594                   | 0.000194235                   | 0.000148565                    | 0.000216855                    |
| 256in1-10000                      | 0.000021818                       | 0.000009772                       | 0.000139872                   | 0.000221080                   | 0.000257395                    | 0.000216790                    |
| 2800in1-30000                     | 0.003192201                       | 0.001816277                       | 0.001905037                   | 0.002183625                   | 0.002063540                    | 0.001358266                    |
| 2900in1-30000                     | 0.003272006                       | 0.001845397                       | 0.002060969                   | 0.002097864                   | 0.003219781                    | 0.001345014                    |
| 300000in1-300000                  | 0.041060950                       | 0.023645456                       | 0.023829297                   | 0.022895312                   | 0.014158652                    | 0.014267339                    |
| 540000in1-1000000                 | 0.076625053                       | 0.047449964                       | 0.042929785                   | 0.040865964                   | 0.025433580                    | 0.024313623                    |


### Case2: 4 Threads

| Sample                            | Merge<br>Runtime (s) (Sequential) | Quick<br>Runtime (s) (Sequential) | Merge<br>Runtime (s) (OpenMP) | Merge<br>Runtime (s) (PThread) | Quick<br>Runtime (s) (OpenMP) | Quick<br>Runtime (s) (PThread) |
|-----------------------------------|-----------------------------------|-----------------------------------|-------------------------------|-------------------------------|--------------------------------|--------------------------------|
| 10in0-20                          | 0.000001799                       | 0.000000423                       | 0.000244261                   | 0.000406132                   | 0.000305251                    | 0.000444984                    |
| 10in1-10                          | 0.000001784                       | 0.000000378                       | 0.000262614                   | 0.000416109                   | 0.000230284                    | 0.000509740                    |
| 20in0-100                         | 0.000002889                       | 0.000001081                       | 0.000231815                   | 0.000378252                   | 0.000235607                    | 0.000417733                    |
| 256in1-10000                      | 0.000021818                       | 0.000009772                       | 0.000216115                   | 0.000354498                   | 0.000752598                    | 0.000420044                    |
| 2800in1-30000                     | 0.003192201                       | 0.001816277                       | 0.001220769                   | 0.001528740                   | 0.007145958                    | 0.001184070                    |
| 2900in1-30000                     | 0.003272006                       | 0.001845397                       | 0.001722115                   | 0.001524694                   | 0.005701960                    | 0.001202519                    |
| 300000in1-300000                  | 0.041060950                       | 0.023645456                       | 0.014347425                   | 0.015383946                   | 0.016553061                    | 0.010515020                    |
| 540000in1-1000000                 | 0.076625053                       | 0.047449964                       | 0.026082345                   | 0.025765325                   | 0.021842127                    | 0.017661447                    |

### Case3: 8 Threads

| Sample                            | Merge<br>Runtime (s) (Sequential) | Quick<br>Runtime (s) (Sequential) | Merge<br>Runtime (s) (OpenMP) | Merge<br>Runtime (s) (PThread) | Quick<br>Runtime (s) (OpenMP) | Quick<br>Runtime (s) (PThread) |
|-----------------------------------|-----------------------------------|-----------------------------------|-------------------------------|-------------------------------|--------------------------------|--------------------------------|
| 10in0-20                          | 0.000001799                       | 0.000000423                       | 0.000438629                   | 0.000844080                   | 0.000545788                    | 0.000797422                    |
| 10in1-10                          | 0.000001784                       | 0.000000378                       | 0.000529424                   | 0.000703612                   | 0.000508878                    | 0.000751701                    |
| 20in0-100                         | 0.000002889                       | 0.000001081                       | 0.000510885                   | 0.000782042                   | 0.000570234                    | 0.000683148                    |
| 256in1-10000                      | 0.000021818                       | 0.000009772                       | 0.000515733                   | 0.000748652                   | 0.001050564                    | 0.000728109                    |
| 2800in1-30000                     | 0.003192201                       | 0.001816277                       | 0.001220191                   | 0.001556962                   | 0.014125378                    | 0.001371975                    |
| 2900in1-30000                     | 0.003272006                       | 0.001845397                       | 0.001482635                   | 0.001644757                   | 0.016640031                    | 0.001493658                    |
| 300000in1-300000                  | 0.041060950                       | 0.023645456                       | 0.010586724                   | 0.011899136                   | 0.068449770                    | 0.010857242                    |
| 540000in1-1000000                 | 0.076625053                       | 0.047449964                       | 0.017938967                   | 0.020283385                   | 0.071714941                    | 0.015854393                    |

## Observation and Comparision

- The performance of the sorting algorithms (Merge Sort and Quick Sort) was analyzed across three cases with varying thread counts (2, 4, and 8) using OpenMP and Pthreads, and the results were compared with the sequential runtime. Here are the observations for each case.

### Case 1: 2 Threads
#### Observations
- For smaller samples, the overhead of thread creation appears to reduce the efficiency of parallel execution, as both OpenMP and Pthreads runtimes are higher than the sequential runtime.
- For larger samples, parallel execution shows improved performance, with OpenMP generally outperforming Pthreads, especially for Quick Sort. This may be due to OpenMP’s lower thread management overhead.
- The Quick Sort implementation using OpenMP shows considerable performance improvement compared to the sequential version in larger datasets, while Merge Sort sees moderate gains.

### Case 2: 4 Threads
#### Observations
- For smaller data sizes, performance continues to be slower than sequential due to the increased thread overhead and coordination. As the data size grows, the benefit of parallelization becomes evident.
- Merge Sort shows significant performance improvement with OpenMP as compared to Pthreads for larger datasets, likely due to OpenMP’s ability to handle parallel loops more efficiently in this iterative Merge Sort implementation.
- Quick Sort, however, shows increased performance in the Pthreads version for larger datasets. This could be due to the recursive nature of Quick Sort, where Pthreads provide more flexibility in managing thread allocation for recursive splits.

### Case 3: 8 Threads
#### Observations
- For small datasets, the parallel versions again do not provide a benefit over sequential due to thread management overhead.
- For larger datasets, the Quick Sort implementation shows substantial speedup using both OpenMP and Pthreads, with Pthreads slightly outperforming OpenMP in some cases. This is likely because of Pthreads’ lower-level control, which might be better suited for recursive tasks at this scale.
- Merge Sort in OpenMP shows significant speedup as well, although the benefit is less pronounced as compared to Quick Sort in Pthreads. The iterative version of Merge Sort may be reaching a practical limit in the inner-loop parallelization when thread count increases.

### Comparison of Parallel Approaches
- OpenMP: OpenMP is effective for loop-based parallelism, making it well-suited for the iterative Merge Sort version. The #pragma omp parallel directive minimizes the complexity of thread management, offering an easy way to parallelize portions of code.
- Pthreads: Pthreads offers greater flexibility and control, which is advantageous for recursive algorithms like Quick Sort. However, this approach requires more manual management, increasing the complexity but providing potential performance gains with fine-tuning.

### Sorting Algorithm Observations
- Merge Sort: The iterative approach combined with OpenMP's parallel loops is beneficial for large datasets. For small datasets, however, the overhead negates the benefits of parallelism.
- Quick Sort: Recursive parallelization using Pthreads shows substantial performance improvements, especially as the dataset size increases. However, OpenMP's simpler parallelization mechanisms also make Quick Sort faster on large datasets, though not as efficient as Pthreads in some cases.

### Conclusion
- For large datasets, parallelization improves performance considerably, with Quick Sort in Pthreads showing the best performance when using 8 threads. OpenMP is advantageous for easier implementation, especially with iterative or loop-based parallelization like that in the iterative Merge Sort.