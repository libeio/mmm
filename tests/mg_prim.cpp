
#include "mgraph.hpp"
#include "inode.hpp"

int main()
{
    MGraph<Vertex<VNode>, Arc<ANode>> mg(UN);

    mg.upsert_arc(Vertex<VNode>(VNode("A")), Vertex<VNode>(VNode("B")), Arc<ANode>(6));
    mg.upsert_arc(Vertex<VNode>(VNode("A")), Vertex<VNode>(VNode("D")), Arc<ANode>(7));
    mg.upsert_arc(Vertex<VNode>(VNode("A")), Vertex<VNode>(VNode("E")), Arc<ANode>(12));
    mg.upsert_arc(Vertex<VNode>(VNode("B")), Vertex<VNode>(VNode("E")), Arc<ANode>(9));
    mg.upsert_arc(Vertex<VNode>(VNode("B")), Vertex<VNode>(VNode("C")), Arc<ANode>(8));
    mg.upsert_arc(Vertex<VNode>(VNode("C")), Vertex<VNode>(VNode("E")), Arc<ANode>(4));
    mg.upsert_arc(Vertex<VNode>(VNode("C")), Vertex<VNode>(VNode("D")), Arc<ANode>(6));
    mg.upsert_arc(Vertex<VNode>(VNode("D")), Vertex<VNode>(VNode("E")), Arc<ANode>(5));
    
    printf("顶点(%lu):\n", mg.vertex_size());
    for (size_t i = 0; i < mg.vertex_size(); i++) {
        Vertex<VNode> x = mg.get_vertex(i);
        printf("%2lu: %s ", i, x.get()._name.c_str());
    }
    printf("\n");

    auto vpp = mg.prim();
    for (auto && pp : vpp) {
        printf("%u: %s-%s\n", std::get<2>(pp), std::get<0>(pp).get()._name.c_str(), std::get<1>(pp).get()._name.c_str());
    }

    return 0;
}
