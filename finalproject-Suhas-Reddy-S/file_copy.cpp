#include "main.hpp"
#include "stacks.hpp"
#include "queues.hpp"
#include <algorithm>
#include <condition_variable>

using namespace std;

void copyFile(uint32_t *ip_arr, uint32_t *op_arr, ContainerType container_type, size_t num_threads, int len) {
	// stack have push(uint32_t ), uint32_t pop(), and bool is_empty() methods
	// queue have enqueue(uint32_t ), uint32_t dequeue, bool is_empty() methods
	switch(container_type) {
		case SGL_Q: {
			SGL_QUEUE queue;
			vector<thread> threads;

            // Calculate workload distribution
            size_t elements_per_thread = len / num_threads;
            size_t remaining_elements = len % num_threads;

            // Producer threads to enqueue
            auto producer = [&](int start, int end) {
                for (int i = start; i < end; ++i) {
                    queue.enqueue(ip_arr[i]);
                }
            };

            // Consumer threads to dequeue
            auto consumer = [&](int start, int end) {
                for (int i = start; i < end; ++i) {
                    while (queue.is_empty()) {
                        this_thread::yield(); // Wait for producer to enqueue
                    }
                    op_arr[i] = queue.dequeue();
                }
            };

            // Launch producer threads
            int start_index = 0;
            for (size_t i = 0; i < num_threads; ++i) {
                int end_index = start_index + elements_per_thread + (remaining_elements > 0 ? 1 : 0);
                remaining_elements = remaining_elements > 0 ? remaining_elements - 1 : 0;
                threads.emplace_back(producer, start_index, end_index);
                start_index = end_index;
            }

            // Launch consumer threads
            start_index = 0;
            remaining_elements = len % num_threads;
            for (size_t i = 0; i < num_threads; ++i) {
                int end_index = start_index + elements_per_thread + (remaining_elements > 0 ? 1 : 0);
                remaining_elements = remaining_elements > 0 ? remaining_elements - 1 : 0;
                threads.emplace_back(consumer, start_index, end_index);
                start_index = end_index;
            }

            // Join all threads
            for (auto &t : threads) {
                t.join();
            }
            break;
		}
		case FC_SGL_Q: {
			FLAT_COMBINE_SGL_QUEUE queue;
			vector<thread> threads;

            // Calculate workload distribution
            size_t elements_per_thread = len / num_threads;
            size_t remaining_elements = len % num_threads;

            // Producer threads to enqueue
            auto producer = [&](int start, int end) {
                for (int i = start; i < end; ++i) {
                    queue.enqueue(ip_arr[i]);
                }
            };

            // Consumer threads to dequeue
            auto consumer = [&](int start, int end) {
                for (int i = start; i < end; ++i) {
                    while (queue.is_empty()) {
                        this_thread::yield(); // Wait for producer to enqueue
                    }
                    op_arr[i] = queue.dequeue();
                }
            };

            // Launch producer threads
            int start_index = 0;
            for (size_t i = 0; i < num_threads; ++i) {
                int end_index = start_index + elements_per_thread + (remaining_elements > 0 ? 1 : 0);
                remaining_elements = remaining_elements > 0 ? remaining_elements - 1 : 0;
                threads.emplace_back(producer, start_index, end_index);
                start_index = end_index;
            }

            // Launch consumer threads
            start_index = 0;
            remaining_elements = len % num_threads;
            for (size_t i = 0; i < num_threads; ++i) {
                int end_index = start_index + elements_per_thread + (remaining_elements > 0 ? 1 : 0);
                remaining_elements = remaining_elements > 0 ? remaining_elements - 1 : 0;
                threads.emplace_back(consumer, start_index, end_index);
                start_index = end_index;
            }

            // Join all threads
            for (auto &t : threads) {
                t.join();
            }
            break;
		}
		case MS_Q: {
			MS_QUEUE queue;
			vector<thread> threads;

            // Calculate workload distribution
            size_t elements_per_thread = len / num_threads;
            size_t remaining_elements = len % num_threads;

            // Producer threads to enqueue
            auto producer = [&](int start, int end) {
                for (int i = start; i < end; ++i) {
                    queue.enqueue(ip_arr[i]);
                }
            };

            // Consumer threads to dequeue
            auto consumer = [&](int start, int end) {
                for (int i = start; i < end; ++i) {
                    while (queue.is_empty()) {
                        this_thread::yield(); // Wait for producer to enqueue
                    }
                    op_arr[i] = queue.dequeue();
                }
            };

            // Launch producer threads
            int start_index = 0;
            for (size_t i = 0; i < num_threads; ++i) {
                int end_index = start_index + elements_per_thread + (remaining_elements > 0 ? 1 : 0);
                remaining_elements = remaining_elements > 0 ? remaining_elements - 1 : 0;
                threads.emplace_back(producer, start_index, end_index);
                start_index = end_index;
            }

            // Launch consumer threads
            start_index = 0;
            remaining_elements = len % num_threads;
            for (size_t i = 0; i < num_threads; ++i) {
                int end_index = start_index + elements_per_thread + (remaining_elements > 0 ? 1 : 0);
                remaining_elements = remaining_elements > 0 ? remaining_elements - 1 : 0;
                threads.emplace_back(consumer, start_index, end_index);
                start_index = end_index;
            }

            // Join all threads
            for (auto &t : threads) {
                t.join();
            }
            break;
		}
		case FC_MS_Q: {
			FLAT_COMBINE_MS_QUEUE queue;
			vector<thread> threads;

            // Calculate workload distribution
            size_t elements_per_thread = len / num_threads;
            size_t remaining_elements = len % num_threads;

            // Producer threads to enqueue
            auto producer = [&](int start, int end) {
                for (int i = start; i < end; ++i) {
                    queue.enqueue(ip_arr[i]);
                }
            };

            // Consumer threads to dequeue
            auto consumer = [&](int start, int end) {
                for (int i = start; i < end; ++i) {
                    while (queue.is_empty()) {
                        this_thread::yield(); // Wait for producer to enqueue
                    }
                    op_arr[i] = queue.dequeue();
                }
            };

            // Launch producer threads
            int start_index = 0;
            for (size_t i = 0; i < num_threads; ++i) {
                int end_index = start_index + elements_per_thread + (remaining_elements > 0 ? 1 : 0);
                remaining_elements = remaining_elements > 0 ? remaining_elements - 1 : 0;
                threads.emplace_back(producer, start_index, end_index);
                start_index = end_index;
            }

            // Launch consumer threads
            start_index = 0;
            remaining_elements = len % num_threads;
            for (size_t i = 0; i < num_threads; ++i) {
                int end_index = start_index + elements_per_thread + (remaining_elements > 0 ? 1 : 0);
                remaining_elements = remaining_elements > 0 ? remaining_elements - 1 : 0;
                threads.emplace_back(consumer, start_index, end_index);
                start_index = end_index;
            }

            // Join all threads
            for (auto &t : threads) {
                t.join();
            }
            break;
		}
		case SGL_STK: {
			if (len == 0 || num_threads == 0) {
				cerr << "Error: Invalid input. 'len' and 'num_threads' must be greater than 0." << endl;
				break;
			}

			FLAT_COMBINE_SGL_STACK stack;
			vector<thread> threads;

			mutex stack_mutex;                   // Protects access to the stack

			size_t chunk_size = (len + num_threads - 1) / num_threads; // Ensure at least one element per thread

			if (chunk_size == 0) {
				cerr << "Error: Chunk size is 0, adjust the number of threads or data size." << endl;
				break;
			}

			// Single thread to push and pop
			threads.emplace_back([&stack, &ip_arr, &op_arr, len, &stack_mutex]() {
				// Push all elements from ip_arr to stack
				for (size_t i = 0; i < static_cast<size_t>(len); ++i) {
					{
						lock_guard<mutex> lock(stack_mutex); // Lock the stack
						stack.push(ip_arr[i]); // Push element to stack
					}
				}

				// Pop elements from the stack and store them in op_arr
				for (size_t i = 0; i < static_cast<size_t>(len); ++i) {
					uint32_t value;
					if (!stack.is_empty()) {
						value = stack.pop(); // Safe to pop as the stack is not empty
						op_arr[i] = value; // Store the popped value in op_arr
					}
				}
			});
			// Join the thread
			for (auto& t : threads) t.join();
			break;
		}

		case FC_SGL_STACK: {
			if (len == 0 || num_threads == 0) {
				cerr << "Error: Invalid input. 'len' and 'num_threads' must be greater than 0." << endl;
				break;
			}

			FLAT_COMBINE_SGL_STACK stack;
			vector<thread> threads;

			mutex stack_mutex;                   // Protects access to the stack

			size_t chunk_size = (len + num_threads - 1) / num_threads; // Ensure at least one element per thread

			if (chunk_size == 0) {
				cerr << "Error: Chunk size is 0, adjust the number of threads or data size." << endl;
				break;
			}

			// Single thread to push and pop
			threads.emplace_back([&stack, &ip_arr, &op_arr, len, &stack_mutex]() {
				// Push all elements from ip_arr to stack
				for (size_t i = 0; i < static_cast<size_t>(len); ++i) {
					{
						lock_guard<mutex> lock(stack_mutex); // Lock the stack
						stack.push(ip_arr[i]); // Push element to stack
					}
				}

				// Pop elements from the stack and store them in op_arr
				for (size_t i = 0; i < static_cast<size_t>(len); ++i) {
					uint32_t value;
					if (!stack.is_empty()) {
						value = stack.pop(); // Safe to pop as the stack is not empty
						op_arr[i] = value; // Store the popped value in op_arr
					}
				}
			});
			// Join the thread
			for (auto& t : threads) t.join();
			break;
		}
		case E_SGL_STACK: {
			
			if (len == 0 || num_threads == 0) {
				cerr << "Error: Invalid input. 'len' and 'num_threads' must be greater than 0." << endl;
				break;
			}

			ELIMINATION_SGL_STACK stack;
			vector<thread> threads;

			mutex stack_mutex;                   // Protects access to the stack

			size_t chunk_size = (len + num_threads - 1) / num_threads; // Ensure at least one element per thread

			if (chunk_size == 0) {
				cerr << "Error: Chunk size is 0, adjust the number of threads or data size." << endl;
				break;
			}

			// Single thread to push and pop
			threads.emplace_back([&stack, &ip_arr, &op_arr, len, &stack_mutex]() {
				// Push all elements from ip_arr to stack
				for (size_t i = 0; i < static_cast<size_t>(len); ++i) {
					{
						lock_guard<mutex> lock(stack_mutex); // Lock the stack
						stack.push(ip_arr[i]); // Push element to stack
					}
				}

				// Pop elements from the stack and store them in op_arr
				for (size_t i = 0; i < static_cast<size_t>(len); ++i) {
					uint32_t value;
					if (!stack.is_empty()) {
						value = stack.pop(); // Safe to pop as the stack is not empty
						op_arr[i] = value; // Store the popped value in op_arr
					}
				}
			});
			// Join the thread
			for (auto& t : threads) t.join();
			break;
		}
		case T_STACK: {
			
			if (len == 0 || num_threads == 0) {
				cerr << "Error: Invalid input. 'len' and 'num_threads' must be greater than 0." << endl;
				break;
			}

			TRIEBER_STACK stack;
			vector<thread> threads;

			mutex stack_mutex;                   // Protects access to the stack

			size_t chunk_size = (len + num_threads - 1) / num_threads; // Ensure at least one element per thread

			if (chunk_size == 0) {
				cerr << "Error: Chunk size is 0, adjust the number of threads or data size." << endl;
				break;
			}

			// Single thread to push and pop
			threads.emplace_back([&stack, &ip_arr, &op_arr, len, &stack_mutex]() {
				// Push all elements from ip_arr to stack
				for (size_t i = 0; i < static_cast<size_t>(len); ++i) {
					{
						lock_guard<mutex> lock(stack_mutex); // Lock the stack
						stack.push(ip_arr[i]); // Push element to stack
					}
				}

				// Pop elements from the stack and store them in op_arr
				for (size_t i = 0; i < static_cast<size_t>(len); ++i) {
					uint32_t value;
					if (!stack.is_empty()) {
						value = stack.pop(); // Safe to pop as the stack is not empty
						op_arr[i] = value; // Store the popped value in op_arr
					}
				}
			});
			// Join the thread
			for (auto& t : threads) t.join();
			break;
		}
		case FC_T_STACK: {
			
			if (len == 0 || num_threads == 0) {
				cerr << "Error: Invalid input. 'len' and 'num_threads' must be greater than 0." << endl;
				break;
			}

			FLAT_COMBINE_T_STACK stack;
			vector<thread> threads;

			mutex stack_mutex;                   // Protects access to the stack

			size_t chunk_size = (len + num_threads - 1) / num_threads; // Ensure at least one element per thread

			if (chunk_size == 0) {
				cerr << "Error: Chunk size is 0, adjust the number of threads or data size." << endl;
				break;
			}

			// Single thread to push and pop
			threads.emplace_back([&stack, &ip_arr, &op_arr, len, &stack_mutex]() {
				// Push all elements from ip_arr to stack
				for (size_t i = 0; i < static_cast<size_t>(len); ++i) {
					{
						lock_guard<mutex> lock(stack_mutex); // Lock the stack
						stack.push(ip_arr[i]); // Push element to stack
					}
				}

				// Pop elements from the stack and store them in op_arr
				for (size_t i = 0; i < static_cast<size_t>(len); ++i) {
					uint32_t value;
					if (!stack.is_empty()) {
						value = stack.pop(); // Safe to pop as the stack is not empty
						op_arr[i] = value; // Store the popped value in op_arr
					}
				}
			});
			// Join the thread
			for (auto& t : threads) t.join();
			break;
		}
		case E_T_STACK: {
			
			if (len == 0 || num_threads == 0) {
				cerr << "Error: Invalid input. 'len' and 'num_threads' must be greater than 0." << endl;
				break;
			}

			ELIMINATION_T_STACK stack;
			vector<thread> threads;

			mutex stack_mutex;                   // Protects access to the stack

			size_t chunk_size = (len + num_threads - 1) / num_threads; // Ensure at least one element per thread

			if (chunk_size == 0) {
				cerr << "Error: Chunk size is 0, adjust the number of threads or data size." << endl;
				break;
			}

			// Single thread to push and pop
			threads.emplace_back([&stack, &ip_arr, &op_arr, len, &stack_mutex]() {
				// Push all elements from ip_arr to stack
				for (size_t i = 0; i < static_cast<size_t>(len); ++i) {
					{
						lock_guard<mutex> lock(stack_mutex); // Lock the stack
						stack.push(ip_arr[i]); // Push element to stack
					}
				}

				// Pop elements from the stack and store them in op_arr
				for (size_t i = 0; i < static_cast<size_t>(len); ++i) {
					uint32_t value;
					if (!stack.is_empty()) {
						value = stack.pop(); // Safe to pop as the stack is not empty
						op_arr[i] = value; // Store the popped value in op_arr
					}
				}
			});
			// Join the thread
			for (auto& t : threads) t.join();
			break;
		}
		default: {
			cout << "No such conatiner avaialable\n";
			break;
		}
	}

}