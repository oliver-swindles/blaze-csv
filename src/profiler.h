#include <iostream>
#include <chrono>

struct Timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::duration<float> duration;

    // When object is created, start timer
    Timer() {
        start = std::chrono::high_resolution_clock::now();
    }

    // When obj is destroyed, end timer
    ~Timer() {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        float ms = duration.count() * 1000.0f;
        std::cout << std::endl
                  << "\n--- Profiling Report ---\n"
                  << "Program took: " <<  ms << "ms to run."
                  << std::endl;
    }
};