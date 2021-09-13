#ifndef CONNECTION_DELEGATE_HPP
#define CONNECTION_DELEGATE_HPP
#include <string>

namespace pbf {
class Client;
class ConnectionDelegate {
 public:
  virtual void OnClientConnect(Client* client) {}
  virtual void OnClientDisconnect(Client* client) {}
  virtual void OnMessage(Client* client, const std::string& message) {}

 protected:
  virtual ~ConnectionDelegate() {}
};
}  // namespace pbf

#endif  // CONNECTION_DELEGATE_HPP
