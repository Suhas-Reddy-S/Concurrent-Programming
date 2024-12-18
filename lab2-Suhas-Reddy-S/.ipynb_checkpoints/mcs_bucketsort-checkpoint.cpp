/* @file bucketsort.cpp in_arr
* @reference https://en.cppreference.com/w/cpp/thread/mutex
* https://en.cppreference.com/w/cpp/container/map
*/

#include "mysort.h"
#include "locks.h"

using namespace std;

mcs_lock mcs_lk;
static atomic<bool> done(false); 
MCS_Node* my_nodes[100];

// Struct for bucket sort
typedef struct bucket_sort_params {
	const uint32_t* in_arr;
	size_t start;
	size_t end;
} bucket_sort_params_t;

// Function to insert elements into buckets
static void insert(uint32_t* in_arr, map<uint32_t, uint32_t>* buckets, const uint32_t* mins, size_t k, queue<bucket_sort_params_t>& tasks, int tid) {
	while (true) {
		bucket_sort_params_t task;
		// Lock to safely access tasks queue
		mcs_lk.lock(my_nodes[tid]);
		while (tasks.empty() && !done) { // Wait if tasks are empty but not done
			mcs_lk.unlock(my_nodes[tid]);
			this_thread::yield();
			mcs_lk.lock(my_nodes[tid]);
		}
		
		// If tasks are empty and done is true, break out of loop
		if (tasks.empty() && done) {
			mcs_lk.unlock(my_nodes[tid]);
			break;
		}
		
		// Retrieve and remove the task from queue
		task = tasks.front();
		tasks.pop();
		mcs_lk.unlock(my_nodes[tid]);

		// Process the task
		for (size_t i = task.start; i < task.end; ++i) {
			uint32_t value = in_arr[i];
			for (size_t j = 1; j < k + 1; ++j) {
				if (value < mins[j]) {
					// Lock while inserting into the bucket

					mcs_lk.lock(my_nodes[tid]);
					buckets[j - 1][value]++;
					mcs_lk.unlock(my_nodes[tid]);
					break;
				}
			}
		}
	}
}

// Function to concatenate sorted buckets
static void concat(const map<uint32_t, uint32_t>* buckets, uint32_t* result, size_t k, size_t& idx) {
	for (size_t i = 0; i < k; ++i) {
		for (const auto& entry : buckets[i]) {
			for (uint32_t count = 0; count < entry.second; ++count) {
				result[idx++] = entry.first;
			}
		}
	}
}

// Parallelized bucket sort
void bucket_sort_mcs(uint32_t* in_arr, size_t len, size_t k, size_t num_threads) {
	struct timespec startTime, endTime;
	map<uint32_t, uint32_t>* buckets = new map<uint32_t, uint32_t>[k];
	uint32_t* mins = new uint32_t[k + 1];
	mins[0] = 0; 
	mins[k] = 0xFFFFFFFF; 

	// Find min and max values in the input array
	uint32_t min_value = *min_element(in_arr, in_arr + len);
	uint32_t max_value = *max_element(in_arr, in_arr + len);

	// Define bucket ranges based on min and max values
	double range = static_cast<double>(max_value - min_value) / k;
	for (size_t i = 1; i < k; ++i) {
		mins[i] = min_value + static_cast<uint32_t>(i * range);
	}

	// Task queue
	queue<bucket_sort_params_t> tasks;
	size_t segments = len / num_threads;
	for (size_t i = 0; i < num_threads; ++i) {
		size_t start = i * segments;
		size_t end = (i == num_threads - 1) ? len : (i + 1) * segments;
		tasks.push({in_arr, start, end});
	}

	for (int i = 0; i < (int)num_threads; i++) {
		my_nodes[i] = new MCS_Node();
	}
	clock_gettime(CLOCK_MONOTONIC, &startTime);

	// Thread creation
	vector<thread> threads(num_threads);
	for (size_t i = 0; i < num_threads; i++) {
		threads[i] = thread(insert, in_arr, buckets, mins, k, ref(tasks), (int)i);
		#if DEBUG_MODE
		cout << "Thread: " << i + 1 << " Reporting for duty." << endl;
		#endif
	}

	// Notify threads after task assignment
	done = true;

	// Join threads
	for (size_t i = 0; i < num_threads; ++i) {
		threads[i].join();
		#if DEBUG_MODE
		cout << "Joined Thread: " << i << endl;
		#endif
	}
	for (int i = 0; i < (int)num_threads; i++) {
		delete my_nodes[i];
	}
	// Concatenate sorted buckets
	size_t idx = 0;
	concat(buckets, in_arr, k, idx);

	clock_gettime(CLOCK_MONOTONIC, &endTime);

	// Free memory
	delete[] buckets;
	delete[] mins;

	// Calculate elapsed time
	unsigned long long elapsed_ns = (endTime.tv_sec - startTime.tv_sec) * 1000000000 + (endTime.tv_nsec - startTime.tv_nsec);
	printf("Elapsed (ns): %llu\n", elapsed_ns);
	double elapsed_s = static_cast<double>(elapsed_ns) / 1000000000.0;
	printf("Elapsed (s): %lf\n", elapsed_s);

	#if DEBUG_MODE
	cout << "Sorting Complete" << endl;
	#endif
}
