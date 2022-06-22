//
// Created by zhaohuiqiang on 2022/6/22.
//

#pragma once

#include <unordered_map>
#include <string>

using namespace std;

class [[maybe_unused]] StopWatch {
public:
    // start a new mark
    [[maybe_unused]] inline void Mark(const string &mark = default_mark()) {
        mark_[mark] = chrono::high_resolution_clock::now();
    }

    // return a mark's cost in milliseconds
    [[maybe_unused]] inline double Show(const string &mark = default_mark()) {
        return chrono::duration_cast<chrono::duration<double, milli >>(
                chrono::high_resolution_clock::now() - mark_[mark])
                .count();
    }

    // return a mark's cost in milliseconds
    [[maybe_unused]] inline string ShowTickMills(const string &mark = default_mark()) {
        double cost = Show(mark);
        return string(to_string(cost) + "ms");
    }

    // return a mark's cost in seconds
    [[maybe_unused]] inline string ShowTickSeconds(const string &mark = default_mark()) {
        double cost = Show(mark);
        return string(to_string(cost / 1000.0) + "s");
    }

    // remove mark and return its cost
    [[maybe_unused]] inline double Stop(const string &mark = default_mark()) {
        double cost = Show(mark);
        mark_.erase(mark);
        return cost;
    }

    StopWatch(void) { Mark("__default__"); }

protected:
    static inline string &default_mark(void) {
        static string inst("__default__");
        return inst;
    }

    unordered_map<string, chrono::high_resolution_clock::time_point> mark_;
};
