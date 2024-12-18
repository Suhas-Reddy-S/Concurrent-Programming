/*
@Author Suhas 
@file mysort.cpp
@brief Sorts a text file containing integers in ascending order and saves it to an output.txt file.
@date 11th November 2024
*/

#include <cstring>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <thread>
#include <vector>
#include <ctime>
#include <cmath>
#include <omp.h>
#include "mysort.h"

using namespace std;

/* Global Definitions */

// Global variables
size_t NUM_THREADS = 4;

int main(int argc, char *argv[]) {

	// Function Pointer to select sorting algorithm
	void (*sort)(uint32_t *in_arr, size_t len);
	
	sort = quick_sort;

	// Boolean to handle parsing commands properly 
	bool alg_selected = false, ip_provided = false, op_provided = false;

	// String variables to hold input and output file names
	string ip_file, op_file;

	// Variables to parse the file and compute len
	string line;
	int len = 0;

	if(argc > 8) {
		cout << "Too many arguments." << endl;
		cerr << "Usage: mysort --name -i <inputfile.txt> -o <outputfile.txt> --alg=<forkjoin/quick>" << endl;
	} else if(argc < 2) {
		cout << "Too less arguments." << endl;
		cerr << "Usage: mysort --name -i <inputfile.txt> -o <outputfile.txt> --alg=<forkjoin/quick>" << endl;
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
		} else if(strcmp(argv[i], "-t") == 0) {
			op_provided = true; // Set flag to indicate that output has been provided
			i++;
			NUM_THREADS = stoi(argv[i]);
			omp_set_num_threads(NUM_THREADS);
		} else if((strcmp(argv[i], "--alg=merge") == 0) && !alg_selected) {
			alg_selected = true; // Set flag to indicate that sort algorithm has been selected
			sort = merge_sort;
			cout << "Merge Sort Algorithm Selected" << endl;
		} else if((strcmp(argv[i], "--alg=quick") == 0) && !alg_selected) {
			alg_selected = true;
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
		cout << "No/wrong sorting algorithm selected." << endl << "By default Quick sort algorithm will be used" << endl;
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

	// Call Sorting
	sort(value, len);

	// Add sort values to output file
	for (int i = 0; i < len; i++) {
		output_text << value[i] << '\n';
	}

	// Close the output file
	output_text.close();

	// Free the dynamically memeory allocated 
	delete[] value;

	return 0;
}