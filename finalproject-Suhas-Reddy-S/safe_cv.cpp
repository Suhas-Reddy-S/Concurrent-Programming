#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <vector>
#include <functional>

using namespace std;

class SafeConditionVariable {
public:
    void wait(unique_lock<mutex>& lock, function<bool()> predicate) {
        // Retry on spurious wakeups
        while (!predicate()) {
            cv.wait(lock);
        }
    }

    void notify_one() {
        cv.notify_one();
    }

    void notify_all() {
        cv.notify_all();
    }

private:
    condition_variable cv;
};

// Shared variables for testing
int shared_data = 0;
bool ready = false;
mutex mtx;
SafeConditionVariable safe_cv;

// Worker thread function to wait on the condition
void worker(int id) {
    unique_lock<mutex> lock(mtx);
    safe_cv.wait(lock, [] { return ready; });
    cout << "Thread " << id << " woke up. Shared data: " << shared_data << endl;
}

// Test case
void test_safe_condition_variable() {
    const int num_threads = 5;
    vector<thread> threads;

    // Launch worker threads
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker, i);
    }

    // Simulate some work, then set ready flag and notify all
    {
        unique_lock<mutex> lock(mtx);
        shared_data = 42;
        ready = true;
    }
    safe_cv.notify_all();

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    cout << "All threads completed." << endl;
}

// Comparison with std::condition_variable
void test_std_condition_variable() {
    condition_variable cv;
    bool std_ready = false;
    mutex std_mtx;

    auto std_worker = [&](int id) {
        unique_lock<mutex> lock(std_mtx);
        cv.wait(lock, [&] { return std_ready; });
        cout << "[condition_variable] Thread " << id << " woke up." << endl;
    };

    const int num_threads = 5;
    vector<thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(std_worker, i);
    }

    {
        unique_lock<mutex> lock(std_mtx);
        std_ready = true;
    }
    cv.notify_all();

    for (auto& t : threads) {
        t.join();
    }

    cout << "All threads using condition_variable completed." << endl;
}

int main() {
    cout << "Testing SafeConditionVariable:" << endl;
    test_safe_condition_variable();

    cout << "\nTesting condition_variable:" << endl;
    test_std_condition_variable();

    return 0;
}
