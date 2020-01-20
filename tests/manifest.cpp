#include "../src/manifest.h"
#include <fstream>
#include <gtest/gtest.h>
#include <stack>

static std::string const test_path = TEST_DIR;

class manifest_test : public ::testing::Test {
public:
  void TearDown() override { manifest::_proxy.clear(); }
};

TEST_F(manifest_test, bad_format) {
  ASSERT_THROW(manifest m("BAD FORMAT"), std::error_code);
}

TEST_F(manifest_test, manifest_mux_dev) {
  for (int i = 0; i < 6; i++) {
    std::ifstream file(test_path + "mux_dev-" + std::to_string(1 + i) +
                       ".m3u8");
    std::string manifest_data((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());

    ASSERT_NO_THROW(manifest m(manifest_data));
    ASSERT_EQ(manifest::_proxy.size(), 0u);
  }
}

TEST_F(manifest_test, manifest_akamaihd) {
  std::stack<std::size_t> result;

  result.push(53);
  result.push(0);
  for (int i = 0; i < 2; i++) {
    std::ifstream file(test_path + "akamaihd-" + std::to_string(1 + i) +
                       ".m3u8");
    std::string manifest_data((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());

    ASSERT_NO_THROW(manifest m(manifest_data));
    ASSERT_EQ(manifest::_proxy.size(), result.top());
    result.pop();
  }
}