#include "queues.hpp"
#include <stdexcept>
#include <cassert>

using namespace std;

bool SGL_QUEUE::is_empty() {
	sgl.lock();
	if(head == nullptr) {
		sgl.unlock();
		return true;
	}
	sgl.unlock();
	return false;
}

void SGL_QUEUE::enqueue(uint32_t val) {
	sgl.lock();
	node_t *new_node = new node_t(val);
	
	if(head == nullptr) {
		head = new_node;
		tail = new_node;
	} else {
		tail->next = new_node;
		tail = new_node;
	}
	sgl.unlock();
	return;
}

uint32_t SGL_QUEUE::dequeue() {
	sgl.lock();
	if(head == nullptr) {
		sgl.unlock();
		throw runtime_error("Dequeue: Queue is empty!");
	}
	
	node_t *old_head = head;
	head = head->next;
	uint32_t val = old_head->val;
	
	if(head == nullptr) {
		tail = nullptr;
	}
	delete old_head;
	sgl.unlock();
	return val;
}

// Destructor
SGL_QUEUE::~SGL_QUEUE() {
	while(head) {
		node_t *temp = head;
		head = head->next;
		delete temp;
	}
	tail = nullptr;
}

// MSQ
MS_QUEUE::MS_QUEUE() {
	msq_node_t *dummy = new msq_node_t(0);
	head.store(dummy, RELEASE);
	tail.store(dummy, RELEASE);
}

bool MS_QUEUE::is_empty() {
	msq_node_t* current_node = head.load(ACQUIRE);
	msq_node_t* next_node = current_node->next.load(ACQUIRE);
	
	return next_node == nullptr;
}

void MS_QUEUE::enqueue(uint32_t val) {
	msq_node_t *new_node = new msq_node_t(val);
	msq_node_t *current_tail, *tail_next;
	msq_node_t* null_ptr = nullptr;
	
	while(true) {
		current_tail = tail.load(ACQUIRE);
		tail_next = current_tail->next.load(ACQUIRE);
		if(current_tail == tail.load(ACQUIRE)) {
			if(tail_next == nullptr && current_tail->next.compare_exchange_strong(null_ptr, new_node, ACQ_REL)) {
				break;
			} else if(tail_next != nullptr) {
				tail.compare_exchange_strong(current_tail, tail_next, ACQ_REL);
			}
		}
	}
}

uint32_t MS_QUEUE::dequeue() {
	msq_node_t *next_node, *current_tail, *current_head;
	while(true) {
		current_head = head.load(ACQUIRE);
		current_tail = tail.load(ACQUIRE);
		next_node =  current_head->next.load(ACQUIRE);
		if(current_head == head.load(ACQUIRE)) {
			if(current_head == current_tail) {
				if(next_node == nullptr) {
					throw runtime_error("Dequeue: Queue is empty!");
				} else {
					tail.compare_exchange_strong(current_tail, next_node, ACQ_REL);
				}
			} else {
				uint32_t val = next_node->val;
				if(head.compare_exchange_strong(current_head, next_node, ACQ_REL)) {
					return val;
				}
			}
		}
	}
}

// Destructor
MS_QUEUE::~MS_QUEUE() {
	while(!is_empty()) {
		try {
			dequeue();
		} catch (const runtime_error&) {
			break;
		}
	}
	
	msq_node_t* dummy = head.load(ACQUIRE);
	delete dummy;
}

/*Flat Combining*/
bool FLAT_COMBINE_SGL_QUEUE::is_empty() {
    return atomic_head.load(ACQUIRE) == nullptr;
}

FLAT_COMBINE_SGL_QUEUE::FLAT_COMBINE_SGL_QUEUE() {
    head = nullptr;
    tail = nullptr;
    atomic_head.store(nullptr);
    atomic_tail.store(nullptr);
}

void FLAT_COMBINE_SGL_QUEUE::combine_operations() {
    for (int i = 0; i < COMBINING_ARRAY_SIZE; ++i) {
        Operation& op = combining_array[i];
        if (op.type == Operation::ENQUEUE && !op.completed.load(ACQUIRE)) {
            node_t* new_node = new node_t(op.value);

            node_t* current_tail;
            do {
                current_tail = atomic_tail.load(ACQUIRE);
                if (current_tail) {
                    current_tail->next = new_node; // Link new node to the end of the queue
                } else {
                    atomic_head.store(new_node, RELEASE); // If queue is empty, set new node as head
                }
            } while (!atomic_tail.compare_exchange_weak(current_tail, new_node, RELEASE));

            op.completed.store(true, RELEASE);
        } else if (op.type == Operation::DEQUEUE && !op.completed.load(ACQUIRE)) {
            node_t* current_head;

            do {
                current_head = atomic_head.load(ACQUIRE);
                if (!current_head) {
                    op.result.store(0, RELEASE); // Indicate empty queue
                    op.completed.store(true, RELEASE);
                    break;
                }
            } while (!atomic_head.compare_exchange_weak(current_head, current_head->next, RELEASE));

            if (current_head) {
                op.result.store(current_head->val, RELEASE);
                delete current_head;
                op.completed.store(true, RELEASE);
            }
        }
    }
}

void FLAT_COMBINE_SGL_QUEUE::enqueue(uint32_t value) {
    // Find an available slot in the combining array
    Operation* my_op = nullptr;
    for (int i = 0; i < COMBINING_ARRAY_SIZE; ++i) {
        if (combining_array[i].type == Operation::NONE) {
            my_op = &combining_array[i];
            break;
        }
    }

    if (!my_op) {
        throw runtime_error("No available slot in combining array");
    }

    my_op->type = Operation::ENQUEUE;
    my_op->value = value;
    my_op->completed.store(false, RELEASE);

    {
        lock_guard<mutex> lock(global_lock);
        combine_operations();
    }

    // Wait for the operation to complete
    while (!my_op->completed.load(ACQUIRE)) {
        this_thread::yield();
    }

    my_op->type = Operation::NONE;
}

uint32_t FLAT_COMBINE_SGL_QUEUE::dequeue() {
    // Find an available slot in the combining array
    Operation* my_op = nullptr;
    for (int i = 0; i < COMBINING_ARRAY_SIZE; ++i) {
        if (combining_array[i].type == Operation::NONE) {
            my_op = &combining_array[i];
            break;
        }
    }

    if (!my_op) {
        throw runtime_error("No available slot in combining array");
    }

    my_op->type = Operation::DEQUEUE;
    my_op->completed.store(false, RELEASE);

    {
        lock_guard<mutex> lock(global_lock);
        combine_operations();
    }

    // Wait for the operation to complete
    while (!my_op->completed.load(ACQUIRE)) {
        this_thread::yield();
    }

    uint32_t result = my_op->result.load(ACQUIRE);
    my_op->type = Operation::NONE;

    if (result == 0) {
        throw runtime_error("Dequeue: Queue is empty!");
    }

    return result;
}

FLAT_COMBINE_SGL_QUEUE::~FLAT_COMBINE_SGL_QUEUE() {
    node_t* current = atomic_head.load(ACQUIRE);
    while (current) {
        node_t* temp = current;
        current = current->next;
        delete temp;
    }
}

// Constructor initializes an empty queue
FLAT_COMBINE_MS_QUEUE::FLAT_COMBINE_MS_QUEUE() {
    msq_node_t* dummy = new msq_node_t(0);
    head.store(dummy, RELAXED);
    tail.store(dummy, RELAXED);
}

void FLAT_COMBINE_MS_QUEUE::combine_operations() {
    for (int i = 0; i < COMBINING_ARRAY_SIZE; ++i) {
        Operation& op = combining_array[i];
        if (op.type == Operation::ENQUEUE && !op.completed.load(ACQUIRE)) {
            msq_node_t* new_node = new msq_node_t(op.value);
            msq_node_t* current_tail;
            msq_node_t* expected = nullptr; 
            do {
                current_tail = tail.load(ACQUIRE);
            } while (!current_tail->next.compare_exchange_weak(expected, new_node, RELAXED));

            tail.compare_exchange_weak(current_tail, new_node, RELEASE, RELAXED);
            op.completed.store(true, RELEASE);
        } else if (op.type == Operation::DEQUEUE && !op.completed.load(ACQUIRE)) {
            msq_node_t* current_head;
            msq_node_t* next_node;

            do {
                current_head = head.load(ACQUIRE);
                next_node = current_head->next.load(ACQUIRE);
                if (!next_node) {
                    op.result.store(0, RELEASE); // Indicate empty queue
                    op.completed.store(true, RELEASE);
                    break;
                }
            } while (!head.compare_exchange_weak(current_head, next_node, RELAXED));

            if (next_node) {
                op.result.store(next_node->val, RELEASE);
                delete current_head;
                op.completed.store(true, RELEASE);
            }
        }
    }
}

void FLAT_COMBINE_MS_QUEUE::enqueue(uint32_t value) {
    Operation* my_op = nullptr;
    for (int i = 0; i < COMBINING_ARRAY_SIZE; ++i) {
        if (combining_array[i].type == Operation::NONE) {
            my_op = &combining_array[i];
            break;
        }
    }

    if (!my_op) {
        throw runtime_error("No available slot in combining array");
    }

    my_op->type = Operation::ENQUEUE;
    my_op->value = value;
    my_op->completed.store(false, RELEASE);

    {
        lock_guard<mutex> lock(global_lock);
        combine_operations();
    }

    while (!my_op->completed.load(ACQUIRE)) {
        this_thread::yield();
    }

    my_op->type = Operation::NONE;
}

uint32_t FLAT_COMBINE_MS_QUEUE::dequeue() {
    Operation* my_op = nullptr;
    for (int i = 0; i < COMBINING_ARRAY_SIZE; ++i) {
        if (combining_array[i].type == Operation::NONE) {
            my_op = &combining_array[i];
            break;
        }
    }

    if (!my_op) {
        throw runtime_error("No available slot in combining array");
    }

    my_op->type = Operation::DEQUEUE;
    my_op->completed.store(false, RELEASE);

    {
        lock_guard<mutex> lock(global_lock);
        combine_operations();
    }

    while (!my_op->completed.load(ACQUIRE)) {
        this_thread::yield();
    }

    uint32_t result = my_op->result.load(ACQUIRE);
    my_op->type = Operation::NONE;

    if (result == 0) {
        throw runtime_error("Dequeue: Queue is empty!");
    }

    return result;
}

bool FLAT_COMBINE_MS_QUEUE::is_empty() {
    msq_node_t* current_head = head.load(ACQUIRE);
    msq_node_t* next_node = current_head->next.load(ACQUIRE);
    return next_node == nullptr;
}

FLAT_COMBINE_MS_QUEUE::~FLAT_COMBINE_MS_QUEUE() {
    msq_node_t* current = head.load(RELAXED);
    while (current) {
        msq_node_t* next = current->next.load(RELAXED);
        delete current;
        current = next;
    }
}