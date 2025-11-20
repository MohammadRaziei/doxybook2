cmake_minimum_required(VERSION 3.21)
project(${SKBUILD_PROJECT_NAME})

string(REPLACE "-" "_" MODULE_NAME ${SKBUILD_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
find_package(Cython MODULE REQUIRED VERSION 3.0)
include(UseCython)

set(DOXYBOOK_SRC_ROOT ${CMAKE_SOURCE_DIR}/src/third_party/doxybook2)

file(GLOB_RECURSE DOXYBOOK_SOURCES
    ${DOXYBOOK_SRC_ROOT}/src/Doxybook/*.cpp
)
file(GLOB_RECURSE DOXYBOOK_HEADERS ${DOXYBOOK_SRC_ROOT}/include/Doxybook/*.hpp)

add_library(Doxybook2 STATIC ${DOXYBOOK_SOURCES} ${DOXYBOOK_HEADERS})
target_compile_features(Doxybook2 PUBLIC cxx_std_17)
target_include_directories(Doxybook2 PUBLIC
    ${DOXYBOOK_SRC_ROOT}/include
    ${DOXYBOOK_SRC_ROOT}/src
    ${CMAKE_SOURCE_DIR}/src/shim/include
    ${CMAKE_SOURCE_DIR}/src/third_party/inja/single_include
    ${CMAKE_SOURCE_DIR}/src/third_party/inja/third_party/include
)

cython_transpile(src/doxybook2/_doxybook2.pyx LANGUAGE CXX OUTPUT_VARIABLE doxybook2_cpp_file)

python_add_library(${MODULE_NAME} MODULE "${doxybook2_cpp_file}" src/cpp/DoxybookBinding.cpp WITH_SOABI)

target_include_directories(${MODULE_NAME} PRIVATE
    src/cpp
    ${CMAKE_SOURCE_DIR}/src/third_party/doxybook2/include
)

target_link_libraries(${MODULE_NAME} PRIVATE Doxybook2)

set_target_properties(${MODULE_NAME} PROPERTIES
    INSTALL_RPATH "$ORIGIN/cpp/lib"
    BUILD_RPATH "$ORIGIN/cpp/lib"
    SKIP_RPATH FALSE
    INSTALL_RPATH_USE_LINK_PATH TRUE
)

install(TARGETS ${MODULE_NAME}
    DESTINATION ${SKBUILD_PROJECT_NAME}
)

install(TARGETS Doxybook2
    ARCHIVE DESTINATION ${SKBUILD_PROJECT_NAME}/cpp/lib
    LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME}/cpp/lib
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/third_party/doxybook2/include/
    DESTINATION ${SKBUILD_PROJECT_NAME}/cpp/include
)
