cmake_minimum_required(VERSION 3.10)
project(OpenCV_Capture_Image)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Add executable
add_executable(capture_image capture_image.cpp)

# Link OpenCV libraries
target_link_libraries(capture_image ${OpenCV_LIBS})
