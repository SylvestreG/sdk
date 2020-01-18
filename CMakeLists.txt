cmake_minimum_required(VERSION 3.10)
project(sdk)

set(CMAKE_CXX_STANDARD 17)

include(ExternalProject)

ExternalProject_Add(served-lib
  GIT_REPOSITORY https://github.com/meltwater/served
  TIMEOUT 10
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON
  LOG_CONFIGURE ON
  LOG_BUILD ON)

ExternalProject_Get_Property(served-lib source_dir)
include_directories(${source_dir}/src/)
link_directories(include_directories(${source_dir}/lib))

add_executable(sdk main.cpp)
add_dependencies(sdk served-lib)
target_link_libraries(sdk served pthread)