#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

#include "Rparser.h"  // Prototypes for functions are put in Rparser.h
#include "Rparser.h"
#include "NodeList.h"
#include "Node.h"
#include "ResistorList.h"
#include "Resistor.h"

/*
 * This is for parsing the input you enter
 * By determining the first word in your input (stored in lineStream)
 * It judges what command to do and call the respecting function
 * For more explanation about details, see the comments below
 * Most comments about fail cases are presented in insertR and printNode section
 * because they are similar in other section
 * 
 * I have re-written this part in this lab because many parsing conditions from Lab3 are redundant
 */

int Rparser(string line, bool& inputSuccess, NodeList& NList) {
    // Put the line in a stringstream for parsing
    // Making a new stringstream for each line so flags etc. are in a known state
    stringstream lineStream(line);
    string cmd;
    lineStream >> cmd;

    if (cmd == "insertR") {
        insertR(lineStream, inputSuccess, NList);
    }
    else if (cmd == "modifyR") {
        modifyR(lineStream, inputSuccess, NList);
    }
    else if (cmd == "printR") {
        printR(lineStream, inputSuccess, NList);
    }
    else if (cmd == "printNode") {
        printNode(lineStream, inputSuccess, NList);
    }
    else if (cmd == "setV") {
        setV(lineStream, inputSuccess, NList);
    }
    else if (cmd == "unsetV") {
        unsetV(lineStream, inputSuccess, NList);
    }
    else if (cmd == "solve") {
        solve(lineStream, inputSuccess, NList);
    }
    else if (cmd == "deleteR") {
        deleteR(lineStream, inputSuccess, NList);
    }
    else {
        // Command other than the required 6
        lineStream.ignore(1000, '\n');
        cout << "Error: invalid command" << endl;
        lineStream.ignore(1000, '\n');
    }

    return 0;
}

void insertR(stringstream& lineStream, bool& inputSuccess, NodeList& NList) {
    string label;
    double resistance;
    int nodeidStart, nodeidEnd;
    bool resistorExist = false;

    lineStream >> label;
    resistorExist = NList.findResistor(label);
    if (!resistorExist) {
        lineStream >> resistance >> nodeidStart >> nodeidEnd;
        if (nodeidStart != nodeidEnd) {
            cout << fixed << setprecision(2);  // Two digits precise
            cout << "Inserted: resistor " << label << " " << resistance << " Ohms "
                << nodeidStart << " -> " << nodeidEnd << endl;
            inputSuccess = true;  // Only if pass parsor, let inputSuccess be true, same for later cases
        }
        else {
            cout << "Error: both terminals of resistor connect to node " << nodeidEnd << endl;
            lineStream.ignore(1000, '\n');
        }
    }
    else if(resistorExist) {
        cout << "Error: resistor " << label << " already exists" << endl;
        lineStream.ignore(1000, '\n');
    }
    else {
        cout << "Error: there's something I haven't considered about" << endl;
        lineStream.ignore(1000, '\n');
    }
    return;
}

void modifyR(stringstream& lineStream, bool& inputSuccess, NodeList& NList) {
    string label;
    double newResistance;
    bool resistorExist = false;

    lineStream >> label;
    resistorExist = NList.findResistor(label);
    if (resistorExist) {  // Only thing to check in this lab is if the resistor exist
        Resistor* tptr = NList.findResistor(label, resistorExist);
        double oldResistance = tptr->getResistance();
        lineStream >> newResistance;
        cout << fixed << setprecision(2);
        cout << "Modified: resistor " << label << " from " << oldResistance << " Ohms to " << newResistance << " Ohms" << endl;
        inputSuccess = true;
    }
    else if (!resistorExist) {
        cout << "Error: resistor " << label << " not found" << endl;
        lineStream.ignore(1000, '\n');
    }
    else {
        cout << "Error: there's something I haven't considered about";
        lineStream.ignore(1000, '\n');
    }
    return;
}

void printR(stringstream& lineStream, bool& inputSuccess, NodeList& NList) {
    string label;
    bool resistorExist = false;

    lineStream >> label;
    resistorExist = NList.findResistor(label);
    if (resistorExist) {
        inputSuccess = true;
    }
    else if (!resistorExist) {
        cout << "Error: resistor " << label << " not found" << endl;
        lineStream.ignore(1000, '\n');
    }
    else {
        cout << "Error: there's something I haven't considered about";
        lineStream.ignore(1000, '\n');
    }
    return;
}

void printNode(stringstream& lineStream, bool& inputSuccess, NodeList& NList) {
    int nodeID;
    string maybeAll;
    lineStream >> nodeID;
    if (!lineStream.fail()) {
        Node* tptr = NList.findNode(nodeID);
        if (tptr != NULL) {
            inputSuccess = true;
        }
        else {
            cout << "Error: node " << nodeID << " not found" << endl;
            lineStream.ignore(1000, '\n');
        }
    }
    else {
        lineStream.clear();	 // Clear the flag is important
        lineStream >> maybeAll;
        if (!lineStream.fail() && maybeAll == "all") {
            inputSuccess = true;
        }
        else {
            cout << "Input string but not all";
            lineStream.ignore(1000, '\n');
        }
    }
}

void setV(stringstream& lineStream, bool& inputSuccess, NodeList& NList) {
    int nodeID;
    double voltage;
    lineStream >> nodeID;
    Node* tptr = NList.findNode(nodeID);
    if (tptr != NULL) {
        lineStream >> voltage;
        cout << fixed << setprecision(2);
        cout << "Set: node " << nodeID << " to " << voltage << " Volts" << endl;
        inputSuccess = true;
    }
    else {
        cout << "Error: node " << nodeID << " not found" << endl;
        lineStream.ignore(1000, '\n');
    }
}

void unsetV(stringstream& lineStream, bool& inputSuccess, NodeList& NList) {
    int nodeID;
    lineStream >> nodeID;
    Node* tptr = NList.findNode(nodeID);
    if (tptr != NULL) {
        cout << "Unset: the solver will determine the voltage of node " << nodeID << endl;
        inputSuccess = true;
    }
    else {
        cout << "Error: node " << nodeID << " not found" << endl;
        lineStream.ignore(1000, '\n');
    }
}

void solve(stringstream& lineStream, bool& inputSuccess, NodeList& NList) {
    if (NList.checkVoltageSet()) {
        inputSuccess = true;
    }
    else {
        cout << "Error: no nodes have their voltage set" << endl;
        lineStream.ignore(1000, '\n');
    }
}

void deleteR(stringstream& lineStream, bool& inputSuccess, NodeList& NList) {
    string label;
    lineStream >> label;
    if (label == "all") {
        cout << "Deleted: all resistors" << endl;
        inputSuccess = true;
    }
    else if (NList.findResistor(label)) {
        cout << "Deleted: resistor " << label << endl;
        inputSuccess = true;
    }
    else {
        cout << "Error: resistor " << label << " not found" << endl;
        lineStream.ignore(1000, '\n');
    }
}
