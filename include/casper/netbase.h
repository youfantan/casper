#pragma once

#include <exception>
#include <string>

#ifdef __linux__
#define PERROR(x) perror(x)
#endif

namespace casper {
    class generic_exception : public std::exception {
    protected:
        std::string msg_;
        bool with_errno_{false};
        explicit generic_exception(const std::string &msg, bool with_errno) : msg_(msg), with_errno_(with_errno) {}
    public:
        generic_exception() = delete;
        virtual const char *what() {
            return msg_.c_str();
        }
        virtual void perr() = 0;
    };

    class epoll_exception : public generic_exception {
    public:
        explicit epoll_exception(const std::string &msg, bool with_errno) : generic_exception(msg, with_errno) {}
        virtual void perr() override {
            if (with_errno_) {
                printf("Caused By Generic Exception: \n");
                PERROR(msg_.c_str());
            } else {
                printf("Caused By Generic Exception: \n%s\n", msg_.c_str());
            }
        }
    };

    class network_exception : std::exception {
        //TODO: implement class network_exception.
    };

    class socket_exception : public generic_exception {
    public:
        explicit socket_exception(const std::string &msg, bool with_errno) : generic_exception(msg, with_errno) {}
        virtual void perr() override {
            if (with_errno_) {
                printf("Caused By Socket Exception: \n");
                PERROR(msg_.c_str());
            } else {
                printf("Caused By Socket Exception: \n%s\n", msg_.c_str());
            }
        }
    };

    class inet_exception : generic_exception {
    public:
        explicit inet_exception(const std::string &msg, bool with_errno) : generic_exception(msg, with_errno) {}
        virtual void perr() override {
            if (with_errno_) {
                printf("Caused By Inet Exception: \n");
                PERROR(msg_.c_str());
            } else {
                printf("Caused By Inet Exception: \n%s\n", msg_.c_str());
            }
        }
    };

    enum class ippto : unsigned char {
        IPPTO_UNKNOWN = 0,
        IPPTO_V4,
        IPPTO_V6
    };
}