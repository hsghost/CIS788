/*
/
*/

#include <string>
#include <fstream>
#include <unordered_map>
#include <regex>
#include "CNetlist2.h"

#ifndef __CNetlistFileParser_H__
#define __CNetlistFileParser_H__

class CCellTracker
{
	int cellID;
	CCell *cell;
};

class CNetlistFileParser
{
public:
	CNetlistFileParser(std::ifstream *netlistFile, CNetlist2 *netlist, AllCells *allCells);
	~CNetlistFileParser();

private:
	std::ifstream *netlistFile;
	CNetlist2 *netlist;
	AllCells *allCells;

public:
	int ParseFile();
};

#endif
