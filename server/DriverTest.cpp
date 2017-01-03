//
// Created by uriel on 29/12/16.
//

#include "DriverTest.h"
DriverTest::DriverTest(int id, char c) {
    DriverTest::id = id;
    DriverTest::c = c;
}

int DriverTest::getId() {
    return id;
}

DriverTest::DriverTest() {

}
void DriverTest:serialize(Archive &ar, const unsigned int version) {

    ar & id;
    ar & c;

}

char DriverTest::getChar() {
    return c;
}
