//
// Created by amir on 02/12/16.
//

#ifndef EX2AP_LUXURYCAB_H
#define EX2AP_LUXURYCAB_H
#include "TaxiCab.h"

class LuxuryCab: public TaxiCab {
private:
    int stepTurn;
    double tariff;
public:
    double getTariff();

public:
    LuxuryCab(int id, char manufacturer,char colour);
    void move();
    double getStepTurn();

};


#endif //EX2AP_LUXURYCAB_H
