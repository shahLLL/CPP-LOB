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

# Fetch Catch2 Framework
include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.2 # Use a stable Catch2 v3 release tag
)
FetchContent_MakeAvailable(Catch2)

# Collect all C++ test files in the test/ directory
file(GLOB TESTS "tests/*.cpp")

# Create the test executable
add_executable(unit_tests ${TESTS} ${SOURCES})

# Link against the Catch2 v3 main entry point target
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)

# Automatically register test cases with CTest
include(Catch)
catch_discover_tests(unit_tests)

# Collect all C++ beching files in the bench/ directory
file(GLOB BENCH "bench/*.cpp")

# Create bench script executable
add_executable(bench ${BENCH} ${SOURCES})