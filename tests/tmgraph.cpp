
#include "mgraph.hpp"
#include "inode.hpp"
#include <iostream>
#include <vector>
#include <utility>

using PSS = std::pair<std::string, std::string>;

int main()
{
    std::vector<std::string> vvs = { "A", "B", "C", "D" };
    std::vector<PSS> vas = { PSS("A", "B"), PSS("A", "C"), PSS("A", "D"), PSS("C", "A"), PSS("C", "B"), PSS("D", "A") };

    MGraph<Vertex<VNode>, Arc<ANode>> mg(DG);

    for (auto & e : vvs) {
        mg.insert_vertex(Vertex<VNode>(VNode(e)));
    }
    for (auto & e : vas) {
        mg.upsert_arc(Vertex<VNode>(VNode(e.first)), Vertex<VNode>(VNode(e.second)), Arc<ANode>(1));
    }
    mg.display();
    
    mg.upsert_vertex(Vertex<VNode>(VNode("E")));
    mg.display();

    mg.insert_arc(Vertex<VNode>(VNode("C")), Vertex<VNode>(VNode("E")), Arc<ANode>(1));
    mg.display();

    mg.insert_vertex(Vertex<VNode>(VNode("C")));
    mg.display();

    mg.delete_vertex(Vertex<VNode>(VNode("C")));
    mg.display();

    return 0;
}
