#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

/*
 * This is a program for parsing the input you enter
 * By determining the first word in your input (stored in lineStream),
 * It judges what command to do and call the respecting function
 * For more explanation about details, see the comments below
 * Most comments about fail cases are presented in insertR and printNode section,
 * Because they are similar in other section
 */

#define MAX_NODE_NUMBER 5000
#define MIN_NODE_NUMBER 0

int parser();
void insertR(stringstream &lineStream);
void modifyR(stringstream &lineStream);
void printR(stringstream &lineStream);
void printNode(stringstream &lineStream);
void deleteR(stringstream &lineStream);
void peekingSpace(stringstream &lineStream);  // A function for skipping all the spaces
bool peekingCharAndPoint(stringstream &lineStream);  // A function for checking if there is letter or decimal point right after number


int main() {
    parser();
    return 0;
}

int parser() {
    string line, command;
    cout << "> ";
    getline(cin, line);  // Get a line from standard input

    while (!cin.eof()) {
        // Put the line in a stringstream for parsing
        // Making a new stringstream for each line so flags etc. are in a known state
        stringstream lineStream(line);
        lineStream >> command;

        if (command == "insertR") {
            // parse an insertR command
            insertR(lineStream);
        } 
        else if (command == "modifyR") {
            // parse an modifyR command
            modifyR(lineStream);
        } 
        else if (command == "printR") {
            // parse an printR command
            printR(lineStream);
        } 
        else if (command == "printNode") {
            // parse an printNode command
            printNode(lineStream);
        } 
        else if (command == "deleteR") {
            // parse an deleteR command
            deleteR(lineStream);
        } 
        else {
            // command other than the required 6
            lineStream.ignore(1000, '\n');
            cout << "Error: invalid command";
        }

        cout << endl << "> ";
        getline(cin, line);
    }  // End input loop when meet EOF.
    return 0;
}

void insertR(stringstream &lineStream) {
    string name;
    double resistance;
    int nodeidStart, nodeidEnd;
    bool noMoreCharOrPoint = true;

    if (!lineStream.eof()) {  // For case where there is only a command
        lineStream >> name;
        // After each inputting, clear all the spaces after, in case there's nothing next except space
        peekingSpace(lineStream);
        if (!lineStream.fail() && name != "all" && !lineStream.eof()) {
            lineStream >> resistance;
            peekingSpace(lineStream);
            if (!lineStream.fail() && resistance >= 0 && !lineStream.eof()) {
                lineStream >> nodeidStart;
                peekingSpace(lineStream);
                if (!lineStream.fail() 
                        && nodeidStart >= MIN_NODE_NUMBER && nodeidStart <= MAX_NODE_NUMBER && !lineStream.eof()) {
                    lineStream >> nodeidEnd;
                    // After putting the last required stuff into variable, check if there are letters or decimal points next
                    // Return a boolean, false means invalid argument
                    noMoreCharOrPoint = peekingCharAndPoint(lineStream);
                    // Clear all the spaces
                    // Note this should be done after peekChar, in case "... 1 end", too many arguments judged as invalid!!
                    // Similar situation in modifyR and PrintNode
                    peekingSpace(lineStream);
                    if (!lineStream.fail() 
                            && nodeidEnd >= MIN_NODE_NUMBER && nodeidEnd <= MAX_NODE_NUMBER 
                            && nodeidEnd != nodeidStart && lineStream.eof() && noMoreCharOrPoint) {
                        // insertR success
                        cout << fixed << setprecision(2);  // Two digits precise
                        cout << "Inserted: resistor " << name << " " << resistance << " Ohms " 
                            << nodeidStart << " -> " << nodeidEnd;
                    }
                    else if (lineStream.fail() || !noMoreCharOrPoint) {  // invalid e.g: char for int or 1ohm
                        cout << "Error: invalid argument";
                        lineStream.ignore(1000, '\n');
                    }
                    else if (nodeidEnd > MAX_NODE_NUMBER || nodeidEnd < MIN_NODE_NUMBER) {
                        cout << "Error: node " << nodeidEnd << " is out of permitted range " 
                                << MIN_NODE_NUMBER << "-" << MAX_NODE_NUMBER;
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
                    else {  // Just for testing if I've lost something when programming
                        cout << "Error: there's something I haven't considered about";
                        lineStream.ignore(1000, '\n');
                    }
                }
                else if (lineStream.fail()) {  // Invalid argument for nodeidStart
                    cout << "Error: invalid argument";
                    lineStream.ignore(1000, '\n');
                }
                else if (nodeidStart > MAX_NODE_NUMBER || nodeidStart < MIN_NODE_NUMBER) {
                    cout << "Error: node " << nodeidStart << " is out of permitted range " 
                            << MIN_NODE_NUMBER << "-" << MAX_NODE_NUMBER;
                    lineStream.ignore(1000, '\n');
                }
                else if (lineStream.eof()) {  // No more arguments after clearing spaces
                    cout << "Error: too few arguments";
                    lineStream.ignore(1000, '\n');
                }
                else {
                    cout << "Error: there's something I haven't considered about";
                    lineStream.ignore(1000, '\n');
                }
            }
            else if (lineStream.fail()) {  // resistance fail
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
        else if (lineStream.fail()) {  // Name fail
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

void modifyR(stringstream &lineStream) {
    string name;
    double resistance;
    bool noMoreCharOrPoint = true;

    if (!lineStream.eof()) {
        lineStream >> name;
        peekingSpace(lineStream);
        if (!lineStream.fail() && name != "all" && !lineStream.eof()) {
            lineStream >> resistance;
            noMoreCharOrPoint = peekingCharAndPoint(lineStream);  // Similar problems encounterd as insertR
            peekingSpace(lineStream);
            if (!lineStream.fail() && resistance >= 0 && lineStream.eof() && noMoreCharOrPoint) {
                // modifyR success
                cout << fixed << setprecision(2);
                cout << "Modified: resistor " << name << " to " << resistance << " Ohms";
            }
            else if (lineStream.fail() || !noMoreCharOrPoint) {  // resistance fail
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
        else if (lineStream.fail()) {  // name fail
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
        cout << "Error: too few arguments";
    }
    return;
}

void printR(stringstream &lineStream) {
    string name;

    if (!lineStream.eof()) {
        lineStream >> name;
        if (!lineStream.fail() && name == "all" && lineStream.eof()) {
            // printR all resistors success
            cout << "Print: all resistors";
        } 
        else if (!lineStream.fail() && name != "all" && lineStream.eof()) {
            // printR resistor with name success
            cout << "Print: resistor " << name;
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

void printNode(stringstream &lineStream) {
    int nodeid;
    string maybeAll;
    bool noMoreCharOrPoint = true;

    if (!lineStream.eof()) {
        lineStream >> nodeid;  // Try to fit it into int first because numbers also fit for string
        noMoreCharOrPoint = peekingCharAndPoint(lineStream);
        if (!lineStream.fail() && lineStream.eof() && nodeid >= MIN_NODE_NUMBER 
                && nodeid <= MAX_NODE_NUMBER && noMoreCharOrPoint) {
            // print nodeid success
            cout << "Print: node " << nodeid;
        }
        else if (!lineStream.fail()  // separate case judging case like 1st
                && nodeid >= MIN_NODE_NUMBER && nodeid <= MAX_NODE_NUMBER && !noMoreCharOrPoint) {
            cout << "Error: invalid argument";
        }
        else if ((!lineStream.fail() && nodeid > MAX_NODE_NUMBER) 
                || (!lineStream.fail() && nodeid < MIN_NODE_NUMBER)) {
            cout << "Error: node " << nodeid << " is out of permitted range " 
                    << MIN_NODE_NUMBER << "-" << MAX_NODE_NUMBER;
            lineStream.ignore(1000, '\n');
        }
        else if (lineStream.fail()) {
            lineStream.clear();  // Important to reset the flag!!
            lineStream >> maybeAll;  // Then try to fit it into string and see if it is all
            if (!lineStream.fail() && maybeAll == "all" && lineStream.eof()) {
                // Print all nodes success
                cout << "Print: all nodes";
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

void deleteR(stringstream &lineStream) {
    string name;

    if (!lineStream.eof()) {
        lineStream >> name;
        if (!lineStream.fail() && name == "all" && lineStream.eof()) {
            // Delete all resistors success
            cout << "Deleted: all resistors";
        }
        else if (!lineStream.fail() && name != "all" && lineStream.eof()) {
            // Delete resistor with name success
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

void peekingSpace(stringstream &lineStream) {
    // Using while because all spaces has to be cleared
    // Cannot use ignore 1000, because there maybe more arguments after space
    while (!lineStream.eof()) {
        if (lineStream.peek() == 32) {  // ASCII 32 is [space]
            lineStream.ignore(1, '\n');
        }
        else break;
    }
    return;
}

bool peekingCharAndPoint(stringstream &lineStream) {
    char peek;
    if (!lineStream.eof()) {  // just checking the next bit
        peek = lineStream.peek();
        // ASCII 65-90 capital letters, 97-122 low-case letters, 46 decimal point
        if ((peek >= 65 && peek <= 90) || (peek >= 97 && peek <= 122) || peek == 46) {
            return false;
        }
    }
    return true;
}
