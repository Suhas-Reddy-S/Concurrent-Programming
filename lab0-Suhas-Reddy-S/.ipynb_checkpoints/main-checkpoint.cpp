#include "mysort.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

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
            sort = merge_sort;
            cout << "Merge Sort Algorithm Selected" << endl;
        } else if((strcmp(argv[i], "--alg=quick") == 0) && !alg_selected) {
            alg_selected = true;
            sort = quick_sort;
            cout << "Quick Sort Algorithm Selected" << endl;
        }
    }
    
    // Input Error handling 
    
    if(!ip_provided) {
        cerr << "Provide an input file" << endl;
        cerr << "Usage: mysort --name -i <inputfile.txt> -o <outputfile.txt> --alg=<merge/quick>";
        return 0;
    }
    
    if(!op_provided) {
        cerr << "Provide an output file" << endl;
        cerr << "Usage: mysort --name -i <inputfile.txt> -o <outputfile.txt> --alg=<merge/quick>";
        return 0;
    }
    
    if(!alg_selected) {
        cout << "No sorting algorithm selected" << endl << "By default Merge sort algorithm will be used" << endl;
    }
    
    
    // Open input file and check if it exists
    fstream input_text;
    input_text.open(ip_file, ios::in);
    
    if ((input_text.rdstate() & ifstream::failbit ) != 0) {
        cerr << "Error opening input file, file may not exist\n";
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