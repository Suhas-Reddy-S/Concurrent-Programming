/* @Author Suhas
* Modified the code provided in the class
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <thread>
#include <vector>
#include <ctime>
#include <mutex>
#include <barrier>
#include "locks.h"


#define DEBUG 0

using namespace std;

vector<thread*> threads;
size_t NUM_THREADS = 4;
int NUM_ITERATIONS = 10;
static atomic<int> cnt(0);

/* Primitive Declaration*/
tas_lock tas_lk;
ttas_lock ttas_lk;
ticket_lock tk_lk;
peterson_lock pts_lk;
mcs_lock mcs_lk;
mutex pthread_lk;

/*MCS node decalred statically but defined dynamically later.*/
MCS_Node* my_nodes[100];

struct timespec startTime, endTime;

void lock_counter_tas(size_t tid) {

	if(tid==0) {
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}
	for(int i = 0; i < NUM_ITERATIONS; i++) {
        #if DEBUG
		printf("Thread %zu reporting for duty\n",tid);
        #endif
		tas_lk.lock();
        #if DEBUG
		printf("Thread %zu acquired the lock!\n",tid);
        #endif
		cnt++;
		tas_lk.unlock();
        #if DEBUG
		printf("Thread %zu released the unlock!\n",tid);
        #endif
	}
	if(tid==0) {
		clock_gettime(CLOCK_MONOTONIC,&endTime);
	}
}

void lock_counter_ttas(size_t tid) {

	if(tid==0) {
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}
	for(int i = 0; i < NUM_ITERATIONS; i++) {
        #if DEBUG
		printf("Thread %zu reporting for duty\n",tid);        
        #endif
		ttas_lk.lock();
        #if DEBUG
		printf("Thread %zu acquired the lock!\n",tid);
        #endif
		cnt++;
		ttas_lk.unlock();
        #if DEBUG
		printf("Thread %zu released the unlock!\n",tid);
        #endif
	}
	if(tid==0) {
		clock_gettime(CLOCK_MONOTONIC,&endTime);
	}
}

void lock_counter_tk(size_t tid) {

	if(tid==0) {
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}
	for(int i = 0; i < NUM_ITERATIONS; i++) {
        #if DEBUG
		printf("Thread %zu reporting for duty\n",tid);
        #endif
		tk_lk.lock();
        #if DEBUG
		printf("Thread %zu acquired the lock!\n",tid);
        #endif
		cnt++;
		tk_lk.unlock();
        #if DEBUG
		printf("Thread %zu released the unlock!\n",tid);
        #endif
	}
	if(tid==0) {
		clock_gettime(CLOCK_MONOTONIC,&endTime);
	}
}

void lock_counter_mcs(size_t tid) {

	if(tid==0) {
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}
	for(int i = 0; i < NUM_ITERATIONS; i++) {
        #if DEBUG
		printf("Thread %zu reporting for duty\n",tid);
		#endif
        mcs_lk.lock(my_nodes[tid]);
		#if DEBUG
        printf("Thread %zu acquired the lock!\n",tid);
		#endif
        cnt++;
		mcs_lk.unlock(my_nodes[tid]);
		#if DEBUG
        printf("Thread %zu released the unlock!\n",tid);
		#endif
	}
	if(tid==0) {
		clock_gettime(CLOCK_MONOTONIC,&endTime);
	}
}

void lock_counter_pts(size_t tid) {

	if(tid==0) {
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}
	for(int i = 0; i < NUM_ITERATIONS; i++) {
        #if DEBUG
		printf("Thread %zu reporting for duty\n",tid);
        #endif
		pts_lk.lock(tid);
        #if DEBUG
		printf("Thread %zu acquired the lock!\n",tid);
        #endif
		cnt++;
		pts_lk.unlock(tid);
        #if DEBUG
		printf("Thread %zu released the unlock!\n",tid);
        #endif
	}
	if(tid==0) {
		clock_gettime(CLOCK_MONOTONIC,&endTime);
	}
}

void lock_counter_pthread(size_t tid) {

	if(tid==0) {
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}
	for(int i = 0; i < NUM_ITERATIONS; i++) {
        #if DEBUG
		printf("Thread %zu reporting for duty\n",tid);
        #endif
		pthread_lk.lock();
        #if DEBUG
		printf("Thread %zu acquired the lock!\n",tid);
        #endif
		cnt++;
		pthread_lk.unlock();
        #if DEBUG
		printf("Thread %zu released the unlock!\n",tid);
        #endif
	}
	if(tid==0) {
		clock_gettime(CLOCK_MONOTONIC,&endTime);
	}
}

/********************************Barriers********************************/

void bar_counter_sense(size_t tid, reverse_sense_barrier &bar_sense) {

	if(tid==1) {
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}
    for(int i = 0; i < NUM_ITERATIONS; i++) {
        bar_sense.arrive_and_wait();
        #if DEBUG
        printf("Thread %zu reporting for duty\n",tid);
        #endif
		cnt++;
        bar_sense.arrive_and_wait();
        #if DEBUG
        printf("Thread %zu completed its duty\n", tid);
        #endif
    }
	if(tid==1) {
		clock_gettime(CLOCK_MONOTONIC,&endTime);
	}
	return;
}

void bar_counter_pthread(size_t tid, barrier<> &bar_pthread) {
	if(tid == 0) {
		clock_gettime(CLOCK_MONOTONIC,&startTime);
	}
    for(int i = 0; i < NUM_ITERATIONS; i++) {
        bar_pthread.arrive_and_wait();
        #if DEBUG
        printf("Thread %zu reporting for duty\n",tid);
        #endif
		cnt++;
        bar_pthread.arrive_and_wait();
        #if DEBUG
        printf("Thread %zu completed its duty\n", tid);
        #endif
    }
	if(tid == 0) {
		clock_gettime(CLOCK_MONOTONIC,&endTime);
	}
	return;
}

int main(int argc, const char* argv[]) {
	
	NUM_THREADS = 4;

	bool sense_bar = false, pthread_bar = false;

    void (*lock_thread_sel)(size_t tid);

    lock_thread_sel = lock_counter_tas;

    string op_file;

    // Command Parser
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "--name") == 0) {
            cout << "Suhas Srinivasa Reddy" << endl;
        } else if(strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            NUM_THREADS = stoi(argv[++i]);
        } else if(strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            NUM_ITERATIONS = stoi(argv[++i]);
        } else if(strcmp(argv[i], "--bar=sense") == 0) {
            sense_bar = true;
            cout << "Sense Barrier Selected" << endl;
        } else if(strcmp(argv[i], "--bar=pthread") == 0) {
            pthread_bar = true;
            cout << "Pthread Barrier Selected" << endl;
        } else if(strcmp(argv[i], "--lock=tas") == 0) {
            lock_thread_sel = lock_counter_tas;
            cout << "Test and Set Lock Selected" << endl;
        } else if(strcmp(argv[i], "--lock=ttas") == 0) {
            lock_thread_sel = lock_counter_ttas;
            cout << "Test and Test and Set Lock Selected" << endl;
        } else if(strcmp(argv[i], "--lock=ticket") == 0) {
            lock_thread_sel = lock_counter_tk;
            cout << "Ticket Lock Selected" << endl;
        }  else if(strcmp(argv[i], "--lock=mcs") == 0) {
            lock_thread_sel = lock_counter_mcs;
            cout << "MCS Lock Selected" << endl;
        }  else if(strcmp(argv[i], "--lock=peterson") == 0) {
            lock_thread_sel = lock_counter_pts;
            cout << "Peterson Lock Selected" << endl;
        }  else if(strcmp(argv[i], "--lock=pthread") == 0) {
            lock_thread_sel = lock_counter_pthread;
            cout << "Pthread Lock Selected" << endl;
        } else if(strcmp(argv[i], "-o") == 0) {
			op_file = argv[++i];
        }
    }
	
	if (lock_thread_sel == lock_counter_pts && NUM_THREADS > 2) {
		cout << "Peterson Lock works for only 2 threads, using only 2 threads" << endl;
		NUM_THREADS = 2;
	}
	
	if (lock_thread_sel == lock_counter_mcs) {
		for (int i = 0; i < (int)NUM_THREADS; i++) {
			my_nodes[i] = new MCS_Node();
		}
	}
	
	barrier<> bar_pthread(NUM_THREADS);
	// launch threads
	size_t i;
	threads.resize(NUM_THREADS);
	if (!(sense_bar || pthread_bar)) {
		for(i=0; i< NUM_THREADS; i++) {
			threads[i] = new thread(lock_thread_sel, i);
		}
	} else if(sense_bar) {
		reverse_sense_barrier bar_sense(NUM_THREADS);
		for(i=0; i< NUM_THREADS; i++) {
			threads[i] = new thread(bar_counter_sense, i, ref(bar_sense));
		}
	} else if(pthread_bar) {
		// barrier<> bar_pthread(NUM_THREADS);
		for(i=0; i< NUM_THREADS; i++) {
			threads[i] = new thread(bar_counter_pthread, i, ref(bar_pthread));
		}
	}

	// join threads
	for(size_t i=0; i<NUM_THREADS; i++){
		threads[i]->join();
		printf("joined thread %zu\n", i+1);
		delete threads[i];
	}
	
	if (lock_thread_sel == lock_counter_mcs) {
		for (int i = 0; i < (int)NUM_THREADS; i++) {
			delete my_nodes[i];
		}
	}
    ofstream output_text(op_file);

    output_text << cnt.load(RELAXED) << endl;

    output_text.close();

	printf("Count : %d\n", cnt.load(RELAXED));
	unsigned long long elapsed_ns;
	elapsed_ns = (endTime.tv_sec-startTime.tv_sec)*1000000000 + (endTime.tv_nsec-startTime.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
	double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Elapsed (s): %lf\n",elapsed_s);
}