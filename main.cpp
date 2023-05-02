#include <iostream>
#include <thread>
#include "include/casper/tcp_socket.h"
int main() {
    using namespace casper;
    try {
        tcp_socket socket;
        socket.bind({"127.0.0.1", 2333, ippto::IPPTO_V4});
        socket.listen(10);
        std::thread client_thread ([]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            try {
                tcp_socket socket;
                socket.connect({"127.0.0.1", 2333, ippto::IPPTO_V4});
                socket.close();
            } catch (socket_exception &e) {
                e.perr();
            }
        });
        client_thread.join();
        tcp_socket client = socket.accept();
        if (!client.invalid()) {
            printf("new connection: %s:%d\n", client.get_address().get_host().c_str(), client.get_address().get_port());
            client.close();
        }
        socket.close();
    } catch (socket_exception &e) {
        e.perr();
    } catch (inet_exception &e) {
        e.perr();
    }
}
