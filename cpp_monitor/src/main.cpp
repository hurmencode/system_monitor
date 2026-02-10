#include <iostream>

#include "cpu.h"
#include "json_output.h"
#include "ram.h"
#include "uptime.h"

int main() {
    double cpu = GetCpuUsage();
    RamInfo ram = GetRamInfo();
    double uptime = GetUptimeSeconds();

    auto ram_used = (ram.total_kb - ram.available_kb) / 1024;
    auto ram_total = ram.total_kb / 1024;

    std::string json = BuildJsonOutput (cpu, ram_used, ram_total, uptime);

    std::cout << json << std::endl;
}