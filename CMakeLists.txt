cmake_minimum_required(VERSION 3.10)
project(sdk)

set(CMAKE_CXX_STANDARD 17)

include(ExternalProject)

ExternalProject_Add(pistache-lib
  GIT_REPOSITORY "https://github.com/oktal/pistache.git"
  TIMEOUT 10
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON
  LOG_CONFIGURE ON
  LOG_BUILD ON)

ExternalProject_Get_Property(pistache-lib source_dir)
ExternalProject_Get_Property(pistache-lib binary_dir)
include_directories(${source_dir}/include)
include_directories(${binary_dir}/include)
link_directories(include_directories(${binary_dir}/src))

add_executable(sdk main.cpp)
add_dependencies(sdk pistache-lib)
target_link_libraries(sdk pistache)