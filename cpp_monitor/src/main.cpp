#include <iostream>

#include "cpu.h"
#include "ram.h"

int main() {
    double cpu = GetCpuUsage();
    RamInfo ram = GetRamInfo();

    std::cout << "CPU usage: " << cpu << "%" << std::endl;
    std::cout << "RAM: "
              << (ram.total_kb - ram.available_kb) / 1024
              << " / "
              << ram.total_kb / 1024
              << " MB" << std::endl;
}