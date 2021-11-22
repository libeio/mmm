#pragma once

#include "primitive.hpp"
#include "compare.hpp"

#include <vector>
#include <type_traits>

#include <stdint.h>
#include <stddef.h>

template<typename T>
class Vertex
{
public:
    using value_type      = T;
    using decay_type      = typename std::decay<T>::type;
    using proto_type      = typename std::remove_pointer<decay_type>::type;
    using reference       = typename std::add_lvalue_reference<proto_type>::type;
    using const_reference = typename std::add_const<reference>::type;
    using pointer         = typename std::add_pointer<proto_type>::type;
    using const_pointer   = typename std::add_const<pointer>::type;
    using size_type       = size_t;
    using return_type     = typename std::remove_const<T>::type;
protected:
    T _o;
public:
    return_type O() const { return _o; }
    void O(T o) { _o = o; };

    Vertex(T o) : _o(o) {}
    ~Vertex() {}
};

template<typename T>
class Arc
{
public:
    using value_type      = T;
    using decay_type      = typename std::decay<T>::type;
    using proto_type      = typename std::remove_pointer<decay_type>::type;
    using reference       = typename std::add_lvalue_reference<proto_type>::type;
    using const_reference = typename std::add_const<reference>::type;
    using pointer         = typename std::add_pointer<proto_type>::type;
    using const_pointer   = typename std::add_const<pointer>::type;
    using size_type       = size_t;
    using return_type     = typename std::remove_const<T>::type;
protected:
    uint32_t _w;
    T        _o;
public:
    uint32_t W() { return _w; }
    void W(const uint32_t w) { _w = w; }
    return_type O() const { return _o; }
    void O(T o) { _o = o; };

    Arc(T o) : _o(o), _w(INFINITY_MAX) {}
    ~Arc() {}
};

template<typename V, typename A>
class MGraph
{
protected:
    std::vector<V> _vex;
    std::vector<std::vector<A>> _arc;
    size_t _arcnum;
    GraphKind _kind;
public:
    void create(GraphKind kind);
};

template<typename V, typename A>
void
MGraph<V,A>::create(GraphKind kind)
{
    _kind = kind;
}
