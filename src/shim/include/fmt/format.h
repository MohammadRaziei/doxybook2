#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace fmt {

namespace detail {
template <typename T>
std::string to_string(const T &value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

inline void collect(std::vector<std::string> &) {}

template <typename T, typename... Rest>
void collect(std::vector<std::string> &values, const T &first, const Rest &...rest) {
    values.push_back(to_string(first));
    collect(values, rest...);
}
} // namespace detail

template <typename... Args>
std::string format(const std::string &pattern, const Args &...args) {
    std::vector<std::string> values;
    detail::collect(values, args...);

    std::string result;
    std::size_t arg_index = 0;
    for (std::size_t i = 0; i < pattern.size(); ++i) {
        if (pattern[i] == '{' && (i + 1) < pattern.size() && pattern[i + 1] == '}' && arg_index < values.size()) {
            result += values[arg_index++];
            ++i;
        } else {
            result.push_back(pattern[i]);
        }
    }

    return result;
}

} // namespace fmt

