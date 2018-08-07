#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

#include "Rparser.h"  //prototypes for functions are put in Rparser.h
#include "Node.h"
#include "Resistor.h"

/*This is a program for parsing the input you put
 * By determining the first word in your input (stored in lineStream)
 * It judges what command to do and call the respecting function
 * For more explanation about details, see the comments below
 * Most comments about fail cases are presented in insertR and printNode section
 * because they are similar in other section
 */

#define MIN_NODE_NUMBER 0

int Rparser(string line, string & command, bool & inputSuccess, Node** & N, Resistor** & R, int & maxNodeNumber, int & curNumRes, int & maxResNumber) {
	// Put the line in a stringstream for parsing
	// Making a new stringstream for each line so flags etc. are in a known state
	stringstream lineStream(line);
	lineStream >> command;

	if (command == "maxVal") {
		// parse an maxVal command
		maxVal(lineStream, inputSuccess, N, R, maxNodeNumber, curNumRes, maxResNumber);
	}
	else if (command == "insertR") {
		// parse an insertR command
		insertR(lineStream, inputSuccess, N, R, maxNodeNumber, curNumRes, maxResNumber);
	}
	else if (command == "modifyR") {
		// parse an modifyR command
		modifyR(lineStream, inputSuccess, R, maxNodeNumber, curNumRes);
	}
	else if (command == "printR") {
		// parse an printR command
		printR(lineStream, inputSuccess, R, maxNodeNumber, curNumRes);
	}
	else if (command == "printNode") {
		// parse an printNode command
		printNode(lineStream, inputSuccess, maxNodeNumber);
	}
	else if (command == "deleteR") {
		// parse an deleteR command
		deleteR(lineStream, inputSuccess);
	}
	else {
		// command other than the required 6
		lineStream.ignore(1000, '\n');
		cout << "Error: invalid command";
		lineStream.ignore(1000, '\n');
	}

    return 0;
}

void maxVal(stringstream &lineStream, bool & inputSuccess, Node** & N, Resistor** & R, int & maxNodeNumber, int & curNumRes, int & maxResNumber) {
	//delete all the current Node and Resistor list first
	if (R != NULL) {
		for (int i = 0; i < curNumRes; i++) {
			R[i] = NULL;
		}
		R = NULL;
		curNumRes = 0;
	}
	
	if (N != NULL) {
		for (int i = 0; i <= maxNodeNumber; i++) {
			N[i] = NULL;
		}
		N = NULL;
	}
	
	lineStream >> maxNodeNumber >> maxResNumber;
	inputSuccess = false;
	if (maxNodeNumber > 0 && maxResNumber > 0) {
		inputSuccess = true;
		cout << "New network: max node number is " << maxNodeNumber << "; max resistors is " << maxResNumber;
	}
	else {
		cout << "Error: maxVal arguments must be greater than 0";
		lineStream.ignore(1000, '\n');
	}
}

void insertR(stringstream &lineStream, bool & inputSuccess, Node** N, Resistor** R, int & maxNodeNumber, int & curNumRes, int & maxResNumber) {
    string name;
    double resistance;
    int nodeidStart, nodeidEnd;
    bool noMoreCharOrPoint = true;
	bool resNameAlrExist = false;
	inputSuccess = false;

    if (!lineStream.eof()) {//for case where there is only a command
        lineStream >> name;
        //after each inputting, clear all the spaces after, in case there's nothing next except space
        peekingSpace(lineStream);
        if (!lineStream.fail() && name != "all" && !lineStream.eof()) {
            lineStream >> resistance;
            peekingSpace(lineStream);
            if (!lineStream.fail() && resistance >= 0 && !lineStream.eof()) {
                lineStream >> nodeidStart >> nodeidEnd;
                peekingSpace(lineStream);
                if (!lineStream.fail() 
                        && nodeidStart >= MIN_NODE_NUMBER && nodeidStart <= maxNodeNumber 
						&& nodeidEnd >= MIN_NODE_NUMBER && nodeidEnd <= maxNodeNumber
						&& lineStream.eof()) {
                    //after putting the last required stuff into variable, check if there are letters or decimal points next
                    //return a boolean, false means invalid argument
                    noMoreCharOrPoint = peekingCharAndPoint(lineStream);
                    //clear all the spaces
                    //note this should be done after peekChar, in case "... 1 end", too many arguments judged as invalid!!
                    //similar situation in modifyR and PrintNode
                    peekingSpace(lineStream);
					for (int i = 0; i < curNumRes; i++) {
						if (R[i]->getName() == name) {
							resNameAlrExist = true;
							break;
						}
					}
                    if (!lineStream.fail() 
                            && nodeidEnd >= MIN_NODE_NUMBER && nodeidEnd <= maxNodeNumber
                            && nodeidEnd != nodeidStart && lineStream.eof() && noMoreCharOrPoint
							&& N[nodeidStart]->canAddResistor(nodeidStart) && N[nodeidEnd]->canAddResistor(nodeidEnd)
							&& curNumRes < maxResNumber && !resNameAlrExist) {//NEW arguments added: canAddresistor, name not exist, and have position in this node
                        //insertR success
                        cout << fixed << setprecision(2);//two digits precise
                        cout << "Inserted: resistor " << name << " " << resistance << " Ohms " 
                                << nodeidStart << " -> " << nodeidEnd;
						inputSuccess = true; //only if pass parsor, let inputSuccess be true, same for later cases
                    } 
					else if (curNumRes >= maxResNumber) {
						cout << "Error: resistor array is full";
						lineStream.ignore(1000, '\n');
					}
					else if (!N[nodeidStart]->canAddResistor(nodeidStart) || !N[nodeidEnd]->canAddResistor(nodeidEnd)) {
						cout << "Error: node is full";
						lineStream.ignore(1000, '\n');
					}
                    else if (lineStream.fail() || !noMoreCharOrPoint) {//invalid e.g: char for int or 1ohm
                        cout << "Error: invalid argument";
                        lineStream.ignore(1000, '\n');
                    } 
					else if (resNameAlrExist) {
						cout << "Error: resistor " << name << " already exists";
						lineStream.ignore(1000, '\n');
					}
                    else if (nodeidEnd == nodeidStart) {
                        cout << "Error: both terminals of resistor connect to node " << nodeidEnd;
                        lineStream.ignore(1000, '\n');
                    } 
                    else if (!lineStream.eof()) {
                        cout << "Error: too many arguments";
                        lineStream.ignore(1000, '\n');
                    } 
                    else {//just for testing if I've lost something when programming
                        cout << "Error: there's something I haven't considered about";
                        lineStream.ignore(1000, '\n');
                    }
                } 
				else if (nodeidEnd > maxNodeNumber || nodeidEnd < MIN_NODE_NUMBER) {
					cout << "Error: node " << nodeidEnd << " is out of permitted range "
						<< MIN_NODE_NUMBER << "-" << maxNodeNumber;
					lineStream.ignore(1000, '\n');
				}
                else if (lineStream.fail()) {//invalid argument for nodeidStart
                    cout << "Error: invalid argument";
                    lineStream.ignore(1000, '\n');
                } 
                else if (nodeidStart > maxNodeNumber || nodeidStart < MIN_NODE_NUMBER) {
                    cout << "Error: node " << nodeidStart << " is out of permitted range " 
                            << MIN_NODE_NUMBER << "-" << maxNodeNumber;
                    lineStream.ignore(1000, '\n');
                }
				/*
                else if (lineStream.eof()) { //no more arguments after clearing spaces
                    cout << "Error: too few arguments";
                    lineStream.ignore(1000, '\n');
                } 
				*/
                else {
                    cout << "Error: there's something I haven't considered about";
                    lineStream.ignore(1000, '\n');
                }
            } 
            else if (lineStream.fail()) {//resistance fail
                cout << "Error: invalid argument";
                lineStream.ignore(1000, '\n');
            } 
            else if (resistance < 0) {
                cout << "Error: negative resistance";
                lineStream.ignore(1000, '\n');
            } 
            else if (lineStream.eof()) {
                cout << "Error: too few arguments";
                lineStream.ignore(1000, '\n');
            } 
            else {
                cout << "Error: there's something I haven't considered about";
                lineStream.ignore(1000, '\n');
            }
        } 
        else if (lineStream.fail()) {//name fail
            cout << "Error: invalid argument";
            lineStream.ignore(1000, '\n');
        } 
        else if (name == "all") {
            cout << "Error: resistor name cannot be the keyword \"all\"";
            lineStream.ignore(1000, '\n');
        } 
        else if (lineStream.eof()) {
            cout << "Error: too few arguments";
            lineStream.ignore(1000, '\n');
        } 
        else {
            cout << "Error: there's something I haven't considered about";
            lineStream.ignore(1000, '\n');
        }
    } 
    else {
        cout << "Error: too few arguments";
    }
    return;
}

void modifyR(stringstream &lineStream, bool & inputSuccess, Resistor** R, int & maxNodeNumber, int & curNumRes) {
    string name;
    double resistance;
    bool noMoreCharOrPoint = true;
	bool gotNameOnList = false;
	inputSuccess = false;

    if (!lineStream.eof()) {
        lineStream >> name;
		for (int i = 0; i < curNumRes; i++) {
			if (R[i]->getName() == name) {
				gotNameOnList = true;
				break;
			}
		}
		if (gotNameOnList) {
			peekingSpace(lineStream);
			if (!lineStream.fail() && name != "all" && !lineStream.eof()) {
				lineStream >> resistance;
				noMoreCharOrPoint = peekingCharAndPoint(lineStream);//similar problems encounterd as insertR
				peekingSpace(lineStream);
				if (!lineStream.fail() && resistance >= 0 && lineStream.eof() && noMoreCharOrPoint) {
					//modifyR success
//					cout << fixed << setprecision(2);
//					cout << "Modified: resistor " << name << " to " << resistance << " Ohms";
					inputSuccess = true;
				}
				else if (lineStream.fail() || !noMoreCharOrPoint) {//resistance fail
					cout << "Error: invalid argument";
					lineStream.ignore(1000, '\n');
				}
				else if (resistance < 0) {
					cout << "Error: negative resistance";
					lineStream.ignore(1000, '\n');
				}
				else if (!lineStream.eof()) {
					cout << "Error: too many arguments";
					lineStream.ignore(1000, '\n');
				}
				else {
					cout << "Error: there's something I haven't considered about";
					lineStream.ignore(1000, '\n');
				}
			}
			else if (lineStream.fail()) {//name fail
				cout << "Error: invalid argument";
				lineStream.ignore(1000, '\n');
			}
			else if (name == "all") {
				cout << "Error: resistor name cannot be the keyword \"all\"";
				lineStream.ignore(1000, '\n');
			}
			else if (lineStream.eof()) {
				cout << "Error: too few arguments";
			}
			else {
				cout << "Error: there's something I haven't considered about";
				lineStream.ignore(1000, '\n');
			}
		}
		else {
			cout << "Error: resistor " << name << " not found";
			lineStream.ignore(1000, '\n');
		}
    } 
    else {
        cout << "Error: too few arguments";
    }
    return;
}

void printR(stringstream &lineStream, bool & inputSuccess, Resistor** R, int & maxNodeNumber, int & curNumRes) {
    string name;
	bool gotNameOnList = false;
	inputSuccess = false;

    if (!lineStream.eof()) {
        lineStream >> name;
		if (name == "all") gotNameOnList = true;
		for (int i = 0; i < curNumRes; i++) {
			if (R[i]->getName() == name) {
				gotNameOnList = true;
				break;
			}
		}
		if (gotNameOnList) {
			if (!lineStream.fail() && name == "all" && lineStream.eof()) {
				//printR all resistors success
				//cout << "Print: all resistors";
				inputSuccess = true;
			}
			else if (!lineStream.fail() && name != "all" && lineStream.eof()) {
				//printR resistor with name success
				//cout << "Print: resistor " << name;
				inputSuccess = true;
			}
			else if (lineStream.fail()) {
				cout << "Error: invalid argument";
				lineStream.ignore(1000, '\n');
			}
			else if (!lineStream.eof()) {
				cout << "Error: too many arguments";
				lineStream.ignore(1000, '\n');
			}
			else {
				cout << "Error: there's something I haven't considered about";
				lineStream.ignore(1000, '\n');
			}
		}
		else {
			cout << "Error: resistor " << name << " not found";
			lineStream.ignore(1000, '\n');
		} 
    } 
    else {
        cout << "Error: too few arguments";
    }
    return;
}

void printNode(stringstream &lineStream, bool & inputSuccess, int & maxNodeNumber) {
    int nodeid;
    string maybeAll;
    bool noMoreCharOrPoint = true;
	inputSuccess = false;

    if (!lineStream.eof()) {
        lineStream >> nodeid;// try to fit it into int first because numbers also fit for string
        noMoreCharOrPoint = peekingCharAndPoint(lineStream);
        if (!lineStream.fail() && lineStream.eof() && nodeid >= MIN_NODE_NUMBER 
                && nodeid <= maxNodeNumber && noMoreCharOrPoint) {
            //print nodeid success
            //cout << "Print: node " << nodeid;
			inputSuccess = true;
        }
        else if (!lineStream.fail() //separate case judging case like 1st
                && nodeid >= MIN_NODE_NUMBER && nodeid <= maxNodeNumber && !noMoreCharOrPoint) {
            cout << "Error: invalid argument";
			lineStream.ignore(1000, '\n');
        }
        else if ((!lineStream.fail() && nodeid > maxNodeNumber) 
                || (!lineStream.fail() && nodeid < MIN_NODE_NUMBER)) {
            cout << "Error: node " << nodeid << " is out of permitted range " 
                    << MIN_NODE_NUMBER << "-" << maxNodeNumber;
            lineStream.ignore(1000, '\n');
        }
        else if (lineStream.fail()){
            lineStream.clear();//important to reset the flag!!
            lineStream >> maybeAll;//then try to fit it into string and see if it is all
            if (!lineStream.fail() && maybeAll == "all" && lineStream.eof()) {
                //print all nodes success
                //cout << "Print: all nodes";
				inputSuccess = true;
            }
            else if ((!lineStream.fail() && maybeAll != "all") || lineStream.fail()) {
                cout << "Error: invalid argument";
                lineStream.ignore(1000, '\n');
            }
            else if (!lineStream.eof()) {
                cout << "Error: too many arguments";
                lineStream.ignore(1000, '\n');
            }
            else {
                cout << "Error: there's something I haven't considered about";
                lineStream.ignore(1000, '\n');
            }
        }
        else if (!lineStream.eof()) {
            cout << "Error: too many arguments";
            lineStream.ignore(1000, '\n');
        }
        else {
            cout << "Error: there's something I haven't considered about";
            lineStream.ignore(1000, '\n');
        }
    }
    else {
        cout << "Error: too few arguments";
    }
    return;
}

void deleteR(stringstream &lineStream, bool & inputSuccess) {
    string name;
	inputSuccess = false;

    if (!lineStream.eof()) {
        lineStream >> name;
        if (!lineStream.fail() && name == "all" && lineStream.eof()) {
            //Delete all resistors success
            cout << "Deleted: all resistors";
			inputSuccess = true;
        } 
        else if (!lineStream.fail() && name != "all" && lineStream.eof()) {
            //Delete resistor with name success
            cout << "Deleted: resistor " << name;
        } 
        else if (lineStream.fail()) {
            cout << "Error: invalid argument";
            lineStream.ignore(1000, '\n');
        } 
        else if (!lineStream.eof()) {
            cout << "Error: too many arguments";
            lineStream.ignore(1000, '\n');
        } 
        else {
            cout << "Error: there's something I haven't considered about";
            lineStream.ignore(1000, '\n');
        }
    } 
    else {
        cout << "Error: too few arguments";
    }
    return;
}

void peekingSpace(stringstream &lineStream){
    //using while because all spaces has to be cleared
    //cannot use ignore 1000, because there maybe more arguments after space
    while (!lineStream.eof()) {
        if (lineStream.peek() == 32){//ASCII 32 is [space]
            lineStream.ignore(1, '\n');
        }
        else break;
    }
    return;
}

bool peekingCharAndPoint(stringstream &lineStream){
    char peek;
    if (!lineStream.eof()) {//just checking the next bit
        peek = lineStream.peek();
        //ASCII 65-90 capital letters, 97-122 low-case letters, 46 decimal point
        if ((peek >= 65 && peek <= 90) || (peek >= 97 && peek <= 122) || peek == 46){
            return false;
        }
    }
    return true;
}