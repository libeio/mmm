
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

    printf("顶点(%lu):\n", mg.vertex_size());
    for (size_t i = 0; i < mg.vertex_size(); i++) {
        Vertex<Station> x = mg.get_vertex(i);
        printf("%2lu: %s ", i, x.get()._name.c_str());
    }
    printf("\n");
    printf("边(%lu):\n", mg.arc_size());

    // mg.display();

    return 0;
}