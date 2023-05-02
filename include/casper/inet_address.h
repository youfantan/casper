#pragma once

#include <string>
#include <utility>
#include <cstring>

#ifdef __linux__

#include <arpa/inet.h>
#include <unistd.h>
#include "netbase.h"

#endif

namespace casper {
    class inet_address {
    public:
    private:
        ippto protocol_{ippto::IPPTO_UNKNOWN};
        std::string host_{};
        uint16_t port_{};
        char sock_addr_bytes_[sizeof(sockaddr_in6)]{};

        void data_helper() {
            if (reinterpret_cast<sockaddr_in6*>(sock_addr_bytes_)->sin6_family == AF_INET) {
                protocol_ = ippto::IPPTO_V4;
                char host_str[64]{};
                port_ = ntohs(reinterpret_cast<sockaddr_in*>(sock_addr_bytes_)->sin_port);
                if (inet_ntop(AF_INET, &reinterpret_cast<sockaddr_in*>(sock_addr_bytes_)->sin_addr, host_str, sizeof(host_str)) != nullptr) {
                    host_ = std::string {host_str};
                } else {
                    throw inet_exception("error in convert in_addr to string", true);
                }
            } else if (reinterpret_cast<sockaddr_in6*>(sock_addr_bytes_)->sin6_family == AF_INET6) {
                protocol_ = ippto::IPPTO_V6;
                char host_str[64]{};
                port_ = ntohs(port_);
                if (inet_ntop(AF_INET6, &reinterpret_cast<sockaddr_in6*>(sock_addr_bytes_)->sin6_addr, host_str, sizeof(in6_addr)) != nullptr) {
                    host_ = std::string {host_str};
                } else {
                    throw inet_exception("error in convert in6_addr to string", true);
                }
            } else {
                protocol_ = ippto::IPPTO_UNKNOWN;
                throw inet_exception("unknown IP protocol, check if the class is void", false);
            }
        }
    public:
        inet_address() {}
        explicit inet_address(ippto protocol) : protocol_(protocol) {}
        inet_address(std::string host, uint16_t port, ippto protocol) : host_(std::move(host)), port_(port), protocol_(protocol) {
            if (protocol_ == ippto::IPPTO_V4) {
                sockaddr_in tmp_addr{};
                tmp_addr.sin_family = AF_INET;
                tmp_addr.sin_port = htons(port_);
                if (inet_pton(AF_INET, host_.c_str(), &tmp_addr.sin_addr) != -1) {
                    memcpy(sock_addr_bytes_, &tmp_addr, sizeof(sockaddr_in));
                } else {
                    throw inet_exception("error in create inet address", true);
                }
            } else if (protocol == ippto::IPPTO_V6) {
                sockaddr_in6 tmp_addr{};
                tmp_addr.sin6_family = AF_INET6;
                tmp_addr.sin6_port = htons(port_);
                if (inet_pton(AF_INET6, host_.c_str(), &tmp_addr.sin6_addr) != -1) {
                    memcpy(sock_addr_bytes_, &tmp_addr, sizeof(sockaddr_in));
                } else {
                    throw inet_exception("error in create inet6 address", true);
                }
            } else {
                throw inet_exception("unknown IP protocol", false);
            }
        }

        sockaddr *get() {
            return reinterpret_cast<sockaddr*>(sock_addr_bytes_);
        }

        std::string &get_host() {
            if (host_.empty()) {
                data_helper();
            }
            return host_;
        }

        uint16_t &get_port() {
            if (host_.empty()) {
                data_helper();
            }
            return port_;
        }

        socklen_t len() {
            if (protocol_ == ippto::IPPTO_V4) {
                return sizeof(sockaddr_in);
            } else if (protocol_ == ippto::IPPTO_V6) {
                return sizeof(sockaddr_in6);
            } else {
                return sizeof(sock_addr_bytes_);
            }
        }

        //for debugger
        void _manual_reset() {
            data_helper();
        }
    };
}