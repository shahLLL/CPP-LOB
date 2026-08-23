cmake_minimum_required(VERSION 3.12)
project(lob LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable CMake's built-in testing features (CTest)
include(CTest)

# Collect all C++ files in the src/ directory
file(GLOB SOURCES "src/*.cpp")

# Define the executable target
add_executable(lob.exe main.cpp ${SOURCES})

# Apply compiler warning flags
target_compile_options(lob.exe PRIVATE -Wall -Wextra)