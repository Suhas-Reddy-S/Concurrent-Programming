#include "main.hpp"
#include "stacks.hpp"
#include "queues.hpp"

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
			SGL_STACK stack;
			std::vector<std::thread> producers, consumers;

			size_t half_threads = num_threads / 2; // Half for producers, half for consumers
			size_t chunk_size = (len + half_threads - 1) / half_threads; // Divide work among producers

			// Producer threads
			for (size_t i = 0; i < half_threads; ++i) {
				producers.emplace_back([&stack, &ip_arr, i, chunk_size, half_threads, len]() {
					size_t start = i * chunk_size;
					size_t end = (i == half_threads - 1) ? len : start + chunk_size;
					for (size_t j = start; j < end; ++j) {
						stack.push(ip_arr[j]);
					}
				});
			}

			// Consumer threads
			for (size_t i = 0; i < half_threads; ++i) {
				consumers.emplace_back([&stack, &op_arr, i, chunk_size, half_threads, len]() {
					size_t start = i * chunk_size;
					size_t end = (i == half_threads - 1) ? len : start + chunk_size;
					for (size_t j = start; j < end; ++j) {
						while (stack.is_empty()) {
							std::this_thread::yield(); // Wait until the stack has elements
						}
						op_arr[j] = stack.pop();
					}
				});
			}

			// Join threads
			for (auto &t : producers) t.join();
			for (auto &t : consumers) t.join();

            break;
		}
		case FC_SGL_STACK: {
			FLAT_COMBINE_SGL_STACK stack;
			std::vector<std::thread> producers, consumers;

			size_t half_threads = num_threads / 2; // Half for producers, half for consumers
			size_t chunk_size = (len + half_threads - 1) / half_threads; // Divide work among producers

			// Producer threads
			for (size_t i = 0; i < half_threads; ++i) {
				producers.emplace_back([&stack, &ip_arr, i, chunk_size, half_threads, len]() {
					size_t start = i * chunk_size;
					size_t end = (i == half_threads - 1) ? len : start + chunk_size;
					for (size_t j = start; j < end; ++j) {
						stack.push(ip_arr[j]);
					}
				});
			}

			// Consumer threads
			for (size_t i = 0; i < half_threads; ++i) {
				consumers.emplace_back([&stack, &op_arr, i, chunk_size, half_threads, len]() {
					size_t start = i * chunk_size;
					size_t end = (i == half_threads - 1) ? len : start + chunk_size;
					for (size_t j = start; j < end; ++j) {
						while (stack.is_empty()) {
							std::this_thread::yield(); // Wait until the stack has elements
						}
						op_arr[j] = stack.pop();
					}
				});
			}

			// Join threads
			for (auto &t : producers) t.join();
			for (auto &t : consumers) t.join();

            break;
		}
		case E_SGL_STACK: {
			ELIMINATION_SGL_STACK stack;
			std::vector<std::thread> producers, consumers;

			size_t half_threads = num_threads / 2; // Half for producers, half for consumers
			size_t chunk_size = (len + half_threads - 1) / half_threads; // Divide work among producers

			// Producer threads
			for (size_t i = 0; i < half_threads; ++i) {
				producers.emplace_back([&stack, &ip_arr, i, chunk_size, half_threads, len]() {
					size_t start = i * chunk_size;
					size_t end = (i == half_threads - 1) ? len : start + chunk_size;
					for (size_t j = start; j < end; ++j) {
						stack.push(ip_arr[j]);
					}
				});
			}

			// Consumer threads
			for (size_t i = 0; i < half_threads; ++i) {
				consumers.emplace_back([&stack, &op_arr, i, chunk_size, half_threads, len]() {
					size_t start = i * chunk_size;
					size_t end = (i == half_threads - 1) ? len : start + chunk_size;
					for (size_t j = start; j < end; ++j) {
						while (stack.is_empty()) {
							std::this_thread::yield(); // Wait until the stack has elements
						}
						op_arr[j] = stack.pop();
					}
				});
			}

			// Join threads
			for (auto &t : producers) t.join();
			for (auto &t : consumers) t.join();

            break;
		}
		case T_STACK: {
			TRIEBER_STACK stack;
			std::vector<std::thread> producers, consumers;

			size_t half_threads = num_threads / 2; // Half for producers, half for consumers
			size_t chunk_size = (len + half_threads - 1) / half_threads; // Divide work among producers

			// Producer threads
			for (size_t i = 0; i < half_threads; ++i) {
				producers.emplace_back([&stack, &ip_arr, i, chunk_size, half_threads, len]() {
					size_t start = i * chunk_size;
					size_t end = (i == half_threads - 1) ? len : start + chunk_size;
					for (size_t j = start; j < end; ++j) {
						stack.push(ip_arr[j]);
					}
				});
			}

			// Consumer threads
			for (size_t i = 0; i < half_threads; ++i) {
				consumers.emplace_back([&stack, &op_arr, i, chunk_size, half_threads, len]() {
					size_t start = i * chunk_size;
					size_t end = (i == half_threads - 1) ? len : start + chunk_size;
					for (size_t j = start; j < end; ++j) {
						while (stack.is_empty()) {
							std::this_thread::yield(); // Wait until the stack has elements
						}
						op_arr[j] = stack.pop();
					}
				});
			}

			// Join threads
			for (auto &t : producers) t.join();
			for (auto &t : consumers) t.join();

            break;
		}
		case FC_T_STACK: {
			FLAT_COMBINE_T_STACK stack;
			std::vector<std::thread> producers, consumers;

			size_t half_threads = num_threads / 2; // Half for producers, half for consumers
			size_t chunk_size = (len + half_threads - 1) / half_threads; // Divide work among producers

			// Producer threads
			for (size_t i = 0; i < half_threads; ++i) {
				producers.emplace_back([&stack, &ip_arr, i, chunk_size, half_threads, len]() {
					size_t start = i * chunk_size;
					size_t end = (i == half_threads - 1) ? len : start + chunk_size;
					for (size_t j = start; j < end; ++j) {
						stack.push(ip_arr[j]);
					}
				});
			}

			// Consumer threads
			for (size_t i = 0; i < half_threads; ++i) {
				consumers.emplace_back([&stack, &op_arr, i, chunk_size, half_threads, len]() {
					size_t start = i * chunk_size;
					size_t end = (i == half_threads - 1) ? len : start + chunk_size;
					for (size_t j = start; j < end; ++j) {
						while (stack.is_empty()) {
							std::this_thread::yield(); // Wait until the stack has elements
						}
						op_arr[j] = stack.pop();
					}
				});
			}

			// Join threads
			for (auto &t : producers) t.join();
			for (auto &t : consumers) t.join();

            break;
		}
		case E_T_STACK: {
			ELIMINATION_T_STACK stack;
			std::vector<std::thread> producers, consumers;

			size_t half_threads = num_threads / 2; // Half for producers, half for consumers
			size_t chunk_size = (len + half_threads - 1) / half_threads; // Divide work among producers

			// Producer threads
			for (size_t i = 0; i < half_threads; ++i) {
				producers.emplace_back([&stack, &ip_arr, i, chunk_size, half_threads, len]() {
					size_t start = i * chunk_size;
					size_t end = (i == half_threads - 1) ? len : start + chunk_size;
					for (size_t j = start; j < end; ++j) {
						stack.push(ip_arr[j]);
					}
				});
			}

			// Consumer threads
			for (size_t i = 0; i < half_threads; ++i) {
				consumers.emplace_back([&stack, &op_arr, i, chunk_size, half_threads, len]() {
					size_t start = i * chunk_size;
					size_t end = (i == half_threads - 1) ? len : start + chunk_size;
					for (size_t j = start; j < end; ++j) {
						while (stack.is_empty()) {
							std::this_thread::yield(); // Wait until the stack has elements
						}
						op_arr[j] = stack.pop();
					}
				});
			}

			// Join threads
			for (auto &t : producers) t.join();
			for (auto &t : consumers) t.join();

            break;
		}
		default: {
			cout << "No such conatiner avaialable\n";
			break;
		}
	}

}