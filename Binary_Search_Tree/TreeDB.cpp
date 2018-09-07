#include <iostream>
#include <string>
using namespace std;

#include "TreeDB.h"
#include "TreeNode.h"
#include "DBentry.h"

/*
 * Core part of this program, contains all functions that implement commands
 * Basic idea is: since we are not allowed to add public functions, put recursive methods as private,
 * and let public members to call them
 */

TreeDB::TreeDB() {
    root = NULL;
    probesCount = 0;
    activeCount = 0;
}

// The destructor, deletes all the entries in the database.
TreeDB::~TreeDB() {
    // If the tree is empty, nothing to do
    if (root == NULL) return;
    // Else there is at least one node, clear them all
    clear();
}

bool TreeDB::insert(DBentry* newEntry) {
    // Inserting the first Node (tree is empty)
    if (root == NULL) {
        TreeNode* newNode = new TreeNode(newEntry);
        root = newNode;
        if (newEntry->getActive())
            activeCount++;
        cout << "Success" << endl;
        return true;
    }

    // This is for updating the activeCount, if the new node is active
    if (find_bst_without_output(newEntry->getName(), root) == NULL) {
        if (newEntry->getActive())
            activeCount++;
    }

    // Do the insertion
    return insert_bst(newEntry, root);
}

// Searches the database for an entry with a key equal to name.
// If the entry is found, a pointer to it is returned.
// If the entry is not found, the NULL pointer is returned.
// Also sets probesCount
DBentry* TreeDB::find(string name) {
    probesCount = 0;
    // List the empty tree or there will be seg fault
    if (root == NULL) {
        cout << "Error: entry does not exist" << endl;
        return NULL;
    }
    // Call the recursive member function
    DBentry* targetEntry = find_bst(name, root);
    
    // Every time find command is done, update active number (for updatestatus command)
    activeCount = 0;
    countActiveNum(root);
    return targetEntry;
}

// Deletes the entry with the specified name (key)  from the database.
// If the entry was indeed in the database, it returns true.
// Returns false otherwise.
// See section 6 of the lab handout for the *required* removal method.
// If you do not use that removal method (replace deleted node by maximum node in the left subtree when the deleted node has two children),
// you will not match exercise's output.
bool TreeDB::remove(string name) {
    DBentry* targetEntry = find(name);
    // Entry not found
    if (targetEntry == NULL) return false;
    // Update activeCount
    if (targetEntry->getActive()) activeCount--;
    
    // This is for deleting the target
    deleteTreeNode(name, root, NULL);
    return true;
}

// Deletes all the entries in the database.
void TreeDB::clear() {
    if (root == NULL) return;
    
    // Call the recursive method
    deleteAll(root);
    root = NULL;
    activeCount = 0;
}

// Prints the number of probes stored in probesCount
void TreeDB::printProbes() const {
    cout << probesCount << endl;
}

// Since there is const here, I am not sure why I cannot call other private function of this class here
// So I come up with the idea of storing the activeCount as private data member
// So this method only serve as printing
void TreeDB::countActive() const {
    cout << activeCount << endl;
}

// Prints the entire tree, in ascending order of key/name
// Again, I cannot call other private function of this class here
// So use this to call the other public overload method
ostream& operator<< (ostream& out, const TreeDB& rhs) {
    operator<<(out, rhs.root);
    return (out);
}

ostream& operator<< (ostream& out, TreeNode* rhs) {
    if (rhs != NULL) {
        // Tranverse in order to print all entries
        operator<<(out, rhs->getLeft());
        operator<<(out, *(rhs->getEntry()));  // Calling the overload operator in DBentry
        operator<<(out, rhs->getRight());
    }
    return out;
}

/* *******Dividing line between public and private mumber functions******* */

bool TreeDB::insert_bst(DBentry* newEntry, TreeNode* localRoot) {
    // Key of localRoot is equal to new entry
    if (newEntry->getName() == localRoot->getEntry()->getName()) {
        cout << "Error: entry already exists" << endl;
        return false;
    }

    // Insert at left
    if (newEntry->getName() < localRoot->getEntry()->getName()) {
        if (localRoot->getLeft() == NULL) {
            TreeNode* newNode = new TreeNode(newEntry);
            localRoot->setLeft(newNode);
            cout << "Success" << endl;
            return true;
        }
        else return insert_bst(newEntry, localRoot->getLeft()); 
    }

    // Insert at right
    if (newEntry->getName() > localRoot->getEntry()->getName()) {
        if (localRoot->getRight() == NULL) {
            TreeNode* newNode = new TreeNode(newEntry);
            localRoot->setRight(newNode);
            cout << "Success" << endl;
            return true;
        }
        else return insert_bst(newEntry, localRoot->getRight());
    }

    cerr << "There's something I haven't considered about!!" << endl;
    return false;
}

DBentry* TreeDB::find_bst(string name, TreeNode* localRoot) {
    probesCount++;  // reaching a new node, update the probe count

    // Base case, found at localRoot
    if (name == localRoot->getEntry()->getName()) {
        // operator<<(cout, *(localRoot->getEntry()));  // Not printing it here because updatestatus also needs to call find
        return localRoot->getEntry();
    }

    // Search left
    if (name < localRoot->getEntry()->getName()) {
        if (localRoot->getLeft() == NULL) {
            cout << "Error: entry does not exist" << endl;
            return NULL;
        }
        else return find_bst(name, localRoot->getLeft());
    }

    // Search right
    if (name > localRoot->getEntry()->getName()) {
        if (localRoot->getRight() == NULL) {
            cout << "Error: entry does not exist" << endl;
            return NULL;
        }
        else return find_bst(name, localRoot->getRight());
    }

    cerr << "There's something I haven't considered about!!" << endl;
    return NULL;
}

DBentry* TreeDB::find_bst_without_output(string name, TreeNode* localRoot) { //similar as above, but without error output
    // Base case, found at localRoot
    if (name == localRoot->getEntry()->getName()) {
        return localRoot->getEntry();
    }

    // Search left
    if (name < localRoot->getEntry()->getName()) {
        if (localRoot->getLeft() == NULL) {
            return NULL;
        }
        else return find_bst_without_output(name, localRoot->getLeft());
    }

    // Search right
    if (name > localRoot->getEntry()->getName()) {
        if (localRoot->getRight() == NULL) {
            return NULL;
        }
        else return find_bst_without_output(name, localRoot->getRight());
    }

    cerr << "There's something I haven't considered about!!" << endl;
    return NULL;
}

// Used for updating countActive after status change
void TreeDB::countActiveNum(TreeNode* localRoot) {
    if (localRoot != NULL) {
        countActiveNum(localRoot->getLeft());
        if (localRoot->getEntry()->getActive()) activeCount++;
        countActiveNum(localRoot->getRight());
    }
}

void TreeDB::deleteTreeNode(string name, TreeNode* localRoot, TreeNode* parentNode) {
    if (localRoot->getEntry()->getName() == name) {  // Found target root

        // Leaf
        if (localRoot->getLeft() == NULL && localRoot->getRight() == NULL) {
            if (parentNode == NULL) {  // Deleting root node
                delete localRoot;
                root = NULL;
            }
            else if (parentNode->getEntry()->getName() > localRoot->getEntry()->getName()) {  // Target Node is left to parent
                delete localRoot;
                parentNode->setLeft(NULL);
            }
            else if (parentNode->getEntry()->getName() < localRoot->getEntry()->getName()) {  // Target Node is right to parent
                delete localRoot;
                parentNode->setRight(NULL);
            }
        }

        // Left subtree only
        else if (localRoot->getLeft() != NULL && localRoot->getRight() == NULL) {
            if (parentNode == NULL) {  // Deleting root node
                root = localRoot->getLeft();
                delete localRoot;
            }
            else if (parentNode->getEntry()->getName() > localRoot->getEntry()->getName()) {  // Target Node is left to parent
                parentNode->setLeft(localRoot->getLeft());
                delete localRoot;
            }
            else if (parentNode->getEntry()->getName() < localRoot->getEntry()->getName()) {  // Target Node is right to parent
                parentNode->setRight(localRoot->getLeft());
                delete localRoot;
            }
        }

        // Right subtree only
        else if (localRoot->getLeft() == NULL && localRoot->getRight() != NULL) {
            if (parentNode == NULL) {  //deleting root node
                root = localRoot->getRight();
                delete localRoot;
            }
            else if (parentNode->getEntry()->getName() > localRoot->getEntry()->getName()) {  // Target Node is left to parent
                parentNode->setLeft(localRoot->getRight());
                delete localRoot;
            }
            else if (parentNode->getEntry()->getName() < localRoot->getEntry()->getName()) {  // Target Node is right to parent
                parentNode->setRight(localRoot->getRight());
                delete localRoot;
            }
        }

        // Have both subtrees, most tricky part!
        else if (localRoot->getLeft() != NULL && localRoot->getRight() != NULL) {
            TreeNode* maxInLeft = localRoot->getLeft();
            TreeNode* maxInLeftParent = localRoot;
            bool leftMaxIsLeaf;  // For considering if leftMax has left child
            // Getting the max in the left most tree, as well as its parent
            getMaxInLeftSubtree(maxInLeft, maxInLeftParent, leftMaxIsLeaf);

            // Only one node for left subtree of localRoot, i.e. maxInLeft didn't move
            if (maxInLeft == localRoot->getLeft() && parentNode == NULL) {
                if (parentNode == NULL && maxInLeftParent == localRoot) {
                    maxInLeft->setRight(localRoot->getRight());
                    root = maxInLeft;
                    delete localRoot;
                }
                else {  // Deleting root node
                    if (!leftMaxIsLeaf) {
                        maxInLeftParent->setRight(maxInLeft->getLeft());
                    }
                    maxInLeft->setLeft(localRoot->getLeft());
                    maxInLeft->setRight(localRoot->getRight());
                    root = maxInLeft;
                    delete localRoot;
                }
            }
            // Only one node for left subtree of localRoot, but not deleting root
            else if (maxInLeft == localRoot->getLeft() && parentNode != NULL) {
                maxInLeft->setRight(localRoot->getRight());
                if (parentNode->getEntry()->getName() > localRoot->getEntry()->getName()) {
                    parentNode->setLeft(maxInLeft);
                }
                else {
                    parentNode->setRight(maxInLeft);
                }
                delete localRoot;
            }
            // The left max position has changed, leftMaxParent will smaller than leftMax
            else {
                if (leftMaxIsLeaf) {
                    maxInLeftParent->setRight(NULL);  // Set the right of parent to null
                    maxInLeft->setLeft(localRoot->getLeft());  // Replacing the localroot(to be deleted) with leftMax
                    maxInLeft->setRight(localRoot->getRight());
                    if (parentNode->getEntry()->getName() > localRoot->getEntry()->getName()) {  // Still have to consider if localRoot is left or right child of its parent
                        parentNode->setLeft(maxInLeft);
                    }
                    else {
                        parentNode->setRight(maxInLeft);
                    }
                }
                else {
                    maxInLeftParent->setRight(maxInLeft->getLeft());  // Connect the child of left max to leftMaxParent
                    maxInLeft->setLeft(localRoot->getLeft());
                    maxInLeft->setRight(localRoot->getRight());
                    if (parentNode->getEntry()->getName() > localRoot->getEntry()->getName()) {
                        parentNode->setLeft(maxInLeft);
                    }
                    else {
                        parentNode->setRight(maxInLeft);
                    }
                }
                delete localRoot;
            }
        }
        else cerr << "There's something I haven't considered about!" << endl;
    }
    // Not the root we want to delete
    else if (localRoot->getEntry()->getName() > name) {  // Search left subtree
        deleteTreeNode(name, localRoot->getLeft(), localRoot);
    }
    else if (localRoot->getEntry()->getName() < name) {  // Search right subtree
        deleteTreeNode(name, localRoot->getRight(), localRoot);
    }
    else cerr << "There's something I haven't considered about!!" << endl;
}

void TreeDB::getMaxInLeftSubtree(TreeNode* & maxInLeft, TreeNode* & maxInLeftParent, bool & leftMaxIsLeaf) {
    if (maxInLeft->getRight() == NULL) {
        // Found, decide if it has a left child
        if (maxInLeft->getLeft() == NULL) leftMaxIsLeaf = true;
        else leftMaxIsLeaf = false;
        return;
    }
    else {
        maxInLeftParent = maxInLeft;
        maxInLeft = maxInLeft->getRight();
        // Call it recursively
        getMaxInLeftSubtree(maxInLeft, maxInLeftParent, leftMaxIsLeaf);
    }
}

void TreeDB::deleteAll(TreeNode* localRoot) {
    // Delete all recursively
    if (localRoot != NULL) {
        deleteAll(localRoot->getLeft());
        deleteAll(localRoot->getRight());
        delete localRoot;
    }
}
