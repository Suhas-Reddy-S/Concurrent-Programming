/* @file quicksort.cpp */

#include "mysort.h"
#include <barrier>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdbool.h>
#include <omp.h>

using namespace std;

// Swaps 32-bit integers
void swap(uint32_t *a, uint32_t *b) {
	uint32_t t = *a;
	*a = *b;
	*b = t;
}

// Funtion to partition around the pivot, used by quick sort
int partition(uint32_t *in_arr, int l, int h) {
	uint32_t pivot = in_arr[h];

	int i = (l - 1);

	for(int j = l; j < h; j++) {
		if(in_arr[j] <= pivot) {
			i++;
			// swap if values at lower indices are less the pivot
			swap(&in_arr[i], &in_arr[j]);
		}
	} 
	// swap the pivot and return the new pivot
	swap(&in_arr[i + 1], &in_arr[h]);

	return (i + 1);
}

// Funtion to perfrom quick sort
void quick_sort_recurse(uint32_t *in_arr, int l, int h) {
	// recurssively call partition until fully sorted
	if(l < h) {
		int pivot = partition(in_arr, l, h);
		
		#ifndef PTHREAD
		#ifndef SEQ
		#pragma omp task
		#endif
		#endif
		{
			quick_sort_recurse(in_arr, l, pivot - 1);
		}
		#ifndef PTHREAD
		#ifndef SEQ
		#pragma omp task
		#endif
		#endif
		{
			quick_sort_recurse(in_arr, pivot + 1, h);
		}
	}
}

// Function for sorting in threads with synchronization using barriers
void quick_sort_thread(uint32_t* in_arr, int l, int h, std::barrier<> &bar, size_t tid) {
	bar.arrive_and_wait(); // Wait until all threads are ready
	#if DEBUG_MODE
	cout << "Thread " << tid << " Reporting to Duty" << endl;
	#endif

	quick_sort_recurse(in_arr, l, h); // Sort the subarray

	bar.arrive_and_wait(); // Wait for all threads to finish sorting before merging
	#if DEBUG_MODE
	cout << "Thread " << tid << " Quick Complete" << endl;
	#endif
}

#ifndef PTHREAD
// Parallel omp quick sort function
void quick_sort(uint32_t *in_arr, size_t len) {
	#ifndef SEQ
	cout << "Parallel Quick Sort using openmp" << endl;
	#else
	cout << "Sequential Quick Sort" << endl;
	#endif
	struct timespec startTime, endTime;
	clock_gettime(CLOCK_MONOTONIC,&startTime);

	#ifndef SEQ
	#pragma omp parallel
	#pragma omp single
	#endif
	{
		quick_sort_recurse(in_arr, 0, (int)len - 1);
	}

	clock_gettime(CLOCK_MONOTONIC,&endTime);
	unsigned long long elapsed_ns;
	elapsed_ns = (endTime.tv_sec-startTime.tv_sec)*1000000000 + (endTime.tv_nsec-startTime.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
}
#else
// Parallel pthread quick sort function
void quick_sort(uint32_t* in_arr, size_t len) {
	cout << "Parallel Quick Sort using pthread" << endl;
	struct timespec startTime, endTime;
	if (len <= 1) {
		return; 
	}

	// Barrier to synchronize thread creation and merging
	std::barrier bar(NUM_THREADS);
	std::vector<std::thread> threads(NUM_THREADS);

	int seg_size = len / NUM_THREADS;
	// int remainder = len % num_threads;

	clock_gettime(CLOCK_MONOTONIC,&startTime);
	// Fork into threads to sort individual segments
	for (size_t tid = 0; tid < NUM_THREADS; ++tid) {
		int l = tid * seg_size;
		int h = (tid == NUM_THREADS - 1) ? (len - 1) : ((tid + 1) * seg_size - 1);

		threads[tid] = thread(quick_sort_thread, in_arr, l, h, ref(bar), tid);
	}

	// Join all the threads
	for (auto& t : threads) {
		t.join();
	}

	// Merge the sorted segments sequentially
	for (size_t step = seg_size; step < len; step *= 2) {
		for (size_t l = 0; l < len; l += 2 * step) {
			size_t m = min(l + step - 1, len - 1);
			size_t h = min(l + 2 * step - 1, len - 1);
			if (m < h) {
				merge(in_arr, l, m, h);
			}
		}
	}
	clock_gettime(CLOCK_MONOTONIC,&endTime);
	
	unsigned long long elapsed_ns;
	elapsed_ns = (endTime.tv_sec-startTime.tv_sec)*1000000000 + (endTime.tv_nsec-startTime.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
	
	#if DEBUG_MODE
	cout << "Sorting Complete" << endl;
	#endif
}
#endif