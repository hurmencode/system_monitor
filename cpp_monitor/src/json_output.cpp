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
    json << "   \"cpu\": {" << std::endl;
    json << "       \"usage\": " << std::fixed << std::setprecision(2) << cpu_usage << " %" << std::endl;
    json << "   }," << std::endl;

    json << "    \"ram\": {" << std::endl;
    json << "       \"used\": " << ram_used << " mb," << std::endl;
    json << "       \"total\": " << ram_total << " mb" << std::endl;
    json << "   }," << std::endl;

    json << "   \"uptime\": " << std::fixed << std::setprecision(2) << uptime << " seconds" << std::endl;

    json << "}";

    return json.str();
}