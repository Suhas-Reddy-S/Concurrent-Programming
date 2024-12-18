/* Defination of all primitives*/

#include "locks.h"

#define REL 1 // Set 1 to use Release consistency else Sequential consistency 

#if REL
/*****************RELEASE CONSISTENCY********************************/
// Test and Set Lock
void tas_lock::lock() {
	while(!tas(flag, ACQUIRE));
}

void tas_lock::unlock() {
	flag.store(false, RELEASE);
}

// Test and Test and Set Lock
void ttas_lock::lock() {
	while(flag.load(ACQUIRE) == true || !tas(flag, ACQUIRE));  // wait for resource
}

void ttas_lock::unlock() {
	flag.store(false, RELEASE);
}

// Ticket Lock
void ticket_lock::lock() {
	int my_num = fai(next_num, 1, RELAXED);  // fetch and increment ticket
	while(now_serving.load(ACQUIRE) != my_num); // busy wait
}

void ticket_lock::unlock() {
	fai(now_serving, 1, RELEASE);
}

// Peterson Lock
void peterson_lock::init() {
	// Initiatlize 
	desires[0].store(false, RELAXED);
	desires[1].store(false, RELAXED);
	turn.store(0, RELAXED);
}

void peterson_lock::lock(int tid) {
	desires[tid].store(true, RELEASE);  // Set desire
	int other_tid = 1 - tid;
	turn.store(other_tid, RELEASE);
	
	atomic_thread_fence(SEQ_CST);  // Use fench to handle load store problem
	
	while(desires[other_tid].load(SEQ_CST) && turn.load(SEQ_CST) == other_tid) {} // Busy wait
}

void peterson_lock::unlock(int tid) {
	desires[tid].store(false, RELEASE);
}

// MCS Lock
void mcs_lock::lock(MCS_Node* &my_node) {
	my_node->next.store(nullptr, RELAXED); // Ensure next pointer is null initially
	my_node->wait.store(true, RELEASE);    // Set wait to true

	// Place this node at the end of the list
	MCS_Node* pred = tail.exchange(my_node, RELEASE);

	if (pred != nullptr) {
		// If there was a predecessor, set its next pointer to this node
		pred->next.store(my_node, RELEASE);

		atomic_thread_fence(SEQ_CST);

		// Wait until predecessor signals this node
		while (my_node->wait.load(ACQUIRE)) {}
	}
}

void mcs_lock::unlock(MCS_Node* &my_node) {
	// Check if this node is at the end of the queue
	if (!my_node->next.load(SEQ_CST)) {
		MCS_Node* expected = my_node;
		if (tail.compare_exchange_strong(expected, nullptr, RELEASE)) {
			// Tail successfully updated to nullptr, no more nodes in the queue
			return;
		}
		atomic_thread_fence(SEQ_CST);
		// Wait for successor node to appear
		while (!my_node->next.load(ACQUIRE)) {}
	}
	atomic_thread_fence(SEQ_CST);
	// Pass the lock to the next node in the queue
	my_node->next.load(ACQUIRE)->wait.store(false, ACQUIRE);
}

// Sense Reverse Barrier
void reverse_sense_barrier::arrive_and_wait() {
	// Each thread has a local sense that flips each time it arrives at the barrier
	bool local_sense = !global_sense.load(ACQUIRE);

	// Decrement the count atomically and check if this is the last thread to arrive
	if (count.fetch_sub(1, ACQUIRE) == 1) {
		// Last thread to arrive resets the count and flips the global sense
		count.store(num_threads, RELEASE); // reset count for next use
		global_sense.store(local_sense, RELEASE);
	} else {
		// Spin-wait until the global sense matches the local sense
		while (global_sense.load(ACQUIRE) != local_sense) {
			// Busy wait until the last thread flips the sense
		}
	}
}

/*****************SEQUENTIAL CONSISTENCY********************************/

#else

// Test and Set Lock
void tas_lock::lock() {
	while(!tas(flag, SEQ_CST));
}

void tas_lock::unlock() {
	flag.store(false, SEQ_CST);
}

// Test and Test and Set Lock
void ttas_lock::lock() {
	while(flag.load(SEQ_CST) == true || !tas(flag, SEQ_CST));
}

void ttas_lock::unlock() {
	flag.store(false, SEQ_CST);
}

// Ticket Lock
void ticket_lock::lock() {
	int my_num = fai(next_num, 1, RELAXED); // get ticket
	while(now_serving.load(SEQ_CST) != my_num);
}

void ticket_lock::unlock() {
	fai(now_serving, 1, SEQ_CST);
}

// Peterson lock
void peterson_lock::init() {
	desires[0].store(false, SEQ_CST);
	desires[1].store(false, SEQ_CST);
	turn.store(0, SEQ_CST);
}

void peterson_lock::lock(int tid) {
	desires[tid].store(true, SEQ_CST);  // Epress desire
	int other_tid = 1 - tid;
	turn.store(other_tid, SEQ_CST);

	while(desires[other_tid].load(SEQ_CST) && turn.load(SEQ_CST) == other_tid) {}
}

void peterson_lock::unlock(int tid) {
	desires[tid].store(false, SEQ_CST);
}

// MCS Lock
void mcs_lock::lock(MCS_Node* &my_node) {
	my_node->next.store(nullptr, SEQ_CST); // Ensure next pointer is null initially
	my_node->wait.store(true, SEQ_CST);    // Set wait to true

	// Place this node at the end of the list
	MCS_Node* pred = tail.exchange(my_node, SEQ_CST);

	if (pred != nullptr) {
		// If there was a predecessor, set its next pointer to this node
		pred->next.store(my_node, SEQ_CST);

		// Wait until predecessor signals this node
		while (my_node->wait.load(SEQ_CST)) {}
	}
}

void mcs_lock::unlock(MCS_Node* &my_node) {
	// Check if this node is at the end of the queue
	if (!my_node->next.load(SEQ_CST)) {
		MCS_Node* expected = my_node;
		if (tail.compare_exchange_strong(expected, nullptr, SEQ_CST)) {
			// Tail successfully updated to nullptr, no more nodes in the queue
			return;
		}
		// Wait for successor node to appear
		while (!my_node->next.load(SEQ_CST)) {}
	}

	// Pass the lock to the next node in the queue
	my_node->next.load(SEQ_CST)->wait.store(false, SEQ_CST);
}

// Reverse Sense Barrier
void reverse_sense_barrier::arrive_and_wait() {
	// Each thread has a local sense that flips each time it arrives at the barrier
	bool local_sense = !global_sense.load(SEQ_CST);

	// Decrement the count atomically and check if this is the last thread to arrive
	if (count.fetch_sub(1, SEQ_CST) == 1) {
		// Last thread to arrive resets the count and flips the global sense
		count.store(num_threads, SEQ_CST); // reset count for next use
		global_sense.store(local_sense, SEQ_CST);
	} else {
		// Spin-wait until the global sense matches the local sense
		while (global_sense.load(SEQ_CST) != local_sense) {
			// Busy wait until the last thread flips the sense
		}
	}
}

#endif