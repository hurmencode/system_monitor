#include "ram.h"

#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

static long long ExtractValueKb(const std::string& line) {
    long long value = 0;
    std::string key;
    std::string unit;

    std::istringstream iss(line);
    iss >> key >> value >> unit;

    return value;
}

RamInfo GetRamInfo() {
    std::ifstream file("/proc/meminfo");
    std::string line;

    RamInfo info = {0, 0};

    while(std::getline(file, line)) {
        if (line.rfind("MemTotal:", 0) == 0) {
            info.total_kb = ExtractValueKb(line);
        } else if (line.rfind("MemAvailable:")) {
            info.available_kb = ExtractValueKb(line);
        }

        if (info.total_kb && info.available_kb){
            break;
        }        
    }

    return info;
}