#include "socket.h"
#include "third_party/easylogging++.h"

#ifdef __linux__

#include <sys/socket.h>

#endif

INITIALIZE_EASYLOGGINGPP

using namespace net_test;

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "Usage: client <IP> <PORT>\n");
    return 1;
  }

  Socket socket(argv[1], argv[2]);
  if (!socket.Create()) { return 1; }
  if (!socket.Connect()) { return 1; }


}
