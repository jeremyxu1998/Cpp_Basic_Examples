#include <iostream>
#include <string>
using namespace std;

#include "TreeNode.h"
#include "DBentry.h"

//Member functions that assist TreeDB to get or change data

TreeNode::TreeNode (DBentry* _entryPtr) {
	entryPtr = _entryPtr;
	left = NULL; //have to be set or might cause trouble in find
	right = NULL;
}

TreeNode::~TreeNode() {
	delete entryPtr;
}

void TreeNode::setLeft(TreeNode* newLeft) {
	left = newLeft;
}

void TreeNode::setRight(TreeNode* newRight) {
	right = newRight;
}

TreeNode* TreeNode::getLeft() const {
	return left;
}

TreeNode* TreeNode::getRight() const {
	return right;
}

DBentry* TreeNode::getEntry() const {
	return entryPtr;
}