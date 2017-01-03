//
// Created by amir on 02/12/16.
//

#ifndef EX2AP_DRIVERSATISFACTION_H
#define EX2AP_DRIVERSATISFACTION_H


class DriverSatisfaction {
private:
    int numOfTrips;
    int averageSatisfaction;
public:
    DriverSatisfaction();
    void updateSatisfaction(int satisfaction);
    int getNumOfTrips();
    int getSatisfaction();
};


#endif //EX2AP_DRIVERSATISFACTION_H
