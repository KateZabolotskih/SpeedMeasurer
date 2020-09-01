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

std::ostream& operator<<(std::ostream & out, const Measure & measure)
{
    return out << measure.timestamp
                << " " << measure.lidar1_data
                << " " << measure.lidar2_data
                << std::endl;
}
std::istream& operator>>(std::istream & in, Measure & measure)
{
    return in >> measure.timestamp >> measure.lidar1_data >> measure.lidar2_data;
}

void handle_connections() {
    ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(),8001));

    while (true) {
        ip::tcp::socket sock(service);
        acceptor.accept(sock);

        streambuf bufIn, bufOut;
        Measure measure;

        read_until(sock, bufIn, "\n");
        std::istream in(&bufIn);
        in >> measure;

        std::cout << measure;
        std::ostream out(&bufOut);
        out << measure << std::endl;
        write(sock, bufOut);
        sock.close();
    }
}

int main(int argc, char* argv[]) {
    handle_connections();
}
