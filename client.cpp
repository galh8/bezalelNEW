//
// Created by yanaiela on 12/10/16.
//

#include "Udp.h"
#include "Driver.h"
#include "Node.h"
#include "LuxuryCab.h"
#include "StandardCab.h"
#include "GridNode.h"
#include "Socket.h"
#include <unistd.h>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>

#include <boost/archive/tmpdir.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/export.hpp>

using namespace std;

BOOST_CLASS_EXPORT_GUID(LuxuryCab,"LuxuryCab")
BOOST_CLASS_EXPORT_GUID(GridNode,"GridNode")
BOOST_CLASS_EXPORT_GUID(StandardCab,"StandardCab")

int main(int argc, char *argv[]) {
    //initialize the udp
    Socket* client = new Udp(0, atoi(argv[2]));
    client->initialize();

    //Dummy variable for getting the ',' and '_'
    char dummy;

    //Driver variables
    int driverID, driverAge, driverExperience, driverVehicleID;
    char driverStatus;

    //buffer of the data
    char buffer[1024];

    //indicates if the drivers needs to move
    int serverOperation;

    //insert new driver
    cin >> driverID;
    cin >> dummy;
    cin >> driverAge;
    cin >> dummy;
    cin >> driverStatus;
    cin >> dummy;
    cin >> driverExperience;
    cin >> dummy;
    cin >> driverVehicleID;

    //creating new driver
    Driver *driver = new Driver(driverID, driverAge, driverStatus,
                                                driverExperience);

    //serialize the info of the driver(the client).
    std::string serial_str1;
    boost::iostreams::back_insert_device<std::string> inserter1(serial_str1);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> >
                                                                 s1(inserter1);
    boost::archive::binary_oarchive oa(s1);
    oa << driver;
    s1.flush();

    //sending the driver
    client->sendData(serial_str1);

    //serialize the id of the taxi of the driver.
    std::string serial_str2;
    boost::iostreams::back_insert_device<std::string> inserter2(serial_str2);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> >
            s2(inserter2);
    boost::archive::binary_oarchive ob(s2);
    ob << driverVehicleID;
    s2.flush();

    //sending the vehicleID of the taxi
    client->sendData(serial_str2);

    //expecting a location
    client->reciveData(buffer, sizeof(buffer));

    //receiving his location
    string str(buffer, sizeof(buffer));
    Node* location;
    boost::iostreams::basic_array_source<char> device(str.c_str(), str.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> >
            s(device);
    boost::archive::binary_iarchive i(s);
    i >> location;

    //set location
    driver->setLocation(location);

    //expecting a taxi
    client->reciveData(buffer, sizeof(buffer));

    //receiving the taxi cab of the driver.
    string str1(buffer, sizeof(buffer));
    TaxiCab* taxiCab;
    boost::iostreams::basic_array_source<char> device1(str1.c_str(), str1.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> >
            s3(device1);
    boost::archive::binary_iarchive ia(s3);
    ia >> taxiCab;

    //setting the taxi to the driver.
    driver->setTaxiCabInfo(taxiCab);

/**
 * loop that reicives 1 or 2.
 * 1 - move the driver.
 * 2 - get new trip.
 * 3 - do nothing.
 */
    while (true){
    //reicives a command to server operation variable.
    client->reciveData(buffer, sizeof(buffer));
    string str3(buffer, sizeof(buffer));
    boost::iostreams::basic_array_source<char> device3(str3.c_str(),
                                                       str3.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> >
            s5(device3);
    boost::archive::binary_iarchive ic(s5);
    ic >> serverOperation;

    // 1 means - move!
    if (serverOperation == 1) {
        //checks that there is a trip
        if (driver->getCurrentTrip() != NULL) {
            //checks if the trip route is larger than 1
            if (driver->getCurrentTrip()->gettripRoute().size() != 0) {
                driver->move();
                Node *driverLocation = driver->getLocation();
                //serialize the id of the taxi of the driver.
                std::string serial_str3;
                boost::iostreams::back_insert_device<std::string> inserter3(
                        serial_str3);
                boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> >
                        s6(inserter3);
                boost::archive::binary_oarchive ob(s6);
                ob << driverLocation;
                s6.flush();
                //sending the location of the driver after moving
                client->sendData(serial_str3);
            }else {
                delete(driver->getCurrentTrip());
                driver->setOccupied(false);
                driver->setTripInfo(NULL);
            }
        }
    //if server operation = 2 the program expecting to get new trip
        } else if(serverOperation==2) { //we need to set a new trip info.
            //expecting a tripInfo.
            client->reciveData(buffer, sizeof(buffer));
            string str2(buffer, sizeof(buffer));
            TripInfo* tripInfo;
            boost::iostreams::basic_array_source<char> device2(str2.c_str(), str2.size());
            boost::iostreams::stream<boost::iostreams::basic_array_source<char> >
                    s4(device2);
            boost::archive::binary_iarchive ib(s4);
            ib >> tripInfo;
            //assigning the trip to the driver.
            if(tripInfo != NULL){
                driver->setOccupied(true);
                //setting trip info to the driver.
                driver->setTripInfo(tripInfo);
            }

        }else if(serverOperation == 4) {

        //deletes everything and exit client.
        delete(client);
        delete(driver->getTaxiCabInfo());
        delete(driver);
        return 0;
    }

    }

}


