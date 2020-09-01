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

size_t read_complete(char * buf, const error_code & err, size_t bytes) {
    if ( err) return 0;
    bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
    // we read one-by-one until we get to enter, no buffering
    return found ? 0 : 1;
}

ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001);
void sync_echo(std::string msg) {
    msg += "\n";
    ip::tcp::socket sock(service);
    sock.connect(ep);
    sock.write_some(buffer(msg));
    char buf[1024];
    int bytes = read(sock, buffer(buf), boost::bind(read_complete,buf,_1,_2));
    std::string copy(buf, bytes - 1);
    msg = msg.substr(0, msg.size() - 1);
    std::cout << "server echoed our " << msg << ": "
              << (copy == msg ? "OK" : "FAIL") << std::endl;
    sock.close();
}

int main(int argc, char* argv[]) {
    // connect several clients
    std::string str1 = "John says hi";
    char *cstr1 = new char[str1.length() + 1];
    strcpy(cstr1, str1.c_str());

    std::string str2 = "so does James";
    char *cstr2 = new char[str2.length() + 1];
    strcpy(cstr2, str2.c_str());

    std::string str3 = "Boost.Asio is Fun!";
    char *cstr3 = new char[str3.length() + 1];
    strcpy(cstr3, str3.c_str());

    char* messages[] = { cstr1, cstr2, cstr3, nullptr };

    boost::thread_group threads;
    for ( char ** message = messages; *message; ++message) {
        threads.create_thread( boost::bind(sync_echo, *message));
        boost::this_thread::sleep( boost::posix_time::millisec(100));
    }
    threads.join_all();
    delete [] cstr1;
    delete [] cstr2;
    delete [] cstr3;
}


