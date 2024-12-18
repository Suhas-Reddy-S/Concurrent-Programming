#ifndef MYSORT_H
#define MYSORT_H

#include <cstdint>

// Funtion Prototypes
void quick_sort(uint32_t *in_arr, int l, int h);
int partition(uint32_t *in_arr, int l, int h);
void merge_sort(uint32_t *in_arr, int l, int h);
void merge(uint32_t *in_arr, int l, int m, int h);
void swap(uint32_t *a, uint32_t *b);

endif // MYSORT_H