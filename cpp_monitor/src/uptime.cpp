#include "uptime.h"

#include <fstream>

double GetUptimeSeconds() {
    std::ifstream file("/proc/uptime");

    double uptime = 0.0;
    double idle = 0.0;

    file >> uptime >> idle;

    return uptime;
}