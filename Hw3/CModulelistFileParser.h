/*
/
*/

#include <string>
#include <fstream>
#include <regex>
#include "CModulelist.h"

#ifndef __CModulelistFileParser_H__
#define __CModulelistFileParser_H__

class CModulelistFileParser
{
public:
	CModulelistFileParser(std::ifstream *modulelistFile, CModulelist *modulelist);
	~CModulelistFileParser();

private:
	std::ifstream *modulelistFile;
	CModulelist *modulelist;

public:
	int parseFile();
};

#endif