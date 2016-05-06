/*
/
*/

#ifndef __COddTreeBuilder_H__
#define __COddTreeBuilder_H__

#include <string>
#include <stack>
#include "COddTree.h"

class COddTreeBuilderException
{
public:
	COddTreeBuilderException(int eNum);

	int eNum;
};

class COddTreeBuilder
{
public:
	COddTreeBuilder(COddTree *tree);
	virtual ~COddTreeBuilder();

private:
	COddTree *oddTree;
	int psuedoCount;
	COddTreeBuilderException *e;
	std::stack<char> *parStack;

public:
	void buildOddTree(std::string *inputExp);
};

#endif