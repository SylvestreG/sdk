#include <chrono>
#include <functional>
#include <iostream>
#include <sstream>
#include <lyra/lyra.hpp>
#include <served/served.hpp>
#include <restclient-cpp/restclient.h>

constexpr char const *red = "\033[0;31m";
constexpr char const *magenta = "\033[0;35m";
constexpr char const *reset = "\033[0m";
constexpr char const *usage = "usage: ./sdk -u url";

auto handle_http_get = [](
  std::string const& root,
  std::string const& base,
  served::response& res,
  const served::request& req) {
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
};

int main(int argc, char const *argv[]) {
  //Parsing args
  std::string url;

  auto cli = lyra::cli_parser()
    | lyra::opt(url, "url")
    ["-u"]["--url"]
      ("m3u8 url");

  auto result = cli.parse({argc, argv});
  if (!result) {
    std::cerr << red << "error: " << reset << result.errorMessage()
              << std::endl;
    std::cerr << usage << std::endl;
    return EXIT_FAILURE;
  }

  //check that an url as been given
  if (url.empty()) {
    std::cerr << red << "error: " << reset << "no url provided" << std::endl;
    std::cerr << usage << std::endl;
    return EXIT_FAILURE;
  }

  //cut the url to get base address and complete addr
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
  mux.handle("/{addr}")
    .get(std::bind(handle_http_get,
                   url,
                   url_base,
                   std::placeholders::_1,
                   std::placeholders::_2));

  // Create the server and run with 10 handler threads.
  try {
    served::net::server server("127.0.0.1", "8080", mux);
    server.run(10);
  } catch (boost::system::system_error const& se) {
    std::cerr << red << "error: " << reset << se.what() << std::endl;
  }
  return (EXIT_SUCCESS);
}