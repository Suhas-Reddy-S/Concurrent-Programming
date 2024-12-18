#include <iostream>
#include <cstring>
#include <fstream>
#include "queues.hpp"
#include "stacks.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    // Parse command-line arguments
    string ip_file;
    string op_file;
    size_t num_threads = 4;
    ContainerType container_type = FC_MS_Q; // default
	
	bool conatiner_sel = false;

    if (argc > 8) {
        cerr << "Usage: ./concurrent_containers -i <input.txt> -o <output.txt> -t <num_threads> --container=<container_type>" << endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "-i") {
            ip_file = argv[++i];
        } else if (string(argv[i]) == "-o") {
            op_file = argv[++i];
        } else if (string(argv[i]) == "-t") {
			int n = stoi(argv[++i]);
            num_threads = n > 0 ? n : 4;
        } else if (strcmp(argv[i], "--container=sgl_Q")) {
			container_type = SGL_Q;
			conatiner_sel = true;
			cout << "Single Global Lock Queue Selected\n";
		} else if (strcmp(argv[i], "--container=ms_Q")) {
			container_type = MS_Q;
			conatiner_sel = true;
			cout << "M & S Queue Selected\n";
		} else if (strcmp(argv[i], "--container=fc_sgl_Q")) {
			container_type = FC_SGL_Q;
			conatiner_sel = true;
			cout << "Flat Combining Single Global Lock Queue Selected\n";
		} else if (strcmp(argv[i], "--container=fc_ms_Q")) {
			container_type = FC_MS_Q;
			conatiner_sel = true;
			cout << "Flat Combining M & S Queue Selected\n";
		} else if (strcmp(argv[i], "--container=sgl_stk")) {
			container_type = SGL_STK;
			conatiner_sel = true;
			cout << "Single Global Lock Stack Selected\n";
		} else if (strcmp(argv[i], "--container=t_stk")) {
			container_type = T_STACK;
			conatiner_sel = true;
			cout << "Trieber Stack Selected\n";
		} else if (strcmp(argv[i], "--container=e_sgl_stk")) {
			container_type = E_SGL_STACK;
			conatiner_sel = true;
			cout << "Elimination Single Global Lock Stack Selected\n";
		} else if (strcmp(argv[i], "--container=e_t_stk")) {
			container_type = E_T_STACK;
			conatiner_sel = true;
			cout << "Elimination Trieber Stack Selected\n";
		} else if (strcmp(argv[i], "--container=fc_sgl_stk")) {
			container_type = FC_SGL_STACK;
			conatiner_sel = true;
			cout << "Flat Combining Single Global Lock Stack Selected\n";
		} else if (strcmp(argv[i], "--container=fc_t_stk")) {
			container_type = FC_T_STACK;
			conatiner_sel = true;
			cout << "Flat Combining Triberk Stack Selected\n";
        } else if(strcmp(argv[i], "--name") == 0) {
			cout << "Suhas Srinivasa Reddy" << endl;
		} else if(strcmp(argv[i], "-h") == 0) {
			cout << "Usage: ./concurrent_containers -i <input.txt> -o <output.txt> -t <num_threads> --container=<container_type>\n";
			cout << "Containers available: \nsgl_Q\nms_Q\nfc_sgl_Q\nfc_ms_Q\nsgl_stk\nt_stk\ne_sgl_stk\ne_t_stk\nfc_sgl_stk\nfc_t_stk\n";
			cout << "\nsgl => Single Global Lock\nQ => Queue\nstk => Stack\nE => Elimination Optimization\nfc => Flat Combining Optimization\n"; 
		}
    }

	if(!conatiner_sel) {
		cout << "No Container Selected\nBy Default Flat Combining M & S Queue will be used\n";
	}
    // Open input file and check if it exists
	fstream input_text;
	input_text.open(ip_file, ios::in);

	if ((input_text.rdstate() & ifstream::failbit ) != 0) {
		cerr << "Error opening input file, file may not exist" << endl;
		return 0;
	}

	int len = 0;
	string line;
	// Get number of lines in the text files
	while(getline(input_text, line)) {
		len++;
	}

	if(len < 1) {
		cerr << "File Empty" << endl;
	}

	// Create an 32 integer arrays for io 
	uint32_t* ip_arr = new uint32_t[len];
	uint32_t* op_arr = new uint32_t[len];

	// Reset cursor to begining of the file;
	input_text.clear();
	input_text.seekg (0, ios::beg);
	len = 0;

	// Convert integer in text and add to the array 
	while(getline(input_text, line)) {
		ip_arr[len] = stoull(line);
		len++;
	}

	// Close the input file
	input_text.close();
	
	// Concurrent Containers to copy files 
	struct timespec startTime, endTime;
	clock_gettime(CLOCK_MONOTONIC,&startTime);
	copyFile(ip_arr, op_arr, container_type, num_threads, len);
	clock_gettime(CLOCK_MONOTONIC,&endTime);
	unsigned long long elapsed_ns;
	elapsed_ns = (endTime.tv_sec-startTime.tv_sec)*1000000000 + (endTime.tv_nsec-startTime.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
	printf("Number of Threads: %zu\n", num_threads);
	
	// Open or create output file
	ofstream output_text(op_file);
	
	// Add sort values to output file
	for (int i = 0; i < len; i++) {
		output_text << op_arr[i] << '\n';
	}

	// Close the output file
	output_text.close();

	// Free the dynamically memeory allocated 
	delete[] ip_arr;
	delete[] op_arr;

    return 0;
}