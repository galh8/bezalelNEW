//
// Created by amir on 02/12/16.
//

#include "LuxuryCab.h"

void LuxuryCab::move() {

}

/**
 * the constructor.
 * @param id - the id of the taxi.
 * @param manufacturer  - the manufacturer of the taxi.
 * @param colour - the color of the taxi.
 */
LuxuryCab::LuxuryCab(int id, char manufacturer, char colour)
        : TaxiCab(id, manufacturer, colour) {
    stepTurn = 1;
    tariff = 2.0;
}

/**
 *
 * @return - the step of each taxi.
 */
double LuxuryCab::getStepTurn() {
    return stepTurn;
}

double LuxuryCab::getTariff() {
    return tariff;
}
