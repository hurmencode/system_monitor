#include "processes.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

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

std::vector<ProcessInfo> GetProcesses() {
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

        processes.push_back(proc);
    }

    std::sort(processes.begin(), processes.end(),
    [](const ProcessInfo& a, const ProcessInfo& b){
        return a.pid > b.pid;
    });

    return processes;
}