#include <thread>
#include <vector>
#include <iostream>
#include <chrono>
#include <cmath>

void cpuStress(int iterations) {
    double result = 0.0;
    for (int i = 0; i < iterations; i++) {
        result += sin(i) * cos(i);
    }
}

int main() {
    const int numThreads = std::thread::hardware_concurrency();
    const int iterations = 1e7;
    std::vector<std::thread> threads;

    auto start = std::chrono::steady_clock::now();
    while (true) {
        for (int i = 0; i < numThreads; i++) {
            threads.emplace_back(cpuStress, iterations);
        }
        for (auto& t : threads) {
            t.join();
        }
        threads.clear();

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        if (elapsed >= 270) { // Stop after 270 seconds
            break;
        }
    }
    std::cout << "CPU stress completed after 4.5 minutes\n";
    return 0;
}
