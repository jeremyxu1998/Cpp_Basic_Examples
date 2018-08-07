#include <iostream>
#include <string>
using namespace std;

#include "TreeDB.h"
#include "TreeNode.h"
#include "DBentry.h"

/* core part of this program, contains all functions that implement commands
 * Basic idea is: since we are not allowed to add public functions, put recursive methods as private
 * and let public members to call them
 */

TreeDB::TreeDB() {
	root = NULL;
	probesCount = 0;
	activeCount = 0;
}

// the destructor, deletes all the entries in the database.
TreeDB::~TreeDB() {
	// if the tree is empty, nothing to do
	if (root == NULL) return;
	// else there is at least one node, clear them all
	clear();
}

bool TreeDB::insert(DBentry* newEntry) {
	//inserting the first Node (tree is empty)
	if (root == NULL) {
		TreeNode* newNode = new TreeNode(newEntry);
		root = newNode;
		if (newEntry->getActive()) activeCount++;
		cout << "Success" << endl;
		return true;
	}
	
	//this is for updating the activeCount, if the new node is active
	if (find_bst_without_output(newEntry->getName(), root) == NULL) {
		if (newEntry->getActive()) activeCount++;
	}
	
	//do the insertion
	return insert_bst(newEntry, root);
}

// searches the database for an entry with a key equal to name.
// If the entry is found, a pointer to it is returned.
// If the entry is not found, the NULL pointer is returned.
// Also sets probesCount
DBentry* TreeDB::find(string name) {
	probesCount = 0;
	//list the empty tree or there will be seg fault
	if (root == NULL) {
		cout << "Error: entry does not exist" << endl;
		return NULL;
	}
	//call the recursive member function
	DBentry* targetEntry = find_bst(name, root);
	
	//every time find command is done, update active number (for updatestatus command)
	activeCount = 0;
	countActiveNum(root);
	return targetEntry;
}

// deletes the entry with the specified name (key)  from the database.
// If the entry was indeed in the database, it returns true.
// Returns false otherwise.
// See section 6 of the lab handout for the *required* removal method.
// If you do not use that removal method (replace deleted node by
// maximum node in the left subtree when the deleted node has two children)
// you will not match exercise's output.
bool TreeDB::remove(string name) {
	DBentry* targetEntry = find(name);
	//Entry not found
	if (targetEntry == NULL) return false;
	//update activeCount
	if (targetEntry->getActive()) activeCount--;
	
	//this is for deleting the target
	deleteTreeNode(name, root, NULL);
}

// deletes all the entries in the database.
void TreeDB::clear() {
	if (root == NULL) return;
	
	//call the recursive method
	deleteAll(root);
	root = NULL;
	activeCount = 0;
}

// prints the number of probes stored in probesCount
void TreeDB::printProbes() const {
	cout << probesCount << endl;
}

//since there is const here, I am not sure why I cannot call other private function of this class here
//So I come up with the idea of storing the activeCount as private data member
//So this method only serve as printing
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
		//Tranverse in order to print all entries
		operator<<(out, rhs->getLeft());
		operator<<(out, *(rhs->getEntry()));  //calling the overload operator in DBentry
		operator<<(out, rhs->getRight());
	}
	return out;
}


//********Dividing line between public and private mumber functions********

bool TreeDB::insert_bst(DBentry* newEntry, TreeNode* localRoot) {
	//Key of localRoot is equal to new entry
	if (newEntry->getName() == localRoot->getEntry()->getName()) {
		cout << "Error: entry already exists" << endl;
		return false;
	}

	//insert at left
	if (newEntry->getName() < localRoot->getEntry()->getName()) {
		if (localRoot->getLeft() == NULL) {
			TreeNode* newNode = new TreeNode(newEntry);
			localRoot->setLeft(newNode);
			cout << "Success" << endl;
			return true;
		}
		else return insert_bst(newEntry, localRoot->getLeft()); 
		//"else return"!!! not "else", or the return boolean with be incorrect and cause error
	}

	//insert at right
	if (newEntry->getName() > localRoot->getEntry()->getName()) {
		if (localRoot->getRight() == NULL) {
			TreeNode* newNode = new TreeNode(newEntry);
			localRoot->setRight(newNode);
			cout << "Success" << endl;
			return true;
		}
		else return insert_bst(newEntry, localRoot->getRight());
	}
}

DBentry* TreeDB::find_bst(string name, TreeNode* localRoot) {
	probesCount++;  //reaching a new node, update the probe count

	//base case, found at localRoot
	if (name == localRoot->getEntry()->getName()) {
//		operator<<(cout, *(localRoot->getEntry()));  //not printing it here because updatestatus also needs to call find
		return localRoot->getEntry();
	}

	//search left
	if (name < localRoot->getEntry()->getName()) {
		if (localRoot->getLeft() == NULL) {
			cout << "Error: entry does not exist" << endl;
			return NULL;
		}
		else return find_bst(name, localRoot->getLeft());
	}

	//search right
	if (name > localRoot->getEntry()->getName()) {
		if (localRoot->getRight() == NULL) {
			cout << "Error: entry does not exist" << endl;
			return NULL;
		}
		else return find_bst(name, localRoot->getRight());
	}
}

DBentry* TreeDB::find_bst_without_output(string name, TreeNode* localRoot) { //similar as above, but without error output
	//base case, found at localRoot
	if (name == localRoot->getEntry()->getName()) {
		return localRoot->getEntry();
	}

	//search left
	if (name < localRoot->getEntry()->getName()) {
		if (localRoot->getLeft() == NULL) {
			return NULL;
		}
		else return find_bst_without_output(name, localRoot->getLeft());
	}

	//search right
	if (name > localRoot->getEntry()->getName()) {
		if (localRoot->getRight() == NULL) {
			return NULL;
		}
		else return find_bst_without_output(name, localRoot->getRight());
	}
}

//used for updating countActive after status change
void TreeDB::countActiveNum(TreeNode* localRoot) {
	if (localRoot != NULL) {
		countActiveNum(localRoot->getLeft());
		if (localRoot->getEntry()->getActive()) activeCount++;
		countActiveNum(localRoot->getRight());
	}
}

void TreeDB::deleteTreeNode(string name, TreeNode* localRoot, TreeNode* parentNode) {
	if (localRoot->getEntry()->getName() == name) {  //found target root
		
		//leaf
		if (localRoot->getLeft() == NULL && localRoot->getRight() == NULL) {
			if (parentNode == NULL) {  //deleting root node
				delete localRoot;
				root = NULL;
			}
			else if (parentNode->getEntry()->getName() > localRoot->getEntry()->getName()) {  //target Node is left to parent
				delete localRoot;
				parentNode->setLeft(NULL);
			}
			else if (parentNode->getEntry()->getName() < localRoot->getEntry()->getName()) {  //target Node is right to parent
				delete localRoot;
				parentNode->setRight(NULL);
			}
			// else cout << "There's something I haven't considered about" << endl;
		}
		
		//left subtree only
		else if (localRoot->getLeft() != NULL && localRoot->getRight() == NULL) {
			if (parentNode == NULL) {  //deleting root node
				root = localRoot->getLeft();
				delete localRoot;
			}
			else if (parentNode->getEntry()->getName() > localRoot->getEntry()->getName()) {  //target Node is left to parent
				parentNode->setLeft(localRoot->getLeft());
				delete localRoot;
			}
			else if (parentNode->getEntry()->getName() < localRoot->getEntry()->getName()) {  //target Node is right to parent
				parentNode->setRight(localRoot->getLeft());
				delete localRoot;
			}
			// else cout << "There's something I haven't considered about" << endl;
		}
		
		//right subtree only
		else if (localRoot->getLeft() == NULL && localRoot->getRight() != NULL) {
			if (parentNode == NULL) {  //deleting root node
				root = localRoot->getRight();
				delete localRoot;
			}
			else if (parentNode->getEntry()->getName() > localRoot->getEntry()->getName()) {  //target Node is left to parent
				parentNode->setLeft(localRoot->getRight());
				delete localRoot;
			}
			else if (parentNode->getEntry()->getName() < localRoot->getEntry()->getName()) {  //target Node is right to parent
				parentNode->setRight(localRoot->getRight());
				delete localRoot;
			}
			// else cout << "There's something I haven't considered about" << endl;
		}
		
		//Have both subtrees, most tricky part!
		else if (localRoot->getLeft() != NULL && localRoot->getRight() != NULL) {
			TreeNode* maxInLeft = localRoot->getLeft();
			TreeNode* maxInLeftParent = localRoot;
			bool leftMaxIsLeaf;  //for considering if leftMax has left child
			// getting the max in the left most tree, as well as its parent
			getMaxInLeftSubtree(maxInLeft, maxInLeftParent, leftMaxIsLeaf);
			
			//only one node for left subtree of localRoot, i.e. maxInLeft didn't move
			if (maxInLeft == localRoot->getLeft() && parentNode == NULL) {
				if (parentNode == NULL && maxInLeftParent == localRoot) {
					maxInLeft->setRight(localRoot->getRight());
					root = maxInLeft;
					delete localRoot;
				}
				else {  //deleting root node
					if (!leftMaxIsLeaf) {
						maxInLeftParent->setRight(maxInLeft->getLeft());
					}
					maxInLeft->setLeft(localRoot->getLeft());
					maxInLeft->setRight(localRoot->getRight());
					root = maxInLeft;
					delete localRoot;
				}
			}
			//only one node for left subtree of localRoot, but not deleting root
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
			//the left max position has changed, leftMaxParent will smaller than leftMax
			else {
				if (leftMaxIsLeaf) {
					maxInLeftParent->setRight(NULL); //set the right of parent to null
					maxInLeft->setLeft(localRoot->getLeft());  //replacing the localroot(to be deleted) with leftMax
					maxInLeft->setRight(localRoot->getRight());
					if (parentNode->getEntry()->getName() > localRoot->getEntry()->getName()) { //still have to consider if localRoot is left or right child of its parent
						parentNode->setLeft(maxInLeft);
					}
					else {
						parentNode->setRight(maxInLeft);
					}
				}
				else {
					maxInLeftParent->setRight(maxInLeft->getLeft());  //connect the child of left max to leftMaxParent
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
		else cout << "There's something I haven't considered about!" << endl;
	}
	//not the root we want to delete
	else if (localRoot->getEntry()->getName() > name) { //search left subtree
		deleteTreeNode(name, localRoot->getLeft(), localRoot);
	}
	else if (localRoot->getEntry()->getName() < name) { //search right subtree
		deleteTreeNode(name, localRoot->getRight(), localRoot);
	}
	else cout << "There's something I haven't considered about!!" << endl;
}

void TreeDB::getMaxInLeftSubtree(TreeNode* & maxInLeft, TreeNode* & maxInLeftParent, bool & leftMaxIsLeaf) {
	if (maxInLeft->getRight() == NULL) {
		//found, decide if it has a left child
		if (maxInLeft->getLeft() == NULL) leftMaxIsLeaf = true;
		else leftMaxIsLeaf = false;
		return;
	}
	else {
		maxInLeftParent = maxInLeft;
		maxInLeft = maxInLeft->getRight();
		//call it recursively
		getMaxInLeftSubtree(maxInLeft, maxInLeftParent, leftMaxIsLeaf);
	}
}

void TreeDB::deleteAll(TreeNode* localRoot) {
	//delete all recursively
	if (localRoot != NULL) {
		deleteAll(localRoot->getLeft());
		deleteAll(localRoot->getRight());
		delete localRoot;
	}
}