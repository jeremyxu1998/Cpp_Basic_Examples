#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "ResistorList.h"
#include "Resistor.h"

/* Basically are supplementary methods for Node to call */

ResistorList::ResistorList() {
    head = NULL;
}

ResistorList::~ResistorList() {
}

void ResistorList::insertResAtTail(string label, double resistance, int* endpointNodeIDs, Resistor* resHead) {
    nptr = new Resistor(label, resistance, endpointNodeIDs);
    if (head == NULL) {  // Empty List, avoiding seg fault for getNext() on a NULL!!
        head = nptr;
        resHead = head;
        nptr->setNext(NULL);
    }
    else {
        Resistor* tptr = head;
        while (tptr->getNext() != NULL)
            tptr = tptr->getNext();
        tptr->setNext(nptr);
        nptr->setNext(NULL);
    }
    return;
}

bool ResistorList::findResistorByLabel(string ResLabel) {
    Resistor* tptr = head;
    bool resistorExist = false;
    while (tptr != NULL) {
        if (tptr->getLabel() == ResLabel) {
            resistorExist = true;
            break;
        }
        tptr = tptr->getNext();
    }
    return resistorExist;
}

Resistor* ResistorList::findResistorByLabel(string ResLabel, bool& resistorExist) {
    Resistor* tptr = head;
    while (tptr != NULL) {
        if (tptr->getLabel() == ResLabel) {
            resistorExist = true;
            break;
        }
        tptr = tptr->getNext();
    }
    return tptr;
}

Resistor* ResistorList::getHead() {
    return head;
}

int ResistorList::getResNum() {
    Resistor* tptr = head;
    if (tptr == NULL) return 0;
    int counter = 0;
    while (tptr != NULL) {
        counter++;
        tptr = tptr->getNext();
    }
    return counter;
}

int ResistorList::getThisResOtherNodeID(Resistor* tarptr, int NodeID) {
    return tarptr->getOtherNodeID(NodeID);
}

void ResistorList::printResList() {
    Resistor* tptr = head;
    if (tptr == NULL) return;
    while (tptr != NULL) {
        cout << "  ";
        tptr->print();
        tptr = tptr->getNext();
    }
}

void ResistorList::deleteResistor(string ResLabel) {
    if(head == NULL) return;
    Resistor* tptr = head;
    Resistor* pptr = NULL;
    while (tptr != NULL) {
        if (tptr->getLabel() == ResLabel) break;
        pptr = tptr;
        tptr = tptr->getNext();
    }
    if(tptr == head) {
        head = head->getNext();
        delete tptr;
        return;
    }
    pptr->setNext(tptr->getNext());
    delete tptr;
}

void ResistorList::deleteAllRes() {
    while(head != NULL) {
        Resistor* tptr = head;
        head = tptr->getNext();
        delete tptr;
    }
}
