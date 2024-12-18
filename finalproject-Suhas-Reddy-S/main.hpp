#ifndef MAIN_HPP
#define MAIN_HPP

#include <atomic>
#include <mutex>
#include <vector>
#include <thread>
#include <stdexcept>
#include <iostream>

using namespace std;

#define SEQ_CST memory_order_seq_cst
#define RELAXED memory_order_relaxed
#define RELEASE memory_order_release
#define ACQUIRE memory_order_acquire
#define ACQ_REL memory_order_acq_rel

#define COMBINING_ARRAY_SIZE 64

enum ContainerType {
    SGL_Q, SGL_STK, MS_Q, T_STACK, E_SGL_STACK, E_T_STACK, FC_SGL_STACK, FC_T_STACK, FC_SGL_Q, FC_MS_Q
};

struct node_t {
    uint32_t val;
    node_t *next;
	
	node_t(uint32_t v) : val(v), next(nullptr) {}
};

struct msq_node_t {
    uint32_t val;
    atomic<msq_node_t*> next;
	
	msq_node_t(uint32_t v) : val(v), next(nullptr) {}
};

struct atm_node_t {
    atomic<uint32_t> val;
    atomic<atm_node_t*> next;
	
	atm_node_t(uint32_t v = 0, atm_node_t* n = nullptr) : val(v), next(n) {}
};

void copyFile(uint32_t *ip_arr, uint32_t *op_arr, ContainerType, size_t num_threads, int len);

#endif