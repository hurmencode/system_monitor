#include <chrono>
#include <iostream>
#include <thread>

#include "cpu.h"
#include "json_output.h"
#include "processes.h"
#include "ram.h"
#include "uptime.h"

int main(int argc, char* argv[]) {
    int watch_interval = 0;

    GetCpuUsage();

    //auto pids = GetPids();

    //std::cout << "Processes: " << pids.size() << std::endl;

    bool use_json = true;
    for(int i = 1; i < argc; ++i){
        std::string arg = argv[i];

        if(arg == "--human") {
            use_json = false;
        } else if(arg == "--json") {
            use_json = true;
        } else if (arg == "--watch" && i + 1 < argc) {
            watch_interval = std::stoi(argv[i + 1]);
            i++;
        }
    }

    do{
        double cpu = GetCpuUsage();
        RamInfo ram = GetRamInfo();
        double uptime = GetUptimeSeconds();

        auto ram_used = (ram.total_kb - ram.available_kb) / 1024;
        auto ram_total = ram.total_kb / 1024;

        if (!use_json && watch_interval > 0) {
            std::cout << "=== REFRESH ===" << std::endl;
            std::cout << "\033[2J\033[H";
            //std::cerr << "Watch mode is not supported with JSON output\n";
            //return 1;
        }

        if (use_json) {
            std::cout << BuildJsonOutput(cpu, ram_used, ram_total, uptime) << std::endl;
        } else {
            std::cout << BuildHumanOutput(cpu, ram_used, ram_total, uptime) << std::endl;
        }

        if (watch_interval > 0 ){
            std::this_thread::sleep_for(std::chrono::seconds(watch_interval));
        }
        
    } while (watch_interval > 0);
}