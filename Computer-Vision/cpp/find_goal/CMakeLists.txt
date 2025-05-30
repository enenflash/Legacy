cmake_minimum_required(VERSION 3.10)
project(OpenCV_Capture_Image)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Set optimization flags for Release build type
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto -funroll-loops -fprofile-generate")

# Add executable
add_executable(find_goal main.cpp)

# Link OpenCV libraries
target_link_libraries(find_goal ${OpenCV_LIBS})

# Optionally, you can define the Debug flags (no optimizations, debug symbols)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
