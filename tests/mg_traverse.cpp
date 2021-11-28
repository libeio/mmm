
#include "mgraph.hpp"
#include "inode.hpp"

int main()
{
    MGraph<Vertex<VNode>, Arc<ANode>> mg(DG);

    mg.upsert_arc(Vertex<VNode>(VNode("A")), Vertex<VNode>(VNode("B")), Arc<ANode>(6));
    mg.upsert_arc(Vertex<VNode>(VNode("A")), Vertex<VNode>(VNode("F")), Arc<ANode>(14));
    mg.upsert_arc(Vertex<VNode>(VNode("B")), Vertex<VNode>(VNode("C")), Arc<ANode>(8));
    mg.upsert_arc(Vertex<VNode>(VNode("B")), Vertex<VNode>(VNode("F")), Arc<ANode>(12));
    mg.upsert_arc(Vertex<VNode>(VNode("D")), Vertex<VNode>(VNode("C")), Arc<ANode>(3));
    mg.upsert_arc(Vertex<VNode>(VNode("D")), Vertex<VNode>(VNode("E")), Arc<ANode>(4));
    mg.upsert_arc(Vertex<VNode>(VNode("D")), Vertex<VNode>(VNode("F")), Arc<ANode>(7));
    mg.upsert_arc(Vertex<VNode>(VNode("E")), Vertex<VNode>(VNode("A")), Arc<ANode>(9));
    
    
    printf("顶点(%lu):\n", mg.vertex_size());
    for (size_t i = 0; i < mg.vertex_size(); i++) {
        Vertex<VNode> x = mg.get_vertex(i);
        printf("%2lu: %s ", i, x.get()._name.c_str());
    }
    printf("\n");

    printf("DFS:\n");
    mg.dfs();
    
    printf("BFS:\n");
    mg.bfs();

    return 0;
}
