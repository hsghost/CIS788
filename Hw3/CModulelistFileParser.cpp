/*
/
*/

#include "CModulelistFileParser.h"

CModulelistFileParser::CModulelistFileParser(std::ifstream *modulelistFile, CModulelist *modulelist):
modulelistFile(modulelistFile), modulelist(modulelist)
{
	;
}

CModulelistFileParser::~CModulelistFileParser()
{
	this->modulelistFile = NULL;
	this->modulelist = NULL;
}

int CModulelistFileParser::parseFile()
{
	if (this->modulelistFile->is_open())
	{
		std::string line;
		line.reserve(64);
		CModule *module = NULL;
		int moduleID;

		std::regex reg_Modulelist = std::regex("^([0-9]+) +?([0-9]+.[0-9]+) +?([0-9]+.[0-9]+)$");
		std::regex reg_Module = std::regex("^([0-9]+) +?([0-9]+.[0-9]+) +?([0-9]+.[0-9]+) +?([0-9]+.[0-9]+) +?([1-2])$");

		std::smatch sm;

		while (this->modulelistFile->good())
		{
			std::getline(*this->modulelistFile, line);

			if (std::regex_match(line, sm, reg_Modulelist))
			{
				this->modulelist->numModules = std::atoi(sm[1].str().c_str());
				this->modulelist->p = std::atoi(sm[2].str().c_str());
				this->modulelist->q = std::atoi(sm[3].str().c_str());
			}
			else if (std::regex_match(line, sm, reg_Module))
			{
				module = new CModule(std::atoi(sm[1].str().c_str()), std::atof(sm[2].str().c_str()), std::atof(sm[3].str().c_str()), std::atof(sm[4].str().c_str()), std::atoi(sm[5].str().c_str()) == 2 ? true : false);
				this->modulelist->modules.push_back(module);
			}
			else
			{
				; // TODO: Throw some exception.
			}
		}
		return (0);
	}
	else
		return (1);
}
