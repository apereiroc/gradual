cmake_minimum_required(VERSION 3.20)
project(Gradual 
    VERSION 0.1.0
    DESCRIPTION "Minimalist C++20 automatic differentiation library"
    LANGUAGES CXX
)

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Find dependencies
find_package(fmt REQUIRED)

# Header-only library target
add_library(gradual INTERFACE)
target_include_directories(gradual INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(gradual INTERFACE cxx_std_20)
target_link_libraries(gradual INTERFACE fmt::fmt)

# Add subdirectories
add_subdirectory(examples)

# Testing
option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installation
install(DIRECTORY include/gradual
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

install(TARGETS gradual
    EXPORT GradualTargets
)

install(EXPORT GradualTargets
    FILE GradualTargets.cmake
    NAMESPACE Gradual::
    DESTINATION lib/cmake/Gradual
)

# Generate and install package config
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/GradualConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/GradualConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/GradualConfig.cmake"
    INSTALL_DESTINATION lib/cmake/Gradual
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/GradualConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/GradualConfigVersion.cmake"
    DESTINATION lib/cmake/Gradual
)
