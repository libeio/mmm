#pragma once

#include "xpack/json.h"

#include <string>
#include <vector>

struct MetroPP
{
    std::string p1;
    std::string p2;
    int32_t dut;
    int32_t dis;
    std::vector<std::string> lnos;

    XPACK(O(p1,p2,dut,dis,lnos));
};
