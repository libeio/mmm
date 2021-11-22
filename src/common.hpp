#pragma once

#include <stdio.h>
#include <stdlib.h>

#define ERRO_FD stderr
#define INFO_FD stdout

#define _ECHO(type, format, ...)              \
    do {                                      \
        fprintf(type, format, ##__VA_ARGS__); \
    } while (0)

#define ECHO(type, format, ...)                                                 \
    do {                                                                        \
        _ECHO(type ## _FD, "%s %4d: " format "\n",                              \
                          __FUNCTION__, __LINE__, ##__VA_ARGS__);               \
    } while (0)

#define _Assert(exp) { if (!(exp)) { exit(EXIT_FAILURE); } }
