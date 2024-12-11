#ifndef TIMER_H
#define TIMER_H
#include <chrono>
#include <iostream>
#include <thread> // For std::this_thread::sleep_for in the example

class Timer {
public:
    // Type aliases for convenience
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    using Duration = std::chrono::duration<double>;

    // Constructor initializes the timer but does not start it
    Timer() : m_elapsed(Duration::zero()), m_running(false) {}

    // Starts or resumes the timer
    void start() {
        if (!m_running) {
            m_start = Clock::now();
            m_running = true;
        }
    }

    // Stops or pauses the timer
    void stop() {
        if (m_running) {
            auto end = Clock::now();
            m_elapsed += end - m_start;
            m_running = false;
        }
    }

    // Resets the timer to zero
    void reset() {
        m_elapsed = Duration::zero();
        m_running = false;
    }

    // Returns the total elapsed time in seconds
    double elapsed() const {
        if (m_running) {
            auto current = Clock::now();
            return (m_elapsed + (current - m_start)).count();
        } else {
            return m_elapsed.count();
        }
    }

private:
    TimePoint m_start;    // Start time point
    Duration m_elapsed;   // Accumulated elapsed time
    bool m_running;       // Timer state
};
#endif
