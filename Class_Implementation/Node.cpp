#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

/*
 * What each command do to Node array are put here
 * All the variables(include the Node**) are all passed by reference, so that all variables can be changed without return
 */

#include "Node.h"
#define MAX_RESISTORS_PER_NODE 5

void NodeArray(string line, Node** & N, int & maxNodeNumber, int & oldMaxNodeNumber) {
	stringstream lineStream(line);
	string cmd;
	lineStream >> cmd;
	if (cmd == "maxVal") {
		lineStream >> maxNodeNumber;
		N = new Node*[maxNodeNumber];
		//new all the nodes needed right in maxVal command
		for (int i = 0; i <= maxNodeNumber; i++) {
			N[i] = new Node;
		}
		oldMaxNodeNumber = maxNodeNumber;
	}
	else if (cmd == "insertR") {
		string name;
		double resistance;
		int endpointNodeIDs[2];
		//A sneaky move: create arrays in Node class that store all the info for resistors, so that we don't need to get resistor info from resistor class
		lineStream >> name >> resistance >> endpointNodeIDs[0] >> endpointNodeIDs[1];
		N[endpointNodeIDs[0]]->addResistorPro(name, resistance, endpointNodeIDs);
		N[endpointNodeIDs[1]]->addResistorPro(name, resistance, endpointNodeIDs);
	}
	else if (cmd == "modifyR") {
		string name;
		double resistance;
		lineStream >> name >> resistance;
		for (int i = 0; i < maxNodeNumber; i++) {//search for every Node and every resistor, find target by name
			N[i]->changeResistance(name, resistance);
		}
	}
	else if (cmd == "printNode") {
		int nodeid;
		string maybeAll;
		cout << "Print:";
		lineStream >> nodeid;
		if (!lineStream.fail()) {
			N[nodeid]->print(nodeid);
		}
		else {
			lineStream.clear();//clear the flag is important
			lineStream >> maybeAll;
			if (!lineStream.fail() && maybeAll == "all") {
				for (int j = 0; j <= maxNodeNumber; j++) {
					N[j]->print(j);
				}
			}
			else cout << "Mistake for Parsor part.";
		}
	}
	else if (cmd == "deleteR") {
		for (int i = 0; i <= oldMaxNodeNumber; i++) {
//			delete N[i];
			N[i] = NULL;
		}
//		delete[] N;
		N = NULL;
		N = new Node*[maxNodeNumber];
		for (int i = 0; i <= maxNodeNumber; i++) {
			N[i] = new Node;
		}
	}
	else {

	}
	return;
}

Node::Node() {
}

Node::~Node() {
}

bool Node::canAddResistor(int rIndex) { //check if the node is full
	if (numRes < MAX_RESISTORS_PER_NODE) return true;
	else return false;
}

// do not use the addResistor method provided but write another method
void Node::addResistorPro(string name_, double resistance_, int endpoints_[2]) {
	resNameArray[numRes] = name_;
	resResArray[numRes] = resistance_;
	reseEndpointsArray[numRes][0] = endpoints_[0];
	reseEndpointsArray[numRes][1] = endpoints_[1];
	numRes++;
	return;
}

void Node::changeResistance(string name_, double resistance_){
	for(int i = 0;i < numRes; i++){
		if(resNameArray[i] == name_){
			resResArray[i] = resistance_;
			break;
		}
	}
}

void Node::print(int nodeIndex) {
	cout << endl << "Connections at node " << nodeIndex << ": " << numRes << " resistor(s)";
	for (int i = 0; i < numRes; i++) {
		cout << endl;
		cout << "  ";
		cout.width(20);
		cout << left << resNameArray[i] << " ";
		cout << fixed << setprecision(2);
		cout.width(8);
		cout << right << resResArray[i] << " Ohms " << reseEndpointsArray[i][0] << " -> " << reseEndpointsArray[i][1];
	}
	return;
}