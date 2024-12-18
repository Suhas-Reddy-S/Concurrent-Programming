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

extern size_t NUM_THREADS;

void merge_sort(uint32_t *in_arr, size_t len);
void quick_sort(uint32_t *in_arr, size_t len);
void merge(uint32_t* in_arr, int l, int m, int h);

#endif