#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

#include "Rparser.h"
#include "NodeList.h"
#include "Node.h"
#include "ResistorList.h"
#include "Resistor.h"

int Rparser(string line, bool& inputSuccess, NodeList& NList);
void NodeListFunc(string line, NodeList& NList);

/*
 * This program is for simulating node circuit network behavior
 * Main idea of the structure of this program is: parser part is only responsible for checking if the statement is correct
 * While NodeList.cpp is responsible for the move for what each command do in Node and Resistor
 * It divides up what a command should do into different functions, which seems easier to figure out what might goes wrong
 */

int main() {
    string line;
    NodeList NList;
    bool inputSuccess;

    cout << "> ";
    getline(cin, line);  // Get a line from standard input

    while (!cin.eof()) {  // It's clear in the while loop that for each line of input, the task is divided into two parts
        inputSuccess = false;
        Rparser(line, inputSuccess, NList); //use parser to judge if input is valid
        if (inputSuccess) {  // Only if inputSuccess is sent back, the whole action for circuit would deploy
            NodeListFunc(line, NList);
        }

        cout << "> ";
        getline(cin, line);
    }  // End input loop until EOF.

    // It's very important that when leaving the while loop, the whole NodeList should be deleted, or it would cause memory leak
    NList.deleteAll();
    return 0;
}
