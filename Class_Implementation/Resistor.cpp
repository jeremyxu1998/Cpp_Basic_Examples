#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

#include "Resistor.h"

/*
 * What each command do to Resistor array are put here
 * All the variables(include the Resistor**) are all passed by reference, so that all variables can be changed without return
 */

void ResistorArray(string line, Resistor** & R, int & curNumRes, int & maxResNumber, int & oldNumRes) {
	stringstream lineStream(line);
	string cmd;
	lineStream >> cmd;
	if (cmd == "maxVal") {
//		if (R != NULL) {
//			for (int i = 0; i < oldNumRes; i++) {
//				delete R[i];
//				R[i] = NULL;
//			}
//			delete[] R;
//			R = NULL;
//			curNumRes = 0;
//		}
		
		int maxNodeGar;
		lineStream >> maxNodeGar >> maxResNumber;
		R = new Resistor*[maxResNumber];
	}
	else if (cmd == "insertR") {
		string name;
		double resistance;
		int endpointNodeIDs[2];
		lineStream >> name >> resistance >> endpointNodeIDs[0] >> endpointNodeIDs[1];
		//unlike Node, new each Resistor object when a resistor is inserted
		R[curNumRes] = new Resistor(curNumRes, name, resistance, endpointNodeIDs);
		curNumRes++;
		oldNumRes = curNumRes;
	}
	else if (cmd == "modifyR") {
		string name;
		double resistance;
		lineStream >> name >> resistance;
		for (int i = 0; i < curNumRes; i++) {
			if (R[i]->getName() == name) {
				cout << "Modified: resistor " << name << " from " << R[i]->getResistance() << " Ohms to " << resistance << " Ohms";
				R[i]->setResistance(resistance);
				break;
			}
		}
	}
	else if (cmd == "printR") {
		string name;
		cout << "Print:";
		lineStream >> name;
		if (name == "all") {
			for (int i = 0; i < curNumRes; i++) R[i]->print();
		}
		else {
			for (int i = 0; i < curNumRes; i++) {
				if (R[i]->getName() == name) {
					R[i]->print();
					break;
				}
			}
		}
	}
	else if (cmd == "deleteR") {
		for (int i = 0; i < oldNumRes; i++) {
			delete R[i];
			R[i] = NULL;
		}
		delete[] R;
		R = NULL;
		R = new Resistor*[maxResNumber];
		curNumRes = 0;
	}
	else {

	}
	return;
}

Resistor::Resistor(int rIndex_, string name_, double resistance_, int endpoints_[2]) {
	resID = rIndex_;
	name = name_;
	resistance = resistance_;
	endpointNodeIDs[0] = endpoints_[0];
	endpointNodeIDs[1] = endpoints_[1];
}

Resistor::~Resistor() {
}

string Resistor::getName() const{
	return this->name;
} // returns the name

double Resistor::getResistance() const {
	return this->resistance;
}// returns the resistance

void Resistor::setResistance(double resistance_) {
	resistance = resistance_;
}

void Resistor::print() {
	cout << endl;
	cout.width(20);
	cout << left << name << " ";
	cout << fixed << setprecision(2);
	cout.width(8);
	cout << right << resistance << " Ohms " << endpointNodeIDs[0] << " -> " << endpointNodeIDs[1];
}