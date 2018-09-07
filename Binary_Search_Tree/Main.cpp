#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

#include "TreeDB.h"
#include "TreeNode.h"
#include "DBentry.h"

/*
 * This is a program that can store websites and their IP address in the sturcture of binary search tree
 * It can also update the address status: active or inactive
 */

int main() {
    string line, command;
    TreeDB myTree;
    string name;
    unsigned int IPaddress;
    string status;

    cout << "> ";
    getline(cin, line);  // Get a line from standard input

    while (!cin.eof()) {
        stringstream lineStream(line);
        lineStream >> command;
        // Parser part is easy, so put them in the main function
        if (command == "insert") {
            lineStream >> name >> IPaddress >> status;
            bool statusBool;
            bool insertSuccess;  // Input status is string, but the input for insert is bool
            if (status == "active") statusBool = true;
            else if (status == "inactive") statusBool = false;
            DBentry* newEntry = new DBentry(name, IPaddress, statusBool);
            insertSuccess = myTree.insert(newEntry);
            if (!insertSuccess) delete newEntry;  // Delete the created entry if insert fail(have same name), or it'll be memory leak
        }
        else if (command == "find") {
            lineStream >> name;
            DBentry* tempEntry = myTree.find(name);
            if (tempEntry != NULL) operator<<(cout, *tempEntry);  // If found, print it
        }
        else if (command == "printall") {
            operator<<(cout, myTree);  // Call the overload << operator and pass cout as input
        }
        else if (command == "printprobes") {
            lineStream >> name;
            if (myTree.find(name)) {
                myTree.printProbes();
            }
        }
        else if (command == "countactive") {
            myTree.countActive();
        }
        else if (command == "updatestatus") {
            lineStream >> name >> status;
            // Because there is no public function for update status, I had to use find to update the number
            DBentry* targetEntry = myTree.find(name);
            if (targetEntry != NULL) {
                if (status == "active") targetEntry->setActive(true);
                else targetEntry->setActive(false);
                cout << "Success" << endl;
                myTree.find(name);	// Update activeCount after status changed
            }
        }
        else if (command == "remove") {
            lineStream >> name;
            myTree.remove(name);
            cout << "Success" << endl;
        }
        else if (command == "removeall") {
            myTree.clear();
            cout << "Success" << endl;
        }
        else {
            cout << "Error: invalid command" << endl;
            lineStream.ignore(1000, '\n');
        }

        cout << "> ";
        getline(cin, line);
    }  // End input loop until EOF.

    myTree.clear();  // After everything is done, delete the whole tree
    return 0;
}
