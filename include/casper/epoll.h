#pragma once
#ifdef __linux__
//epoll is a Linux feature (since Kernel 2.5.44)
#include <sys/epoll.h>
#include <atomic>
#include <vector>

namespace casper {
    class epoll_evt;
    class epoll {
        friend epoll_evt;
    private:
        int evt_ref_counter_{};
        int epollfd_{-1};
        std::vector<epoll_event> events_;
    public:
        epoll();
    };
    class epoll_evt {
    public:
        using epoll_evt_data = epoll_data_t;
        enum class listen_event : uint32_t {
            AVAILABLE_FOR_READ = EPOLLIN,
            AVAILABLE_FOR_WRITE = EPOLLOUT,
            MET_ERROR = EPOLLERR,
            NOTIFY_ONCE = EPOLLONESHOT,
            TRIGGER_ONCE = EPOLLET,
            TRIGGER_PERSIST = 0
        };
        enum class operation : int {
            EPOLL_ADD = EPOLL_CTL_ADD,
            EPOLL_MODIFY = EPOLL_CTL_MOD,
            EPOLL_DELETE = EPOLL_CTL_DEL
        };
    private:
        int fd_;
        operation op_{};
        epoll &ep_;
        int epfd_;
        epoll_event &evt_;
        bool deleted{false};

        void update();
    public:
        epoll_evt() = delete;
        explicit epoll_evt(int fd, epoll &epoll, listen_event levt, epoll_evt_data d);
        void update(listen_event event, epoll_evt_data data);
        void del();
        void wait(int timeout);
        ~epoll_evt();
    };
}
#endif
