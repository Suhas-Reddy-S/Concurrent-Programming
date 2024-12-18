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

void merge_sort(uint32_t *in_arr, size_t len, size_t num_threads);
void bucket_sort(uint32_t* input_array, size_t array_size, size_t k, size_t num_threads);

#endif