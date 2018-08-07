#pragma once
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "ResistorList.h"

class Node {
private:
	int NodeID;
	ResistorList RList;
	Resistor* resHead;
	bool setV;
	double voltage;
	double lastVoltageChange;
	Node* next;

public:
	Node();
	Node(int NodeIDin);
	~Node();

	void insertResAtTail(string label, double resistance, int* endpointNodeIDs, Resistor* resHead);
	bool findResistorInNode(string ResLabel);
	Resistor* findResistorInNode(string ResLabel, bool& resistorExist);
	void changeResistanceInNode(string ResLabel, double newResistance);
	int getNodeID();
	int getResNum();
	Resistor* & getResHead();
	bool getSetV();
	void changeSetV(bool newSetV);
	double getVoltage();
	void setVoltage(double newVoltage);
	double getLastVoltageChange();
	void setLastVoltageChange(double newVoltageChange);
	//void calcNodeVoltage(int numRes);
	Resistor* getRListHead();
	int getThisResOtherNodeID(Resistor* tarptr);
	Node* getNext();
	void setNext(Node* tarptr);
	void print();
	bool deleteRes(string ResLabel);
	void deleteAllRes();
};