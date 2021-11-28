#pragma once

#include "primitive.hpp"
#include "compare.hpp"
#include "show.hpp"

#include <vector>
#include <type_traits>
#include <algorithm>
#include <iterator>
#include <string>
#include <queue>
#include <set>
#include <tuple>
#include <map>

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
    size_t vertex_size() { return _vexnum; }
    size_t arc_size() { return _arcnum; }
    std::vector<std::tuple<V, V, uint32_t>> prim();
    std::vector<std::tuple<V, V, uint32_t>> kruskal();
    std::vector<std::tuple<V, V, uint32_t>> shortest_path(V v, V w);
private:
    void _dfs_internal(V v);
    bool _min_tree_internal();
    void _dijkstra_internal(V v, std::vector<std::vector<int>>& path, std::vector<uint32_t>& dist);
private:
    std::vector<int> _visited;
    std::vector<std::vector<int>> _path;
    std::vector<uint32_t> _dist;
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

    return (int)_vex.size() - 1;
}

template<typename V, typename A>
int
MGraph<V, A>::upsert_vertex(V v)
{
    int i = locate_vertex(v);
    if (i != -1) {
        std::replace_if(_vex.begin(), _vex.end(), [&v](const V& ve) { return _compare_equal(v._o, ve._o); }, std::move(v));
        return i;
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

    if (((_mask & DG || _mask & UG) && (_arc[vi][wi]._w == 1)) || ((_mask & DN || _mask & UN) && (_arc[vi][wi]._w != INFINITY_MAX))) {
        return -1;
    }

    return upsert_arc(v, w, a);
}

template<typename V, typename A>
int
MGraph<V, A>::upsert_arc(V v, V w, A a)
{
    int vi = upsert_vertex(v);
    int wi = upsert_vertex(w);
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

#include "common.hpp"

template<typename V, typename A>
void
MGraph<V, A>::bfs()
{
    std::vector<int> vt;
    _visited.swap(vt);
    _visited.resize(_vexnum, 0);

    std::queue<int> q;
    for (size_t i = 0; i < _vexnum; i++) {
        if (! _visited[i]) {
            _visited[i] = 1;
            _show_vertex(_vex[i]._o);
            q.push(i);

            while (! q.empty()) {
                size_t i = q.front();
                for (size_t j = 0; j < _vexnum; j++) {
                    if (((_mask & DG || _mask & UG) && (_arc[i][j]._w == 1)) || ((_mask & DN || _mask & UN) && (_arc[i][j]._w != INFINITY_MAX))) {
                        if (! _visited[j]) {
                            _visited[j] = 1;
                            _show_vertex(_vex[j]._o);
                            q.push(j);
                        }
                    }
                }
                q.pop();
            }
        }
    }
}

template<typename V, typename A>
void
MGraph<V, A>::dfs()
{
    std::vector<int> vt;
    _visited.swap(vt);
    _visited.resize(_vexnum, 0);

    for (size_t i = 0; i < _vexnum; i++) {
        if (! _visited[i]) {
            _dfs_internal(_vex[i]);
        }
    }
}

template<typename V, typename A>
void
MGraph<V, A>::_dfs_internal(V v)
{
    int i = locate_vertex(v);
    _visited[i] = 1;
    _show_vertex(v._o);
    for (int j = 0; j < (int)_vexnum; j++) {
        if (((_mask & DG || _mask & UG) && (_arc[i][j]._w == 1)) || ((_mask & DN || _mask & UN) && (_arc[i][j]._w != INFINITY_MAX))) {
            if (! _visited[j]) {
                _dfs_internal(_vex[j]);
            }
        }
    }
}

template<typename V, typename A>
bool
MGraph<V, A>::_min_tree_internal()
{
    if (_mask & DG || _mask & UG) {
        printf("图没有最小生成树\n");
        return false;
    }

    if (_mask & DN) {
        printf("暂不支持有向网构造最小生成树\n");
        return false;
    }
    
    if (_vexnum == 0) {
        printf("网中没有顶点\n");
        return false;
    }

    std::vector<int> vt;
    _visited.swap(vt);
    _visited.resize(_vexnum, 0);

    size_t counter = 0;
    std::queue<int> q;
    
    _visited[0] = 1;
    counter++;
    q.push(0);
    while (! q.empty()) {
        size_t i = q.front();
        for (size_t j = 0; j < _vexnum; j++) {
            if (((_mask & DG || _mask & UG) && (_arc[i][j]._w == 1)) || ((_mask & DN || _mask & UN) && (_arc[i][j]._w != INFINITY_MAX))) {
                if (! _visited[j]) {
                    _visited[j] = 1;
                    counter++;
                    q.push(j);
                }
            }
        }
        q.pop();
    }
    if (counter < _vexnum) {
        printf("网是非连通的，无法产生最小生成树\n");
        return false;
    }

    return true;
}


template<typename V, typename A>
std::vector<std::tuple<V, V, uint32_t>>
MGraph<V, A>::prim()
{
    using RT = std::vector<std::tuple<V, V, uint32_t>>;

    if (! _min_tree_internal()) {
        return RT();
    }

    typedef struct {
        V adjv;
        uint32_t lowcost;
    } CloserEdge;

    std::vector<CloserEdge> ce;
    std::set<int> Ui, Vi;
    ce.resize(_vexnum);
    for (int i = 0; i < (int)_vexnum; i++) {
        Vi.insert(i);
    }

    int k = 0;
    Ui.insert(k);
    Vi.erase(k);
    // 初始时，在数组中记录 V-U 中各顶点与 k 顶点的边权
    for (int j = 0; j < (int)_vexnum; j++) {
        ce[j].adjv = _vex[k];
        ce[j].lowcost = _arc[k][j]._w;
    }

    RT vpp;
    // 与剩下的 _vexnum - 1 个顶点比较
    for (int i = 1; i < (int)_vexnum; i++) {
        uint32_t min_cost = INFINITY_MAX;
        int k = -1;
        for (auto it = Vi.begin(); it != Vi.end(); it++) {
            if (ce[*it].lowcost < min_cost) {
                min_cost = ce[*it].lowcost;
                k = *it;
            }
        }
        if (k < 0) {
            printf("非法边导致的异常\n");
            return RT();
        }

        vpp.emplace_back(ce[k].adjv, _vex[k], ce[k].lowcost);  // first -> second
        
        Ui.insert(k);
        Vi.erase(k);

        // 已经从 V - U 中选择了一个顶点，使得该顶点到 k 顶点权重最小。
        // 那么就将 V - U 中其他顶点到 k 顶点的权重置为无限，方便后续重新赋值
        for (auto it = Vi.begin(); it != Vi.end(); it++) {
            ce[*it].lowcost = INFINITY_MAX;
        }
        // 新顶点加入 U 集后，重新将最小边存入数组
        for (auto x = Vi.begin(); x != Vi.end(); x++) {
            for (auto y = Ui.begin(); y != Ui.end(); y++) {
                if (_arc[*x][*y]._w < ce[*x].lowcost) {
                    ce[*x].adjv = _vex[*y];
                    ce[*x].lowcost = _arc[*x][*y]._w;
                }
            }
        }
    }

    return vpp;
}

template<typename V, typename A>
std::vector<std::tuple<V, V, uint32_t>> 
MGraph<V, A>::kruskal()
{
    using RT = std::vector<std::tuple<V, V, uint32_t>>;

    if (! _min_tree_internal()) {
        return RT();
    }

    std::vector<int> sets;      // 顶点到集合的映射，下标为顶点，值为映射集合编号(实际并不存在)
    std::multimap<uint32_t, std::pair<int, int>> edges;

    int i, j, k;
    
    for (i = 0; i < (int)_vexnum; i++) {
        sets.push_back(i);
    }

    for (i = 0; i < (int)_vexnum; i++) {    /** 矩阵上三角获得所有边及对应权值 */
        for (j = i + 1; j < (int)_vexnum; j++) {
            edges.emplace(_arc[i][j]._w, std::pair<int, int>(i, j));  /** 缺省排序 */
        }
    }

    RT vpp;

    while (! edges.empty()) {
        auto iter = edges.begin();
        if (sets[iter->second.first] != sets[iter->second.second]) {
            vpp.emplace_back(_vex[iter->second.first], _vex[iter->second.second], iter->first);

            int ou = sets[iter->second.second];          // 记下旧的集合
            for (i = 0; i < (int)sets.size(); i++) {     // 合并集合
                if (sets[i] == ou) {
                    sets[i] = sets[iter->second.first];
                }
            }
        }
        edges.erase(iter);  
    }

    return vpp;
}

template<typename V, typename A>
void
MGraph<V, A>::_dijkstra_internal(V v, std::vector<std::vector<int>>& path, std::vector<uint32_t>& dist)
{
    size_t vi, wi, i;
    uint32_t min;
    std::vector<int> S; // 集合 S 。用于记录 v0 到索引对应顶点的最短路径是否求出

    int v0 = locate_vertex(v);

    path.resize(_vexnum);
    for (i = 0; i < path.size(); i++) {
        path[i].resize(_vexnum);
    }

    dist.resize(_vexnum);
    S.resize(_vexnum);

    // 初始化
    for (vi = 0; vi < _vexnum; vi++) {
        S[vi] = 0;
        dist[vi] = _arc[v0][vi]._w;
        for (wi = 0; wi < _vexnum; wi++) {
            path[vi][wi] = 0;
        }
        if (dist[vi] < INFINITY_MAX) {  // 如果从 v0 到 vi 存在直接路径，则利用此路径初始化数组
            path[vi][v0] = 1;
            path[vi][vi] = 1;
        }
    }

    dist[v0] = 0;   // v0 到 v0 的路径为 0
    S[v0] = 1;      // v0 顶点并入集合 S

    // 计算从 v0 到其余各顶点的最短路径，并将计算后的顶点并入集合 S
    for (i = 1; i < _vexnum; i++) {    // 只需要执行 _vexnum - 1 次
        min = INFINITY_MAX;
        for (wi = 0; wi < _vexnum; wi++) {
            if (! S[wi] && dist[wi] < min) {   // 在不属于 S 的顶点中，找到离 v0 最近的顶点
                vi = wi;
                min = dist[wi];
            }
        }
        
        S[vi] = 1;     // 将顶点 vi 并入到集合 S
        for (wi = 0; wi < _vexnum; wi++) {
            /**
             * 一共有四个判断条件，缺一不可。按这样的顺序进行判断的意义是:
             *  ! S[wi]: 要更新的顶点 wi 应不属于集合 S
             *  min < INFINITY_MAX && _arc[vi][wi]._w < INFINITY_MAX: 且 v0 到 wi 确实存在一条路径，
             *    其中 min=Min{v0到vi的最短路径}。 min < INFINITY_MAX 说明 v0 到 vi 是存在路径的，
             *    _arc[vi][wi]._w < INFINITY_MAX 说明 vi 到 wi 也是存在路径的。两者结合说明 v0 到 wi
             *    是存在一条路径的。
             *  min + _arc[vi][wi]._w < dist[wi]: 这个比较容易理解，就是 v0 通过 vi 点到 wi 的路径长度
             *    如果比原来记录的要小，则替换之。
             */
            if (! S[wi] && 
                min < INFINITY_MAX && _arc[vi][wi]._w < INFINITY_MAX &&
                (min + _arc[vi][wi]._w < dist[wi]))
            {
                dist[wi] = min + _arc[vi][wi]._w;
                // 更新路径映射，意会...
                for (size_t k = 0; k < _vexnum; k++) {
                    path[wi][k] = path[vi][k];
                }
                path[wi][wi] = 1;
            }
        }
    }
}

template<typename V, typename A>
std::vector<std::tuple<V, V, uint32_t>>
MGraph<V, A>::shortest_path(V v, V w)
{
    using RT = std::vector<std::tuple<V, V, uint32_t>>;

    for (size_t i = 0; i < _path.size(); i++) {
        _path[i].clear();
    }
    _path.clear();
    _dist.clear();
    
    _dijkstra_internal(v, _path, _dist);

    RT vpp;

    int vi = locate_vertex(v);
    for (int i = 0; i < (int)_vexnum; i++) {
        if (i != vi) {
            vpp.emplace_back(_vex[vi], _vex[i], _dist[i]);
        }
    }

    int wi = locate_vertex(w);
    

    return vpp;
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
    printf("%10s", "");
    size_t i, j, size = _vex.size();
    for (i = 0; i < size; i++) {
        printf("%10lu", i);
    }
    printf("\n");

    for (i = 0; i < size; i++) {
        printf("%10lu", i);
        for (j = 0; j < size; j++) {
            printf("%10u", _arc[i][j]._w);
        }
        printf("\n");
    }
}
