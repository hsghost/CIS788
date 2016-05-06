/*
/
*/

#ifndef __COddTree_H__
#define __COddTree_H__

#include "COddTreeNode.h"

class COddTree
{
	friend class COddTreeBuilder;
	friend class COddTreeParser;

public:
	COddTree();
	virtual ~COddTree();

private:
	COddTreeNode *root;
	bool directionCount;

public:
	template <class T> void dfsNode(COddTreeNode *current, nodeVisitingOperations preOperation, nodeVisitingOperations postOperation, T *result)
	{
		if (current->numChildren != 0)
		{
			current->visitNode(preOperation, result);
			for (int i = 0; i < current->numChildren; i++)
				COddTree::dfsNode(current->children[i], preOperation, postOperation, result);
			current->visitNode(postOperation, result);
		}
		else
		{
			current->visitNode(preOperation, result);
			current->visitNode(postOperation, result);
		}
	}

	template <class T> void dfsTree(nodeVisitingOperations preOperation, nodeVisitingOperations postOperation, T *result)
	{
		COddTree::dfsNode(this->root, preOperation, postOperation, result);
	}
};

#endif
