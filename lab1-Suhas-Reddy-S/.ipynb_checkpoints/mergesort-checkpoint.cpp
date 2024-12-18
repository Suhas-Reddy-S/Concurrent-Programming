/* @file mergesort.cpp */

#include "mysort.h"
#include <barrier>
#include <thread>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdbool.h>

using namespace std;

// Function to merge segmented subarrays
void merge(uint32_t* in_arr, int l, int m, int h) {
	int n1 = m - l + 1; // size of left subarray
	int n2 = h - m;     // size of right subarray

	// Dynamically create Left and Right sub-arrays
	uint32_t* L = new uint32_t[n1];
	uint32_t* R = new uint32_t[n2];

	// Populate Left and Right sub-arrays
	for (int i = 0; i < n1; i++) {
		L[i] = in_arr[l + i];
	}
	for (int j = 0; j < n2; j++) {
		R[j] = in_arr[m + 1 + j];
	}

	// Merge the subarrays back into in_arr
	int i = 0, j = 0, k = l;
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			in_arr[k++] = L[i++];
		} else {
			in_arr[k++] = R[j++];
		}
	}

	// Copy remaining elements of L[] and R[]
	while (i < n1) {
		in_arr[k++] = L[i++];
	}
	while (j < n2) {
		in_arr[k++] = R[j++];
	}

	delete[] L; // Free dynamically allocated memory
	delete[] R;
}

// Recursive merge sort function
void merge_sort_recurse(uint32_t *in_arr, int l, int h) {
	if (l < h) {
		int m = l + (h - l) / 2;
		merge_sort_recurse(in_arr, l, m);
		merge_sort_recurse(in_arr, m + 1, h);
		merge(in_arr, l, m, h);
	}
}

// Function for sorting in threads with synchronization using barriers
void sorting_thread(uint32_t* in_arr, int l, int h, std::barrier<> &bar, size_t tid) {
	bar.arrive_and_wait(); // Wait until all threads are ready
	#if DEBUG_MODE
	cout << "Thread " << tid << " Reporting to Duty" << endl;
	#endif

	merge_sort_recurse(in_arr, l, h); // Sort the subarray

	bar.arrive_and_wait(); // Wait for all threads to finish sorting before merging
	#if DEBUG_MODE
	cout << "Thread " << tid << " Merge Complete" << endl;
	#endif
}

// Parallel merge sort function
void merge_sort(uint32_t* in_arr, size_t len, size_t num_threads) {
	struct timespec startTime, endTime;
	if (len <= 1) {
		return; 
	}

	// Barrier to synchronize thread creation and merging
	std::barrier bar(num_threads);
	std::vector<std::thread> threads(num_threads);

	int seg_size = len / num_threads;
	// int remainder = len % num_threads;

	clock_gettime(CLOCK_MONOTONIC,&startTime);
	// Fork into threads to sort individual segments
	for (size_t tid = 0; tid < num_threads; ++tid) {
		int l = tid * seg_size;
		int h = (tid == num_threads - 1) ? (len - 1) : ((tid + 1) * seg_size - 1);

		threads[tid] = thread(sorting_thread, in_arr, l, h, ref(bar), tid);
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