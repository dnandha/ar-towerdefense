/**
* ARTD (Augmented Reality Tower Defense)
* Copyright (C) 2019 Jaeger,Stegmueller,Boche,Nandha 
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _COMMON_H
#define _COMMON_H

#include <vector>
#include <chrono>

//typedef std::vector<double> Position;
typedef std::vector<int> WinSize;

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
