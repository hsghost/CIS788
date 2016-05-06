/*
/
*/

#include <deque>
#include <list>
#include <tuple>
#include "CModulelist.h"

#ifndef __CSlicingTree_H__
#define __CSlicingTree_H__

enum SlicingTreeExpressionElementType
{
	exp_Module,
	exp_HorizontalCut,
	exp_VerticalCut,
};

class CCornerpoint
{
	friend class CSlicingTreeNode;
	friend class CSlicingTree;

	enum cornerpointVisitingOperations
	{
		op_nullOp,
		op_writeOptimalShape
	};

public:
	CCornerpoint(double x, double y, CSlicingTreeNode *onNode);
	CCornerpoint(std::tuple<double, double> *cornerpoint, CSlicingTreeNode *onNode);
	~CCornerpoint();

private:
	std::tuple<double, double> *cornerpoint;
	CCornerpoint *leftParent, *rightParent;
	CSlicingTreeNode *onNode;
	double area;
	double ratio;

public:
	double getArea();
	double getRatio();
	int writeOptimalShape();

	template <class T> int visitCornerpoint(cornerpointVisitingOperations operation, T *result)
	{
		switch (operation)
		{
		case CCornerpoint::op_nullOp:
			return (0);
			break;
		case CCornerpoint::op_writeOptimalShape:
			return (this->writeOptimalShape());
			break;
		default:
			return (1);
			break;
		}
	}

	template <class T> static int dfsCorenrpoint(CCornerpoint *currentCornerpoint, cornerpointVisitingOperations preOperation, cornerpointVisitingOperations postOperation, T *result)
	{
		int errorCode = 0;

		if (currentCornerpoint->onNode->isLeaf)
		{
			errorCode = currentCornerpoint->visitCornerpoint(preOperation, result) + currentCornerpoint->visitCornerpoint(postOperation, result);
		}
		else
		{
			errorCode = currentCornerpoint->visitCornerpoint(preOperation, result);
			errorCode += CCornerpoint::dfsCorenrpoint(currentCornerpoint->leftParent, preOperation, postOperation, result);
			errorCode += CCornerpoint::dfsCorenrpoint(currentCornerpoint->rightParent, preOperation, postOperation, result);
			errorCode += currentCornerpoint->visitCornerpoint(postOperation, result);
		}

		return (errorCode);
	}
};

class CSlicingTreeNode
{
	friend class CCornerpoint;
	friend class CSlicingTree;

	enum nodeVisitingOperations
	{
		op_nullOp,
		op_generateCornerpoints,
	};

	enum SlicingTreeNodeType
	{
		Cut,
		Module
	};

	enum SlicingTreeNodeCutType
	{
		Nonapplicable,
		HorizontalCut,
		VerticalCut
	};

public:
	CSlicingTreeNode(SlicingTreeNodeCutType cutType);
	CSlicingTreeNode(CModule *module);
	~CSlicingTreeNode();

private:
	SlicingTreeNodeType type;
	SlicingTreeNodeCutType cutType;
	const CModule *module;	// NULL if node is a cut
	std::list<CCornerpoint*> cornerpoints;
	bool regenerateCornerpoints;
	bool isLeaf;
	bool isLeftChild;
	CSlicingTreeNode *parent, *leftChild, *rightChild;

	int clearCornerpoints();

public:
	bool getIsLeftChild();
	CSlicingTreeNode *getParent(), *getLeftChild(), *getRightChild();
	int complementCutType();
	int generateCornerpoints();

	template <class T> int visitNode(nodeVisitingOperations operation, T *result)
	{
		switch (operation)
		{
		case CSlicingTreeNode::op_nullOp:
			return (0);
			break;
		case CSlicingTreeNode::op_generateCornerpoints:
			return (this->generateCornerpoints());
			break;
		default:
			return (1);
			break;
		}
	}

	template <class T> static int dfsNode(CSlicingTreeNode *currentNode, nodeVisitingOperations preOperation, nodeVisitingOperations postOperation, T *result)
	{
		int errorCode = 0;

		if (currentNode->isLeaf)
		{
			errorCode = currentNode->visitNode(preOperation, result) + currentNode->visitNode(postOperation, result);
		}
		else
		{
			errorCode = currentNode->visitNode(preOperation, result);
			errorCode += CSlicingTreeNode::dfsNode(currentNode->leftChild, preOperation, postOperation, result);
			errorCode += CSlicingTreeNode::dfsNode(currentNode->rightChild, preOperation, postOperation, result);
			errorCode += currentNode->visitNode(postOperation, result);
		}

		return (errorCode);
	}
};

class CPostfixExpressionElement
{
public:
	CPostfixExpressionElement(SlicingTreeExpressionElementType elementType, CSlicingTreeNode *correspondingTreeNode);
	~CPostfixExpressionElement();

private:
	SlicingTreeExpressionElementType elementType;
	CSlicingTreeNode *correspondingTreeNode;

public:
	SlicingTreeExpressionElementType getElementType();
	void setElementType(SlicingTreeExpressionElementType type);
	CSlicingTreeNode *getCorrespondingTreeNode();
	static int swapElements(CPostfixExpressionElement *leftElement, CPostfixExpressionElement *rightElement);
};

class CSlicingTree
{
public:
	CSlicingTree(CModulelist *modulelist);
	~CSlicingTree();

private:
	CSlicingTreeNode *root;
	unsigned numNodes;
	std::deque<CPostfixExpressionElement> postfixExp;
	double p, q;
	CCornerpoint *minAreaCornerpoint;
	double minArea;

	int _swapExpressionElements(CPostfixExpressionElement *leftElement, CPostfixExpressionElement *rightElement);
	int _swapNodes(CSlicingTreeNode *leftNode, CSlicingTreeNode *rightNode);
	int _complementCuts(CSlicingTreeNode *startNode, CSlicingTreeNode *endNode);

public:
	unsigned getNumNodes();
	CCornerpoint *getMinAreaCornerpoint();
	double getMinArea();
	int swapNodes(int movingPoint, bool isMove1);
	int complementCuts(int movingPoint);
	int calculateMinArea();

	template <class T> int dfsTree(CSlicingTreeNode::nodeVisitingOperations preOperation, CSlicingTreeNode::nodeVisitingOperations postOpertation, T *result)
	{
		return (CSlicingTreeNode::dfsNode(this->root, preOperation, postOpertation, result));
	}
};

// Todo: Temp, AllocTemp, FreeTemp

#endif
