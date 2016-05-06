/*
/
*/

#ifndef __COddTreeParser_H__
#define __COddTreeParser_H__

#include <string>
#include "COddTree.h"

class COddTreeParser
{
public:
	COddTreeParser(COddTree *tree, std::string *seqence, CNetlist *netlist);
	virtual ~COddTreeParser();

private:
	COddTree *oddTree;
	std::string *gateSequence;
	CNetlist *netlist;

public:
	void getNetlist();
	void parseOddTree();
};

#endif

