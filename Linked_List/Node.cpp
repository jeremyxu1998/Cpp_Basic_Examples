#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "Node.h"
#include "ResistorList.h"

/*Basically are supplementary methods for NodeList to call*/

Node::Node(int NodeIDin) {
	NodeID = NodeIDin;
	setV = false;
	resHead = NULL;
}

Node::~Node() {
	
}

void Node::insertResAtTail(string label, double resistance, int* endpointNodeIDs, Resistor* resHead) {
	RList.insertResAtTail(label, resistance, endpointNodeIDs, resHead);
}

bool Node::findResistorInNode(string ResLabel) {
	//if (resHead == NULL) return false;
	bool resistorExist = false;
	resistorExist = RList.findResistorByLabel(ResLabel);
	return resistorExist;
}

Resistor* Node::findResistorInNode(string ResLabel, bool& resistorExist) {
	if (RList.findResistorByLabel(ResLabel)) {
		Resistor* tptr = RList.findResistorByLabel(ResLabel, resistorExist);
		return tptr;
	}
}

void Node::changeResistanceInNode(string ResLabel, double newResistance) {
	if (RList.findResistorByLabel(ResLabel)) {
		bool dummy;
		Resistor* tptr = RList.findResistorByLabel(ResLabel, dummy);
		tptr->setResistance(newResistance);
	}
}

int Node::getNodeID() {
	return NodeID;
}

int Node::getResNum() {
	return RList.getResNum();
}

Resistor* & Node::getResHead() {
	return resHead;
}

bool Node::getSetV() {
	return setV;
}

void Node::changeSetV(bool newSetV) {
	setV = newSetV;
}

double Node::getVoltage() {
	return voltage;
}

void Node::setVoltage(double newVoltage) {
	voltage = newVoltage;
}

double Node::getLastVoltageChange() {
	return lastVoltageChange;
}

void Node::setLastVoltageChange(double newVoltageChange) {
	lastVoltageChange = newVoltageChange;
}

//not a successful attempt
/*void Node::calcNodeVoltage(int numRes) {
	double leftMultiDenominator = 0;
	double rightMuliplicand = 0;
}*/

Resistor* Node::getRListHead() {
	return RList.getHead();
}

int Node::getThisResOtherNodeID(Resistor* tarptr) {
	return RList.getThisResOtherNodeID(tarptr, NodeID);
}

Node* Node::getNext() {
	return next;
}

void Node::setNext(Node* tarptr) {
	next = tarptr;
}

void Node::print() {
	cout << "Connections at node " << NodeID << ": " << getResNum() << " resistor(s)" << endl;
	RList.printResList();
}

//it's a bool to decide whether a node has been cleared totally
bool Node::deleteRes(string ResLabel) {
	RList.deleteResistor(ResLabel);
	if(RList.getResNum() == 0) { //Last Resistor of Node has been deleted
		return true;
	}
	else return false;
}

void Node::deleteAllRes() {
	RList.deleteAllRes();
	resHead = NULL;
}