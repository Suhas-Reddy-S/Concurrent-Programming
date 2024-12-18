#ifndef LOCKS_H
#define LOCKS_H

#include <atomic>
#include <thread>

using namespace std;

#define SEQ_CST memory_order_seq_cst
#define RELAXED memory_order_relaxed
#define RELEASE memory_order_release
#define ACQUIRE memory_order_acquire

class MCS_Node {
public:
	atomic<MCS_Node*> next;
	atomic<bool> wait;
	MCS_Node() : next(nullptr), wait(false) {}
};

bool tas(atomic<bool> &x, memory_order MEM);
int fai(atomic<int> &x, int val, memory_order MEM);
bool cas(atomic<MCS_Node*> &x, MCS_Node* expected, MCS_Node* desired, memory_order MEM);

class tas_lock {
	atomic<bool> flag = false;
public:
	void lock();
	void unlock();
};

class ttas_lock {
	atomic<bool> flag = false;
public:
	void lock();
	void unlock();
};

class ticket_lock {
	atomic<int> next_num = 0;
	atomic<int> now_serving = 0;
public:
	void lock();
	void unlock();
};

class peterson_lock {
	atomic<bool> desires[2];
	atomic<int> turn;
public:
	void init();
	void lock(int tid);
	void unlock(int tid);
};

class mcs_lock {
	atomic<MCS_Node*> tail = nullptr;
public:
	void lock(MCS_Node* &my_node);
	void unlock(MCS_Node* &my_node);
};

class reverse_sense_barrier {
public:
    reverse_sense_barrier(size_t num_threads)
        : num_threads(num_threads), count(num_threads), global_sense(false) {}
    void arrive_and_wait();
private:
    const size_t num_threads;
    std::atomic<size_t> count;
    std::atomic<bool> global_sense;
};


#endif // LOCKS_H
