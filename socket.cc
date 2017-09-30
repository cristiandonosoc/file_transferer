#include "socket.h"

#include "make_unique.h"

#include "third_party/easylogging++.h"

#include <cstdio>
#include <cstring>
#include <unistd.h>

namespace net_test {

#define CONNECTION_BACKLOG 20

#define INVALID_SOCKET_MSG() \
  LOG(ERROR) << "Cannot call " << __FUNCTION__ << " on an invalid socket";

#define INVALID_SOCKET_CHECK(return_type) \
  if (!Valid()) {                         \
    INVALID_SOCKET_MSG();                 \
    return return_type;                   \
  }

/**
 * CONSTRUCTORS
 **/

Socket::Socket() {}

Socket::Socket(const std::string& ip_str) : ip_str(ip_str) { SetIp(ip_str); }

Socket::Socket(const std::string& ip_str, const std::string& port_str)
    : ip_str(ip_str) {
  SetIp(ip_str);
  SetPort(port_str);
}

Socket::Socket(sockaddr_in sock_data) : sock_data(sock_data) {
  ip_str = inet_ntoa(sock_data.sin_addr);
  ip_set = true;
  port_set = true;
}

/**
 * DESTRUCTOR
 **/

Socket::~Socket() {
  Close();
}

/**
 * SETUP
 **/

bool Socket::SetIp(const std::string& ip_str) {
  if (!inet_aton(ip_str.c_str(), &sock_data.sin_addr)) {
    LOG(ERROR) << "INVALID IP: " << ip_str.c_str();
    ip_set = false;
  } else {
    ip_set = true;
    memset(&(sock_data.sin_zero), 0, 8);
  }
  return ip_set;
}

bool Socket::SetIp(uint32_t ip) {
  sock_data.sin_addr.s_addr = ip;
  ip_str = inet_ntoa(sock_data.sin_addr);
  ip_set = true;
  return ip_set;
}

bool Socket::SetPort(const std::string& port_str) {
  uint16_t port = strtoul(port_str.c_str(), NULL, 10);
  if (!port) {
    LOG(ERROR) << "INVALID PORT CONVERSION: " << port_str.c_str();
    port_set = false;
  } else {
    sock_data.sin_port = htons(port);
    port_set = true;
  }
  return port_set;
}

bool Socket::Valid() const {
  return ip_set && port_set;
}

/**
 * INTERFACE
 **/
const in_addr& Socket::GetIpAddr() const {
  return sock_data.sin_addr;
}

const std::string& Socket::GetIpStr() const {
  return ip_str;
}

uint16_t Socket::GetPort() const {
  return ntohs(sock_data.sin_port);
}

/**
 * SOCKET INTERFACE
 **/

bool Socket::Create() {
  INVALID_SOCKET_CHECK(false);

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    std::string error(strerror(errno));
    LOG(ERROR) << "Creating Socket: " << error;
    return false;
  }
  sock_data.sin_family = AF_INET;
  LOG(INFO) << "Created Socket for " << GetIpStr() << ":" << GetPort();
  return true;
}


bool Socket::Bind() {
  INVALID_SOCKET_CHECK(false);

  int res = bind(sock_fd, (sockaddr*)&sock_data, sizeof(sockaddr));
  if (res == -1) {
    std::string error(strerror(errno));
    binded = false;
    LOG(ERROR) << "Binding Socket: " << error;
  } else {
    binded = true;
    LOG(INFO) << "Binded Socket for " << GetIpStr() << ":" << GetPort();
  }
  return binded;
}

bool Socket::Connect() {
  INVALID_SOCKET_CHECK(false);

  int res = connect(sock_fd, (sockaddr*)&sock_data, sizeof(sockaddr));
  if (res == -1) {
    std::string error(strerror(errno));
    connected = false;
    LOG(ERROR) << "Connect Socket: " << error;
  } else {
    connected = true;
    LOG(INFO) << "Connected Socket for " << GetIpStr() << ":" << GetPort();
  }

  return connected;
}

bool Socket::Listen() {
  INVALID_SOCKET_CHECK(false);
  if (!binded) {
    LOG(ERROR) << "Cannot call listen on unbinded socket";
    return false;
  }

  int res = listen(sock_fd, CONNECTION_BACKLOG);
  if (res == -1) {
    listening = false;
    LOG(ERROR) << "Listen Socket: " << strerror(errno);
  } else {
    listening = true;
    LOG(INFO) << "Listening Socket for " << GetIpStr() << ":" << GetPort();
  }

  return listening;
}

std::unique_ptr<Socket> Socket::Accept() {
  INVALID_SOCKET_CHECK(nullptr);

  if (!listening) {
    LOG(ERROR) << "Cannot call accept on unlistening socket";
    return nullptr;
  }

  sockaddr_in accepted_socket;
  uint32_t inaddr_size = sizeof(sockaddr_in);
  socket_fd_t new_fd = accept(sock_fd, (sockaddr*)&accepted_socket,
                              &inaddr_size);
  if (new_fd == -1) {
    LOG(ERROR) << "Accept Socket: " << strerror(errno);
    return nullptr;
  }

  auto con_sock = make_unique<Socket>(accepted_socket);
  con_sock->sock_fd = new_fd;
  con_sock->connected = true;
  LOG(INFO) << "Accepted connection for " << con_sock->GetIpStr() << ":"
            << con_sock->GetPort();
  return con_sock;
}

bool Socket::Close() {
  if (sock_fd != -1) {
    close(sock_fd);
    sock_fd = -1;
    LOG(INFO) << "Closed socket for " << GetIpStr() << ":" << GetPort();
    return true;
  }
  return false;
}

}   // namespace net_test
