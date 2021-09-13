#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <chrono>
#include <cstdint>
#include <string>

namespace pbf {

class Client {
 public:
  Client(std::string name, uint16_t port, std::chrono::seconds seconds);
  ~Client();
  bool Connect();

  void LoopSendDataToServer();

 protected:
  bool SendMessage();

 private:
  std::string name_;
  uint16_t port_{};
  std::chrono::seconds seconds_;
  int client_fd_{};
  bool is_exit_ = false;
};

}  // namespace pbf

#endif  // CLIENT_HPP
