#include "cpu.h"

#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

static std::vector<long long> ReadCpuTimes() {
    std::ifstream file("/proc/stat");
    std::string line;

    std::getline(file, line);

    std::istringstream iss(line);
    std::string cpu;
    iss >> cpu;

    std::vector<long long> times;
    long long value;

    while (iss >> value) {
        times.push_back(value);
    }

    return times;    
}

static long long GetTotal(const std::vector<long long>& times) {
    long long sum = 0;

    for (auto value : times) {
        sum += value;
    }

    return sum;
}

static long long GetIdle(const std::vector<long long>& times) {
    return times[3] + times[4];
}

double GetCpuUsage() {
    static long long prev_total = 0;
    static long long prev_idle = 0;
    static bool first = true;;

    auto times_2 = ReadCpuTimes();
    auto total_2 = GetTotal(times_2);
    auto idle_2 = GetIdle(times_2);

    if (first) {
        prev_total = total_2;
        prev_idle = idle_2;
        first = false;
        return 0.0;
    }
    auto total_delta = total_2 - prev_total;
    auto idle_delta = idle_2 - prev_idle;

    prev_total = total_2;
    prev_idle =idle_2;

    if (total_delta == 0) {
        return 0.0;
    }

    return (1.0 - (double)idle_delta / total_delta) * 100.0;
}