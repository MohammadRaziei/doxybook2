#pragma once

#include <optional>
#include <string>

namespace doxybook2 {

int generate(const std::string &input_dir,
             const std::string &output_dir,
             const std::optional<std::string> &config_path,
             const std::optional<std::string> &templates_path,
             bool json_only,
             bool quiet,
             bool debug_templates);

}

