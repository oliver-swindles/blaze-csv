#include <iostream>
#include <chrono>
#include <string>

struct Timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::duration<float> duration;
    std::string timerName;

    // When object is created, start timer
    Timer(std::string profileName) {
        start = std::chrono::high_resolution_clock::now();
        timerName = profileName;
    }

    // When obj is destroyed, end timer
    ~Timer() {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        float ms = duration.count() * 1000.0f;
        std::cout << std::endl
                //   << "\n--- Profiling Report ---\n"
                //   << "Program took: " <<  ms << "ms to run."
                  << timerName << " took " << ms << "ms to run."
                  << std::endl;
    }
};