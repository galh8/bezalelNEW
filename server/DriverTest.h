//
// Created by uriel on 29/12/16.
//

#ifndef OZ_DRIVERTEST_H
#define OZ_DRIVERTEST_H
#include <boost/serialization/access.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

class DriverTest {
private:
    int id;
    char c;



    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version) {

        ar & id;
        ar & c;

    }
public:
    DriverTest();

    DriverTest(int id, char c);
    int getId();
    char getChar();
};


#endif //OZ_DRIVERTEST_H
