
find_path(spdlog_INCLUDE spdlog/spdlog.h HINTS "${PROJECT_SOURCE_DIR}/deps/" NO_DEFAULT_PATH)

if (spdlog_INCLUDE)
    set(SPDLOG_FOUND TRUE)
    message(STATUS "${Green}Found SPDLOG include at: ${spdlog_INCLUDE}${Reset}")
else()
    message(FATAL_ERROR "${Red}Failed to locate SPDLOG module.${Reset}" )
endif()

message(STATUS "${Blue}spdlog_INCLUDE=${spdlog_INCLUDE}${Reset}")
