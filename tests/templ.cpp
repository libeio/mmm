
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

        int x = vi.get();
        std::cout << "before: x=" << x << std::endl;

        vi.set(900);
        x = vi.get();
        std::cout << "after: x=" << x << std::endl;
    }
    // pod 指针类型测试
    {
        int i = 9;
        Vertex<int*> vi(&i);

        int * x = vi.get();
        std::cout << "before: *x=" << *x << std::endl;

        int j = 900;
        vi.set(&j);
        x = vi.get();
        std::cout << "after: *x=" << *x << std::endl;
    }

    // 结构体非指针类型测试
    {
        MyStruct st(10);

        Vertex<MyStruct> vst(st);

        auto x = vst.get();
        std::cout << "before: MyStruct.i=" << x.i << std::endl;

        x.i = 100;
        std::cout << "modify: MyStruct.i=" << x.i << std::endl;

        vst.set(MyStruct(999));
        x = vst.get();
        std::cout << "after: MyStruct.i=" << x.i << std::endl;
    }

    // 结构体指针类型测试
    {
        MyStruct st(20);

        Vertex<MyStruct*> vst(&st);

        auto x = vst.get();
        std::cout << "before: MyStruct.i=" << x->i << std::endl;

        x->i = 200;
        std::cout << "modify: MyStruct.i=" << x->i << std::endl;

        MyStruct _st(777);
        vst.set(&_st);
        x = vst.get();
        std::cout << "after: MyStruct.i=" << x->i << std::endl;
    }

    return 0;
}