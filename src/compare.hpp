#pragma once

#include "inode.hpp"

template<typename V>
bool _compare_equal(V v1, V v2)
{
    (void) v1;
    (void) v2;
    return false;
}

template<>
bool _compare_equal(VNode v1, VNode v2)
{
    return v1._name == v2._name;
}
