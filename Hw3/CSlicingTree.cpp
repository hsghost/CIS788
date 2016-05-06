/*
/
*/

#include <cmath>
#include <algorithm>
#include <sstream>
#include "CSlicingTree.h"

#define PIECE_LENGTH 0.1

CCornerpoint::CCornerpoint(double x, double y, CSlicingTreeNode *onNode):
cornerpoint(&std::make_tuple(x, y)), onNode(onNode), leftParent(NULL), rightParent(NULL), area(x * y), ratio(y / x)
{
	;
}

CCornerpoint::CCornerpoint(std::tuple<double, double> *cornerpoint, CSlicingTreeNode *onNode):
cornerpoint(cornerpoint), onNode(onNode), leftParent(NULL), rightParent(NULL), area(std::get<0>(*cornerpoint) * std::get<1>(*cornerpoint)), ratio(std::get<1>(*cornerpoint) / std::get<0>(*cornerpoint))
{
	;
}

CCornerpoint::~CCornerpoint()
{
	delete this->cornerpoint;
	this->cornerpoint = NULL;
	this->onNode = NULL;
	this->leftParent = NULL;
	this->rightParent = NULL;
}

double CCornerpoint::getArea()
{
	return (this->area);
}

double CCornerpoint::getRatio()
{
	return (this->ratio);
}

int CCornerpoint::writeOptimalShape()
{
	CModule *module = *(CModule * const *) this->onNode->module;
	module->optimalShape = this->cornerpoint;

	return (0);
}

CSlicingTreeNode::CSlicingTreeNode(SlicingTreeNodeCutType cutType):
type(Cut), cutType(cutType), module(NULL), regenerateCornerpoints(true), isLeaf(false), parent(NULL), leftChild(NULL), rightChild(NULL)
{
	;
}

CSlicingTreeNode::CSlicingTreeNode(CModule *module):
type(Module), cutType(Nonapplicable), module(module), regenerateCornerpoints(true), isLeaf(true), parent(NULL), leftChild(NULL), rightChild(NULL)
{
	;
}

CSlicingTreeNode::~CSlicingTreeNode()
{
	delete this->leftChild;
	this->leftChild = NULL;
	delete this->rightChild;
	this->rightChild = NULL;

	this->clearCornerpoints();

	if (this->module)
		this->module = NULL;
}

int CSlicingTreeNode::clearCornerpoints()
{
	for (std::list<CCornerpoint*>::iterator it = this->cornerpoints.begin(); it != this->cornerpoints.end(); ++it)
	{
		delete *it;
		*it = NULL;
	}

	this->cornerpoints.clear();

	return (0);
}

bool CSlicingTreeNode::getIsLeftChild()
{
	return (this->isLeftChild);
}

CSlicingTreeNode *CSlicingTreeNode::getParent()
{
	return (this->parent);
}

CSlicingTreeNode *CSlicingTreeNode::getLeftChild()
{
	return (this->leftChild);
}

CSlicingTreeNode *CSlicingTreeNode::getRightChild()
{
	return (this->rightChild);
}

int CSlicingTreeNode::complementCutType()
{
	if (this->cutType == HorizontalCut)
	{
		this->cutType = VerticalCut;
		this->regenerateCornerpoints = true;
		return (0);
	}
	else if (this->cutType == VerticalCut)
	{
		this->cutType = HorizontalCut;
		this->regenerateCornerpoints = true;
		return (0);
	}
	else return 1;
}

int CSlicingTreeNode::generateCornerpoints()
{
	if (this->regenerateCornerpoints)
	{
		CModule *currentModule;
		std::tuple<double, double> *currentPoint;
		CCornerpoint *currentCornerpoint;
		double a, r, s, r_x, r_y, s_x, s_y;
		std::list<CCornerpoint*>::iterator it, end, lIt, lEnd, rIt, rEnd;

		this->clearCornerpoints();

		if (this->type == Module)
		{
			a = (currentModule = *(CModule * const *) this->module)->getA();
			r = currentModule->getR();
			s = currentModule->getS();
			r_x = std::sqrt(a * r);
			r_y = std::sqrt(a / r);
			this->cornerpoints.push_back(new CCornerpoint(r_x, r_y, this));
			if (r != s)
			{
				s_x = std::sqrt(a * s);
				s_y = std::sqrt(a / s);

				for (double x = 0; x < std::abs(s_x - r_x); x += PIECE_LENGTH)
				{
					this->cornerpoints.push_back(new CCornerpoint(x, a / x, this));
				}
				this->cornerpoints.push_back(new CCornerpoint(s_x, s_y, this));
			}
			if (currentModule->getRotatable())
			{
				end = this->cornerpoints.end();
				for (it = this->cornerpoints.begin(); it != end; ++it)
				{
					this->cornerpoints.push_back(new CCornerpoint(std::get<1>(*(*it)->cornerpoint), std::get<0>(*(*it)->cornerpoint), this));
				}
			}
		}
		else if (this->type == Cut)
		{
			lEnd = this->leftChild->cornerpoints.end();
			rEnd = this->rightChild->cornerpoints.end();
			for (lIt = this->leftChild->cornerpoints.begin(); lIt != lEnd; ++it)
			{
				for (rIt = this->rightChild->cornerpoints.begin(); rIt != rEnd; ++rIt)
				{
					for (it = this->cornerpoints.begin(); it != this->cornerpoints.end(); ++it)
					{
						if (this->cutType == HorizontalCut)
						{
							if (!((*it)->cornerpoint <= (currentPoint = &std::make_tuple(std::get<0>(*(*lIt)->cornerpoint) + std::get<0>(*(*rIt)->cornerpoint), std::max(std::get<1>(*(*lIt)->cornerpoint), std::get<1>(*(*rIt)->cornerpoint))))))
							{
								this->cornerpoints.push_back(currentCornerpoint = new CCornerpoint(currentPoint, this));
								currentCornerpoint->leftParent = *lIt;
								currentCornerpoint->rightParent = *rIt;
							}
						}
						else if (this->cutType == VerticalCut)
						{
							if (!((*it)->cornerpoint <= (currentPoint = &std::make_tuple(std::max(std::get<0>(*(*lIt)->cornerpoint), std::get<0>(*(*rIt)->cornerpoint)), std::get<1>(*(*lIt)->cornerpoint) + std::get<1>(*(*rIt)->cornerpoint)))))
							{
								this->cornerpoints.push_back(currentCornerpoint = new CCornerpoint(currentPoint, this));
								currentCornerpoint->leftParent = *lIt;
								currentCornerpoint->rightParent = *rIt;
							}
						}
					}
				}
			}
		}

		this->parent->regenerateCornerpoints = true;
		this->regenerateCornerpoints = false;

		currentModule = NULL;
		currentPoint = NULL;
		currentCornerpoint = NULL;
	}

	return (0);
}

CPostfixExpressionElement::CPostfixExpressionElement(SlicingTreeExpressionElementType elementType, CSlicingTreeNode *correspondingTreeNode):
elementType(elementType), correspondingTreeNode(correspondingTreeNode)
{
	;
}

CPostfixExpressionElement::~CPostfixExpressionElement()
{
	this->correspondingTreeNode = NULL;
}

SlicingTreeExpressionElementType CPostfixExpressionElement::getElementType()
{
	return (this->elementType);
}

void CPostfixExpressionElement::setElementType(SlicingTreeExpressionElementType type)
{
	this->elementType = type;
}

CSlicingTreeNode *CPostfixExpressionElement::getCorrespondingTreeNode()
{
	return (this->correspondingTreeNode);
}

int CPostfixExpressionElement::swapElements(CPostfixExpressionElement *leftElement, CPostfixExpressionElement *rightElement)
{
	if (leftElement != NULL && rightElement != NULL)
	{
		SlicingTreeExpressionElementType elementType;
		CSlicingTreeNode *correspondingTreeNode;

		elementType = leftElement->elementType;
		correspondingTreeNode = leftElement->correspondingTreeNode;

		leftElement->elementType = rightElement->elementType;
		leftElement->correspondingTreeNode = rightElement->correspondingTreeNode;

		rightElement->elementType = elementType;
		rightElement->correspondingTreeNode = correspondingTreeNode;

		return (0);
	}
	else return (1);
}

CSlicingTree::CSlicingTree(CModulelist *modulelist):
root(NULL), numNodes(0), p(0), q(0), minAreaCornerpoint(NULL), minArea(DBL_MAX)
{
	CSlicingTreeNode *currentNode;
	CSlicingTreeNode::SlicingTreeNodeCutType currentNodeCutType;

	this->postfixExp.clear();

	if (modulelist && (this->numNodes = modulelist->getNumModules()))
	{
		this->p = modulelist->getP();
		this->q = modulelist->getQ();

		this->root = new CSlicingTreeNode(CSlicingTreeNode::HorizontalCut);
		this->postfixExp.emplace_front(exp_HorizontalCut, this->root);
		currentNode = this->root;

		for (unsigned i = 0; i < this->numNodes - 2; i++)
		{
			currentNode->rightChild = new CSlicingTreeNode(currentNodeCutType = (currentNode->cutType == CSlicingTreeNode::HorizontalCut ? CSlicingTreeNode::VerticalCut : CSlicingTreeNode::HorizontalCut));
			currentNode->rightChild->isLeftChild = false;
			currentNode->rightChild->parent = currentNode;
			this->postfixExp.emplace_front(currentNodeCutType == CSlicingTreeNode::HorizontalCut ? exp_HorizontalCut : exp_VerticalCut, currentNode->rightChild);
			currentNode->leftChild = new CSlicingTreeNode(modulelist->getModules()[i]);
			currentNode->leftChild->isLeftChild = true;
			currentNode->leftChild->parent = currentNode;
			this->postfixExp.emplace_front(exp_Module, currentNode->leftChild);
			currentNode = currentNode->rightChild;
		}

		currentNode->rightChild = new CSlicingTreeNode(modulelist->getModules()[this->numNodes - 1]);
		currentNode->rightChild->isLeftChild = false;
		currentNode->rightChild->parent = currentNode;
		this->postfixExp.emplace_front(exp_Module, currentNode->rightChild);
		currentNode->leftChild = new CSlicingTreeNode(modulelist->getModules()[this->numNodes - 2]);
		currentNode->leftChild->isLeftChild = true;
		currentNode->leftChild->parent = currentNode;
		this->postfixExp.emplace_front(exp_Module, currentNode->leftChild);
	}

	currentNode = NULL;
}

CSlicingTree::~CSlicingTree()
{
	delete this->root;
	this->root = NULL;
}

unsigned CSlicingTree::getNumNodes()
{
	return (this->numNodes);
}

CCornerpoint *CSlicingTree::getMinAreaCornerpoint()
{
	if (!this->minAreaCornerpoint)
		this->calculateMinArea();
	return (this->minAreaCornerpoint);
}

double CSlicingTree::getMinArea()
{
	if (!this->minAreaCornerpoint)
		this->calculateMinArea();
	return (this->minArea);
}

int CSlicingTree::_swapExpressionElements(CPostfixExpressionElement *leftElement, CPostfixExpressionElement *rightElement)
{
	return (CPostfixExpressionElement::swapElements(leftElement, rightElement));
}

int CSlicingTree::_swapNodes(CSlicingTreeNode *leftNode, CSlicingTreeNode *rightNode)			// Sub-level of the swapNodes rutine does the dirty work
{
	if (leftNode->isLeaf == rightNode->isLeaf == true)										// Move1: Simply swap the two leaf nodes
	{
		CSlicingTreeNode *parent, *leftChild, *rightChild;
		bool isLeftChild;

		parent = leftNode->parent;
		leftChild = leftNode->leftChild;
		rightChild = leftNode->rightChild;
		isLeftChild = rightNode->isLeftChild;

		leftNode->parent = rightNode->parent;
		leftNode->leftChild = rightNode->leftChild;
		leftNode->rightChild = rightNode->rightChild;
		if (rightNode->isLeftChild)
		{
			rightNode->parent->leftChild = leftNode;
			leftNode->isLeftChild = true;
		}
		else
		{
			rightNode->parent->rightChild = leftNode;
			leftNode->isLeftChild = false;
		}
		rightNode->parent = parent;
		rightNode->leftChild = leftChild;
		rightNode->rightChild = rightChild;
		if (isLeftChild)
		{
			parent->leftChild = rightNode;
			rightNode->isLeftChild = true;
		}
		else
		{
			parent->rightChild = rightNode;
			rightNode->isLeftChild = false;
		}

		parent = leftChild = rightChild = NULL;

		leftNode->parent->regenerateCornerpoints = rightNode->parent->regenerateCornerpoints = true;
	}
	else if (leftNode->isLeaf == true)														// Move3: MC Type
	{
		CSlicingTreeNode *&mNode = leftNode, *&cNode = rightNode;

		cNode->rightChild = cNode->leftChild;
		if (cNode->isLeftChild)															// MC Type: Sub-type DP
		{
			cNode->leftChild = this->root->leftChild;
			this->root->leftChild->parent = cNode;
			cNode->parent->leftChild = mNode;
		}
		else																			// MC Type: Sbu-type SP
		{
			cNode->leftChild = cNode->parent->leftChild;
			cNode->parent->rightChild = mNode;
		}
		mNode->parent = cNode->parent;
		this->root->leftChild = cNode;
		cNode->parent = this->root;

		cNode->regenerateCornerpoints = mNode->parent->regenerateCornerpoints = true;
	}
	else if (rightNode->isLeaf == true)														// Move3: CM Type
	{
		CSlicingTreeNode *&cNode = leftNode, *&mNode = rightNode;

		cNode->parent->leftChild = cNode->leftChild;
		cNode->leftChild->parent = cNode->parent;
		cNode->leftChild = cNode->rightChild;
		if (cNode->parent == mNode->parent)												// CM Type: Sub-type SP
		{
			mNode->parent->rightChild = cNode;
		}
		else																			// CM Type: Sub-Type DP
		{
			mNode->parent->leftChild = cNode;
			cNode->parent = mNode->parent;
		}
		cNode->rightChild = mNode;
		mNode->parent = cNode;

		cNode->regenerateCornerpoints = true;
	}
	else return (1);

	this->minAreaCornerpoint = NULL;

	return (0);
}

int CSlicingTree::swapNodes(int movingPoint, bool isMove1)
{
	CPostfixExpressionElement *leftElement, *rightElement;
	std::deque<CPostfixExpressionElement>::iterator it;
	unsigned operandCount = 0, operatorCount = 0;

	leftElement = &this->postfixExp[movingPoint];
	rightElement = &this->postfixExp[movingPoint + 1];

	if (leftElement->getElementType() != exp_Module && rightElement->getElementType() != exp_Module)				// This valid for Move2, thus not applicable to this function.
		return (1);
	else if (leftElement->getElementType() == exp_Module && isMove1)												// Move1
	{
		
		for (it = this->postfixExp.begin() + movingPoint; (*it).getElementType() != exp_Module; ++it)
		{
			;
		}

		rightElement = &*it;
	}
	else if (this->postfixExp[movingPoint - 1].getElementType() == rightElement->getElementType() != exp_Module || 
		leftElement->getElementType() == this->postfixExp[movingPoint + 2].getElementType() != exp_Module)			// Move3's Skewed Tree check
		return (1);
	else if (leftElement->getElementType() == exp_Module && rightElement->getElementType() != exp_Module)			// Move3's Balloting Property check
	{
		for (it = this->postfixExp.begin(); it != this->postfixExp.begin() + movingPoint; ++it)
		{
			if ((*it).getElementType() == exp_Module)
				operandCount++;
			else
				operatorCount++;
		}

		if (operandCount <= operatorCount + 1)
			return (1);
	}

	this->_swapExpressionElements(leftElement, rightElement);

	return (this->_swapNodes(leftElement->getCorrespondingTreeNode(), rightElement->getCorrespondingTreeNode()));
}

int CSlicingTree::_complementCuts(CSlicingTreeNode *startNode, CSlicingTreeNode *endNode)
{
	int error = 0;

	CSlicingTreeNode *currentNode = startNode;

	while (currentNode != endNode->parent)
	{
		if (error |= currentNode->complementCutType())
			break;
		currentNode = currentNode->parent;
	}

	this->minAreaCornerpoint = NULL;

	currentNode = NULL;

	return (error);
}

int CSlicingTree::complementCuts(int movingPoint)
{
	CPostfixExpressionElement *startElement, *endElement;
	SlicingTreeExpressionElementType currentElementType;
	std::deque<CPostfixExpressionElement>::iterator mp, it;

	if (this->postfixExp[movingPoint].getElementType() == exp_Module)
		return (1);

	for (it = mp = this->postfixExp.begin() + movingPoint; (currentElementType = (*it).getElementType()) != exp_Module && it != this->postfixExp.begin(); --it)
	{
		(*it).setElementType(currentElementType == exp_HorizontalCut ? exp_VerticalCut : exp_HorizontalCut);
	}
	startElement = &*(it == this->postfixExp.begin() ? it : it + 1);
	for (it = mp + 1; (currentElementType = (*it).getElementType()) != exp_Module && it != this->postfixExp.end(); ++it)
	{
		(*it).setElementType(currentElementType == exp_HorizontalCut ? exp_VerticalCut : exp_HorizontalCut);
	}
	endElement = &*(it == this->postfixExp.end() ? it : it - 1);

	return (this->_complementCuts(startElement->getCorrespondingTreeNode(), endElement->getCorrespondingTreeNode()));
}

int CSlicingTree::calculateMinArea()
{
	int error = 0;

	double area;

	if (!(error |= CSlicingTree::dfsTree(CSlicingTreeNode::op_nullOp, CSlicingTreeNode::op_generateCornerpoints, (void*)NULL)))
	{
		this->minArea = DBL_MAX;

		for (std::list<CCornerpoint*>::iterator it = this->root->cornerpoints.begin(); it != this->root->cornerpoints.end(); ++it)
		{
			if (*it && (*it)->getRatio() >= this->p && (*it)->getRatio() <= this->q && (area = (*it)->getArea()) < this->minArea)
			{
				this->minAreaCornerpoint = *it;
				this->minArea = area;
			}
		}
	}

	error |= CCornerpoint::dfsCorenrpoint(this->minAreaCornerpoint, CCornerpoint::op_writeOptimalShape, CCornerpoint::op_nullOp, (void *)NULL);
	
	return (error);
}
