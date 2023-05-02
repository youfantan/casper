#include <utility>

#include "../../include/casper/tcp_socket.h"

casper::tcp_socket::tcp_socket(ippto protocol) {
    if (protocol == ippto::IPPTO_V4) {
        if ((sockfd_ = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
            throw socket_exception("error in create socket", true);
        }
    } else if (protocol == ippto::IPPTO_V6) {
        if ((sockfd_ = socket(PF_INET6, SOCK_STREAM, 0)) == -1) {
            throw socket_exception("error in create socket", true);
        }
    } else {
        throw socket_exception("error in create socket, unknown protocol", true);
    }
}

casper::tcp_socket::tcp_socket(int sockfd, casper::inet_address address) : sockfd_(sockfd), addr_(std::move(address)) {
}

void casper::tcp_socket::connect(casper::inet_address address) const {
    if (::connect(sockfd_, address.get(), address.len()) == -1) {
        throw socket_exception("error in connect socket", true);
    }
}

void casper::tcp_socket::bind(casper::inet_address address) const {
    if (::bind(sockfd_, address.get(), address.len()) == -1) {
        throw socket_exception("error in bind socket", true);
    }
}

void casper::tcp_socket::listen(int backlog) const {
    if (::listen(sockfd_, backlog) == -1) {
        throw socket_exception("error in listen socket", true);
    }
}

casper::tcp_socket casper::tcp_socket::accept() const {
    int cli_sockfd{};
    inet_address cli_address;
    sockaddr *addr = cli_address.get();
    socklen_t cli_address_len = cli_address.len();
    if ((cli_sockfd = ::accept(sockfd_, addr, &cli_address_len)) == -1) {
        if (errno != EWOULDBLOCK || errno != EAGAIN) {
            throw socket_exception("error in accept socket", true);
        } else {
            return tcp_socket();
        }
    } else {
#ifdef _GLIBCXX_DEBUG_ONLY
        cli_address._manual_reset();
#endif
        return {cli_sockfd, cli_address};
    }
}

void casper::tcp_socket::shutdown(casper::tcp_socket::shutdown_operation op) const {
    switch (op) {
        case shutdown_operation::READ : ::shutdown(sockfd_, SHUT_RD);
            break;
        case shutdown_operation::WRITE : ::shutdown(sockfd_, SHUT_WR);
            break;
        case shutdown_operation::READ_WRITE : ::shutdown(sockfd_, SHUT_RDWR);
            break;
    }
}

void casper::tcp_socket::close() const {
    ::close(sockfd_);
}
