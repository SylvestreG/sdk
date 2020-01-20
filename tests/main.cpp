#include <gtest/gtest.h>

int main(int argc, char *argv[]) {
  // GTest initialization.
  testing::InitGoogleTest(&argc, argv);

  // Run all tests.
  return (RUN_ALL_TESTS());
}