#inlcude "mysort.h"

// Swaps 32-bit integers
void swap(uint32_t *a, uint32_t *b) {
	uint32_t t = *a;
	*a = *b;
	*b = t;
}

// Funtion to partition around the pivot, used by quick sort
int partition(sort_params_t *input) {
	uint32_t *in_arr = input->in_arr;
	int l = input->l;
	int h = input->h;

	// Set pivot
	uint32_t pivot = in_arr[h];

	int i = l - 1;

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
void quick_sort(sort_params_t *input) {
	if (input->l < input->h) {
		sort_params_t pivot_input = *input;
		int pivot = partition(&pivot_input);

		sort_params_t left_input = {input->in_arr, input->l, pivot - 1};
		quick_sort(&left_input);

		sort_params_t right_input = {input->in_arr, pivot + 1, input->h};
		quick_sort(&right_input);
	}
}