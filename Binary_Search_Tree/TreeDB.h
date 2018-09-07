#ifndef _TREEDB_H
#define _TREEDB_H

#include "TreeNode.h"
#include "DBentry.h"

class TreeDB {

private:
    TreeNode* root;
    int probesCount;
    int activeCount;  // For storing number of active nodes are there in the tree

    // You will need to add additional private functions
    bool insert_bst(DBentry* newEntry, TreeNode* localRoot);
    DBentry* find_bst(string name, TreeNode* localRoot);
    DBentry* find_bst_without_output(string name, TreeNode* localRoot);
    void countActiveNum(TreeNode* localRoot);
    void deleteTreeNode(string name, TreeNode* localRoot, TreeNode* parentNode);
    void getMaxInLeftSubtree(TreeNode* & maxInLeft, TreeNode* & maxInLeftParent, bool & leftMaxIsLeaf);
    void deleteAll(TreeNode* localRoot);

public:
    // The default constructor, creates an empty database.
    TreeDB();

    // The destructor, deletes all the entries in the database.
    ~TreeDB();

    // Inserts the entry pointed to by newEntry into the database. 
    // If an entry with the same key as newEntry's exists 
    // in the database, it returns false. Otherwise, it returns true.
    bool insert(DBentry* newEntry); 

    // Searches the database for an entry with a key equal to name.
    // If the entry is found, a pointer to it is returned.
    // If the entry is not found, the NULL pointer is returned.
    // Also sets probesCount
    DBentry* find(string name);

    // Deletes the entry with the specified name (key)  from the database.
    // If the entry was indeed in the database, it returns true.
    // Returns false otherwise.
    // See section 6 of the lab handout for the *required* removal method.
    // If you do not use that removal method (replace deleted node by
    // maximum node in the left subtree when the deleted node has two children)
    // You will not match exercise's output.
    bool remove(string name);

    // Deletes all the entries in the database.
    void clear();

    // Prints the number of probes stored in probesCount
    void printProbes() const;

    // Computes and prints out the total number of active entries
    // in the database (i.e. entries with active==true).
    void countActive () const;

    // Prints the entire tree, in ascending order of key/name
    friend ostream& operator<< (ostream& out, const TreeDB& rhs);
}; 

// You *may* choose to implement the function below to help print the 
// tree.  You do not have to implement this function if you do not wish to.
ostream& operator<< (ostream& out, TreeNode* rhs);
   
#endif
