#include <iostream>
#include "Udp.h"
#include "Driver.h"
#include "TaxiCenter.h"
#include "LuxuryCab.h"
#include "StandardCab.h"

BOOST_CLASS_EXPORT_GUID(LuxuryCab,"LuxuryCab")
BOOST_CLASS_EXPORT_GUID(StandardCab,"StandardCab")
BOOST_CLASS_EXPORT_GUID(GridNode,"GridNode")

int main() {
    int timePassed = 0;

    int sizeX, sizeY;
    int numOfObstacles, obs_x, obs_y;
    int command;
    //Driver variables
    int driverID, driverAge, driverExperience;
    char driverStatus;
    //trip variables
    int tripID, tripStart_x, tripStart_y, tripEnd_x, tripEnd_y, tripNumPassengers, tripStartTime;
    //The expected drivers amount.
    int numOfDrivers;
    double tripTariff;
    //vehicle variables
    int vehicleID, vehicleType;
    char vehicleManufacturer, vehicleColor;


    //    //Dummy variable for getting the ',' and '_'
    char dummy;
    //The input
    cin >> sizeX;
//    cin>>dummy;
    cin >> sizeY;
    //creating the taxi center with it's grid.
    TaxiCenter *taxiCenter = new TaxiCenter(sizeX, sizeY);


    cin >> numOfObstacles;
    //for loop for getting and setting the obstacles.
    for (int i = 0; i < numOfObstacles; i++) {
        cin >> obs_x;
        cin >> dummy;
        cin >> obs_y;
        taxiCenter->addMapObstacles(Point(obs_x, obs_y));
    }
    //Initializing socket.
    Udp udp(1, 5555);
    udp.initialize();
    char buffer[1024];
do {
    cin>>command;

    switch (command) {
        case 1:
            cin >> numOfDrivers;
            for (int i = 0; i < numOfDrivers; ++i) {
                int driverVehicleID;
                Driver *driver;

                //receive the serialized driver from the client.
                udp.reciveData(buffer, sizeof(buffer));
                string str2(buffer, sizeof(buffer));
                //Deserialize the driver received from the client.
                boost::iostreams::basic_array_source<char> device2(str2.c_str(), str2.size());
                boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s3(device2);
                boost::archive::binary_iarchive ib(s3);
                ib >> driver;

                //receive the serialized vehicleID from the client.
                udp.reciveData(buffer, sizeof(buffer));
                string str(buffer, sizeof(buffer));
                //Deserialize the vehcleID received from the client.
                boost::iostreams::basic_array_source<char> device(str.c_str(), str.size());
                boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
                boost::archive::binary_iarchive ia(s2);
                ia >> driverVehicleID;
                //Add the driver to the taxi center
                taxiCenter->addDriver(driver, driverVehicleID);

                Node* driverLocation = taxiCenter->getDriverLocation(driver->getID());
                //serialize the location of the driver on the grid.
                std::string serial_str3;
                boost::iostreams::back_insert_device<std::string> inserter7(serial_str3);
                boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> >
                        s4(inserter7);
                boost::archive::binary_oarchive o(s4);
                o << driverLocation;
                s4.flush();
                //sending the cab
                udp.sendData(serial_str3);


                //serialize the info of the driver(the client).
                std::string serial_str1;
                TaxiCab *taxiCab = driver->getTaxiCabInfo();
                boost::iostreams::back_insert_device<std::string> inserter1(serial_str1);
                boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> >
                        s1(inserter1);
                boost::archive::binary_oarchive oa(s1);
                oa << taxiCab;
                s1.flush();
                //sending the cab
                udp.sendData(serial_str1);

            }
            break;

        case 2:
            cin >> tripID;
            cin >> dummy;
            cin >> tripStart_x;
            cin >> dummy;
            cin >> tripStart_y;
            cin >> dummy;
            cin >> tripEnd_x;
            cin >> dummy;
            cin >> tripEnd_y;
            cin >> dummy;
            cin >> tripNumPassengers;
            cin >> dummy;
            cin >> tripTariff;
            cin >> dummy;
            cin >> tripStartTime;
            taxiCenter->receiveTripInfo(tripID, tripStart_x, tripStart_y, tripEnd_x,
                                        tripEnd_y, tripNumPassengers, tripTariff, tripStartTime);
            break;

        case 3:
            cin >> vehicleID;
            cin >> dummy;
            cin >> vehicleType;
            cin >> dummy;
            cin >> vehicleManufacturer;
            cin >> dummy;
            cin >> vehicleColor;
            taxiCenter->addTaxi(vehicleID, vehicleType, vehicleManufacturer, vehicleColor);
            break;

        case 4:
            //the id of the driver we want to find.
            cin >> driverID;
            cout << taxiCenter->getDriverLocation(driverID)->valueString() << endl;
            break;

        case 9:
            taxiCenter->linkDriversTrips(timePassed,udp);
            taxiCenter->runAllTrips(timePassed,udp);
            ++timePassed;
            break;


    }
}while (command!=7);
    return 0;
}


/*
GridPoint *gp2;
boost::iostreams::basic_array_source<char> device(serial_str.c_str(), serial_str.size());
boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
boost::archive::binary_iarchive ia(s2);
ia >> gp2;*/