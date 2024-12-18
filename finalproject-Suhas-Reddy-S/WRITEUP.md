# FINAL PROJECT WRITE-UP

# CONCURRENT CONTAINERS

Submitted by- Suhas Srinivasa Reddy
Date: 13th December 2024

## Description
For the final project, concurrent containers were implemented using a Single Global Lock-based Queue and Stack, M & S Queue, and Trieber Stack. Flat Combining optimization was performed for all queues and stacks, with Elimination applied only to stacks. Additionally, the Spurious Conditional Variable Wakeup issue was also addressed.

## Code Organization

The code is written in C++ is organised as follows:

finalproject-Suhas-Reddy-S|
<br>|--main.cpp
<br>|--main.hpp
<br>|--queues.cpp
<br>|--queues.hpp
<br>|--stacks.cpp 
<br>|--stacks.hpp 
<br>|--file_copy.cpp 
<br>|--test_cases <directory>
<br>|--test_script.sh
<br>|--perf_test_script.sh
<br>|--safe_cv.cpp
<br>|--safe_cv.sh
<br>|--Makefile 

## Files Submitted and Description

<br>|--main.cpp                   : Read input file and performs copying using concurrent conatiners
<br>|--main.hpp                   : Contains necessary defines and prototypes
<br>|--queues.cpp                 : Contains all the queue definitions
<br>|--queues.hpp                 : Conatins all th queue class declarations
<br>|--stacks.cpp                 : Contains all the stack definitions
<br>|--stacks.hpp                 : Conatins all th stack class declarations
<br>|--file_copy.cpp              : Slects and creates threads for concurrent operations (Queues have fraction of threads as producers and remaining as consumers, stacks have one handle push and pop of an element)
<br>|--test_cases <directory>     : Contains test files of various sizes
<br>|--test_script.sh             : Test script to test the concurrent container operations
<br>|--perf_test_script.sh        : Test script to test the concurrent container operations
<br>|--safe_cv.cpp                : Implements wrapper for conditional varibale to acoid spurious Wakeup
<br>|--safe_cv.sh                 : Script to build and clean
<br>|--Makefile                   : Build all the source file and provides an executable

## Usage
Usage: ./concurrent_containers -i <input.txt> -o <output.txt> -t <num_threads> --container=<container_type>\n

Containers available:\n
sgl_Q\n
ms_Q\n
fc_sgl\n
fc_ms_Q\n
sgl_stk\n
t_stk\n
e_sgl_stk\n
e_t_stk\n
fc_sgl_stk\n
fc_t_stk\n\n

sgl => Single Global Lock\n
Q => Queue\n
stk => Stack\n
E => Elimination Optimization\n
fc => Flat Combining Optimization\n

## Extant Bugs
All the test cases provided have passed.
- Producer Consumer approach used to concurrently handle the queues fail every now nad then but pass eventually.

## Performance Analysis 

### Test File 2048.txt 
- 2048.txt contains 2048 unit32_t integers so the number of operations require are 4096

### Plot
![plot](https://github.com/user-attachments/assets/a45c309d-9494-49a3-be94-940d7de18bff)

### SGL Queue
| Threads | Operations   | Time     | Branch Misses |
|---------|--------------|----------|----------------|
| 1       | 250765.2749  | 0.016334 | 2.03           |
| 2       | 220511.4401  | 0.018575 | 2.13           |
| 4       | 171884.1796  | 0.02383  | 2.3            |
| 8       | 189647.1896  | 0.021598 | 2.57           |
| 16      | 169186.2867  | 0.02421  | 2.46           |
| 32      | 188842.7847  | 0.02169  | 2.14           |

### MS Queue
| Threads | Operations   | Time     | Branch Misses |
|---------|--------------|----------|----------------|
| 1       | 158956.8457  | 0.025768 | 2.19           |
| 2       | 308480.1928  | 0.013278 | 2.28           |
| 4       | 228291.1604  | 0.017942 | 2.37           |
| 8       | 320450.6337  | 0.012782 | 2.45           |
| 16      | 138922.8056  | 0.029484 | 2.56           |
| 32      | 162204.9739  | 0.025252 | 2.16           |

### FC SGL Queue
| Threads | Operations   | Time     | Branch Misses |
|---------|--------------|----------|----------------|
| 1       | 243649.9911  | 0.016811 | 1.44           |
| 2       | 186291.8998  | 0.021987 | 1.55           |
| 4       | 190866.7288  | 0.02146  | 1.75           |
| 8       | 162455.8759  | 0.025213 | 1.95           |
| 16      | 143749.5613  | 0.028494 | 2.13           |
| 32      | 117870.5036  | 0.03475  | 1.65           |

### FC MS Queue
| Threads | Operations   | Time     | Branch Misses |
|---------|--------------|----------|----------------|
| 1       | 232727.2727  | 0.0176   | 1.42           |
| 2       | 223617.4046  | 0.018317 | 1.63           |
| 4       | 292362.5981  | 0.01401  | 1.75           |
| 8       | 277412.8005  | 0.014765 | 1.83           |
| 16      | 120587.6292  | 0.033967 | 1.86           |
| 32      | 122590.686   | 0.033412 | 1.71           |

### SGL Stack
| Threads | Operations   | Time     | Branch Misses |
|---------|--------------|----------|----------------|
| 1       | 346912.8483  | 0.011807 | 1.69           |
| 2       | 224647.617   | 0.018233 | 1.73           |
| 4       | 383952.006   | 0.010668 | 1.71           |
| 8       | 404263.7189  | 0.010132 | 1.69           |
| 16      | 480018.7507  | 0.008533 | 1.71           |
| 32      | 184454.6519  | 0.022206 | 1.69           |

### Elimination SGL Stack
| Threads | Operations   | Time     | Branch Misses |
|---------|--------------|----------|----------------|
| 1       | 28251.97785  | 0.144981 | 3.64           |
| 2       | 28691.51023  | 0.14276  | 3.66           |
| 4       | 29858.57997  | 0.13718  | 3.69           |
| 8       | 26407.74696  | 0.155106 | 3.64           |
| 16      | 27155.86112  | 0.150833 | 3.7            |
| 32      | 28414.64853  | 0.144151 | 3.69           |

### FC SGL Stack
| Threads | Operations   | Time     | Branch Misses |
|---------|--------------|----------|----------------|
| 1       | 238778.1276  | 0.017154 | 1.64           |
| 2       | 269402.7887  | 0.015204 | 1.66           |
| 4       | 375263.399   | 0.010915 | 1.65           |
| 8       | 248604.0301  | 0.016476 | 1.71           |
| 16      | 180472.3299  | 0.022696 | 1.65           |
| 32      | 227227.3383  | 0.018026 | 1.68           |

### Trieber Stack
| Threads | Operations   | Time     | Branch Misses |
|---------|--------------|----------|----------------|
| 1       | 344462.1983  | 0.011891 | 2.02           |
| 2       | 415500.1014  | 0.009858 | 1.93           |
| 4       | 401726.1671  | 0.010196 | 2.02           |
| 8       | 388541.0738  | 0.010542 | 1.96           |
| 16      | 192490.2486  | 0.021279 | 2              |
| 32      | 387621.8416  | 0.010567 | 1.95           |

### Elimination SGL Stack (Repetition)
| Threads | Operations   | Time     | Branch Misses |
|---------|--------------|----------|----------------|
| 1       | 29242.73037  | 0.140069 | 4.1            |
| 2       | 25052.29422  | 0.163498 | 4.47           |
| 4       | 27489.01044  | 0.149005 | 3.76           |
| 8       | 25378.57196  | 0.161396 | 4.26           |
| 16      | 29910.25463  | 0.136943 | 4.02           |
| 32      | 27530.58207  | 0.14878  | 3.6            |

### FC SGL Stack (Repetition)
| Threads | Operations   | Time     | Branch Misses |
|---------|--------------|----------|----------------|
| 1       | 240276.8816  | 0.017047 | 1.27           |
| 2       | 259503.2945  | 0.015784 | 1.28           |
| 4       | 268678.2552  | 0.015245 | 1.28           |
| 8       | 242826.654   | 0.016868 | 1.32           |
| 16      | 235605.407   | 0.017385 | 1.31           |
| 32      | 255138.9062  | 0.016054 | 1.27           |


## Brief Discussion of Performance

### Performance:

The performance data of different queue and stack implementations, as seen in the tables, highlights how the number of threads and the type of data structure affect the operations, execution time, and branch misses.

- **SGL Queue**: The operations per second decrease as the number of threads increases, with a peak at 1 thread (250765.27 ops) and a slight decrease at 32 threads (188842.78 ops). The execution time increases slightly with more threads, but the branch misses stay relatively stable, indicating that the SGL queue scales well with thread count, though its efficiency is slightly reduced as concurrency increases.
  
- **MS Queue**: The MS queue shows a significant increase in operations per second as threads increase from 1 to 2 (158956.85 to 308480.19 ops) but then starts to level off. Notably, the execution time decreases with more threads, peaking at 8 threads (320450.63 ops). However, the branch misses also trend upward slightly with more threads, which suggests some overhead in handling branch predictions.

- **FC SGL Queue**: The FC SGL queue performs optimally with 1 thread (243649.99 ops) and starts decreasing as the thread count rises, especially at 32 threads (117870.50 ops). Despite the decrease in operations, the branch misses remain relatively low, indicating that this structure is less sensitive to the thread count than other queues.

- **FC MS Queue**: The FC MS queue performs similarly to the FC SGL Queue, showing the highest performance at 4 threads (292362.60 ops) with a drop at higher thread counts. This implementation also maintains a steady number of branch misses, but execution time increases with higher threads, suggesting inefficiencies in concurrency at higher scales.

- **SGL Stack**: The SGL stack performs the best at 32 threads (184454.65 ops), with a drop-off in operations at lower thread counts. The time per operation reduces significantly at higher thread counts, with branch misses remaining relatively constant, signaling good efficiency for multi-threaded operations.

- **Elimination SGL Stack**: This stack exhibits high execution times, especially as thread count increases, with execution times rising to over 0.15 seconds at higher thread counts. The branch misses are much higher compared to other stacks, particularly for higher threads, indicating that the elimination stack is less optimal for performance with increasing concurrency.

- **Trieber Stack**: The Trieber stack is highly efficient with 2 threads (415500.10 ops) and performs well at 32 threads (387621.84 ops). Its branch misses are consistently low, and the operations remain stable across thread counts, suggesting strong scalability and lower overhead for concurrent operations.

### Summary:
- **SGL and MS Queues**: Both benefit from moderate thread counts but show some performance degradation at higher concurrency due to increased execution time.
- **FC Queues**: Display stable performance at lower threads but drop as concurrency rises, especially for the SGL queue.
- **Stack Implementations**: The **Trieber Stack** provides the best scalability and low overhead, whereas the **Elimination SGL Stack** suffers from high execution times and branch misses, particularly at higher thread counts.

Overall, while thread concurrency improves performance in many cases, it is crucial to select the appropriate data structure based on the application’s needs, particularly considering the balance between operations, time, and branch misses.

## Challenges included:

- Task distribution of tasks between threads for concurrent operation. Producer consumer appraoch is haunting and unpredictable.
