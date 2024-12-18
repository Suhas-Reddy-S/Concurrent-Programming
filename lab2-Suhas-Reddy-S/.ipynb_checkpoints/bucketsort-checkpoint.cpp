/* @file bucketsort.cpp in_arr
* @reference https://en.cppreference.com/w/cpp/thread/lock_guard
* https://en.cppreference.com/w/cpp/container/map
*/

#include "mysort.h"

using namespace std;

mutex lkbucket; // Mutex for bucket access
condition_variable th_sync; // Condition variable for thread synchronization
atomic<bool> done(false); 

// Struct for bucket sort
typedef struct bucket_sort_params {
	const uint32_t* in_arr;
	size_t start;
	size_t end;
} bucket_sort_params_t;

// Function to insert elements into buckets
void insert(uint32_t* in_arr, map<uint32_t, uint32_t>* buckets, const uint32_t* mins, size_t k, queue<bucket_sort_params_t>& tasks) {
	while (true) {
		bucket_sort_params_t task;
		{
			// Lock the task queue
			unique_lock<mutex> lock(lkbucket);
			if (tasks.empty()) {
				if (done) { 
					break; // Exit if no more tasks and done
				}
				th_sync.wait(lock); // Wait for new tasks
				continue;
			}
			task = tasks.front();
			tasks.pop();
		}

		// Process the task
		for (size_t i = task.start; i < task.end; ++i) {
			uint32_t value = in_arr[i];
			for (size_t j = 1; j < k + 1; ++j) {
				if (value < mins[j]) {
					// Lock while inserting
					lock_guard<mutex> lock(lkbucket);
					buckets[j - 1][value]++; // Insert element into the appropriate bucket
					break;
				}
			}
		}
	}
}

// Function to concatenate sorted buckets
void concat(const map<uint32_t, uint32_t>* buckets, uint32_t* result, size_t k, size_t& idx) {
	for (size_t i = 0; i < k; ++i) {
		for (const auto& entry : buckets[i]) {
			for (uint32_t count = 0; count < entry.second; ++count) {
				result[idx++] = entry.first;
			}
		}
	}
}

// Parallelized bucket sort
void bucket_sort(uint32_t* in_arr, size_t len, size_t k, size_t num_threads) {
	struct timespec startTime, endTime;
	// k number of bucket to hold 32 bit values
	map<uint32_t, uint32_t>* buckets = new map<uint32_t, uint32_t>[k];

	// An array to hold min values for each bucket
	uint32_t* mins = new uint32_t[k + 1];
	mins[0] = 0; 
	mins[k] = 0xFFFFFFFF; 

	// find the min and max values in the input array
	uint32_t min_value = *min_element(in_arr, in_arr + len);
	uint32_t max_value = *max_element(in_arr, in_arr + len);

	// Define bucket ranges based on min and max values
	double range = static_cast<double>(max_value - min_value) / k;
	for (size_t i = 1; i < k; ++i) {
		mins[i] = min_value + static_cast<uint32_t>(i * range);
	}

	// Task queue 
	queue<bucket_sort_params_t> tasks;             
	size_t segments = len / num_threads;           // Divide the into task segments
	for (size_t i = 0; i < num_threads; ++i) {
		size_t start = i * segments;
		size_t end = (i == num_threads - 1) ? len : (i + 1) * segments; // Handle remainder elements if they exist
		tasks.push({in_arr, start, end});          // Add tasks to queue
	}

	clock_gettime(CLOCK_MONOTONIC,&startTime);
	// Thread creation
	vector<thread> threads(num_threads);
	for (size_t i = 0; i < num_threads; ++i) {
		threads[i] = thread(insert, in_arr, buckets, mins, k, ref(tasks));
		#if DEBUG_MODE
		cout << "Thread: " << i << " Reporting for duty." << endl;
		#endif
	}

	// Notify threads after task assignment
	{
		lock_guard<mutex> lock(lkbucket);
		done = true;
	}

	// Notify all threads to check for tasks 
	th_sync.notify_all(); 

	// Thread join
	for (size_t i = 0; i < num_threads; ++i) {
		threads[i].join();
		#if DEBUG_MODE
		cout << "Joined Thread: " << i << endl;
		#endif
	}

	// Concatenate sorted buckets
	size_t idx = 0;
	concat(buckets, in_arr, k, idx);

	clock_gettime(CLOCK_MONOTONIC,&endTime);

	// Free memory
	delete[] buckets;
	delete[] mins;

	unsigned long long elapsed_ns;
	elapsed_ns = (endTime.tv_sec-startTime.tv_sec)*1000000000 + (endTime.tv_nsec-startTime.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
	
	#if DEBUG_MODE
	cout << "Sorting Complete" << endl;
	#endif
}