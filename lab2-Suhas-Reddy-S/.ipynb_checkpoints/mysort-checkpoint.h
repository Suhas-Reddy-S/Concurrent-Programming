#ifndef MYSORT_H
#define MYSORT_H


#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <climits>
#include <condition_variable>
#include <queue>
#include <atomic>

#define DEBUG_MODE 0

using namespace std;

void bucket_sort_pthread(uint32_t* input_array, size_t array_size, size_t k, size_t num_threads);
void bucket_sort_tas(uint32_t* input_array, size_t array_size, size_t k, size_t num_threads);
void bucket_sort_ttas(uint32_t* input_array, size_t array_size, size_t k, size_t num_threads);
void bucket_sort_tk(uint32_t* input_array, size_t array_size, size_t k, size_t num_threads);
void bucket_sort_pts(uint32_t* input_array, size_t array_size, size_t k, size_t num_threads);
void bucket_sort_mcs(uint32_t* input_array, size_t array_size, size_t k, size_t num_threads);
void bucket_sort_bar_sense(uint32_t* in_arr, size_t len, size_t k, size_t num_threads);
void bucket_sort_bar_pthread(uint32_t* in_arr, size_t len, size_t k, size_t num_threads);

#endif