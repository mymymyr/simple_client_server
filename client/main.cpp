#include <iostream>

#include "client.hpp"

int main(int argc, char* argv[]) {
  if (argc < 4)
    return 1;
  pbf::Client client(argv[1], std::stoll(argv[2]),
                     std::chrono::seconds(std::stoll(argv[3])));
  if (client.Connect()) {
    std::cout << "is connect" << std::endl;
    client.LoopSendDataToServer();
  }

  return 0;
}
