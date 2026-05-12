#pragma once

#include <string>
#include <vector>

struct ProcessInfo {
    int pid;
    std::string name;
};


std::string GetProcessName(int pid);
std::vector<ProcessInfo> GetProcesses();