#pragma once

#include "primitive.hpp"
#include "compare.hpp"

#include <vector>
#include <type_traits>
#include <algorithm>
#include <iterator>
#include <string>

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

template<typename T>
class Vertex
{
    template<typename V, typename A>
    friend class MGraph;
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
    // avoid to use it
    return_type get() const { return _o; }
    void set(T o) { _o = o; };

    // provide to user
    Vertex(const T& o) : _o(o) {}
    ~Vertex() {}
};

template<typename T>
class Arc
{
    template<typename V, typename A>
    friend class MGraph;
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
    // avoid to use it
    uint32_t get_w() { return _w; }
    void set_w(const uint32_t w) { _w = w; }
    return_type get() const { return _o; }
    void set(T o) { _o = o; };

    // provide to user
    Arc(const T& o, uint32_t w) : _o(o), _w(w) {}
    // only for value initialize
    Arc() {}
    ~Arc() {}
};

template<typename V, typename A>
class MGraph
{
protected:
    std::vector<V> _vex;
    std::vector<std::vector<A>> _arc;
    size_t _arcnum;
    uint32_t _mask;
public:
    MGraph(GraphKind kind);
    ~MGraph();

    int locate_vertex(V v);
    int insert_vertex(V v);
    int upsert_vertex(V v);
    int delete_vertex(V v);
    int insert_arc(V v, V w, A a);
    int upsert_arc(V v, V w, A a);
    int delete_arc(V v, V w);
    void display();
};

template<typename V, typename A>
MGraph<V, A>::MGraph(GraphKind kind)
{
    if (kind != DG && kind != DN && kind != UG && kind != UN) {
        throw "illegal graph";
    }

    uint32_t iv = 0;
    if (kind == DN || kind == UN) {
        iv = INFINITY_MAX;
    }

    const size_t size = 256;
    _vex.reserve(size);
    _arc.resize(size);
    for (size_t i = 0; i < size; i++) {
        _arc[i].resize(size);
        for (size_t j = 0; j < size; j++) {
            _arc[i][j]._w = iv;
        }
    }

    _arcnum = 0;
    _mask |= kind;
}

template<typename V, typename A>
MGraph<V, A>::~MGraph()
{
    _vex.clear();

    for (size_t i = 0; i < _arc.size(); i++) {
        _arc[i].clear();
    }
    _arc.clear();

    _arcnum = 0;
    _mask = NO;
}

template<typename V, typename A>
int
MGraph<V, A>::locate_vertex(V v)
{
    auto it = std::find_if(_vex.begin(), _vex.end(), [v](const V& ve) {
        return _compare_equal(v, ve);
    });
    if (it != _vex.end()) {
        return std::distance(_vex.begin(), it);
    }
    
    return -1;
}

template<typename V, typename A>
int
MGraph<V, A>::insert_vertex(V v)
{
    if (locate_vertex(v) != -1) {
        return -1;
    }

    _vex.push_back(v);
        if (_vex.size() > _arc.size()) {
        uint32_t iv = 0;
        if (_mask & DN || _mask & UN) {
            iv = INFINITY_MAX;
        }
        const size_t old_size = _arc.size();
        const size_t new_size = old_size + (_arc.size() / 2 ? _arc.size() / 2 : 1);
        _arc.resize(new_size);
        for (size_t i = 0; i < new_size; i++) {
            _arc[i].resize(new_size);
            for (size_t j = 0; j < new_size; j++) {
                if (i >= old_size || j >= old_size) {
                    _arc[i][j]._w = iv;
                }
            }
        }
    }

    return 0;
}

template<typename V, typename A>
int
MGraph<V, A>::upsert_vertex(V v)
{
    if (locate_vertex(v) != -1) {
        std::replace_if(_vex.begin(), _vex.end(), [&v](const V& ve) { return _compare_equal(v, ve); }, std::move(v));
        return 0;
    }

    return insert_vertex(v);
}

template<typename V, typename A>
int
MGraph<V, A>::insert_arc(V v, V w, A a)
{
    return 0;
}

template<typename V, typename A>
int
MGraph<V, A>::upsert_arc(V v, V w, A a)
{
    int vi = locate_vertex(v);
    int wi = locate_vertex(w);
    if (vi < 0 || wi < 0) {
        return -1;
    }

    uint32_t weight = a._w;
    // 图的权重为 1
    if (_mask & DN || _mask & UN) {
        weight = 1;
    }
    
    _arc[vi][wi] = a;
    _arc[vi][wi]._w = weight;
    // 无向图或者无向网
    if (_mask & UG || _mask & UN) {
        _arc[wi][vi] = a;
        _arc[wi][vi]._w = weight;
    }

    _arcnum++;
    
    return 0;
}

template<typename V, typename A>
void
MGraph<V, A>::display()
{
    std::string graph_type = (_mask & DG) ? "有向图" : (_mask & DN) ? "有向网" :
                             (_mask & UG) ? "无向图" : (_mask & UN) ? "无向网" : "";
    if (graph_type.empty()) {
        printf("非法输出\n");
        return;
    }

    std::string arc_type = (_mask & DG || _mask & DN) ? "弧" :
                           (_mask & UG || _mask & UN) ? "边" : "";


    printf("%s具有 %lu 个顶点, %lu 条%s\n", graph_type.c_str(), _vex.size(), _arcnum, arc_type.c_str());
    printf("%16s", "");
    size_t i, j;
    for (i = 0; i < _vex.size(); i++) {
        // printf("%16s", _vex[i].);
    }
}

// void DisplayGraph(MGraph G)
// {
//     size_t i, j;
    
//     printf("图具有 %lu 个顶点, %lu 条弧\n", G.vex.size(), G.arcnum);
//     printf("%8s", "");
//     for (i = 0; i < G.vex.size(); i++) {    /** 输出图的顶点 */
//         printf("%8s", G.vex[i].name.c_str());
//     }
//     printf("\n");
    
//     for (i = 0; i < G.arc.size(); i++) {
//         printf("%8s", G.vex[i].name.c_str());
//         for (j = 0; j < G.arc[i].size(); j++) {
//             printf("%8d", G.arc[i][j].adj);
//         }
//         printf("\n");
//     }
// }