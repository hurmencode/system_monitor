#include "processes.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

std::vector<int> GetPids() {
    std::vector<int> pids;
    std::error_code ec;
    
    for (const auto& entry : std::filesystem::directory_iterator("/proc", ec)) {
        if (!entry.is_directory()) { 
            continue; 
        }
        
        if (ec) {
            break;
        }

        std::string name = entry.path().filename().string();

        if (std::all_of(name.begin(), name.end(), 
                [](unsigned char c) {return std::isdigit(c); })) {
            pids.push_back(std::stoi(name));
        }
    }

    std::sort(pids.begin(), pids.end());
    return pids;
}

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