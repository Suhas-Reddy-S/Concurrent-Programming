#include "mysort.h"

// Funtion to merge segmented sub arrays, used my merge sort
void merge(sort_params_t *input) {
	uint32_t *in_arr = input->in_arr;
	int l = input->l;
	int h = input->h;
	int m = input->m;

	int i, j, k;

	int n1 = m - l + 1; // size of left subarray
	int n2 = h - m;     // size of right subarray

	uint32_t *L = new uint32_t[n1]; // Dynamically create Left sub-array
	uint32_t *R = new uint32_t[n2]; // Dynamically create right sub-array


	// Populate Left and Right sub-arrays

	for (i = 0; i < n1; i++) {
		L[i] = in_arr[l + i];
	}

	for (j = 0; j < n2; j++) {
		R[j] = in_arr[m + 1 + j];
	}

	// Reset indices
	i = 0;
	j = 0;
	k = l;

	while (i < n1 && j < n2) {
		if(L[i] <= R[j]) {
			in_arr[k] = L[i];
			i++;
		} else {
			in_arr[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1) {
		in_arr[k] = L[i];
		i++;
		k++;
	}

	while (j < n2) {
		in_arr[k] = R[j];
		j++;
		k++;
	}

}

// Function to perform merge sort
void merge_sort(sort_params_t *input) {
	if (input->l < input->h) {
		int m = input->l + (input->h - input->l) / 2;

		sort_params_t left_input = {input->in_arr, input->l, m};
		merge_sort(&left_input);

		sort_params_t right_input = {input->in_arr, m + 1, input->h};
		merge_sort(&right_input);

		sort_params_t merge_input = {input->in_arr, input->l, input->h, m};
		merge(&merge_input);
	}
}
