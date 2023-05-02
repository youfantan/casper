#pragma once

#include "inet_address.h"

namespace casper {
    class tcp_socket {
    public:
        enum class shutdown_operation : unsigned char {
            READ = 0,
            WRITE,
            READ_WRITE
        };
    private:
        int sockfd_{-1};
        inet_address addr_{ippto::IPPTO_UNKNOWN};
    public:
        bool invalid() const {return sockfd_ == -1;}
        inet_address &get_address() {return addr_;};
        int &get_fd() {return sockfd_;};
        explicit tcp_socket(ippto protocol = ippto::IPPTO_V4);
        tcp_socket(int sockfd, inet_address address);
        void connect(inet_address address) const;
        void bind(inet_address address) const;
        void listen(int backlog) const;
        tcp_socket accept() const;
        void shutdown(shutdown_operation op) const;
        void close() const;
        ~tcp_socket() {
            close();
        }
    };
}