#include "json_output.h"

#include <sstream>
#include <iomanip>

std::string BuildJsonOutput(
    double cpu_usage,
    long long ram_used,
    long long ram_total,
    double uptime
) {
    std::ostringstream json;

    json << "{" << std::endl;
    json << "  \"cpu\": {" << std::endl;
    json << "    \"usage\": " << std::fixed << std::setprecision(2) << cpu_usage << std::endl;
    json << "  }," << std::endl;

    json << "  \"ram\": {" << std::endl;
    json << "    \"used\": " << ram_used << "," << std::endl;
    json << "    \"total\": " << ram_total << std::endl;
    json << "  }," << std::endl;

    json << "  \"uptime\": " << std::fixed << std::setprecision(2) << uptime << std::endl;

    json << "}";

    return json.str();
}

std::string BuildHumanOutput(
    double cpu, 
    long long ram_used, 
    long long ram_total, 
    double uptime
) {
    std::ostringstream output;

    output << "CPU Usage: " << std::fixed << std::setprecision(2) << cpu << " %" << std::endl;
    output << "Ram: " << ram_used << " / " << ram_total << " MB" << std::endl;
    output << "Uptime " << std::fixed << std::setprecision(2) << uptime << " s" << std::endl;

    return output.str();
}
