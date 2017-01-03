//
// Created by amir on 01/12/16.
//

#ifndef EX2AP_TRIPINFO_H
#define EX2AP_TRIPINFO_H


#include <cmath>
#include "Point.h"
#include "Node.h"
//#include "Driver.h"

class TripInfo {
private:
    int rideID;
    //Driver* tripDriver;
    double totalMeters;
    Node* startingPoint;
    Node* endingPoint;
    vector<Node*> tripRoute;
    int numOfPassengers;
    double tariff;
    int ratesReceived;
public:
    TripInfo(int driverId,Node *start, Node *dest, int currentNumOfPassengers,
             double currentTariff);
    int getRideID();
    double getTotalMeters();
    void updateMeters(double meters);
    Node* getStartingPoint();
    Node* getEndingPoint();
    int getNumOfPassengers();
    double getTariff();
    void setTotalMeters(double meters);
    void setEndingPoint(Node* endingPoint);
    void setTariff(double tariff);
    vector<Node*> gettripRoute();

};


#endif //EX2AP_TRIPINFO_H
