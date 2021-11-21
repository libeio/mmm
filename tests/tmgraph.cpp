
#include "mgraph.hpp"
#include <iostream>

struct MyStruct
{
    MyStruct(int i) : i(i) {}
    ~MyStruct() {}

    int i;
};

int main()
{
    MGraph<Vertex<int>, Arc<double>> m(UG);

    // 空图中定位

    // 插入节点后定位

    // 插入已存在节点

    // 插入不存在节点

    // 插入已存在节点超容量

    // 插入不存在节点超容量
    
    return 0;
}
