#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <memory>
#include <string>
#include <thread>

namespace pbf {
class ConnectionDelegate;
class Client {
 public:
  Client(ConnectionDelegate* delegate, int server_socket_fd);
  ~Client();
  bool accept();
  void read(const std::string& split = "\n");
  bool read_async(const std::string& split = "\n");

  bool is_connect() { return is_connect_; }

 private:
  int server_socket_fd_;
  int client_sock_fd_;
  std::string buffer_;
  bool is_connect_ = false;
  std::unique_ptr<std::thread> thread_;
  ConnectionDelegate* delegate_ = nullptr;
};
}  // namespace pbf

#endif  // CLIENT_HPP
