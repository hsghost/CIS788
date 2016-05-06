/*
/
*/

#include "COddTree.h"

COddTree::COddTree()
{
	this->root = new COddTreeNode();
}

COddTree::~COddTree()
{
	this->dfsTree(op_nullOp, op_deleteNode, (void*) NULL);
	this->root = NULL;
}

