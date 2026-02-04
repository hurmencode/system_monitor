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
    auto times_1 = ReadCpuTimes();
    auto total_1 = GetTotal(times_1);
    auto idle_1 = GetIdle(times_1);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto times_2 = ReadCpuTimes();
    auto total_2 = GetTotal(times_2);
    auto idle_2 = GetIdle(times_2);

    auto total_delta = total_2 - total_1;
    auto idle_delta = idle_2 - idle_1;

    return (1.0 - (double)idle_delta / total_delta) * 100.0;
}