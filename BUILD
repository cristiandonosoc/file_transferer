cc_library(
    name = "make_unique_lib",
    hdrs = ["make_unique.h"]
)

cc_library(
    name = "socket_lib",
    srcs = ["socket.cc"],
    hdrs = ["socket.h"],
    copts = ["-Wno-unused-variable"],
    deps = [
        ":make_unique_lib",
        "//third_party:easyloggingpp_lib"
    ]
)


cc_binary(
    name = "server",
    srcs = ["server.cc"],
    copts = ["--std=c++11 -Wno-unused-variable"],
    deps = [
        ":socket_lib",
        "//third_party:easyloggingpp_lib"
    ],
)

cc_binary(
    name = "client",
    srcs = ["client.cc"],
    copts = ["--std=c++11 -Wno-unused-variable"],
    deps = [
        ":socket_lib",
        "//third_party:easyloggingpp_lib"
    ],
)

