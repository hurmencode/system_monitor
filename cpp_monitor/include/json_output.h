#pragma once

#include <string>

std::string BuildJsonOutput(
    double cpu_usage,
    long long ram_used,
    long long ram_total,
    double uptime
);

std::string BuildHumanOutput(
    double cpu, 
    long long ram_used, 
    long long ram_total, 
    double uptime
);