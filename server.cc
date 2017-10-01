#include "socket.h"
#include "third_party/easylogging++.h"

#include <cstdio>
#include <string>
#include <cstring>

#ifdef __linux__

#include <sys/socket.h>

#endif

INITIALIZE_EASYLOGGINGPP

using namespace net_test;

#define KILO 1024
#define MEGA 1024 * 1024
#define GIGA 1024 * 1024 * 1024

uint8_t message[512 * KILO];

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "Usage: server <IP> <PORT>\n");
    return 1;
  }

  // We attempt to create the socket
  Socket socket;
  socket.SetIp(INADDR_ANY);
  socket.SetPort(argv[2]);

  if (!socket.Create()) { return 1; }
  if (!socket.Bind()) { return 1; }
  if (!socket.Listen()) { return 1; }

  auto con_sock = socket.Accept();
  char buffer[1500];
  for(;;) {
    int read = recv(con_sock->sock_fd, buffer, sizeof(buffer) - 1, 0);
    if (read == -1) {
      LOG(ERROR) << "Error reading: " << strerror(errno);
    } else if (read == 0) {
      LOG(INFO) << "Connection closed from " << con_sock->GetIpStr()
        << ":" << con_sock->GetPort();
      break;
    } else {
      buffer[read] = 0;
      LOG(INFO) << "Read " << read << " bytes: " << buffer;
      if (strcmp(buffer, "SEND") == 0) {
        int sent = send(con_sock->sock_fd, message, sizeof(message), 0);
        LOG(INFO) << "Sent " << sent << " bytes";
      }
    }
  }
}
