cmake_minimum_required(VERSION 3.24)
project(libcasper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)
add_executable(libcasper main.cpp include/casper/tcp_socket.h platform/linux/tcp_socket_impl.cpp include/casper/inet_address.h include/casper/netbase.h include/casper/selector.h include/casper/epoll.h platform/linux/epoll_impl.cpp)
