#ifndef RESISTOR_H
#define RESISTOR_H
#include <iostream>
#include <string>
using namespace std;

class Resistor {
private:
    string label;
    double resistance;
    int endpointNodeID[2];
    Resistor* next;

public:
    Resistor();
    Resistor(string labelin, double res, int* endpoint);
    ~Resistor();

    string getLabel();
    double getResistance();
    void setResistance(double newResistance);
    int getEndpoint0();
    int getOtherNodeID(int NodeID);
    Resistor* getNext();
    void setNext(Resistor* tarptr);
    void print();
};

#endif  // RESISTOR_H
