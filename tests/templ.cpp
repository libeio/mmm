
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
    // pod 非指针类型测试
    {
        Vertex<int> vi(9);

        int x = vi.O();
        std::cout << "before: x=" << x << std::endl;

        vi.O(900);
        x = vi.O();
        std::cout << "after: x=" << x << std::endl;
    }
    // pod 指针类型测试
    {
        int i = 9;
        Vertex<int*> vi(&i);

        int * x = vi.O();
        std::cout << "before: *x=" << *x << std::endl;

        int j = 900;
        vi.O(&j);
        x = vi.O();
        std::cout << "after: *x=" << *x << std::endl;
    }

    // 结构体非指针类型测试
    {
        MyStruct st(10);

        Vertex<MyStruct> vst(st);

        auto x = vst.O();
        std::cout << "before: MyStruct.i=" << x.i << std::endl;

        x.i = 100;
        std::cout << "modify: MyStruct.i=" << x.i << std::endl;

        MyStruct _st(999);
        vst.O(_st);
        x = vst.O();
        std::cout << "after: MyStruct.i=" << x.i << std::endl;
    }

    // 结构体指针类型测试
    {
        MyStruct st(20);

        Vertex<MyStruct*> vst(&st);

        auto x = vst.O();
        std::cout << "before: MyStruct.i=" << x->i << std::endl;

        x->i = 200;
        std::cout << "modify: MyStruct.i=" << x->i << std::endl;

        MyStruct _st(777);
        vst.O(&_st);
        x = vst.O();
        std::cout << "after: MyStruct.i=" << x->i << std::endl;
    }

    // MGraph
    {
        MGraph<Vertex<int>, Arc<double>> m(UG);
    }

    return 0;
}