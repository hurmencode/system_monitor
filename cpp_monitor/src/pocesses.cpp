#include "processes.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

static std::unordered_map<int, long long> previous_cpu_times;

std::string GetProcessName (int pid) {
    std::string path = "/proc/" + std::to_string(pid) + "/comm";

    std::ifstream file(path);
    if (!file.is_open()){
        return "";
    }

    std::string name;
    std::getline(file, name);

    return name;
}

std::vector<ProcessInfo> GetProcesses(SortMode sort_mode) {
    std::vector<ProcessInfo> processes;

    for (const auto& entry : std::filesystem::directory_iterator("/proc")) {
        if (!entry.is_directory()) {
            continue;
        }

        std::string dir_name = entry.path().filename().string();

        if (!std::all_of(dir_name.begin(), dir_name.end(), 
        [](unsigned char c) { 
            return std::isdigit(c); 
        })) {
            continue;
        }

        int pid = std::stoi(dir_name);

        ProcessInfo proc;
        proc.pid = pid;
        proc.name = GetProcessName(pid);
        proc.cpu_time = GetProcessCpuTime(pid);
        proc.ram_kb = GetProcessRamKb(pid);

        if (previous_cpu_times.find(pid) != previous_cpu_times.end()) {
            long long prev = previous_cpu_times[pid];

            long long delta = proc.cpu_time - prev;

            proc.cpu_percent = delta / 100.0;
        } else {
            proc.cpu_percent = 0.0;
        }
        
        previous_cpu_times[pid] = proc.cpu_time;

        processes.push_back(proc);
    }

    switch (sort_mode) {
        case SortMode::Cpu:
            std::sort(processes.begin(), processes.end(),
            [](const ProcessInfo& a, const ProcessInfo& b){
                return a.cpu_percent > b.cpu_percent;
            });

            break;

        case SortMode::Ram:
            std::sort(processes.begin(), processes.end(),
            [](const ProcessInfo& a, const ProcessInfo& b){
                return a.ram_kb > b.ram_kb;
            });

            break;

        case SortMode::Pid:
            std::sort(processes.begin(), processes.end(),
            [](const ProcessInfo& a, const ProcessInfo& b){
                return a.pid > b.pid;
            });

            break;
    }

    return processes;
}

long long GetProcessCpuTime (int pid) {
    std::string path = "/proc/" + std::to_string(pid) + "/stat";

    std::ifstream file(path);

    if(!file.is_open()){
        return 0;
    }

    std::string line;
    std::getline(file, line);

    std::istringstream iss(line);

    std::string value;
    long long utime = 0;
    long long stime = 0;

    for (int i = 1; i <= 15; ++i) {
        iss >> value;

        if (i == 14) {
            utime = std::stoll(value);
        }

        if (i == 15) {
            stime = std::stoll(value);
        }
    }

    return utime + stime;
}

long long GetProcessRamKb (int pid) {
    std::string path = "/proc/" + std::to_string(pid) + "/status";

    std::ifstream file(path);

    if (!file.is_open()){
        return 0;
    }

    std::string line;

    while (std::getline(file, line)) {
        if (line.rfind("VmRSS:", 0) == 0) {
            std::istringstream iss(line);

            std::string key;
            long long value;
            std::string unit;

            iss >> key >> value >> unit;

            return value;
        }
    }

    return 0;
}

std::string GetCpuColor (double cpu_percent) {
    if (cpu_percent > 50.0) {
        return "\033[31m";
    }

    if (cpu_percent > 10.0) {
        return "\033[33m";
    }
    
    return "\033[32m";
}