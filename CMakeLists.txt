cmake_minimum_required(VERSION 3.5)
project(mapiX)

set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
find_package(ROOT COMPONENTS REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

set(RapidJSON_DIR $ENV{RapidJSON_CMAKEDIR})
find_package(RapidJSON REQUIRED CONFIG)
include_directories(${RapidJSON_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.h)
add_executable(mapiX main.cpp ${sources} ${headers})

target_link_libraries(mapiX ${ROOT_LIBRARIES} )
