#include <vector>
#include <random>
#include <iostream>
#include <chrono>
#include <thread>

void memoryStress(size_t arraySize) {
    // Allocate a large buffer and initialize it
    std::vector<int> data(arraySize, 42);
    std::mt19937 rng;
    std::uniform_int_distribution<size_t> dist(0, arraySize - 1);

    // Perform repeated random accesses to stress memory bandwidth
    for (size_t i = 0; i < arraySize * 100; i++) {
        data[dist(rng)] = i % 100; // Randomly write data
    }
}

int main() {
    // Allocate a large array (e.g., ~3GB for 32-bit integers)
    const size_t arraySize = 1L * 1024 * 1024 * 1024 / sizeof(int);
    const int numThreads = 8;
    std::vector<std::thread> threads;

    auto start = std::chrono::steady_clock::now();
    while (true) {
        // Launch threads to perform memory stress in parallel
        for (int i = 0; i < numThreads; i++) {
            threads.emplace_back(memoryStress, arraySize);
        }
        // Wait for all threads to finish
        for (auto& t : threads) {
            t.join();
        }
        threads.clear();

        // Check elapsed time
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        if (elapsed >= 270) { // Stop after 270 seconds (4.5 minutes)
            break;
        }
    }
    std::cout << "Memory stress completed after 4.5 minutes\n";
    return 0;
}
