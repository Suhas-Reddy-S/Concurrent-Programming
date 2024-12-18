#include "locks.h"

using namespace std;

bool tas(atomic<bool> &x, memory_order MEM) {
	bool expected = false;
	bool desired = true;
	return x.compare_exchange_strong(expected, desired, MEM);
}

int fai(atomic<int> &x, int val, memory_order MEM) {
	return x.fetch_add(val, MEM);
}

// template <typename T>
bool cas(atomic<MCS_Node*> &x, MCS_Node* expected, MCS_Node* desired, memory_order MEM) {
	MCS_Node* expected_ref = expected;
	return x.compare_exchange_strong(expected_ref, desired, MEM);
}
