//
// Created by amir on 26/11/16.
//

#ifndef EX1TASK3_POINT_H
#define EX1TASK3_POINT_H
#include <iostream>
using namespace std;

/**
 * Point class. Each Point containing a x and y coordinates. The class also
 * allowing to print a point by the format '(x,y)'.
 */
class Point {
private:
    int x;
    int y;
public:
    Point(int xVal, int yVal);
    int getX() const;
    int getY() const;
    friend ostream&operator << (ostream& os, const Point &point);
    const Point operator+(const Point& point)const;
    bool operator==(const Point& point)const;
    bool operator!=(const Point& point)const;
};


#endif //EX1TASK3_POINT_H
