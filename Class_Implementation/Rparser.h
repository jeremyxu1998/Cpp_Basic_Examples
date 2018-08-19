#ifndef RPARSER_H
#define RPARSER_H

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

#include "Node.h"
#include "Resistor.h"

void maxVal(stringstream &lineStream, bool & inputSuccess, Node** & N, Resistor** & R, int & maxNodeNumber, int & curNumRes, int & maxResNumber);
void insertR(stringstream &lineStream, bool & inputSuccess, Node** N, Resistor** R, int & maxNodeNumber, int & curNumRes, int & maxResNumber);
void modifyR(stringstream &lineStream, bool & inputSuccess, Resistor** R, int & maxNodeNumber, int & curNumRes);
void printR(stringstream &lineStream, bool & inputSuccess, Resistor** R, int & maxNodeNumber, int & curNumRes);
void printNode(stringstream &lineStream, bool & inputSuccess, int & maxNodeNumber);
void deleteR(stringstream &lineStream, bool & inputSuccess);
void peekingSpace(stringstream &lineStream);  // Function for skipping all the spaces
bool peekingCharAndPoint(stringstream &lineStream);  // Function for checking if there is letter or decimal point right after number
#endif  // RPARSER_H
