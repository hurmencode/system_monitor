#include <iostream>
#include <fstream>

int main() {
    std::ifstream stat_file("/proc/stat");
    std::string line;
    std::getline(stat_file, line);
    std::cout << line << std::endl;
    stat_file.close();
}