#include "server.hpp"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <set>
#include <thread>

#include "client.hpp"
#include "utils.hpp"

namespace pbf {
namespace {
constexpr char kLogFilename[] = "log.txt";
}

class ServerImpl {
 public:
  enum class Status {
    Ok,
    ErrorInitSocket,
    ErrorBindSocket,
    ErrorListenSocket,
  };

  ServerImpl(Server* owner, int port) : owner_(owner), port_(port) {}
  ~ServerImpl() { Stop(); }

  int GetPort() const noexcept { return port_; }

  Status Start() noexcept {
    struct sockaddr_in server;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port_);
    server.sin_family = AF_INET;
    serv_socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);

    if (serv_socket_fd_ == -1)
      return Status::ErrorInitSocket;
    if (bind(serv_socket_fd_, (struct sockaddr*)&server, sizeof(server)) < 0)
      return Status::ErrorBindSocket;
    if (listen(serv_socket_fd_, 3) < 0)
      return Status::ErrorListenSocket;

    is_running_ = true;
    return Status::Ok;
  }

  void Stop() { is_running_ = false; }

  void Loop() {
    while (is_running_) {
      EraseInactiveClients();
      auto client = make_unique<Client>(owner_, serv_socket_fd_);
      if (client->accept()) {
        (*clients_.insert(std::move(client)).first)->read_async();
      }
    }
  }

  void EraseInactiveClients() {
    for (auto it = clients_.begin(); it != clients_.end();) {
      if (!((*it)->is_connect())) {
        it = clients_.erase(it);
      } else {
        ++it;
      }
    }
  }

  size_t CountClients() const noexcept { return clients_.size(); }

 private:
  Server* owner_ = nullptr;
  int port_ = {};
  int serv_socket_fd_;
  std::atomic<int> is_running_ = {false};
  std::set<std::unique_ptr<Client>> clients_;
};

Server::Server(int port) : server_impl_(make_unique<ServerImpl>(this, port)) {
  log_file_.Open(kLogFilename);
}

Server::~Server() {
  server_impl_->Stop();
}

void Server::StartLoop() {
  if (log_file_.IsOpen() && server_impl_->Start() == ServerImpl::Status::Ok)
    server_impl_->Loop();
}

void Server::OnClientConnect(Client* client) {
  std::cout << "client connect: " << client << std::endl;
}

void Server::OnClientDisconnect(Client* client) {
  std::cout << "client disconnect: " << client << std::endl;
}

void Server::OnMessage(Client* client, const std::string& message) {
  log_file_.Append(message + "\n");
  std::cout << "[" << client << "] : " << message << std::endl;
}

}  // namespace pbf
