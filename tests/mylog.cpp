
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"

int main()
{
    spdlog::cfg::load_env_levels();

    // 占位字符
    spdlog::info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
    // 0 填充
    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    // 指定进制输出
    spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    // 浮点位数保留
    spdlog::info("Support for floats {:03.2f}", 1.23456);
    // 指定占位字符位置
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    // 左右对齐
    spdlog::info("{:>8} aligned, {:<8} aligned", "right", "left");

    return 0;
}