#include "kernel.hpp"

#include "server.hpp"
#include "utils.hpp"

namespace pbf {
Kernel::Kernel() {}

Kernel::~Kernel() {}

bool Kernel::start(int argc, char* argv[]) {
  if (argc > 1) {
    server_ = make_unique<Server>(std::stoll(argv[1]));
    return true;
  }
  return false;
}

void Kernel::sleep() {
  server_->StartLoop();
}

}  // namespace pbf
