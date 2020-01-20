#include "manifest.h"
#include "uuid.h"
#include <iostream>
#include <numeric>
#include <sstream>

std::unordered_map<std::string, std::string> manifest::_proxy;

manifest::manifest(std::string const &data) {

  if (data.substr(0, std::min<std::size_t>(7, data.size())) != "#EXTM3U")
    throw std::make_error_code(std::errc::not_supported);

  std::istringstream is(data);
  for (std::string line; std::getline(is, line);)
    _content.push_back(std::move(line));

  for (auto &line : _content) {
    if (!line.empty() && line[0] != '#') {
      std::string url{line};

      if (line.rfind("../", 0) == 0) {
        uuid u;
        manifest::_proxy[u.to_string()] = line;
        line = u.to_string();
      }
    }
  }
}

std::string manifest::get_manifest_processed_data() {

  return std::accumulate(
      _content.begin(), _content.end(), std::string(),
      [](const std::string &a, const std::string &b) -> std::string {
        return a + b + "\n" ;
      });
}