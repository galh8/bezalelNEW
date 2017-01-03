//
// Created by amir on 26/11/16.
//

#ifndef EX1TASK3_GRID_H
#define EX1TASK3_GRID_H

#include "GridNode.h"
#include <vector>

/**
 * Grid Class, constructing a Grid of grid nodes (implementing by a a vector of
 * vectors (of GridNodes)). The Grid size is rows * columns. The class also
 * determine the neighbors of each gridNode it contains.
 */
class Grid {
private:
    vector<vector<GridNode*> > gridMatrix;
    int rows;
    int columns;
    void setNeighbors();

public:
    Grid(int rowsVal, int columnsVal);
    ~Grid();
    Node* getGridNode(Point point);
    int getRows() const;
    int getColumns() const;
    bool isOnGrid(Point point);
    void addObstacle(Point obsLocation);

};


#endif //EX1TASK3_GRID_H
