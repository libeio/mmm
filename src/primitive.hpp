#pragma once

// #define INFINITY_MAX   1.7976931348623157e+308
// #define INFINITY_MAX   140595803510648
#define INFINITY_MAX   2147483647
#define INFINITY_MIN   5e-324

typedef enum {
    DG,     // 有向图
    DN,     // 有向网
    UG,     // 无向图
    UN      // 无向网
} GraphKind;