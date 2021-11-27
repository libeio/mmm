#pragma once

#include <string>

struct VNode
{
    std::string _name;

    VNode(){}
    VNode(std::string name) : _name(name) {}
    ~VNode(){}
};

struct ANode
{
    std::string _label;

    ANode(){}
    ANode(std::string label) : _label(label) {}
    ~ANode(){}
};

// 站点
struct Station
{
    std::string _name;
    
    Station(){}
    Station(std::string name) : _name(name) {}
    ~Station(){}
};
