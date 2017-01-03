//
// Created by amir on 26/11/16.
//

#ifndef EX1TASK3_GRIDNODE_H
#define EX1TASK3_GRIDNODE_H

#include "Node.h"
#include "Point.h"

/**
 * GridNode class inherit from node abstract class and represent a
 * data stracture node that its value is Point object.
 */
class GridNode: public Node {

private:
    Point value;

public:
    GridNode(Point value);
    bool beenVisited() ;
    Node* getParent() ;
    vector<Node*> getNeightboors() ;
    std::string valueString()  ;
    void setVisitedFlag(bool visited) ;
    void setParent(Node* parent) ;
    void setNeighbor(Node *neighbor) ;
    void * getValue();
    bool isReachable();
    void setReachable(bool reachFlag);

};


#endif //EX1TASK3_GRIDNODE_H
