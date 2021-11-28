
#include "mgraph.hpp"
#include "inode.hpp"

    // InsertVertex(&G, vd);
    // InsertVertex(&G, vc);
    // InsertVertex(&G, vf);
    // InsertVertex(&G, va);
    // InsertVertex(&G, vb);
    // InsertVertex(&G, ve);

int main()
{
    MGraph<Vertex<VNode>, Arc<ANode>> mg(UN);

    mg.upsert_arc(Vertex<VNode>(VNode("V0")), Vertex<VNode>(VNode("V1")), Arc<ANode>(30));
    mg.upsert_arc(Vertex<VNode>(VNode("V0")), Vertex<VNode>(VNode("V2")), Arc<ANode>(60));
    mg.upsert_arc(Vertex<VNode>(VNode("V0")), Vertex<VNode>(VNode("V4")), Arc<ANode>(150));
    mg.upsert_arc(Vertex<VNode>(VNode("V0")), Vertex<VNode>(VNode("V5")), Arc<ANode>(40));
    mg.upsert_arc(Vertex<VNode>(VNode("V1")), Vertex<VNode>(VNode("V2")), Arc<ANode>(40));
    mg.upsert_arc(Vertex<VNode>(VNode("V1")), Vertex<VNode>(VNode("V3")), Arc<ANode>(100));
    mg.upsert_arc(Vertex<VNode>(VNode("V2")), Vertex<VNode>(VNode("V3")), Arc<ANode>(50));
    mg.upsert_arc(Vertex<VNode>(VNode("V3")), Vertex<VNode>(VNode("V4")), Arc<ANode>(30));
    mg.upsert_arc(Vertex<VNode>(VNode("V4")), Vertex<VNode>(VNode("V5")), Arc<ANode>(10));
    
    printf("顶点(%lu):\n", mg.vertex_size());
    for (size_t i = 0; i < mg.vertex_size(); i++) {
        Vertex<VNode> x = mg.get_vertex(i);
        printf("%2lu: %s ", i, x.get()._name.c_str());
    }
    printf("\n");

    auto vpp = mg.shortest_path(Vertex<VNode>(VNode("V0")), Vertex<VNode>(VNode("V4")));
    for (auto && pp : vpp) {
        printf("%u: %s-%s\n", std::get<2>(pp), std::get<0>(pp).get()._name.c_str(), std::get<1>(pp).get()._name.c_str());
    }

    return 0;
}
