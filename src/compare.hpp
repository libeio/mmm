#pragma once

#include "inode.hpp"

template<typename V>
bool _compare_equal(V v1, V v2)
{
    return false;
}

template<>
bool _compare_equal(const VertexNode& v1, const VertexNode& v2)
{
    return v1.label == v2.label;
}
