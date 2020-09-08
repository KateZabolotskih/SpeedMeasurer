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
#include <ctime>

using namespace boost::asio;
using boost::system::error_code;

io_service service;

std::ostream& operator<<(std::ostream & out, const Measure & measure)
{
    return out << measure.timestamp
               << " " << measure.lidarNumber
               << " " << measure.data
               << std::endl;
}
std::istream& operator>>(std::istream & in, Measure & measure)
{
    return in >> measure.timestamp >> measure.lidarNumber >> measure.data;
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


    //std::cout << "server echoed our " << measure1 << ("OK") << std::endl;
    sock.close();
}

float getRandomNumber(float min, float max) {
    static const float fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return rand() * fraction * (max - min + 1) + min;
}

static int counter = 0;

void signal () {
    counter++;
    Measure measure;
    measure.timestamp = std::time(nullptr);
    switch (train) {
        case coming:
            measure.data = getRandomNumber(50,10000);
            measure.lidarNumber = getRandomNumber(1,2);
            break;
        case weel_1:
            measure.data = getRandomNumber(40,50);
            measure.lidarNumber = getRandomNumber(1,2);
            break;
        case gap:
            measure.data = getRandomNumber(50,10000);
            measure.lidarNumber = getRandomNumber(1,2);
            break;
        case weel_2:
            measure.data = getRandomNumber(40,50);
            measure.lidarNumber = getRandomNumber(1,2);
            break;
        case gone:
            measure.data = getRandomNumber(50,10000);
            measure.lidarNumber = getRandomNumber(1,2);
            break;
    }

    boost::thread_group threads;
    threads.create_thread(boost::bind(sync_echo, measure));
    threads.join_all();
}

void lidarSumulator (float V, float A, float B, float C, float D) {
    float t1 = C / V / 1000.f,
            t2 = A / V / 1000.f,
            t3 = B / V / 1000.f,
            t4 = D / V / 1000.f,
            total = t1 + t2 + t3 + t4;

    std::cout << t1 << " "
                << t2 << " "
                << t3 << " "
                << t4 << " "
                << total << std::endl;

    train = coming;
    long start_time = clock();
    std::cout << "start time " << (float)start_time / CLOCKS_PER_SEC << std::endl;

    while (train != stop) {
        signal();
        boost::this_thread::sleep( boost::posix_time::millisec(50));
        float delta = ((float)(clock() - start_time)) / CLOCKS_PER_SEC;
        //std::cout << "1. delta=" << delta << std::endl;
        if (t1 <= delta) {
            std::cout << "wheel 1 time = " << std::time(nullptr) << std::endl;
            train = weel_1;
        }
        if (t1 + t2 <= delta) {
            train = gap;
        }
        if (t1 + t2 + t3 <= delta) {
            train = weel_2;
        }

        if (t1 + t2 + t3 + t4 <= delta) {
            train = gone;
        }
        if (total + 0.1f <= delta) {
            train = stop;
        }
    }

    std::cout << total;
}

int main(int argc, char* argv[]) {
    lidarSumulator(1, 3, 0.5, 3, 0.5);
    std::cout << "counter="<< counter << std::endl;
}


