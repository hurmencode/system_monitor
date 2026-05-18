#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>

#include "cpu.h"
#include "json_output.h"
#include "processes.h"
#include "ram.h"
#include "uptime.h"

void PrintUsage() {
    std::cout << "Usage:\n";
    std::cout << "  ./monitor [options]\n\n";

    std::cout << "Options:\n";
    std::cout << "  --human               Human-readable output\n";
    std::cout << "  --json                JSON output\n";
    std::cout << "  --watch N             Refresh every N seconds\n";
    std::cout << "  --sort cpu|ram|pid    Sort processes\n";
    std::cout << "  --top N               Show top N processes\n";
    std::cout << "  --log FILE            Write output to file\n";
}

int main(int argc, char* argv[]) {
    int watch_interval = 0;
    int top_count = 10;
    std::string log_file;
    SortMode sort_mode = SortMode::Cpu;

    //GetCpuUsage();

    bool use_json = true;
    for(int i = 1; i < argc; ++i){
        std::string arg = argv[i];

        if(arg == "--human") {
            use_json = false;
        } else if(arg == "--json") {
            use_json = true;
        } else if (arg == "--watch" && i + 1 < argc) {
            try {
                watch_interval = std::stoi(argv[i + 1]);
            } catch (...) {
                std::cerr << "Ivalid watch interval" << std::endl;

                PrintUsage();
                return 1;
            }
            i++;
        } else if (arg == "--sort" && i + 1 < argc) {
            std::string value = argv[i + 1];

            if (value == "cpu") {
                sort_mode = SortMode::Cpu;
            } else if (value == "ram") {
                sort_mode = SortMode::Ram;
            } else if (value == "pid") {
                sort_mode = SortMode::Pid;
            } else {
                std::cerr << "Ivalid sort mode: " << value << std::endl;

                PrintUsage();
                return 1;
            }

            i++;
        }else if (arg == "--help") {
            PrintUsage();
            return 0;
        } else if (arg == "--top" && i + 1 < argc) {
            try {
                top_count = std::stoi(argv[i +1]);

                if(top_count <= 0){
                    throw std::invalid_argument("negative");
                }
            } catch (...) {
                std::cerr << "Invalid top count" << std::endl;

                PrintUsage();
                return 1;
            }

            i++;
        } else if (arg == "--log" && i + 1 < argc) {
            log_file = argv[i + 1];
            i++;
        }else {
            std::cerr << "Unknown comand: " << arg << std::endl;

            PrintUsage();
            return 1;
        }
    }

    std::ofstream log_stream;

    if (!log_file.empty()) {
        log_stream.open(log_file, std::ios::app);

        if (!log_stream.is_open()) {
            std::cerr << "Failed to open log file" << std::endl;
            return 1;
        }
    }

    do{
        double cpu = GetCpuUsage();
        RamInfo ram = GetRamInfo();
        double uptime = GetUptimeSeconds();

        auto ram_used = (ram.total_kb - ram.available_kb) / 1024;
        auto ram_total = ram.total_kb / 1024;

        if (!use_json && watch_interval > 0) {
            std::cout << "\033[2J\033[H\n";
        }

        std::string output;

        if (use_json) {
            output = BuildJsonOutput(cpu, ram_used, ram_total, uptime);
            std::cout << output << std::endl;
        } else {
            output = BuildHumanOutput(cpu, ram_used, ram_total, uptime);
            std::cout << output;

            auto processes = GetProcesses(sort_mode);
            std::cout << "\nTop processes: " 
                      << top_count 
                      << std::endl;

            std::cout << "Sort mode: ";

            switch (sort_mode) {
                case SortMode::Cpu:
                    std::cout << "CPU";
                    break;

                case SortMode::Ram:
                    std::cout << "RAM";
                    break;
                    
                case SortMode::Pid:
                    std::cout << "PID";
                    break;
            }

            std::cout << std::endl;

            std::cout << std::left 
                      << std::setw(10) << "PID" 
                      << std::setw(12) << "CPU%"
                      << std::setw(14) << "RAM (MB)"
                      << "NAME" 
                      << std::endl;

            std::cout << std::string(50, '-') << std::endl;

            for (size_t i = 0; i < std::min<size_t>(processes.size(), top_count); ++i) {
                const auto& proc = processes[i];

                std::cout << std::left
                          << std::setw(10) << proc.pid
                          << GetCpuColor(proc.cpu_percent)
                          << std::setw(12) << std::fixed << std::setprecision(2)
                          << proc.cpu_percent
                          << "\033[0m"
                          << std::setw(14) << std::fixed << std::setprecision(2)
                          << static_cast<double>(proc.ram_kb) / 1024
                          << proc.name
                          << std::endl;
            }
        }

        if (log_stream.is_open()) {
            log_stream << output << std::endl;
        }

        if (watch_interval > 0 ){
            std::this_thread::sleep_for(std::chrono::seconds(watch_interval));
        }
        
    } while (watch_interval > 0);
}