#include "processes.h"

#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

std::vector<int> GetPids() {
    std::vector<int> pids;
    
    for (const auto& entry : std::filesystem::directory_iterator("/proc")) {
        if (!entry.is_directory()) {
            continue;
        }

        std::string name = entry.path().filename().string();

        if (std::all_of(name.begin(), name.end(), ::isdigit)) {
            pids.push_back(std::stoi(name));
        }
    }

    return pids;
}