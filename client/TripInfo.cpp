//
// Created by amir on 01/12/16.
//

#include "TripInfo.h"
#include "BFS.h"

/**
 * the constructor.
 * @param tripId - the id of the trip.
 * @param start - the start node of the trip.
 * @param dest - the dest node of the trip.
 * @param currentNumOfPassengers - the number of passengers.
 * @param currentTariff - the tarrif of the trip.
 */
TripInfo::TripInfo(int tripId,Node *start, Node *dest,
                   int currentNumOfPassengers,
                   double currentTariff) {
    tripRoute = BFS::BFS_Navigate(start,dest);
    totalMeters = tripRoute.size();
    ratesReceived = 0;
    rideID = tripId;
    startingPoint = start;
    endingPoint = dest;
    numOfPassengers = currentNumOfPassengers;
    tariff = currentTariff;
}

/**
 * @return the ride id.
 */
int TripInfo::getRideID() {
    return rideID;
}



/**
 * @return the total meters.
 */
double TripInfo::getTotalMeters() {
    return totalMeters;
}

/**
 * @return - the starting point.
 */
Node *TripInfo::getStartingPoint() {
    return startingPoint;
}

/**
 * @return- the ending point.
 */
Node *TripInfo::getEndingPoint() {
    return endingPoint;
}

/**
 * @return the number of passengers.
 */
int TripInfo::getNumOfPassengers() {
    return numOfPassengers;
}

/**
 * @return - the tariff of the trip.
 */
double TripInfo::getTariff() {
    return tariff;
}

/**
 * @param meters - the total meters of the trip.
 */
void TripInfo::setTotalMeters(double meters) {
    totalMeters = meters;
}

/**
 * @param newEndingPoint - the ending point of the trip.
 */
void TripInfo::setEndingPoint(Node *newEndingPoint) {
    endingPoint = newEndingPoint;
}

/**
 * @param newTariff - the tarrif of the trip.
 */
void TripInfo::setTariff(double newTariff) {
    tariff = newTariff;
}

/**
 * @return  - the route of the trip.
 */
vector<Node *> TripInfo::gettripRoute() {
    return tripRoute;
}

/**
 * @param meters - update the meters of the route.
 */
void TripInfo::updateMeters(double meters) {
    totalMeters += meters;
}
