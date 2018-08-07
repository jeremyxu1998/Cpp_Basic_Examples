#pragma once
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "Node.h"
//#include "Resistor.h"

class NodeList {
private:
	Node* head;
//	Node* nptr;

public:
	NodeList();
	~NodeList();
	
	Node* findNode(int NodeID);
	Node* findOrInsertNode(int NodeID);
	bool findResistor(string ResLabel);
	Resistor* findResistor(string ResLabel, bool& resistorExist);
	void changeResistance(string ResLabel, double newResistance);
	void printAll();
	bool checkVoltageSet();
	void solveCircuit();
	void deleteRes(string ResLabel);
	void deleteAll();
};