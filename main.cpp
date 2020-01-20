#include "manifest.h"
#include "uuid.h"
#include <chrono>
#include <functional>
#include <iostream>
#include <lyra/lyra.hpp>
#include <restclient-cpp/restclient.h>
#include <served/served.hpp>

constexpr char const *red = "\033[0;31m";
constexpr char const *magenta = "\033[0;35m";
constexpr char const *reset = "\033[0m";
constexpr char const *usage = "usage: ./sdk -u url";

// Helper to get information obout the get PURPOSE
auto get_type = [](std::string const &uri, bool &is_manifest) {
  static bool last_is_not_m3u{false};
  std::size_t last_dot{uri.rfind('.')};

  is_manifest = false;

  if (last_dot == std::string::npos || last_dot == uri.size())
    return "[UNKNOWN] ";

  std::string ext{uri.substr(last_dot, uri.size() - ++last_dot)};

  if (ext == "m3u8") {
    if (last_is_not_m3u) {
      last_is_not_m3u = false;
      std::cout << red << "[TRACK SWITCH]" << std::endl;
    }

    is_manifest = true;
    return "[MANIFEST] ";
  }

  last_is_not_m3u = true;

  if (ext == "m3u8") {
    return "[SUBTITLES] ";
  } else if (ext == "ts") {
    return "[SEGMENT] ";
  }

  return "[UNKNOWN] ";
};

// HTTP Get Code
auto handle_http_get = [](std::string const& root, std::string &base,
                          served::response &res, const served::request &req) {
  std::string url;
  std::string const &uri{req.url().URI()};

  // first request, replace it by root url
  if (uri == "/hello.m3u8") {
    url = root;
  } else { // other request, just use it with the base URI
    std::string key;
    if (uri.length() > uuid_str_len) {
      key = uri.substr(1, uuid_str_len);
    }

    auto it = manifest::_proxy.find(key);
    // normal request
    if (it == manifest::_proxy.end()) {
      std::stringstream ss;
      ss << base << uri;
      url = std::move(ss.str());
    } else { // rewritten request
      std::stringstream ss;
      if (!it->second.first) // non-relative
        ss << base << "/" << it->second.second;
      else { // relative, update root
        ss << it->second.second;
        base = it->second.second.substr(0, it->second.second.rfind('/'));
      }
      url = std::move(ss.str());
    }
  }

  bool is_manifest;
  std::string type{get_type(url, is_manifest)};
  std::cout << red << "[IN]" << type << magenta << url << reset << std::endl;
  std::chrono::system_clock timer;

  // Perform GET on remote server
  auto t1 = timer.now();
  RestClient::Response r = RestClient::get(url);
  auto t2 = timer.now();

  if (r.code != 200) {
    std::cout << red << "error: " << reset << uri << std::endl;
  }

  std::cout
      << red << "[OUT]" << type << magenta << url << " ("
      << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
      << "ms)" << reset << std::endl;
  if (is_manifest) {
    manifest mfst(r.body);
    res << mfst.get_manifest_processed_data();
  } else
    res << r.body;
};

int main(int argc, char const *argv[]) {
  // Parsing args
  std::string url;

  auto cli =
      lyra::cli_parser() | lyra::opt(url, "url")["-u"]["--url"]("m3u8 url");

  auto result = cli.parse({argc, argv});
  if (!result) {
    std::cerr << red << "error: " << reset << result.errorMessage()
              << std::endl;
    std::cerr << usage << std::endl;
    return EXIT_FAILURE;
  }

  // check that an url as been given
  if (url.empty()) {
    std::cerr << red << "error: " << reset << "no url provided" << std::endl;
    std::cerr << usage << std::endl;
    return EXIT_FAILURE;
  }

  // cut the url to get base address and complete addr
  std::size_t last_slash = url.rfind('/');
  if (last_slash == std::string::npos) {
    std::cerr << red << "error: " << reset << "not valid url " << url
              << std::endl;
    std::cerr << usage << std::endl;
    return EXIT_FAILURE;
  }

  std::string url_base = url.substr(0, last_slash);

  // Create a multiplexer for handling requests
  served::multiplexer mux;

  // Global http endpoint
  mux.handle("/{addr}").get(std::bind(handle_http_get, std::ref(url), std::ref(url_base),
                                      std::placeholders::_1,
                                      std::placeholders::_2));

  // Create the server and run with 10 handler threads.
  try {
    served::net::server server("127.0.0.1", "8080", mux);
    server.run(10);
  } catch (boost::system::system_error const &se) {
    std::cerr << red << "error: " << reset << se.what() << std::endl;
  }
  return (EXIT_SUCCESS);
}