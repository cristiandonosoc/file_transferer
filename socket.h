#ifndef SOCKET_H
#define SOCKET_H

#include <arpa/inet.h>
#include <memory>
#include <string>
#include <vector>

namespace net_test {

typedef int socket_fd_t;

class Socket {
  // Constructors
 public:
  Socket();
  Socket(const std::string& ip);
  Socket(const std::string& ip, const std::string& port);
  Socket(sockaddr_in sock_ifo);

 public:
  ~Socket();

  // Setup
 public:
  bool SetIp(const std::string&);
  bool SetIp(uint32_t);
  bool SetPort(const std::string&);
  bool Valid() const;

  // Interface
 public:
  const in_addr& GetIpAddr() const;
  const std::string& GetIpStr() const;
  uint16_t GetPort() const;

  // Socket Interface
 public:
  bool Create();
  bool Bind();
  bool Connect();
  bool Listen();
  std::unique_ptr<Socket> Accept();
  bool Close();

 public:
  sockaddr_in sock_data;
  socket_fd_t sock_fd;
  std::string ip_str;

  bool ip_set;
  bool port_set;

  /* HISTORY LOGS */
  bool binded = false;
  bool connected = false;
  bool listening = false;
};

}   // namespace net_test

#endif  // MAKE_UNIQUE_H
