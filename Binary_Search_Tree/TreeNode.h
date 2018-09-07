#ifndef _TREENODE_H
#define _TREENODE_H

#include "DBentry.h"

class TreeNode {
private:
    DBentry* entryPtr;
    TreeNode* left;
    TreeNode* right;

public:
    // A useful constructor
    TreeNode(DBentry* _entryPtr);

    // The destructor
    ~TreeNode();

    // Sets the left child of the TreeNode
    void setLeft(TreeNode* newLeft);

    // Sets the right child of the TreeNode
    void setRight(TreeNode* newRight);

    // Gets the left child of the TreeNode
    TreeNode* getLeft() const;

    // Gets the right child of the TreeNode
    TreeNode* getRight() const;

    // Returns a pointer to the DBentry the TreeNode contains
    DBentry* getEntry() const;
};

#endif
