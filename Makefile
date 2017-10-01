SRCS=socket.cc third_party/easylogging++.cc

all: server client

server: bin
	g++ --std=c++11 $(SRCS) server.cc -o bin/server

client: bin
	g++ --std=c++11 $(SRCS) client.cc -o bin/client

clean:
	rm -rf bin/

bin:
	mkdir -p bin

