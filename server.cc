#include "socket.h"
#include "third_party/easylogging++.h"

#include <cstdio>
#include <string>

#ifdef __linux__

#include <sys/socket.h>

#endif

INITIALIZE_EASYLOGGINGPP

using namespace net_test;

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
  uint8_t buffer[1500];
  for(;;) {
    int read = recv(con_sock->sock_fd, buffer, sizeof(buffer), 0);
    if (read == -1) {
      LOG(ERROR) << "Error reading: " << strerror(errno);

    }

  }
}
