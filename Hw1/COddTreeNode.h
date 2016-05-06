/*
/
*/

#ifndef __COddTreeNode_H__
#define __COddTreeNode_H__

#include <string>
#include "CNetlist.h"

#define MAX_CLAUSE 5

enum halfType_t
{
	h_Real,
	h_Psuedo
};

enum triType_t
{
	Real,
	BottomHalf,
	TopHalf,
	Psuedo,
	Undetermined
};

enum nodeDirection_t
{
	Fowarding,
	Reversed
};

enum nodeVisitingOperations
{
	op_nullOp,
	op_calculateTriType,
	op_getSequence,
	op_deleteNode,
	op_calculateDirection,
	op_calculateNet,
	op_getNetInfo
};

class COddTreeNode
{
	friend class COddTree;
	friend class COddTreeBuilder;
	friend class COddTreeParser;

public:
	COddTreeNode();
	~COddTreeNode();

private:
	halfType_t topHalf;
	halfType_t bottomHalf;
	triType_t triType;
	char symbol;
	COddTreeNode *parent;
	COddTreeNode *children[MAX_CLAUSE];
	int numChildren;
	int siblingIndex;
	bool reversed;
	int drain;
	int source;
	nodeDirection_t nodeDirection;

public:
	// triType_t getTriType();
	char getSymbol();
	void deleteNode();
	void COddTreeNode::calculateTriType();
	void sortChildren();
	void flipChildren();
	void calculateDriection();
	void calculateNet(int *netCount);
	void getNetInfo(CNetlistElement *netlistElement);

	template <class T> void visitNode(nodeVisitingOperations operation, T *result)
	{
		switch (operation)
		{
		case op_nullOp:
			break;
		case op_calculateTriType:
			this->calculateTriType();
			break;
		case op_deleteNode:
			this->deleteNode();
			break;
		case op_getSequence:
			if (this->numChildren == 0)
				*((std::string*) result) += this->getSymbol();
			break;
		case op_calculateDirection:
			this->calculateDriection();
			break;
		case op_calculateNet:
			this->calculateNet((int*) result);
			break;
		case op_getNetInfo:
			if (this->numChildren == 0 && this->symbol >= 'A' && this->symbol <= 'Z')
			{
				CNetlistElement *newElement = new CNetlistElement();
				this->getNetInfo(newElement);
				((CNetlist *)result)->pushQueue(newElement);
			}
			break;
		default:
			break;
		}
	}

};

#endif