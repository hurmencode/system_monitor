#pragma once

struct RamInfo{
    long long total_kb;
    long long available_kb;
};


RamInfo GetRamInfo();