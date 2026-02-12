#include <iostream>

#include "cpu.h"
#include "json_output.h"
#include "ram.h"
#include "uptime.h"

int main(int argc, char* argv[]) {
    double cpu = GetCpuUsage();
    RamInfo ram = GetRamInfo();
    double uptime = GetUptimeSeconds();

    auto ram_used = (ram.total_kb - ram.available_kb) / 1024;
    auto ram_total = ram.total_kb / 1024;

    bool use_json = true;
    for(int i = 1; i < argc; ++i){
        std::string arg = argv[i];

        if(arg == "--human") {
            use_json = false;
        }

        if(arg == "--json") {
            use_json = true;
        }
    }

    if(use_json) {
        std::cout << BuildJsonOutput(cpu, ram_used, ram_total, uptime) << std::endl;
    } else {
        std::cout << BuildHumanOutput(cpu, ram_used, ram_total, uptime) << std::endl;
    }
}