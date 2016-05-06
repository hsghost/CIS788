/*
/
*/

#include "COddTreeParser.h"

COddTreeParser::COddTreeParser(COddTree *tree, std::string *sequence, CNetlist *netlist)
{
	this->oddTree = tree;
	this->gateSequence = sequence;
	this->netlist = netlist;
}

COddTreeParser::~COddTreeParser()
{
	this->oddTree = NULL;
	this->gateSequence = NULL;
	this->netlist = NULL;
}

void COddTreeParser::getNetlist()
{
	this->oddTree->dfsTree(op_nullOp, op_getNetInfo, this->netlist);
}

void COddTreeParser::parseOddTree()
{
	int netCount = 0;

	this->oddTree->dfsTree(op_nullOp, op_calculateTriType, (void*) NULL);
	this->oddTree->dfsTree(op_nullOp, op_getSequence, this->gateSequence);
	this->oddTree->dfsTree(op_calculateDirection, op_nullOp, (void*) NULL);
	this->oddTree->dfsTree(op_calculateNet, op_nullOp, &netCount);
	this->getNetlist();
}