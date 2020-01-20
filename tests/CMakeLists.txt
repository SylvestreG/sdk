ExternalProject_Add(googletest
  GIT_REPOSITORY https://github.com/google/googletest/
  TIMEOUT 10
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON
  LOG_CONFIGURE ON
  LOG_BUILD ON)
ExternalProject_Get_Property(googletest source_dir)
ExternalProject_Get_Property(googletest binary_dir)
include_directories(${source_dir}/googletest/include/)
link_directories(${binary_dir}/lib)

add_definitions(-DTEST_DIR="${CMAKE_SOURCE_DIR}/tests/")

add_executable(ut main.cpp manifest.cpp ../src/manifest.cpp ../src/uuid.cpp)
add_dependencies(ut googletest)
target_link_libraries(ut gtest pthread)