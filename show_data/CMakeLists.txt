cmake_minimum_required(VERSION 3.0.2)
project(show_data)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++14)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    # cv_bridge
    # std_msgs
    # image_transport
    # rosbag
)

find_package(OpenCV REQUIRED)
find_package(gflags REQUIRED)
catkin_package()

include_directories(
    include
    ${catkin_INCLUDE_DIRS}
)

link_directories()

add_executable(offline_viewer 
    src/offline_viewer.cpp
    src/functions.cpp
)
target_link_libraries(offline_viewer ${catkin_LIBRARIES} ${OpenCV_LIBS} gflags)
