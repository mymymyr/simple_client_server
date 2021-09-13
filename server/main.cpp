#include "kernel.hpp"

int main(int argc, char* argv[]) {
  pbf::Kernel kernel;
  if (!kernel.start(argc, argv))
    return 1;
  kernel.sleep();
  return 0;
}
