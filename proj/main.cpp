
#include "xstruct.hpp"
#include "mgraph.hpp"
#include "common.hpp"

#include <vector>
#include <string>
#include <stdio.h>

int main()
{
    std::string metro_json("metro.json");

    std::vector<MetroPP> mpps;
    xpack::json::decode_file(metro_json, mpps);  

    MGraph<Vertex<Station>, Arc<double>> mg(UN);

    for (auto & mpp : mpps) {
        mg.upsert_arc(Vertex<Station>(Station(mpp.p1)), Vertex<Station>(Station(mpp.p2)), Arc<double>(mpp.dut));
    }

    mg.display();

    return 0;
}