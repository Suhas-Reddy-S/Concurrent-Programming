/*
@Author Suhas 
@file mysort.cpp
@brief Sorts a text file containing integers in ascending order and saves it to an output.txt file.
@date 1th September 2024
*/
#include <cstring>
#include <iostream>
#include <fstream>
#include <cstdint>

using namespace std;

// Funtion Prototypes
void quick_sort(uint32_t *in_arr, int l, int h);
int partition(uint32_t *in_arr, int l, int h);
void merge_sort(uint32_t *in_arr, int l, int h);
void merge(uint32_t *in_arr, int l, int m, int h);
void swap(uint32_t *a, uint32_t *b);

// Swaps 32-bit integers
void swap(uint32_t *a, uint32_t *b) {
  uint32_t t = *a;
  *a = *b;
  *b = t;
}

// Funtion to partition around the pivot, used by quick sort
int partition(uint32_t *in_arr, int l, int h) {
    
    // 
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
void quick_sort(uint32_t *in_arr, int l, int h) {
    // recurssively call partition until fully sorted
    if(l < h) {
        int pivot = partition(in_arr, l, h);
        quick_sort(in_arr, l, pivot - 1);
        quick_sort(in_arr, pivot + 1, h);
    }
}

// Funtion to merge segmented sub arrays, used my merge sort
void merge(uint32_t *in_arr, int l, int m, int h) {
    
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
void merge_sort(uint32_t *in_arr, int l, int h) {
    // Recurssively sort and merge until fully sorted
    if (l < h) {
        int m = l + (h - l)/2;
        merge_sort(in_arr, l, m);
        merge_sort(in_arr, m+1, h);
        merge(in_arr, l, m, h);
    }
}

int main(int argc, char *argv[]) {
    // Boolean to handle parsing commands properly 
    bool alg_selected = false, ip_provided = false, op_provided = false;
    
    // String variables to hold input and output file names
    string ip_file, op_file;
    
    // Funtion pointer to select sorting algorithm effectively
    void (*sort)(uint32_t *in_arr, int l, int h);
    
    // Set default sort algorithm as merge sort
    sort = merge_sort;
    
    // Variables to parse the file and compute len
    string line;
    int len = 0;
    
    if(argc > 7) {
        cout << "Too many arguments." << endl;
        cerr << "Usage: mysort --name -i <inputfile.txt> -o <outputfile.txt> --alg=<merge/quick>" << endl;
    } else if(argc < 2) {
        cout << "Too less arguments." << endl;
        cerr << "Usage: mysort --name -i <inputfile.txt> -o <outputfile.txt> --alg=<merge/quick>" << endl;
    }
    
    // Command Parser
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "--name") == 0) {
            cout << "Suhas Srinivasa Reddy" << endl;
        } else if(strcmp(argv[i], "-i") == 0) {
            ip_provided = true; // Set flag to indicate that input has been provided
            i++;
            ip_file = argv[i];
        } else if(strcmp(argv[i], "-o") == 0) {
            op_provided = true; // Set flag to indicate that output has been provided
            i++;
            op_file = argv[i];
        } else if((strcmp(argv[i], "--alg=merge") == 0) && !alg_selected) {
            alg_selected = true; // Set flag to indicate that sort algorithm has been selected
            sort = merge_sort;   // Set function pointer to merge
            cout << "Merge Sort Algorithm Selected" << endl;
        } else if((strcmp(argv[i], "--alg=quick") == 0) && !alg_selected) {
            alg_selected = true;
            sort = quick_sort;  // Set function pointer to merge
            cout << "Quick Sort Algorithm Selected" << endl;
        }
    }
    
    // Input Error handling 
    
    if(!ip_provided) {
        cerr << "Provide an input file" << endl;
        cerr << "Usage: mysort --name -i <inputfile.txt> -o <outputfile.txt> --alg=<merge/quick>" << endl;
        return 0;
    }
    
    if(!op_provided) {
        cerr << "Provide an output file" << endl;
        cerr << "Usage: mysort --name -i <inputfile.txt> -o <outputfile.txt> --alg=<merge/quick>" << endl;
        return 0;
    }
    
    if(!alg_selected) {
        cout << "No/wrong sorting algorithm selected." << endl << "By default Merge sort algorithm will be used" << endl;
    }
    
    
    // Open input file and check if it exists
    fstream input_text;
    input_text.open(ip_file, ios::in);
    
    if ((input_text.rdstate() & ifstream::failbit ) != 0) {
        cerr << "Error opening input file, file may not exist" << endl;
        return 0;
    }
    
    // Open or create output file
    ofstream output_text(op_file);
    
    // Get number of lines in the text files
    while(getline(input_text, line)) {
        len++;
    }
    
    if(len < 1) {
        cerr << "File Empty" << endl;
    }

    // Create an 32 integer array to hold all the integer values compatible with the sorting alg
    uint32_t* value = new uint32_t[len];
    
    // Reset cursor to begining of the file;
    input_text.clear();
    input_text.seekg (0, ios::beg);
    len = 0;
    
    // Convert integer in text and add to the array 
    while(getline(input_text, line)) {
        value[len] = stoi(line);
        len++;
    }
    
    // Close the input file
    input_text.close();
   
    // Call sorting algorithm using a function pointer
    sort(value, 0, len - 1);
    
    // Add sort values to output file
    for (int i = 0; i < len; i++) {
        output_text << value[i] << endl;
    }
    
    // Close the output file
    output_text.close();
    
    // Free the dynamically memeory allocated 
    delete value;
    
    return 0;
}