#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

#include "NodeList.h"
#include "Node.h"
#define MIN_ITERATION_CHANGE 0.0001

/*Core part of this program, because it is the entrance of all the action we do for the circuit
 Therefore, functions in other 3 cpp files are all member functions of the class which will be called by NodeList
 !!! So the most comments are written here
 The advantage of this way is being clear, easy to pass arguments and easy to debug
 disadvantage is when we want to edit a resistor, we have to go through several objects and calling multiple member functions
 */

void NodeListFunc(string line, NodeList& NList) {
	stringstream lineStream(line);
	string cmd;
	lineStream >> cmd;

	if (cmd == "insertR") {
		string label;
		double resistance;
		int endpointNodeIDs[2];
		lineStream >> label >> resistance >> endpointNodeIDs[0] >> endpointNodeIDs[1];
		//First, find or create Node according to endpoints of resistor input, and return pointer to it
		Node* Node0ptr = NList.findOrInsertNode(endpointNodeIDs[0]);
		Node* Node1ptr = NList.findOrInsertNode(endpointNodeIDs[1]);
		//Then, create a resistor list for each node and insert the resistor input at tail
		Node0ptr->insertResAtTail(label, resistance, endpointNodeIDs, Node0ptr->getResHead());
		Node1ptr->insertResAtTail(label, resistance, endpointNodeIDs, Node1ptr->getResHead());
	}
	else if (cmd == "modifyR") {
		string label;
		double newResistance;
		lineStream >> label >> newResistance;
		NList.changeResistance(label, newResistance); //Note, both two copies of Resistor should be changed
	}
	else if (cmd == "printR") {
		string label;
		bool resistorExist;
		lineStream >> label;
		cout << "Print:" << endl; //adjusting the endl position carefully
		Resistor* tptr = NList.findResistor(label, resistorExist); //use member functions to make it concise here
		tptr->print();
	}
	else if (cmd == "printNode") {
		int nodeID;
		string maybeAll;
		cout << "Print:" << endl;
		lineStream >> nodeID;
		if (!lineStream.fail()) { //same as last lab, determine if it is a number, then if it is all
			Node* tptr = NList.findNode(nodeID);
			if (tptr != NULL) {
				tptr->print();
			}
			else cout << "Mistake for Parsor part.";
		}
		else {
			lineStream.clear();	//clear the flag is important
			lineStream >> maybeAll;
			if (!lineStream.fail() && maybeAll == "all") {
				NList.printAll();
			}
			else cout << "Mistake for Parsor part.";
		}
	}
	else if (cmd == "setV") {
		int nodeID;
		double voltage;
		lineStream >> nodeID >> voltage;
		Node* tptr = NList.findNode(nodeID);
		if (tptr != NULL) {
			tptr->changeSetV(true); //change the boolean variable (in resistor object) to be true
			tptr->setVoltage(voltage);
		}
		else cout << "Mistake for Parsor part."; //just in case something wrong with parsor
	}
	else if (cmd == "unsetV") {
		int nodeID;
		lineStream >> nodeID;
		Node* tptr = NList.findNode(nodeID);
		if (tptr != NULL) {
			tptr->changeSetV(false);
		}
		else cout << "Mistake for Parsor part.";
	}
	else if (cmd == "solve") {
		NList.solveCircuit(); //all the work are put in member function, see detailed explanation there
	}
	else if (cmd == "deleteR") {
		string label;
		lineStream >> label;
		if (label == "all") {
			NList.deleteAll();
		}
		else if (NList.findResistor(label)) {
			NList.deleteRes(label);
		}
		else cout << "Mistake for Parsor part." << endl;
	}
	return;
}

NodeList::NodeList() {
	head = NULL;
}

NodeList::~NodeList() {
	
}

Node* NodeList::findNode(int NodeID) {
	Node* tptr = head;
	if (tptr == NULL) return NULL;
	while (tptr != NULL && tptr->getNodeID() <= NodeID) {  //tptr != NULL is very important, or seg fault in exercise 6
		if (tptr->getNodeID() == NodeID) {
			return tptr;
		}
		tptr = tptr->getNext();	//traverse
	}
	return NULL;
}

Node* NodeList::findOrInsertNode(int NodeID) {
	Node* tptr = head;
	Node* nptr; //for creating new node
	
	//A failed attempt that I don't know what's wrong
	/*
	if (head == NULL) {	//NodeList is empty
		Node* nptr = new Node(NodeID);
		nptr->setNext(NULL);
		head = nptr;
		return nptr;
	}
	else if (head->getNext() == NULL) {
		if (tptr->getNodeID() > NodeID) {	//insert Node at head
			Node* nptr = new Node(NodeID);
			nptr->setNext(tptr);
			head = nptr;
			return nptr;
		}
		else {
			Node* nptr = new Node(NodeID);	//insert Node at tail
			tptr->setNext(nptr);
			nptr->setNext(NULL);
			return nptr;
		}
	}
	else {
		while (tptr->getNext() != NULL) {
			if (tptr->getNodeID() == NodeID)	//find Node exist
				return tptr;
			else if (tptr->getNodeID() > NodeID && tptr == head) {	//insert Node at head
				Node* nptr = new Node(NodeID);
				nptr->setNext(tptr);
				head = nptr;
				return nptr;
			}
			else if (tptr->getNodeID() < NodeID && tptr->getNext()->getNodeID() > NodeID) {
				Node* nptr = new Node(NodeID);
				nptr->setNext(tptr->getNext());
				tptr->setNext(nptr);
				return nptr;
			}
			tptr = tptr->getNext();
		}
		Node* nptr = new Node(NodeID);	//insert Node at tail
		tptr->setNext(nptr);
		nptr->setNext(NULL);
		return nptr;
	}
	*/
	
	if (head == NULL) {	//NodeList is empty
		head = new Node(NodeID);
		head->setNext(NULL);
		return head;
	}
	while (tptr !=NULL) {  //check if the nodeID already exist
		if (tptr->getNodeID() == NodeID) return tptr;
		else tptr = tptr->getNext();
	}
	tptr = head;
	while (tptr != NULL) {  //all other cases
		if (tptr->getNext() == NULL && tptr->getNodeID() < NodeID){  //insert at tail
			nptr = new Node(NodeID);
			nptr->setNext(NULL);
			tptr->setNext(nptr);
			return nptr;
		}
		else if (tptr->getNodeID() > NodeID) {  //insert at head
			nptr = new Node(NodeID);
			nptr->setNext(tptr);
			head = nptr;
			return nptr;
		}
		else if (tptr->getNodeID() < NodeID && tptr->getNext()->getNodeID() > NodeID) {  //insert in the middle
			nptr = new Node(NodeID);
			nptr->setNext(tptr->getNext());
			tptr->setNext(nptr);
			return nptr;
		}
		tptr = tptr->getNext();
	}
	return nptr;
}

bool NodeList::findResistor(string ResLabel) {  //the first one I wrote, but not that useful
	Node* tptr = head;
	if (tptr == NULL) return false;
	bool resistorExist = false;
	while (tptr != NULL) {
		resistorExist = tptr->findResistorInNode(ResLabel);
		if (resistorExist) break;
		tptr = tptr->getNext();	//traverse
	}
	return resistorExist;
}

Resistor* NodeList::findResistor(string ResLabel, bool& resistorExist) { //overloading, this one is more useful
	Node* tptr = head;
	while (tptr != NULL) {
		bool resistorExist = false;
		Resistor* resptr = tptr->findResistorInNode(ResLabel, resistorExist);
		if (resistorExist) return resptr;
		tptr = tptr->getNext();
	}
}

void NodeList::changeResistance(string ResLabel, double newResistance) {
	Node* tptr = head;
	while (tptr != NULL) { //loop until the end to ensure both resistor's resistance are changed
		tptr->changeResistanceInNode(ResLabel, newResistance);
		tptr = tptr->getNext();
	}
}

void NodeList::printAll() {
	Node* tptr = head;
	if (tptr == NULL) return;
	while (tptr != NULL) {
		tptr->print(); //print each node, leave the detailed work to the method it calls
		tptr = tptr->getNext();
	}
}

bool NodeList::checkVoltageSet() {
	Node* tptr = head;
	if (tptr == NULL) return false;
	while (tptr != NULL) {
		if (tptr->getSetV()) {
			return true;
		}
		tptr = tptr->getNext();
	}
	return false;
}

void NodeList::solveCircuit() {
	//Initialize
	Node* tptr = head;
	if (tptr == NULL) cout << "Mistake for Parsor part.";
	while (tptr != NULL) {
		if (!tptr->getSetV()) {
			tptr->setVoltage(0);
		}
		tptr = tptr->getNext();
	}
	//Iterative procedure
	//!! An important constraint is, because we want to get node voltage by nodeid, this could only be done within NodeList
	bool notStable = true;
	while (notStable) {
		Node* tptr = head;
		while (tptr != NULL) {
			if (!tptr->getSetV()) {
				double leftMultiDenominator = 0;
				double rightMuliplicand = 0;
				double newNodeVoltage = 0;
				
				Resistor* resptr = tptr->getRListHead();
				//while loop
				//get resistor's other NodeID
				//Tricky part is, have to get resistor head for the looping in a ResistorList
				//use NodeID to get voltage
				//do calculation, update left and right according to the formula
				while (resptr != NULL) {
					int resOtherNodeID = tptr->getThisResOtherNodeID(resptr);
					Node* otherNodeptr = findNode(resOtherNodeID);
					double otherNodeVoltage = otherNodeptr->getVoltage();
					leftMultiDenominator += 1 / (resptr->getResistance());
					rightMuliplicand += otherNodeVoltage / (resptr->getResistance());
					resptr = resptr->getNext();
				}
				//do the calculation, and get the difference(voltage change) first and store it into a variable in Node class
				//Then update the node voltage
				newNodeVoltage = rightMuliplicand / leftMultiDenominator;
				double NodeVoltageChange = abs(newNodeVoltage - tptr->getVoltage());
				tptr->setLastVoltageChange(NodeVoltageChange);
				tptr->setVoltage(newNodeVoltage);
			}
			else tptr->setLastVoltageChange(0); //set voltage change of fixed node to 0
			tptr = tptr->getNext();
		}
		// decide stable or not, after a loop of voltage calculation
		tptr = head;
		bool allChangeLessThanMin = true;
		while (tptr != NULL) {
			if (tptr->getLastVoltageChange() > MIN_ITERATION_CHANGE) {
				allChangeLessThanMin = false; //if one change more than the min, then it's still not stable
				break;
			}
			tptr = tptr->getNext();
		}
		if (allChangeLessThanMin) notStable = false;
	}
	//print result
	cout << "Solve:" << endl;
	Node* ttptr = head;
	while (ttptr != NULL) {
		cout << fixed << setprecision(2);
		cout << "  Node " << ttptr->getNodeID() << ": " << ttptr->getVoltage() << " V" << endl;
		ttptr = ttptr->getNext();
	}
}

void NodeList::deleteRes(string ResLabel) {
	Node* tptr = head;
	Node* pptr = NULL;
	while (tptr != NULL) {
		bool nodeWithoutRes = false;
		if (tptr->findResistorInNode(ResLabel)) {
			nodeWithoutRes = tptr->deleteRes(ResLabel);
		}
		pptr = tptr;
		tptr = tptr->getNext();
	}
}

void NodeList::deleteAll() {
	Node* tptr = head;
	if(tptr == NULL) return;
	//delete resistors first and then nodes
	while (tptr != NULL) {
		tptr->deleteAllRes();
		tptr = tptr->getNext();
	}
	while (head != NULL) {
		//using while loop is actually easier than I thought, just let head points to next node and delete the last node
		//Same idea for deleting resistors
		tptr = head;
		head = tptr->getNext();
		delete tptr;
	}
}