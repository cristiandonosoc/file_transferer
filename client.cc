#include "socket.h"
#include "third_party/easylogging++.h"

#ifdef __linux__

#include <sys/socket.h>

#endif

INITIALIZE_EASYLOGGINGPP

using namespace net_test;

char chunk[1024 * 1024];

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "Usage: client <IP> <PORT>\n");
    return 1;
  }

  Socket socket(argv[1], argv[2]);
  if (!socket.Create()) { return 1; }
  if (!socket.Connect()) { return 1; }

  std::string line;
  for(;;) {
    printf(">> ");
    std::getline(std::cin, line);
    int sent = send(socket.sock_fd, line.c_str(), line.length(), 0);
    if (sent == -1) {
      LOG(ERROR) << "Error sending: " << strerror(errno);
      break;
    } else {
      LOG(INFO) << "Sent " << sent << " bytes";
      if (line == "SEND") {
        int read_count = 0;

        while (true) {
          int read = recv(socket.sock_fd, chunk, sizeof(chunk), 0);
          if (read != -1) {
            read_count += read;
            LOG(INFO) << "Read " << read << "/" << read_count;
          }
        }

      }
    }
  }
}
