/* 
 * File:   Node.h
 * Author: JC and VB
 * Author: TSA
 *
 * Updated on August 24, 2016, 01:40 PM
 * Created on October 6, 2013, 12:58 PM
 */

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

#define MAX_RESISTORS_PER_NODE 5

class Node
{
private:
    int numRes=0;  // number of resistors currently connected
    int resIDArray[MAX_RESISTORS_PER_NODE];  // stores the index of each resistor connected
    // A sneaky move: create arrays in Node class that store all the info for resistors,
    // so that we don't need to get resistor info from resistor class
    string resNameArray[MAX_RESISTORS_PER_NODE];
    double resResArray[MAX_RESISTORS_PER_NODE];
    int reseEndpointsArray[MAX_RESISTORS_PER_NODE][2];

public:
    Node(); 
    ~Node();

    // Checks to see if the resistor can be added to poistion rIndex
    // in the resistor array. Returns true if yes, otherwise false.
    bool canAddResistor(int rIndex);

    // Updates resIDArray to make the resistor in position rIndex in
    // the resistor array connected to this node.
    void addResistor (int rIndex); 
    void addResistorPro(string name_, double resistance_, int endpoints_[2]);

    void changeResistance(string name_, double resistance_);

    // prints the whole node
    // nodeIndex is the position of this node in the node array.
    void print (int nodeIndex);
};

#endif	/* NODE_H */
