#include "Rparser.h"
#include "Node.h"
#include "Resistor.h"

using namespace std;

/*
 * This program is for resistor placing in node network
 * The structure of this program is slightly confusing, because here the parser part is only responsible for checking if the statement is correct
 * While Node.cpp and Resistor.cpp are responsible for the move for what each command do in Node and resistor arrays, respectively
 * It divides up what a command should do into different functions, which seems easier to figure out what might goes wrong
 */

int Rparser(string line, string & command, bool & inputSuccess, Node** & N, Resistor** & R, int & maxNodeNumber, int & curNumRes, int & maxResNumber);
void NodeArray(string line, Node** & N, int & maxNodeNumber, int & oldMaxNodeNumber);
void ResistorArray(string line, Resistor** & R, int & curNumRes, int & maxResNumber, int & oldNumRes);

int main() {
    string line, command;
    Node** N = NULL;  // double pointer, which points to an array of pointers
    Resistor** R = NULL;
    int maxNodeNumber = 0;
    int maxResNumber = 0;
    int curNumRes = 0;
    int oldMaxNodeNumber = 0;  // storing the previous number of maxNode, in case when new maxNode value is in and previous one is covered
    int oldNumRes = 0;
    bool inputSuccess = false;  // only if input is success, it will call the functions in Node.cpp and Resistor.cpp

    cout << "> ";
    getline(cin, line);  // Get a line from standard input

    while (!cin.eof()) {  // it's clear in the while loop that for each line of input, the task is divided into three parts
        Rparser(line, command, inputSuccess, N, R, maxNodeNumber, curNumRes, maxResNumber);
        if (inputSuccess) {
            NodeArray(line, N, maxNodeNumber, oldMaxNodeNumber);
            ResistorArray(line, R, curNumRes, maxResNumber, oldNumRes);
        }

        cout << endl << "> ";
        getline(cin, line);
    }  // End input loop until EOF.
    // After the last command and an eof is pressed, delete all Nodes and Resistors
    if (N != NULL){
        for (int i = 0; i <= oldMaxNodeNumber; i++) {
            if(N[i] != NULL){
                delete N[i];
                N[i] = NULL;
            }
            else continue;
        }
        delete[] N;
        N = NULL;
    }
    if (R != NULL) {
        for (int i = 0; i < oldNumRes; i++) {
            if(R[i] != NULL){
                delete R[i];
                R[i] = NULL;
            }
            else continue;
        }
        delete[] R;
        R = NULL;
        curNumRes = 0;
    }
    return 0;
}
