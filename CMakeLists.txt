cmake_minimum_required(VERSION 3.10)
project(sdk)

set(CMAKE_CXX_STANDARD 17)

include(ExternalProject)

ExternalProject_Add(lyra-lib
  GIT_REPOSITORY https://github.com/bfgroup/lyra
  TIMEOUT 10
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON
  LOG_CONFIGURE ON
  LOG_BUILD ON)
ExternalProject_Get_Property(lyra-lib source_dir)
include_directories(${source_dir}/include/)

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

if (APPLE)
else(APPLE)
  ExternalProject_Add(restclient-lib
    GIT_REPOSITORY https://github.com/mrtazz/restclient-cpp
    TIMEOUT 10
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)
  ExternalProject_Get_Property(restclient-lib source_dir)
  ExternalProject_Get_Property(restclient-lib binary_dir)
  include_directories(${source_dir}/include/)
  include_directories(${binary_dir}/include/)
  link_directories(${binary_dir})
endif(APPLE)

find_package(Boost)

add_executable(sdk src/main.cpp src/manifest.cpp src/uuid.cpp)
if (APPLE)
else(APPLE)
  set(REST_DEP restclient-lib)
endif(APPLE)
add_dependencies(sdk served-lib lyra-lib ${REST_DEP})
link_directories(${Boost_LIBRARY_DIRS})
target_link_libraries(sdk served pthread restclient-cpp boost_system)

add_subdirectory(tests)
