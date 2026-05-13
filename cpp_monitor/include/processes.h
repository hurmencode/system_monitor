#pragma once

#include <string>
#include <vector>

enum class SortMode {
    Cpu,
    Ram,
    Pid
};

struct ProcessInfo {
    int pid;
    std::string name;
    long long cpu_time;
    double cpu_percent;
    long long ram_kb;
};


std::string GetProcessName(int pid);
std::vector<ProcessInfo> GetProcesses(SortMode sort_mode);
long long GetProcessCpuTime(int pid);
long long GetProcessRamKb (int pid);