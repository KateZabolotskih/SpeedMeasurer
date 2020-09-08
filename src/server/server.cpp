#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "speedMeasurerManager.h"
#include "speedMeasurer.h"

#define D 0.5f
#define S 3.f
#define L 1.f

using namespace boost::asio;
using namespace boost::posix_time;
using boost::system::error_code;

io_service service;


std::ostream& operator<<(std::ostream & out, const measure & measure)
{
    return out << measure.timestamp
               << " " << measure.lidarNumber
               << " " << measure.data
               << std::endl;
}
std::istream& operator>>(std::istream & in, measure & measure)
{
    return in >> measure.timestamp >> measure.lidarNumber >> measure.data;
}


void onResult(result result) {
    cout << "status:" << result.status << endl;
    cout << "speed:" << result.speed << endl;
}


void handle_connections() {
    bool flag = true;
    ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(),8001));
    auto * filter = (StreamFilter *)new StreamFloatMedianFilter(3);
    DefaultSpeedMeasurerManager manager(onResult);
    SpeedMeasurer * measurer1 = (SpeedMeasurer *)new SingleLidarSpeedMeasurer(filter, onResult);
    measurer1->setB(45);
    measurer1->setBRange(5);
    measurer1->setL(1);
    measurer1->setD(1);
    manager.addSpeedMeasurer(measurer1);


    while (flag) {
        ip::tcp::socket sock(service);
        acceptor.accept(sock);

        boost::asio::streambuf bufIn, bufOut;
        measure measure;

        read_until(sock, bufIn, "\n");
        std::istream in(&bufIn);
        in >> measure;

        if (false) { flag = false;}

        manager.addMeasuring(measure);
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
