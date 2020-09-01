#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "server.h"

using namespace boost::asio;
using namespace boost::posix_time;
using boost::system::error_code;

io_service service;
size_t read_complete(Measure *measure, const error_code &err, size_t bytes) {
    if (err) return 0;
    bool found = std::find(buff, buff + bytes, '\n') < buff + bytes;
    // we read one-by-one until we get to enter, no buffering
    return found ? 0 : 1;
}

void readAllData(Measure& measure, ip::tcp::socket *sock) {
    int bytes = read(*sock, sizeof(Measure), boost::bind(read_complete, measure, _1,_2));

}

void handle_connections() {
    ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(),8001));
    char buff[1024];
    while (true) {
        ip::tcp::socket sock(service);
        acceptor.accept(sock);
        Measure *measure;

        int bytes = read(sock, buffer(measure, sizeof(Measure)),
                         boost::bind(read_complete,buff,_1,_2));
        std::cout << buff << std::endl;
        std::string msg(buff, bytes);
        sock.write_some(buffer(msg));
        sock.close();
    }
}

int main(int argc, char* argv[]) {
    handle_connections();
}
