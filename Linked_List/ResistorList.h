#ifndef RESISTORLIST_H
#define RESISTORLIST_H
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "Resistor.h"

class ResistorList {
private:
    Resistor* head;
    Resistor* nptr;

public:
    ResistorList();
    ~ResistorList();
    void insertResAtTail(string label, double resistance, int* endpointNodeIDs, Resistor* resHead);
    bool findResistorByLabel(string ResLabel);
    Resistor* findResistorByLabel(string ResLabel, bool& resistorExist);
    Resistor* getHead();
    int getResNum();
    int getThisResOtherNodeID(Resistor* tarptr, int NodeID);
    void printResList();
    void deleteResistor(string ResLabel);
    void deleteAllRes();
};

#endif  // RESISTORLIST_H
