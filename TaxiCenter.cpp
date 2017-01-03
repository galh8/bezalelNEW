//
// Created by amir on 02/12/16.
//

#include "TaxiCenter.h"
#include "TaxiFactory.h"
#include "BFS.h"
#include <cstdlib>

/**
 * the consturctor.
 * @param rows - the number of the rows of the grid.
 * @param columns - the number of the columns of the grid.
 */
TaxiCenter::TaxiCenter(int rows,int columns) {
    map = new Grid(rows,columns);
}

/**
 * destructor. deletes all the realated vectors.
 */
TaxiCenter::~TaxiCenter() {
    int i;
    delete(map);
    for (i=0;i < driversList.size();i++) {
        delete(driversList[i]);
    }
    for (i=0;i < taxisList.size();i++) {
        delete(taxisList[i]);
    }
    for (i=0;i < listOfTrips.size();i++) {
        delete(listOfTrips[i]);
    }
};

void TaxiCenter::sendTaxi(Node *location) {

}

/**
 * @param driverID - gets the driver's id.
 * @return  - the location of the driver.
 */
Node *TaxiCenter::getDriverLocation(int driverID) {
    for(int i =0;i<driversList.size();i++){
        if(driversList[i]->getID()==driverID) {
            return driversList[i]->getLocation();
        }
    }
    return NULL;
}


void TaxiCenter::setDriversList(const vector<Driver> &driversList) {

}

/**
 *
 * @param tripInfo - the inforamtion of the trip
 * @return the choosen driver for the mission
 */
Driver* TaxiCenter::findClosestDriver(TripInfo* tripInfo) {
    vector<Node*> path;
    path = BFS::BFS_Navigate(tripInfo->getStartingPoint(),
                             tripInfo->getEndingPoint());
    unsigned long minDist = 1000000;
    Driver* chosenDriver = NULL;
    for (int i = 0; i < driversList.size(); i++) {
        //can choose a driver just if he is not occupied
        if (!(driversList[i]->isOccupied())) {
            //check the distance of the driver to the starting point of the trip
            path = BFS::BFS_Navigate(driversList[i]->getLocation(),
                                     tripInfo->getStartingPoint());
            /**
             * the senior driver will be the first anyway. there is no need
             * to check if equal / to make a vector of drivers.
             *
             * if that driver path to the trip is the shortest - he will be
             * chosen.
             */

            if (path.size() < minDist)
                chosenDriver = driversList[i];
                minDist = path.size();
        }
    }

    return chosenDriver;
}



/**
 *
 * @param id - the id of the taxi.
 * @param taxi_type - the type of the taxi.
 * @param manufacturer - the manufacturer of the taxi.
 * @param taxiColor - the color of the taxi.
 */
void TaxiCenter::addTaxi(int id, int taxi_type, char manufacturer, char taxiColor) {
    TaxiFactory* taxiFactory = new TaxiFactory;
    taxisList.push_back(taxiFactory->createTaxi(id,taxi_type,manufacturer,
                                               taxiColor));
    delete(taxiFactory);
}

/**
 * @return the list of the taxis.
 */
const vector<TaxiCab *> &TaxiCenter::getTaxisList() const {
    return taxisList;
}

/**
 * adding a driver and assigning him a taxi. (the default stating location
 * is at the point (0,0).
 * @param driver - the driver we want to add.
 */
void TaxiCenter::addDriver(Driver *driver,int taxiId) {
    /**
     * finds the matching taxi for the driver
     */
    for (int i = 0; i < taxisList.size(); i++) {
        if (taxiId == taxisList[i]->getCabID()) {
            driver->setTaxiCabInfo(taxisList[i]);
            break;
        }
    }
    if (driver->getTaxiCabInfo() == NULL) {
        throw "DRIVER GOT WRONG TAXI ID";
    }
    //Setting the driver location at 0,0.
    driver->setLocation(map->getGridNode(Point(0,0)));
     //add the driver to the drivers list.
    driversList.push_back(driver);
}

/**
 * add trip.
 * @param tripId - the id of the trip.
 * @param xStart - the x of the starting point.
 * @param yStart - the y of the starting point.
 * @param xEnd - the x of the ending point.
 * @param yEnd - the y of the ending point.
 * @param numPassengers - the number of passengers.
 * @param tariff - the tariff.
 */
void TaxiCenter::receiveTripInfo(int tripId, int xStart, int yStart, int xEnd,
                                 int yEnd, int numPassengers, double tariff,
                                 int timeOfTrip) {

     TripInfo* newTrip = new TripInfo(tripId,
                                      map->getGridNode(Point(xStart,yStart)),
                                      map->getGridNode(Point(xEnd,yEnd)),
                    numPassengers, tariff,timeOfTrip);
    listOfTrips.push_back(newTrip);
}

/**
 * @return list of trips.
 */
vector<TripInfo *> &TaxiCenter::getListOfTrips() {
    return listOfTrips;
}

/**
 * @return list of drivers.
 */
const vector<Driver *> &TaxiCenter::getDriversList() const {
    return driversList;
}

/**
 * assign the trips to the drivers.
 * if there are more trips than drivers they wont be assigned.
 */
void TaxiCenter::linkDriversTrips(int currentTime,Udp &udp) {

    int serverOperation;

    int i = 0;
    Driver *currentDriver = NULL;
    TripInfo *tripToSend = NULL;
    for (i = 0; i < listOfTrips.size(); i++) {
        //checks if the current time is equal to the trip time.
        if (((listOfTrips[i]->getTimeOfTrip() == currentTime)) && (!(listOfTrips[i]->IsAssigned()))) {
            currentDriver = findClosestDriver(listOfTrips[i]);
            if (currentDriver == NULL)
                break;
            listOfTrips[i]->setIsAssigned(true);
            currentDriver->setTripInfo(listOfTrips[i]);
            currentDriver->setOccupied(true);
            tripToSend = listOfTrips[i];
        }

        if(tripToSend!=NULL){
            //server operation 2 means that the client should get a trip.
            serverOperation = 2;
            //serialize the trip info
            std::string serial_str;
            boost::iostreams::back_insert_device<std::string> inserter(serial_str);
            boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> >
                    s(inserter);
            boost::archive::binary_oarchive ob(s);

            ob << serverOperation;
            s.flush();
            //sending the server operation
            udp.sendData(serial_str);


            //serialize the trip info
            std::string serial_str1;
            boost::iostreams::back_insert_device<std::string> inserter1(serial_str1);
            boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> >
                    s1(inserter1);
            boost::archive::binary_oarchive oa(s1);

            oa << tripToSend;
            s1.flush();
            //sending the trip info
            udp.sendData(serial_str1);
        }else {
            // 3 means that the client shouldnt expect a trip.
            serverOperation = 3;
            //serialize the trip info
            std::string serial_str1;
            boost::iostreams::back_insert_device<std::string> inserter1(serial_str1);
            boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> >
                    s1(inserter1);
            boost::archive::binary_oarchive oa(s1);

            oa << serverOperation;
            s1.flush();
            //sending the server operation
            udp.sendData(serial_str1);
        }

        }
    }


/**
 * running all the trips (that has a driver).
 */
    void TaxiCenter::runAllTrips(int currentTime, Udp &udp) {
        //for loop for running all the drivers to their destination(the path bt the BFS algorithm).
        int i;
        int toMove =2;

        //buffer of the data
        char buffer[1024];

        //telling all the drivers to move - sending 1.
        std::string serial_str1;
        boost::iostreams::back_insert_device<std::string> inserter1(serial_str1);
        boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> >
                s1(inserter1);
        boost::archive::binary_oarchive oa(s1);
        oa << toMove;
        s1.flush();

        //sending the driver
        udp.sendData(serial_str1);
        //getting the new location of the drivers.
        for (int i = 0; i < driversList.size(); i++) {
            //serialize the info of the driver(the client).
            udp.reciveData(buffer, sizeof(buffer));

            //receiving the tripInfo
            string str(buffer, sizeof(buffer));
            Node *newLocation;
            boost::iostreams::basic_array_source<char> device1(str.c_str(),
                                                               str.size());
            boost::iostreams::stream<boost::iostreams::basic_array_source<char> >
                    s2(device1);
            boost::archive::binary_iarchive ia(s2);
            ia >> newLocation;
            if (driversList[i]->isOccupied()) {
                //setting trip info to the driver.
                driversList[i]->setLocation(newLocation);
                //checks if the driver finished the trip
                if ((*((Point *) driversList[i]->getLocation()->getValue())) ==
                    *((Point *) (driversList[i]->getCurrentTrip()->getEndingPoint()->getValue()))) {
                    driversList[i]->setOccupied(false);
                    //TODO We need to erase the trip (ended) from the list of trips..
                    driversList[i]->setTripInfo(NULL);
                   // listOfTrips.erase(listOfTrips.begin());
                    toMove = 2; //the sign that the current trip ended.
                    //telling all the drivers to move - sending 1.
                    std::string serial_str1;
                    boost::iostreams::back_insert_device<std::string> inserter1(
                            serial_str1);
                    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> >
                            s1(inserter1);
                    boost::archive::binary_oarchive oa(s1);
                    oa << toMove;
                    s1.flush();
                    //sending the driver
                    udp.sendData(serial_str1);

                }
            } else {
                toMove = 2; //the sign that the current trip ended.
                //telling all the drivers to move - sending 1.
                std::string serial_str1;
                boost::iostreams::back_insert_device<std::string> inserter1(
                        serial_str1);
                boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> >
                        s1(inserter1);
                boost::archive::binary_oarchive oa(s1);
                oa << toMove;
                s1.flush();
                //sending the driver
                udp.sendData(serial_str1);
            }


        }

    }

///**
// * running all the trips (that has a driver).
// */
//void TaxiCenter::runAllTrips() {
//    //for loop for running all the drivers to their destination(the path bt the BFS algorithm).
//    int i;
//    double stepTurn;
//    for(i=0; i<driversList.size(); i++) {
//        if(driversList[i]->isOccupied()){
//            TripInfo *driverCurrentTrip = driversList[i]->getCurrentTrip();
//            vector<Node*> tripRoute = driverCurrentTrip->gettripRoute();
//            //how much the taxi moves at each step.
//            stepTurn = driversList[i]->getTaxiCabInfo()->getStepTurn();
//            //moving the driver over the trip path.
//            for(int j=0; j<tripRoute.size();j++) {
//                driversList[i]->setLocation(tripRoute[j]);
//                driversList[i]->getCurrentTrip()->setTotalMeters(stepTurn);
//                driversList[i]->getTaxiCabInfo()->
//                                updateKilometersPassed(stepTurn);
//            }
//            //the passengers ranking their driver.
//            for(int j = 0; j<driverCurrentTrip->getNumOfPassengers(); j++) {
//                driversList[i]->setAverageSatisfaction(rand() % 5 + 1);
//            }
//            //deletes the trip and set the driver to be unoccupied.
//            driversList[i]->setOccupied(false);
//            delete(driversList[i]->getCurrentTrip());
//            driversList[i]->setTripInfo(NULL);
//        }
//    }
//}

/**
 * adding an obstacle to the map.
 * @param obsPoint - point with obstacles.
 */
    void TaxiCenter::addMapObstacles(Point obsPoint) {
        map->addObstacle(obsPoint);

    }


