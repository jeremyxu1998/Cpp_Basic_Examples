#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

#include "Resistor.h"

/*Basically are supplementary methods for ResistorList to call*/

Resistor::Resistor(string labelin, double res, int* endpoint) {
	label = labelin;
	resistance = res;
	endpointNodeID[0] = endpoint[0];
	endpointNodeID[1] = endpoint[1];
}

Resistor::~Resistor() {
	
}

string Resistor::getLabel() {
	return label;
}

double Resistor::getResistance() {
	return resistance;
}

void Resistor::setResistance(double newResistance) {
	resistance = newResistance;
}

int Resistor::getEndpoint0() {
	return endpointNodeID[0];
}

int Resistor::getOtherNodeID(int NodeID) {
	if (endpointNodeID[0] == NodeID) return endpointNodeID[1];
	else if (endpointNodeID[1] == NodeID) return endpointNodeID[0];
	else cout << "Something wrong, NodeID not found";
}

/*void Resistor::setNodeVoltageInfo(bool newSetV, double newVoltage, int index) {
	NodesetV[index] = newSetV;
	NodeVoltage[index] = newVoltage;
}*/

Resistor* Resistor::getNext() {
	return next;
}

void Resistor::setNext(Resistor* tarptr) {
	next = tarptr;
}

void Resistor::print() {
	cout.width(20);
	cout << left << label << " ";
	cout << fixed << setprecision(2);
	cout.width(8);
	cout << right << resistance << " Ohms " << endpointNodeID[0] << " -> " << endpointNodeID[1] << endl;
}