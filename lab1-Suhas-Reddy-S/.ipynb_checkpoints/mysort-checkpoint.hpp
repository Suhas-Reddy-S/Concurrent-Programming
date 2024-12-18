#include <cstdint>

// struct for sorting algorithms
typedef struct sort_params_t {
	uint32_t *in_arr;
	int l;
	int h;
	int m;
} sort_params_t;

// Function Prototypes 
void quick_sort(sort_params_t *input);
int partition(sort_params_t *input);
void merge_sort(sort_params_t *input);
void merge(sort_params_t *input);
void swap(uint32_t *a, uint32_t *b);