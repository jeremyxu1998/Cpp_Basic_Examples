#ifndef RPARSER_H
#define RPARSER_H

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

#include "NodeList.h"

// Function prototypes for Rparser

void insertR(stringstream& lineStream, bool& inputSuccess, NodeList& NList);
void modifyR(stringstream& lineStream, bool& inputSuccess, NodeList& NList);
void printR(stringstream& lineStream, bool& inputSuccess, NodeList& NList);
void printNode(stringstream& lineStream, bool& inputSuccess, NodeList& NList);
void setV(stringstream& lineStream, bool& inputSuccess, NodeList& NList);
void unsetV(stringstream& lineStream, bool& inputSuccess, NodeList& NList);
void solve(stringstream& lineStream, bool& inputSuccess, NodeList& NList);
void deleteR(stringstream& lineStream, bool& inputSuccess, NodeList& NList);

#endif  // RPARSER_H
