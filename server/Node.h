//
// Created by amir on 26/11/16.
//
#include "Point.h"
#include <vector>

using namespace std;



#ifndef EX1TASK3_NODE_H
#define EX1TASK3_NODE_H

/**
 * An abstract class representing a node of data structure, allowing to
 * perform some activities on it. The activities make it possible to search inside the data structure.
 */
class Node  {
protected:
    bool visited;
    bool reachable;
    Node *parent;
    vector<Node*> neightboors;
public:
    virtual bool beenVisited() = 0;
    virtual Node* getParent() = 0;
    virtual vector<Node *> getNeightboors() =0;
    virtual void setNeighbor(Node* neighbor) = 0;
    virtual void setVisitedFlag(bool visited) = 0;
    virtual void setParent(Node* parent)=0;
    virtual std::string valueString()  = 0;
    friend ostream&operator << (ostream& os,  Node &n);
    virtual void* getValue() = 0;
    virtual bool isReachable() = 0;
    virtual void setReachable(bool reachFlag) = 0;


};


#endif //EX1TASK3_NODE_H
