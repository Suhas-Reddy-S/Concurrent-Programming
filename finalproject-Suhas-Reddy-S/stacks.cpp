#include "stacks.hpp"
#include <stdexcept>
#include <random>
#include <thread>

using namespace std;

bool SGL_STACK::is_empty() {
    lock_guard<mutex> lock(sgl); // RAII for locking
    return head == nullptr;
}

void SGL_STACK::push(uint32_t val) {
    lock_guard<mutex> lock(sgl); // RAII for locking
    node_t *new_node = new node_t(val);
    new_node->next = head;  // Fix: Maintain the linked list structure
    head = new_node;
}

uint32_t SGL_STACK::pop() {
    lock_guard<mutex> lock(sgl); // RAII for locking

    if (head == nullptr) {
        throw runtime_error("Pop: Stack is empty!"); // Safe exception handling
    }

    node_t *old_head = head;  // Save the current head
    head = head->next;        // Move the head pointer to the next node
    uint32_t val = old_head->val;
    delete old_head;          // Free the old head node

    return val;
}

// Destructor
SGL_STACK::~SGL_STACK() {
    while (head) {
        node_t *temp = head;
        head = head->next;
        delete temp;
    }
}

// Trieber Stack
bool TRIEBER_STACK::is_empty() {
	return head_ptr.load(ACQUIRE) == nullptr;
}

void TRIEBER_STACK::push(uint32_t val) {
	atm_node_t* new_node = new atm_node_t;
	new_node->val.store(val, RELEASE);
	atm_node_t* current_head;
	uint32_t current_cnt;
	do {
	current_head = head_ptr.load(ACQUIRE);
	current_cnt = head_cnt.load(ACQUIRE);
	new_node->next.store(current_head, RELEASE);
	} while(!head_ptr.compare_exchange_strong(current_head, new_node, ACQ_REL) || 
		    !head_cnt.compare_exchange_strong(current_cnt, current_cnt + 1, ACQ_REL));
	return;
}

uint32_t TRIEBER_STACK::pop() {
	atm_node_t* current_head;
	uint32_t current_cnt;
	do {
		current_head = head_ptr.load(ACQUIRE);
		current_cnt = head_cnt.load(ACQUIRE);
		if(current_head == nullptr) {
			throw runtime_error("Pop: Stack is empty!");
		}
	} while(!head_ptr.compare_exchange_strong(current_head, current_head->next.load(ACQUIRE), ACQ_REL) ||
			!head_cnt.compare_exchange_strong(current_cnt, current_cnt + 1, ACQ_REL));
			
	uint32_t val = current_head->val.load(ACQUIRE);
	delete current_head;
	return val;
}

// Destructor
TRIEBER_STACK::~TRIEBER_STACK() {
	while(!is_empty()) {
		pop();
	}
}

bool ELIMINATION_SGL_STACK::is_empty() {
    lock_guard<mutex> lock(sgl); // Protect head access
    return head == nullptr;
}

bool ELIMINATION_SGL_STACK::try_elimination(uint32_t& value, bool is_push) {
    static thread_local int counter = 0; // Thread-local index counter
    int index = counter++ % ELIM_ARRAY_SIZE; // Cyclic index in the elimination array

    uint64_t operation = is_push ? static_cast<uint64_t>(value) : POP;
    uint64_t expected = EMPTY;

    // Try to place the operation in the elimination array
    if (elimination_array[index].compare_exchange_strong(expected, operation, ACQ_REL)) {
        // Sleep briefly to allow another thread to pair with this operation
        this_thread::sleep_for(chrono::milliseconds(5));

        // Check the result after sleep
        uint64_t result = elimination_array[index].exchange(EMPTY, ACQ_REL);
        if (result == operation) {
            return false; // Timeout: no match found
        }

        if (is_push) {
            return true; // Push successfully paired with a pop
        } else {
            value = static_cast<uint32_t>(result); // Retrieve value from paired push
            return true; // Pop successfully paired with a push
        }
    } else {
        // Check if there is a matching operation in the array
        if (is_push && expected == POP) {
            // Match with a pending pop
            elimination_array[index].store(EMPTY, RELAXED);
            return true;
        } else if (!is_push && expected > 0) {
            // Match with a pending push
            value = static_cast<uint32_t>(expected);
            elimination_array[index].store(EMPTY, RELAXED);
            return true;
        }
    }

    return false; // No match found
}

void ELIMINATION_SGL_STACK::push(uint32_t val) {
    if (try_elimination(val, true)) {
        return; // Successfully handled by elimination
    }

    lock_guard<mutex> lock(sgl); // Fallback to stack operation
    node_t* new_node = new node_t(val);
    new_node->next = head; // Link the new node to the current head
    head = new_node;
}

uint32_t ELIMINATION_SGL_STACK::pop() {
    uint32_t value = 0;
    if (try_elimination(value, false)) {
        return value; // Successfully handled by elimination
    }

    lock_guard<mutex> lock(sgl); // Fallback to stack operation
    if (head == nullptr) {
        throw runtime_error("Pop: Stack is empty!");
    }

    node_t* old_head = head;
    head = head->next;
    uint32_t val = old_head->val;
    delete old_head;
    return val;
}

ELIMINATION_SGL_STACK::~ELIMINATION_SGL_STACK() {
    while (head) {
        node_t* temp = head;
        head = head->next;
        delete temp;
    }
}

// Trieber Stack with elimination optimizations
// Check if the stack is empty
bool ELIMINATION_T_STACK::is_empty() {
    return head_ptr.load(ACQUIRE) == nullptr;
}

bool ELIMINATION_T_STACK::try_elimination(uint32_t& value, bool is_push) {
	// Use a static counter to pick an index in a cyclic manner
	static thread_local int counter = 0;
	int index = counter++ % ELIM_ARRAY_SIZE; // Cyclic index in the elimination array

	// The operation to be placed in the array
	uint64_t op = is_push ? static_cast<uint64_t>(value) : static_cast<uint64_t>(-1);
	uint64_t expected = 0;

	// Try to place the operation in the elimination array
	if (elimination_array[index].compare_exchange_strong(expected, op, ACQ_REL)) {
		// Sleep briefly to allow another thread to pair with this operation
		this_thread::sleep_for(chrono::milliseconds(5));

		// Check the result of the operation
		uint64_t result = elimination_array[index].exchange(0, ACQ_REL);
		if (result == op) {
			return false; // Timeout: no match found
		}
		if (is_push) {
			return true; // Push successfully paired with a pop
		} else {
			value = static_cast<uint32_t>(result); // Retrieve value from paired push
			return true; // Pop successfully paired with a push
		}
	} else {
		// Check if an existing operation can be matched
		if (is_push && expected == static_cast<uint64_t>(-1)) {
			// Match with a pending pop
			elimination_array[index].store(0, RELAXED);
			return true;
		} else if (!is_push && expected > 0) {
			// Match with a pending push
			value = static_cast<uint32_t>(expected);
			elimination_array[index].store(0, RELAXED);
			return true;
		}
	}

	return false; // No match
}


// Push operation
void ELIMINATION_T_STACK::push(uint32_t value) {
    if (try_elimination(value, true)) {
        return; // Successfully handled by elimination
    }

    atm_node_t* new_node = new atm_node_t(value);
    atm_node_t* current_head;

    // Standard Treiber stack push
    do {
        current_head = head_ptr.load(ACQUIRE);
        new_node->next.store(current_head, RELAXED);
    } while (!head_ptr.compare_exchange_weak(current_head, new_node, RELEASE));
}

// Pop operation
uint32_t ELIMINATION_T_STACK::pop() {
    uint32_t value = 0;

    if (try_elimination(value, false)) {
        return value; // Successfully handled by elimination
    }

    atm_node_t* current_head;
    atm_node_t* next_node;

    // Standard Treiber stack pop
    do {
        current_head = head_ptr.load(ACQUIRE);
        if (!current_head) {
            throw runtime_error("Pop: Stack is empty!");
        }
        next_node = current_head->next.load(ACQUIRE);
    } while (!head_ptr.compare_exchange_weak(current_head, next_node, RELEASE));

    value = current_head->val;
    delete current_head;
    return value;
}

// Destructor to clean up remaining nodes
ELIMINATION_T_STACK::~ELIMINATION_T_STACK() {
    while (!is_empty()) {
        pop(); // Remove all remaining nodes
    }
}

/* Flat Combining Optimization*/
// Check if the stack is empty
bool FLAT_COMBINE_SGL_STACK::is_empty() {
    unique_lock<mutex> lock(sgl);
    return (head == nullptr);
}

// Process operations in the combining array
void FLAT_COMBINE_SGL_STACK::process_operations() {
    for (auto& op : combining_array) {
        if (op != nullptr && !op->completed.load(ACQUIRE)) {
            if (op->is_push.load(RELAXED)) {
                // Process push operation
                node_t* new_node = new node_t(op->value);
                new_node->next = head;
                head = new_node;
                op->success.store(true, RELEASE);
            } else {
                // Process pop operation
                if (head == nullptr) {
                    op->success.store(false, RELEASE);
                } else {
                    node_t* old_head = head;
                    head = head->next;
                    op->value = old_head->val;
                    delete old_head;
                    op->success.store(true, RELEASE);
                }
            }
            // Mark operation as completed
            op->completed.store(true, RELEASE);
        }
    }
}

// Push a value onto the stack
void FLAT_COMBINE_SGL_STACK::push(uint32_t value) {
    Operation op;
    op.completed.store(false, RELAXED);
    op.is_push.store(true, RELAXED);
    op.value = value;
    op.success.store(false, RELAXED);

    // Place the operation in the combining array
    size_t thread_id = hash<thread::id>{}(this_thread::get_id()) % combining_array.size();
    combining_array[thread_id] = &op;

    // Contend for the lock
    unique_lock<mutex> lock(sgl);
    if (lock.owns_lock()) {
        // If lock-holder, process all operations
        process_operations();
    }

    // Wait for the operation to complete
    while (!op.completed.load(ACQUIRE)) {
        this_thread::yield();
    }
}

// Pop a value from the stack
uint32_t FLAT_COMBINE_SGL_STACK::pop() {
    Operation op;
    op.completed.store(false, RELAXED);
    op.is_push.store(false, RELAXED);
    op.success.store(false, RELAXED);

    // Place the operation in the combining array
    size_t thread_id = hash<thread::id>{}(this_thread::get_id()) % combining_array.size();
    combining_array[thread_id] = &op;

    // Contend for the lock
    unique_lock<mutex> lock(sgl);
    if (lock.owns_lock()) {
        // If lock-holder, process all operations
        process_operations();
    }

    // Wait for the operation to complete
    while (!op.completed.load(ACQUIRE)) {
        this_thread::yield();
    }

    // Check if the pop operation was successful
    if (!op.success.load(RELAXED)) {
        throw runtime_error("Pop: Stack is empty!");
    }

    return op.value;
}

// Destructor to clean up the stack
FLAT_COMBINE_SGL_STACK::~FLAT_COMBINE_SGL_STACK() {
    while (head) {
        node_t* temp = head;
        head = head->next;
        delete temp;
    }
}

// 
bool FLAT_COMBINE_T_STACK::is_empty() {
    cnt_ptr current_head = head_ptr.load(ACQUIRE);
    return current_head.ptr == nullptr;
}

FLAT_COMBINE_T_STACK::FLAT_COMBINE_T_STACK() {
    head_ptr.store(nullptr, RELAXED);
    head_cnt.store(0, RELAXED);
}

void FLAT_COMBINE_T_STACK::combine_operations() {
    for (int i = 0; i < COMBINING_ARRAY_SIZE; ++i) {
        Operation& op = combining_array[i];
        if (op.type == Operation::PUSH && !op.completed.load(ACQUIRE)) {
            atm_node_t* new_node = new atm_node_t(op.value);

            atm_node_t* current_ptr;
            uint32_t current_cnt;

            do {
                current_ptr = head_ptr.load(ACQUIRE);
                current_cnt = head_cnt.load(ACQUIRE);
                new_node->next = current_ptr;
            } while (!head_ptr.compare_exchange_weak(current_ptr, new_node, RELEASE) ||
                     !head_cnt.compare_exchange_weak(current_cnt, current_cnt + 1, RELEASE));

            op.completed.store(true, RELEASE);
        } else if (op.type == Operation::POP && !op.completed.load(ACQUIRE)) {
            atm_node_t* current_ptr;
            uint32_t current_cnt;
            atm_node_t* next_node;

            do {
                current_ptr = head_ptr.load(ACQUIRE);
                current_cnt = head_cnt.load(ACQUIRE);
                if (!current_ptr) {
                    op.result.store(0, RELEASE);
                    op.completed.store(true, RELEASE);
                    break;
                }
                next_node = current_ptr->next;
            } while (!head_ptr.compare_exchange_weak(current_ptr, next_node, RELEASE) ||
                     !head_cnt.compare_exchange_weak(current_cnt, current_cnt + 1, RELEASE));

            if (current_ptr) {
                op.result.store(current_ptr->val, RELEASE);
                delete current_ptr;
                op.completed.store(true, RELEASE);
            }
        }
    }
}

// Push operation
void FLAT_COMBINE_T_STACK::push(uint32_t value) {
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

    my_op->type = Operation::PUSH;
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

uint32_t FLAT_COMBINE_T_STACK::pop() {
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

    my_op->type = Operation::POP;
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
        throw runtime_error("Pop: Stack is empty!");
    }

    return result;
}

// Destructor
FLAT_COMBINE_T_STACK::~FLAT_COMBINE_T_STACK() {
    cnt_ptr current_head = head_ptr.load(ACQUIRE);
    while (current_head.ptr) {
        atm_node_t* temp = current_head.ptr;
        current_head.ptr = current_head.ptr->next;
        delete temp;
    }
}
