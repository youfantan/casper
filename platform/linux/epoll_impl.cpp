#include "../../include/casper/epoll.h"
#include "../../include/casper/netbase.h"

epoll_event &find_available_pos(std::vector<epoll_event> &vct) {
    for(auto &evt : vct) {
        if (evt.data.u64 == -1) {
            return evt;
        }
    }
    int cur_subscript = static_cast<int>(vct.size());
    vct.push_back({});
    for (int i = cur_subscript; i < vct.capacity(); ++i) {
        vct[i].data.u64 = -1;
        vct[i].events = 0;
    }
    return vct[vct.size()];
}

casper::epoll_evt::epoll_evt(int fd, epoll &epoll, listen_event levt, epoll_evt_data d) : ep_(epoll), epfd_(epoll.epollfd_), evt_(find_available_pos(ep_.events_)) {
    evt_.events = static_cast<uint32_t>(levt);
    evt_.data = d;
    fd_ = fd;
    op_ = operation::EPOLL_ADD;
    epoll.evt_ref_counter_++;
    update();

}

void casper::epoll_evt::update(casper::epoll_evt::listen_event event, casper::epoll_evt::epoll_evt_data data) {
    evt_.events = static_cast<uint32_t>(event);
    evt_.data = data;
    update();
}

void casper::epoll_evt::update() {
    epoll_ctl(epfd_, static_cast<int>(op_), fd_, &evt_);
}

void casper::epoll_evt::del() {
    if (epoll_ctl(epfd_, EPOLL_CTL_DEL, fd_, &evt_) == -1) {
        throw epoll_exception("error in control epoll", true);
    }
    ep_.evt_ref_counter_ --;
    deleted = true;
    evt_.data.u64 = -1;
    evt_.events = 0;
}

casper::epoll_evt::~epoll_evt() {
    if (!deleted) {
        del();
    }
}

void casper::epoll_evt::wait(int timeout) {
    if (epoll_wait(epfd_, &ep_.events_[0], static_cast<int>(ep_.events_.size()), timeout) == -1) {
        throw epoll_exception("error in wait epoll", true);
    }
}

casper::epoll::epoll() {
    epoll_create(0); //according to Linux Kernel, 1st parameter of epoll_create(int size) seems to be unused
    for (int i = 0; i < events_.capacity(); ++i) {
        events_[i].data.u64 = -1;
        events_[i].events = 0;
    }
}
