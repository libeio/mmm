#pragma once

#include "inode.hpp"
#include <stdio.h>

template<typename V>
void _show_vertex(V v)
{
    (void) v;
    throw "forbid this template function";
}

template<>
void _show_vertex(VNode v)
{
    printf("%s\n", v._name.c_str());
}

template<>
void _show_vertex(Station s) {
    printf("%s\n", s._name.c_str());
}