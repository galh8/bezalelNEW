//
// Created by yanaiela on 12/10/16.
//

#include "Udp.h"
#include "Driver.h"
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
    std::cout << "Hello, from client" << std::endl;

    cout << argv[1] << endl;
    Udp udp(0, atoi(argv[1]));
    udp.initialize();
    Driver *driver = new Driver(304859,40,'M',5);

    std::string serial_str;
    boost::iostreams::back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive oa(s);
    oa << driver;
    s.flush();


    char buffer[1024];
    udp.sendData(serial_str);
    udp.reciveData(buffer, sizeof(buffer));
    cout << buffer << endl;
/*
    udp.sendData("sup?");
    udp.reciveData(buffer, sizeof(buffer));
    cout << buffer << endl;*/
    return 0;
}