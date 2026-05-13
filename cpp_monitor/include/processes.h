#pragma once

#include <string>
#include <vector>

struct ProcessInfo {
    int pid;
    std::string name;
    long long cpu_time;
    double cpu_percent;
    long long ram_kb;
};


std::string GetProcessName(int pid);
std::vector<ProcessInfo> GetProcesses();
long long GetProcessCpuTime(int pid);
long long GetProcessRamKb (int pid);