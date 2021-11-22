
#include "xstruct.hpp"
#include "common.hpp"

#include <vector>
#include <string>
#include <stdio.h>

int main()
{
    std::string metro_json("metro.json");

    std::vector<MetroPP> mpps;
    xpack::json::decode_file(metro_json, mpps);  

    for (auto & mpp : mpps) {
        ECHO(INFO, "%s,%s,%d,%d", mpp.p1.c_str(), mpp.p2.c_str(), mpp.dut, mpp.dis);
        for (auto & lno : mpp.lnos) {
            ECHO(INFO, "  %s", lno.c_str());
        }
    }

    return 0;
}