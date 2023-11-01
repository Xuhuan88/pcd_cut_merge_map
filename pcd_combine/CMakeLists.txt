cmake_minimum_required(VERSION 3.10)
project(pcd_merge)

set(CMAKE_CXX_STANDARD 14)

find_package(PCL REQUIRED COMPONENTS common io)
find_package(yaml-cpp REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
include_directories(${YAML_CPP_INCLUDE_DIR})  # 添加yaml-cpp库的头文件目录

add_definitions(${PCL_DEFINITIONS})

add_executable(pcd_merge main.cpp)
target_link_libraries(pcd_merge ${PCL_LIBRARIES} ${YAML_CPP_LIBRARIES})  # 链接PCL和yaml-cpp库