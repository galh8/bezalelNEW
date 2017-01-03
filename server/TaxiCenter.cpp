//
// Created by amir on 02/12/16.
//

#include "TaxiCenter.h"
#include "TaxiFactory/TaxiFactory.h"
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
                                 int yEnd, int numPassengers, double tariff) {

     TripInfo* newTrip = new TripInfo(tripId,
                                      map->getGridNode(Point(xStart,yStart)),
                                      map->getGridNode(Point(xEnd,yEnd)),
                    numPassengers, tariff);
    listOfTrips.push_back(newTrip);
}

/**
 * @return list of trips.
 */
const vector<TripInfo *> &TaxiCenter::getListOfTrips() const {
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
void TaxiCenter::linkDriversTrips() {
    int i=0;
    Driver* currentDriver = NULL;
        for ( i = 0; i < listOfTrips.size(); i++) {
            currentDriver = findClosestDriver(listOfTrips[i]);
            if (currentDriver == NULL)
                break;
            currentDriver->setTripInfo(listOfTrips[i]);
            currentDriver->setOccupied(true);
        }
    if (currentDriver != NULL) {
        listOfTrips.erase(listOfTrips.begin(), listOfTrips.begin() + i);
    }
}

/**
 * running all the trips (that has a driver).
 */
void TaxiCenter::runAllTrips() {
    //for loop for running all the drivers to their destination(the path bt the BFS algorithm).
    int i;
    double stepTurn;
    for(i=0; i<driversList.size(); i++) {
        if(driversList[i]->isOccupied()){
            TripInfo *driverCurrentTrip = driversList[i]->getCurrentTrip();
            vector<Node*> tripRoute = driverCurrentTrip->gettripRoute();
            //how much the taxi moves at each step.
            stepTurn = driversList[i]->getTaxiCabInfo()->getStepTurn();
            //moving the driver over the trip path.
            for(int j=0; j<tripRoute.size();j++) {
                driversList[i]->setLocation(tripRoute[j]);
                driversList[i]->getCurrentTrip()->setTotalMeters(stepTurn);
                driversList[i]->getTaxiCabInfo()->
                                updateKilometersPassed(stepTurn);
            }
            //the passengers ranking their driver.
            for(int j = 0; j<driverCurrentTrip->getNumOfPassengers(); j++) {
                driversList[i]->setAverageSatisfaction(rand() % 5 + 1);
            }
            //deletes the trip and set the driver to be unoccupied.
            driversList[i]->setOccupied(false);
            delete(driversList[i]->getCurrentTrip());
            driversList[i]->setTripInfo(NULL);
        }
    }
}

/**
 * adding an obstacle to the map.
 * @param obsPoint - point with obstacles.
 */
void TaxiCenter::addMapObstacles(Point obsPoint) {
    map->addObstacle(obsPoint);

}

