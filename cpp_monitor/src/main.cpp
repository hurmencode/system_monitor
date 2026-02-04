#include <iostream>
#include "cpu.h"

int main() {
    double cpu = GetCpuUsage();
    std::cout << "CPU usage: " << cpu << "%" << std::endl;
}