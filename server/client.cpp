#include "client.hpp"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "connection_delegate.hpp"
#include "utils.hpp"

namespace pbf {

Client::Client(ConnectionDelegate* delegate, int server_socket_fd)
    : server_socket_fd_(server_socket_fd), delegate_(delegate) {}

Client::~Client() {
  if (thread_ && thread_->joinable())
    thread_->join();
}

bool Client::accept() {
  struct sockaddr_in cli_addr;
  socklen_t clilen;
  clilen = sizeof(cli_addr);

  client_sock_fd_ =
      ::accept(server_socket_fd_, (struct sockaddr*)&cli_addr, &clilen);
  if (client_sock_fd_ > 0) {
    delegate_->OnClientConnect(this);
    is_connect_ = true;
    return true;
  }
  return false;
}

void Client::read(const std::string& split) {
  char buffer[255] = {};
  int length = 0;
  do {
    length = ::read(client_sock_fd_, buffer, 255);
    buffer_.append(std::string(buffer, length));
    auto pos = buffer_.find(split);
    while (pos != std::string::npos) {
      delegate_->OnMessage(this, std::string(buffer_.c_str(), pos));
      buffer_.erase(0, pos + split.length());
      pos = buffer_.find(split.length());
    }
  } while (length > 0);
  is_connect_ = false;
  delegate_->OnClientDisconnect(this);
}

bool Client::read_async(const std::string& split) {
  if (thread_)
    return false;

  thread_ = pbf::make_unique<std::thread>([this, split]() { read(split); });
  return true;
}

}  // namespace pbf
