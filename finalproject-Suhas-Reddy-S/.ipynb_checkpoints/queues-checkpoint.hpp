#pragma once
#include <iostream>
#include <mutex>
#include <stdint.h>
#include <vector>
#include <condition_variable>
#include <functional>
#include "main.hpp"

using namespace std;

class SGL_QUEUE {
private:
	node_t *head;
	node_t *tail;
	mutex sgl;

public:
	SGL_QUEUE() : head(nullptr), tail(nullptr) {}
	// Enqueue an element
	void enqueue(uint32_t value);
	// Dequeue an element
	uint32_t dequeue();
	// Check if the queue is empty
	bool is_empty();
	// Destructor to clean up remaining nodes
	~SGL_QUEUE();
};

class MS_QUEUE {
private:
	atomic<msq_node_t*> head;
	atomic<msq_node_t*> tail;

public:
	MS_QUEUE();
	// Enqueue an element
	void enqueue(uint32_t value);
	// Dequeue an element
	uint32_t dequeue();
	// Check if the queue is empty
	bool is_empty();
	// Destructor to clean up remaining nodes
	~MS_QUEUE();
};

class FLAT_COMBINE_SGL_QUEUE {
private:
	struct Operation {
		enum Type { ENQUEUE, DEQUEUE, NONE } type;
		uint32_t value;
		atomic<bool> completed;
		atomic<uint32_t> result;

		Operation() : type(NONE), value(0), completed(false), result(0) {}
	};
    node_t* head;
    node_t* tail;
    atomic<node_t*> atomic_head;
    atomic<node_t*> atomic_tail;

    Operation combining_array[COMBINING_ARRAY_SIZE];
    mutex global_lock;

    // Function to combine operations
    void combine_operations();

public:
    FLAT_COMBINE_SGL_QUEUE();
    void enqueue(uint32_t value);
    uint32_t dequeue();
    bool is_empty();
    ~FLAT_COMBINE_SGL_QUEUE();
};

class FLAT_COMBINE_MS_QUEUE {
private:
	struct Operation {
		enum Type { ENQUEUE, DEQUEUE, NONE } type;
		uint32_t value;
		atomic<bool> completed;
		atomic<uint32_t> result;

		Operation() : type(NONE), value(0), completed(false), result(0) {}
	};
    atomic<msq_node_t*> head;
    atomic<msq_node_t*> tail;
    Operation combining_array[COMBINING_ARRAY_SIZE];
    mutex global_lock;

    void combine_operations();

public:
    FLAT_COMBINE_MS_QUEUE();
    void enqueue(uint32_t value);
    uint32_t dequeue();
    bool is_empty();
    ~FLAT_COMBINE_MS_QUEUE();
};