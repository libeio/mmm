#pragma once

// #define INFINITY_MAX   1.7976931348623157e+308
// #define INFINITY_MAX   140595803510648
#define INFINITY_MAX   9999
#define INFINITY_MIN   5e-324

typedef enum {
    NO     = 0 << 0,
    DG     = 1 << 0,
    DN     = 1 << 1,
    UG     = 1 << 2,
    UN     = 1 << 3,
    DENSE  = 1 << 4,
    SPARSE = 1 << 5
} GraphKind;