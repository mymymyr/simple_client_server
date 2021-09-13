#include "client.hpp"

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iomanip>
#include <sstream>
#include <thread>

namespace pbf {
namespace {
constexpr char kServerName[] = "localhost";

std::string GetDateTime() {
  using namespace std::chrono;
  auto now = system_clock::now();
  auto ms = duration_cast<milliseconds>(now.time_since_epoch());
  std::time_t t = std::chrono::system_clock::to_time_t(now);
  int fractional_seconds = ms.count() % 1000;
  std::tm tm = *std::localtime(&t);

  std::stringstream ss;
  ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S.");
  char buf[4];
  snprintf(buf, 4, "%03d", fractional_seconds);
  return ss.str() + buf;
}

}  // namespace

Client::Client(std::string name, uint16_t port, std::chrono::seconds seconds)
    : name_(std::move(name)), port_(port), seconds_(seconds) {}

Client::~Client() {
  is_exit_ = true;
}

bool Client::Connect() {
  struct sockaddr_in serv_addr;
  struct hostent* server;

  client_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd_ < 0)
    return false;
  server = gethostbyname(kServerName);
  if (server == NULL) {
    return false;
  }
  memset((char*)&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  memcpy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr,
         server->h_length);
  serv_addr.sin_port = htons(port_);
  if (connect(client_fd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    return false;
  return true;
}

void Client::LoopSendDataToServer() {
  while (!is_exit_) {
    if (!SendMessage())
      return;
    std::this_thread::sleep_for(seconds_);
  }
}

bool Client::SendMessage() {
  std::string message;
  message.append("[")
      .append(GetDateTime())
      .append("] ")
      .append(name_)
      .append("\n");
  return ::write(client_fd_, message.data(), message.size()) >= 0;
}

}  // namespace pbf
