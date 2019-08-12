#ifndef _COMMON_H
#define _COMMON_H

#include <vector>
#include <chrono>

//typedef std::vector<double> Position;
//typedef std::vector<int> WinSize;

/**
 * Time namespace
 */
namespace chrono {
/**
 * Get current time in ms
 */
auto now = []() -> int {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
            ).count();
};
}

/**
 * Clock for keeping track of time
 */
class Clock {
    double _time_prev;
    double _time_now;
    double _delta = 0.0;
    Clock() : _time_now(chrono::now()), _time_prev(chrono::now()) {}
    Clock(Clock const&) = delete;
    void operator=(Clock const&) = delete;
public:
    static Clock& GetInstance() {
        static Clock instance;
        return instance;
    }
    /**
     * Update clock and return delta
     */
    double Tick() {
        _time_now = chrono::now();
        _delta = (_time_now - _time_prev) / 1000.0;
        _time_prev = _time_now;

        return _delta;
    }
};

#endif
