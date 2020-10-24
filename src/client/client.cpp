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

void signal (long current_time) {
    counter++;
    Measure measure;
    measure.timestamp = current_time;
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

void lidarSumulator (float V, float wheel_size, float between_wheel, float before_wheels, float after_wheel) {
    float t1 = before_wheels / V / 1000.f,
            t2 = wheel_size / V / 1000.f,
            t3 = between_wheel / V / 1000.f,
            t4 = after_wheel / V / 1000.f,
            total = t1 + t2 + t3 + t2 + t4;

    std::cout << t1 << " "
                << t2 << " "
                << t3 << " "
                << t4 << " "
                << total << std::endl;

    train = coming;
    long start_time = clock();
    std::cout << "start time " << (float)start_time / CLOCKS_PER_SEC << std::endl;

    long last_time, current_time = clock();
    while (train != stop) {
        last_time = current_time;
        current_time = clock();

        long sleep_time = 50 - (current_time - last_time) / CLOCKS_PER_SEC;
        //std::cout << "last_time = " << last_time << " current_time " << current_time << std::endl;
        //std::cout << "sleep_time = " << sleep_time << std::endl;
        boost::this_thread::sleep( boost::posix_time::millisec(sleep_time));
        float delta = ((float)(current_time - start_time)) / CLOCKS_PER_SEC;
        //std::cout << "1. delta=" << delta << std::endl;
        if (t1 + t2 + t3 + t2 + t4 <= delta) {
            std::cout << "stop" << " current time = " << current_time << std::endl;
            train = stop;
        } else if (t1 + t2 + t3 + t2 <= delta) {
            std::cout << "gone" << " current time = " << current_time << std::endl;
            train = gone;
        } else if (t1 + t2 + t3 <= delta) {
            std::cout << "weel 2" << " current time = " << current_time << std::endl;
            train = weel_2;
        } else if (t1 + t2 <= delta) {
            std::cout << "gap" << " current time = " << current_time << std::endl;
            train = gap;
        } else if (t1 <= delta) {
            std::cout << "wheel 1" << " current time = " << current_time << std::endl;
            train = weel_1;
        } else {
            std::cout << "prepare" << "cuurent time = " << current_time << std::endl;
        }
        signal(current_time);
    }

    std::cout << "total=" << total << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "CLOCK_PER_SEC=" << CLOCKS_PER_SEC << std::endl;
    lidarSumulator(1, 1, 3, 3, 3);
    std::cout << "counter="<< counter << std::endl;
}


