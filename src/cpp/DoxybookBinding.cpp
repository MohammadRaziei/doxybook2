#include "DoxybookBinding.hpp"

#include <Doxybook/DefaultTemplates.hpp>
#include <Doxybook/Doxygen.hpp>
#include <Doxybook/Generator.hpp>
#include <Doxybook/Path.hpp>
#include <Doxybook/TextMarkdownPrinter.hpp>
#include <Doxybook/TextPlainPrinter.hpp>
#include <Doxybook/Utils.hpp>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <array>
#include <vector>

namespace doxybook2 {

namespace {
using namespace Doxybook2;

Generator::Filter class_filter() {
    static const Generator::Filter FILTER = {Kind::NAMESPACE,
                                             Kind::CLASS,
                                             Kind::INTERFACE,
                                             Kind::STRUCT,
                                             Kind::UNION,
                                             Kind::JAVAENUM};
    return FILTER;
}

Generator::Filter class_skip_filter() {
    static const Generator::Filter FILTER = {Kind::NAMESPACE};
    return FILTER;
}

Generator::Filter namespaces_filter() {
    static const Generator::Filter FILTER = {Kind::NAMESPACE};
    return FILTER;
}

Generator::Filter modules_filter() {
    static const Generator::Filter FILTER = {Kind::MODULE};
    return FILTER;
}

Generator::Filter files_filter() {
    static const Generator::Filter FILTER = {Kind::DIR, Kind::FILE};
    return FILTER;
}

Generator::Filter language_filter() {
    static const Generator::Filter FILTER = {Kind::NAMESPACE,
                                             Kind::CLASS,
                                             Kind::INTERFACE,
                                             Kind::STRUCT,
                                             Kind::UNION,
                                             Kind::MODULE,
                                             Kind::JAVAENUM};
    return FILTER;
}

Generator::Filter pages_filter() {
    static const Generator::Filter FILTER = {Kind::PAGE};
    return FILTER;
}

Generator::Filter examples_filter() {
    static const Generator::Filter FILTER = {Kind::EXAMPLE};
    return FILTER;
}
} // namespace

int generate(const std::string &input_dir,
             const std::string &output_dir,
             const std::optional<std::string> &config_path,
             const std::optional<std::string> &templates_path,
             bool json_only,
             bool quiet,
             bool debug_templates) {
    using namespace Doxybook2;

    if (quiet) {
        spdlog::set_level(spdlog::level::off);
    }

    Config config;

    if (config_path.has_value()) {
        loadConfig(config, *config_path);
    }

    if (debug_templates) {
        config.debugTemplateJson = true;
    }

    if (json_only) {
        config.useFolders = false;
        config.imagesFolder = "";
    }

    config.outputDir = output_dir;

    Doxygen doxygen(config);
    TextMarkdownPrinter markdownPrinter(config, input_dir, doxygen);
    TextPlainPrinter plainPrinter(config, doxygen);
    JsonConverter jsonConverter(config, doxygen, plainPrinter, markdownPrinter);

    std::optional<std::string> templates = templates_path;

    Generator generator(config, doxygen, jsonConverter, templates);

    const auto shouldGenerate = [&](const FolderCategory category) {
        return std::find(config.foldersToGenerate.begin(), config.foldersToGenerate.end(), category) !=
               config.foldersToGenerate.end();
    };

    if (config.useFolders) {
        static const std::array<FolderCategory, 6> ALL_GROUPS = {
            FolderCategory::CLASSES,
            FolderCategory::NAMESPACES,
            FolderCategory::FILES,
            FolderCategory::MODULES,
            FolderCategory::PAGES,
            FolderCategory::EXAMPLES,
        };

        for (const auto &group : ALL_GROUPS) {
            if (shouldGenerate(group)) {
                Utils::createDirectory(Path::join(config.outputDir, typeFolderCategoryToFolderName(config, group)));
            }
        }
    }

    Generator::Filter languageFilter;
    if (shouldGenerate(FolderCategory::CLASSES)) {
        const auto classes = class_filter();
        languageFilter.insert(classes.begin(), classes.end());
    }
    if (shouldGenerate(FolderCategory::NAMESPACES)) {
        const auto namespaces = namespaces_filter();
        languageFilter.insert(namespaces.begin(), namespaces.end());
    }
    if (shouldGenerate(FolderCategory::MODULES)) {
        const auto modules = modules_filter();
        languageFilter.insert(modules.begin(), modules.end());
    }

    if (!languageFilter.empty()) {
        generator.print(languageFilter, json_only ? class_skip_filter() : Generator::Filter{});
    }

    if (shouldGenerate(FolderCategory::FILES)) {
        generator.print(files_filter(), {});
    }

    if (shouldGenerate(FolderCategory::PAGES)) {
        generator.print(pages_filter(), {});
    }

    if (shouldGenerate(FolderCategory::EXAMPLES)) {
        generator.print(examples_filter(), {});
    }

    if (shouldGenerate(FolderCategory::CLASSES)) {
        generator.printIndex(FolderCategory::CLASSES, class_filter(), {});
    }

    if (shouldGenerate(FolderCategory::NAMESPACES)) {
        generator.printIndex(FolderCategory::NAMESPACES, namespaces_filter(), {});
    }

    if (shouldGenerate(FolderCategory::MODULES)) {
        generator.printIndex(FolderCategory::MODULES, modules_filter(), {});
    }

    if (shouldGenerate(FolderCategory::FILES)) {
        generator.printIndex(FolderCategory::FILES, files_filter(), {});
    }

    if (shouldGenerate(FolderCategory::PAGES)) {
        generator.printIndex(FolderCategory::PAGES, pages_filter(), {});
    }

    if (shouldGenerate(FolderCategory::EXAMPLES)) {
        generator.printIndex(FolderCategory::EXAMPLES, examples_filter(), {});
    }

    generator.manifest();

    return EXIT_SUCCESS;
}

} // namespace doxybook2

