
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
        mg.upsert_arc(Vertex<VNode>(VNode(e.first)), Vertex<VNode>(VNode(e.second)), Arc<ANode>(100));
    }
    mg.display();
    
    // 增加顶点(如果存在则更新)
    mg.upsert_vertex(Vertex<VNode>(VNode("E")));
    mg.display();
    
    // 增加边(如果存在则返回)
    mg.insert_arc(Vertex<VNode>(VNode("B")), Vertex<VNode>(VNode("E")), Arc<ANode>(10));
    mg.insert_arc(Vertex<VNode>(VNode("E")), Vertex<VNode>(VNode("D")), Arc<ANode>(15));
    mg.display();

    // 增加顶点(如果存在则返回)
    mg.insert_vertex(Vertex<VNode>(VNode("C")));
    mg.display();

    // 删除顶点
    mg.delete_vertex(Vertex<VNode>(VNode("C")));
    mg.display();

    mg.delete_arc(Vertex<VNode>(VNode("A")), Vertex<VNode>(VNode("B")));
    mg.display();

    // 增加新顶点
    mg.insert_vertex(Vertex<VNode>(VNode("F")));
    mg.display();

    // 增加新边
    mg.insert_arc(Vertex<VNode>(VNode("A")), Vertex<VNode>(VNode("F")), Arc<ANode>(20));
    mg.display();

    Vertex<VNode> x = mg.get_vertex(2);
    std::cout << x.get()._name << std::endl;
    
    return 0;
}
