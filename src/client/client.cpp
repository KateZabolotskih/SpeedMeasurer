#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include "client.h"

using namespace boost::asio;
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

ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001);

void sync_echo(Measure & measure) {
    streambuf bufOut, bufIn;
    Measure measure1;

    ip::tcp::socket sock(service);
    sock.connect(ep);

    std::ostream out(&bufOut);
    out << measure;
    write(sock, bufOut);

    read_until(sock, bufIn, "\n");
    std::istream in(&bufIn);
    in >> measure1;


    std::cout << "server echoed our " << measure1 << ("OK") << std::endl;
    sock.close();
}

int main(int argc, char* argv[]) {
    // connect several clients
    Measure measure1{123, 12.3,5.4},
            measure2{124, 14.3, 13.22222},
            measure3{125, 8.4429438274, 7.2323};


    Measure* messages[] = { &measure1, &measure2, &measure3, nullptr };

    boost::thread_group threads;
    for ( Measure ** message = messages; *message; ++message) {
        threads.create_thread( boost::bind(sync_echo, **message));
        boost::this_thread::sleep( boost::posix_time::millisec(100));
    }
    threads.join_all();
}


