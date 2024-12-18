#include <iostream>
#include <mutex>
#include "main.hpp"

#define ELIM_ARRAY_SIZE 64
static constexpr uint64_t EMPTY = 0;   // Sentinel value for an empty slot
static constexpr uint64_t POP = UINT64_MAX; // Sentinel value for a pending pop

using namespace std;

class SGL_STACK {
private:
    node_t *head;
    mutex sgl;

public:
    SGL_STACK() : head(nullptr) {}
    // Push an element
    void push(uint32_t value);
    // Pop an element
    uint32_t pop();
    // Check if the stack is empty
    bool is_empty();
    // Destructor to clean up remaining nodes
    ~SGL_STACK();
};

class TRIEBER_STACK {
private: 
	struct cnt_ptr {
		atm_node_t* ptr;
		uint32_t cnt;

		cnt_ptr(atm_node_t* p = nullptr, uint32_t c = 0) : ptr(p), cnt(c) {}
	};

	atomic<atm_node_t*> head_ptr;
	atomic<uint32_t> head_cnt;
public:
	TRIEBER_STACK() : head_ptr(nullptr), head_cnt(0) {}
	// Push an element
    void push(uint32_t value);
    // Pop an element
    uint32_t pop();
    // Check if the stack is empty
    bool is_empty();
    // Destructor to clean up remaining nodes
    ~TRIEBER_STACK();
};

class ELIMINATION_SGL_STACK {
private:
    node_t* head = nullptr;                   // Top of the stack
    std::mutex sgl;                           // Mutex for stack operations
    std::atomic<uint64_t> elimination_array[ELIM_ARRAY_SIZE]; // Elimination array

    bool try_elimination(uint32_t& value, bool is_push);

public:
    ELIMINATION_SGL_STACK() {
        // Initialize the elimination array to EMPTY
        for (size_t i = 0; i < ELIM_ARRAY_SIZE; ++i) {
            elimination_array[i].store(EMPTY, std::memory_order_relaxed);
        }
    }
    ~ELIMINATION_SGL_STACK();                // Destructor

    bool is_empty();                         // Check if stack is empty
    void push(uint32_t val);                 // Push a value onto the stack
    uint32_t pop();                          // Pop a value from the stack
};

class ELIMINATION_T_STACK {
private:
    struct cnt_ptr {
        atm_node_t* ptr;
        uint32_t cnt;

        cnt_ptr(atm_node_t* p = nullptr, uint32_t c = 0) : ptr(p), cnt(c) {}
    };

    atomic<atm_node_t*> head_ptr;      // Head pointer of the stack
    atomic<uint32_t> head_cnt;         // Counter for the head to avoid ABA problem
    atomic<uint64_t> elimination_array[ELIM_ARRAY_SIZE]; // Elimination array

    // Helper function to perform elimination optimization
    bool try_elimination(uint32_t& value, bool is_push);

public:
    ELIMINATION_T_STACK() : head_ptr(nullptr), head_cnt(0) {
        for (int i = 0; i < 16; ++i) {
            elimination_array[i].store(0); // Initialize the elimination array with 0
        }
    }

	// Push an element
    void push(uint32_t value);
    // Pop an element
    uint32_t pop();
    // Check if the stack is empty
    bool is_empty();
    // Destructor to clean up remaining nodes
    ~ELIMINATION_T_STACK();
};

class FLAT_COMBINE_SGL_STACK {
private:
    struct Operation {
        std::atomic<bool> completed{false};
        std::atomic<bool> is_push{false};
        std::atomic<bool> success{false};
        uint32_t value = 0;
    };
    node_t* head = nullptr;                   // Top of the stack
    std::mutex sgl;                           // Mutex for stack operations
    std::vector<Operation*> combining_array;  // Combining array

    void process_operations();                // Process operations in combining array

public:
    FLAT_COMBINE_SGL_STACK(size_t num_threads = std::thread::hardware_concurrency())
        : combining_array(num_threads, nullptr) {}

    ~FLAT_COMBINE_SGL_STACK();                // Destructor

    bool is_empty();                          // Check if stack is empty
    void push(uint32_t value);                // Push a value onto the stack
    uint32_t pop();                           // Pop a value from the stack
};

class FLAT_COMBINE_T_STACK {
private:
	struct Operation {
		enum Type { PUSH, POP, NONE } type;
		uint32_t value;
		atomic<bool> completed;
		atomic<uint32_t> result;

		Operation() : type(NONE), value(0), completed(false), result(0) {}
	};
    struct cnt_ptr {
        atm_node_t* ptr;
        uint32_t cnt;

        cnt_ptr(atm_node_t* p = nullptr, uint32_t c = 0) : ptr(p), cnt(c) {}
    };

    atomic<atm_node_t*> head_ptr; 
	atomic<uint32_t> head_cnt;  
    Operation combining_array[COMBINING_ARRAY_SIZE];
    mutex global_lock;

    // Function to combine operations
    void combine_operations();

public:
    FLAT_COMBINE_T_STACK();
    void push(uint32_t value);
    uint32_t pop();
    bool is_empty();
    ~FLAT_COMBINE_T_STACK();
};