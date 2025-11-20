#pragma once

#include <iostream>
#include <string>

#include "fmt/format.h"

namespace spdlog {
namespace level {
enum level_enum { off, err, warn, info, debug, trace };
}

inline void set_level(level::level_enum) {}
inline void set_pattern(const std::string &) {}

template <typename... Args>
inline void info(const std::string &pattern, const Args &...args) {
    std::cout << fmt::format(pattern, args...) << std::endl;
}

template <typename... Args>
inline void warn(const std::string &pattern, const Args &...args) {
    std::cerr << fmt::format(pattern, args...) << std::endl;
}

template <typename... Args>
inline void error(const std::string &pattern, const Args &...args) {
    std::cerr << fmt::format(pattern, args...) << std::endl;
}

} // namespace spdlog

