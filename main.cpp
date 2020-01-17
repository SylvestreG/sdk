#include "pistache/endpoint.h"

using namespace Pistache;

class HelloHandler : public Http::Handler {
public:
  HTTP_PROTOTYPE(HelloHandler)

  void onRequest(const Http::Request &request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "Hello, World");
  }
};

int main() {
  Address addr(Ipv4::any(), Port(9080));

  auto opts = Http::Endpoint::options().threads(1);
  Http::Endpoint server(addr);
  server.init(opts);
  server.setHandler(std::make_shared<HelloHandler>());
  server.serve();
  return 0;
}
