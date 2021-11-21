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
    Vertex() {}
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
    T        _o;
    uint32_t _w;
public:
    // avoid to use it
    uint32_t get_w() { return _w; }
    void set_w(const uint32_t w) { _w = w; }
    return_type get() const { return _o; }
    void set(T o) { _o = o; };

    // provide to user
    Arc(const T& o, uint32_t w) : _o(o), _w(w) {}
    Arc(uint32_t w) : _o(T()), _w(w) {}
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
    size_t _vexnum;
    uint32_t _mask;
public:
    MGraph(GraphKind kind);
    ~MGraph();

    int locate_vertex(V v);
    int insert_vertex(V v);
    int upsert_vertex(V v);
    void delete_vertex(V v);
    V get_vertex(int i);
    int insert_arc(V v, V w, A a);
    int upsert_arc(V v, V w, A a);
    void delete_arc(V v, V w);
    void bfs();
    void dfs();
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

    const size_t size = 4;
    _vex.reserve(size);
    _arc.resize(size);
    for (size_t i = 0; i < size; i++) {
        _arc[i].resize(size);
        for (size_t j = 0; j < size; j++) {
            _arc[i][j]._w = iv;
        }
    }

    _arcnum = 0;
    _vexnum = 0;
    _mask = 0;
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
    _vexnum = 0;
    _mask = NO;
}

template<typename V, typename A>
int
MGraph<V, A>::locate_vertex(V v)
{
    auto it = std::find_if(_vex.begin(), _vex.end(), [v](const V& ve) {
        return _compare_equal(v._o, ve._o);
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

    _vexnum++;

    return 0;
}

template<typename V, typename A>
int
MGraph<V, A>::upsert_vertex(V v)
{
    if (locate_vertex(v) != -1) {
        std::replace_if(_vex.begin(), _vex.end(), [&v](const V& ve) { return _compare_equal(v._o, ve._o); }, std::move(v));
        return 0;
    }

    return insert_vertex(v);
}

template<typename V, typename A>
void
MGraph<V, A>::delete_vertex(V v)
{
    int x = locate_vertex(v);
    if (x == -1) {
        return ;
    }

    _vex[x].~V();

    int size = (int)_vex.size();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (_mask & DG) {
                if (i == x || j == x) {
                    if (_arc[i][j]._w == 1) {
                        _arc[i][j].~A();
                        _arc[i][j] = A(0);
                        _arcnum--;
                    }
                }
            } else if (_mask & DN) {
                if (i == x || j == x) {
                    if (_arc[i][j]._w != INFINITY_MAX) {
                        _arc[i][j].~A();
                        _arc[i][j] = A(INFINITY_MAX);
                        _arcnum--;
                    }
                }
            } else if (_mask & UG) {
                if ((i == x || j == x) && i > j) {
                    if (_arc[i][j]._w == 1) {
                        _arc[i][j].~A();
                        _arc[i][j] = A(0);
                        _arc[j][i].~A();
                        _arc[j][i] = A(0);
                        _arcnum--;
                    }
                }
            } else if (_mask & UN) {
                if ((i == x || j == x) && i > j) {
                    if (_arc[i][j]._w != INFINITY_MAX) {
                        _arc[i][j].~A();
                        _arc[i][j] = A(INFINITY_MAX);
                        _arc[j][i].~A();
                        _arc[j][i] = A(INFINITY_MAX);
                        _arcnum--;
                    }
                }
            }
        }
    }

    int len = (int)_vex.size();
    int k = len - 1;
    for (int i = 0; i < len; i++) {
        _arc[i][x] = _arc[i][k];
        if (_mask & DG || _mask & UG) {
            _arc[i][k] = A();
        } else if (_mask & DN || _mask & UN) {
            _arc[i][k] = A(INFINITY_MAX);
        }
    }

    for (int j = 0; j < len; j++) {
        _arc[x][j] = _arc[k][j];
        if (_mask & DG || _mask & UG) {
            _arc[k][j] = A();
        } else if (_mask & DN || _mask & UN) {
            _arc[k][j] = A(INFINITY_MAX);
        }
    } 

    _vex[x] = _vex[k];
    
    _vex.resize(--_vexnum);

    return ;
}

template<typename V, typename A>
V
MGraph<V, A>::get_vertex(int i)
{
    return _vex.at(i);
}

template<typename V, typename A>
int
MGraph<V, A>::insert_arc(V v, V w, A a)
{
    int vi = locate_vertex(v);
    int wi = locate_vertex(w);
    if (vi < 0 || wi < 0) {
        return -1;
    }

    if (_arc[vi][wi]._w >= 1 && _arc[vi][wi]._w < INFINITY_MAX) {
        return -1;
    }

    return upsert_arc(v, w, a);
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

    decltype(a._w) old_w = _arc[vi][wi]._w;

    if (_mask & DG) {
        _arc[vi][wi] = a;
        _arc[vi][wi]._w = 1;
        if (old_w == 0) {
            _arcnum++;
        }
    } else if (_mask & DN) {
        _arc[vi][wi] = a;
        if (old_w == INFINITY_MAX) {
            _arcnum++;
        }
    } else if (_mask & UG) {
        _arc[vi][wi] = a;
        _arc[vi][wi]._w = 1;
        _arc[wi][vi] = a;
        _arc[wi][vi]._w = 1;
        if (old_w == 0) {
            _arcnum++;
        }
    } else if (_mask & UN) {
        _arc[vi][wi] = a;
        _arc[wi][vi] = a;
        if (old_w == INFINITY_MAX) {
            _arcnum++;
        }
    }
    
    return 0;
}

template<typename V, typename A>
void
MGraph<V, A>::delete_arc(V v, V w)
{
    int vi = locate_vertex(v);
    int wi = locate_vertex(w);
    if (vi < 0 || wi < 0) {
        return ;
    }

    if (_mask & DG) {
        _arc[vi][wi].~A();
        _arc[vi][wi] = A(0);
        _arcnum--;
    } else if (_mask & DN) {
        _arc[vi][wi].~A();
        _arc[vi][wi] = A(INFINITY_MAX);
        _arcnum--;
    } else if (_mask & UG) {
        _arc[vi][wi].~A();
        _arc[vi][wi] = A(0);
        _arc[wi][vi].~A();
        _arc[wi][vi] = A(0);
        _arcnum--;
    } else if (_mask & UN) {
        _arc[vi][wi].~A();
        _arc[vi][wi] = A(INFINITY_MAX);
        _arc[wi][vi].~A();
        _arc[wi][vi] = A(INFINITY_MAX);
        _arcnum--;
    }

    return ;
}

template<typename V, typename A>
void
MGraph<V, A>::bfs()
{

}

template<typename V, typename A>
void
MGraph<V, A>::dfs()
{

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


    printf("%s具有 %lu 个顶点, %lu 条%s\n", graph_type.c_str(), _vexnum, _arcnum, arc_type.c_str());
    printf("%16s", "");
    size_t i, j, size = _vex.size();
    for (i = 0; i < size; i++) {
        printf("%16lu", i);
    }
    printf("\n");

    for (i = 0; i < size; i++) {
        printf("%16lu", i);
        for (j = 0; j < size; j++) {
            printf("%16u", _arc[i][j]._w);
        }
        printf("\n");
    }
}
