#include <chrono>
#include <iostream>
#include <sstream>
#include <served/served.hpp>
#include <restclient-cpp/restclient.h>

constexpr char const *base = "https://test-streams.mux.dev/x36xhzz";
constexpr char const
  *root = "https://test-streams.mux.dev/x36xhzz/x36xhzz.m3u8";
constexpr char const *red = "\033[0;31m";
constexpr char const *magenta = "\033[0;35m";
constexpr char const *reset = "\033[0m";

int main(int argc, char const *argv[]) {
  // Create a multiplexer for handling requests
  served::multiplexer mux;

  // Global http endpoint
  mux.handle("/{addr}")
    .get([](served::response& res, const served::request& req) {
      std::string url;

      if (req.url().URI() == "/hello.m3u8") {
        url = root;
      } else {
        std::stringstream ss;
        ss << base << req.url().URI();
        url = std::move(ss.str());
      }

      std::cout << red << "[IN] " << magenta << url << reset << std::endl;
      std::chrono::system_clock timer;

      auto t1 = timer.now();
      RestClient::Response r = RestClient::get(url);
      auto t2 = timer.now();

      std::cout << red << "[OUT] " << magenta << url << " ("
                << std::chrono::duration_cast<std::chrono::milliseconds>(
                  t2 - t1).count()
                << "ms)" << reset << std::endl;
      res << r.body;
    });

  // Create the server and run with 10 handler threads.
  served::net::server server("127.0.0.1", "8080", mux);
  server.run(10);

  return (EXIT_SUCCESS);
}