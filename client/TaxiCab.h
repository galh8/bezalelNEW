//
// Created by amir on 02/12/16.
//

#ifndef EX2AP_TAXICAB_H
#define EX2AP_TAXICAB_H


#include <cmath>
#include <iostream>
enum color {RED,BLUE,GREEN,PINK,WHITE};
enum carManufacturer {HONDA,SUBARU,TESLA,FIAT};

class TaxiCab {
protected:
    int cabID;
    carManufacturer manufacturer;
    color cabColor;
    double kilometersPassed;
    double tariff;
    carManufacturer getCarManufacturer();
    color getColor();


public:
    //TaxiCab(int id, carManufacturer manufacturer,color colour);
    TaxiCab(int id,char manufacturer,char colour);
    void updateKilometersPassed(double kmPassed);
    double getKilometersPassed();
    virtual double getTariff()=0;
    virtual void move()=0;
    int getCabID();
    virtual double getStepTurn() = 0;
};


#endif //EX2AP_TAXICAB_H
