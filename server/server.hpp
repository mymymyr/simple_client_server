#ifndef SERVER_HPP
#define SERVER_HPP
#include <memory>

#include "connection_delegate.hpp"
#include "threadsafefile.hpp"

namespace pbf {
class ServerImpl;

class Server : public ConnectionDelegate {
 public:
  Server(int port);
  ~Server();

  void StartLoop();

  // ConnectionDelegate:
  void OnClientConnect(Client* client) override;
  void OnClientDisconnect(Client* client) override;
  void OnMessage(Client* client, const std::string& message) override;

 private:
  std::unique_ptr<ServerImpl> server_impl_;
  ThreadSafeFile log_file_;
};

}  // namespace pbf

#endif // SERVER_HPP
