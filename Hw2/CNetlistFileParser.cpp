/*
/
*/

//#include <iostream>
#include "CNetlistFileParser.h"

CNetlistFileParser::CNetlistFileParser(std::ifstream *netlistFile, CNetlist2 *netlist, AllCells *allCells) :
netlistFile(netlistFile), netlist(netlist), allCells(allCells)
{
	;
}

CNetlistFileParser::~CNetlistFileParser()
{
	this->netlistFile = NULL;
	this->netlist = NULL;
	this->allCells = NULL;
}

int CNetlistFileParser::ParseFile()
{
	if (this->netlistFile->is_open())
	{
		std::string line;
		line.reserve(64);
		CNet *net = NULL;
		CCell *cell = NULL;
		int cellID;

		std::regex reg_NumCells = std::regex("^(Num_Cells=)+?([0-9]+)$");
		std::regex reg_NumMcells = std::regex("^(Num_Mcells=)+?([0-9]+)$");
		std::regex reg_NumFcells = std::regex("^(Num_Fcells=)+?([0-9]+)$");
		std::regex reg_NumNets = std::regex("^(Num_Nets=)+?([0-9]+)$");
		std::regex reg_Width = std::regex("^(W=)+?([0-9]+)$");
		std::regex reg_Height = std::regex("^(H=)+?([0-9]+)$");
		std::regex reg_Net = std::regex("^N+?\\(+?([0-9]+)\\)+?$");
		std::regex reg_Cell = std::regex("^C+?\\(+?([0-9]+)\\)+?\\(+?([0-9]+),+?\\s*?([0-9]+)\\)+?\\(+?(-*?[0-9]+),+?\\s*?(-*?[0-9]+)\\)+?(F|M)(\\(+?([0-9]+),+?\\s*?([0-9]+)\\)+?)*$");

		std::smatch sm;
		// std::cmatch cm;
		std::unordered_map<int, CCell*>::iterator allCells_it;

		while (this->netlistFile->good())
		{
			std::getline(*this->netlistFile, line);
			if (std::regex_match(line, sm, reg_NumCells))
			{
				this->netlist->numCells = atoi(sm[2].str().c_str());
				this->allCells->reserve(this->netlist->numCells);
			}
			else if (std::regex_match(line, sm, reg_NumMcells))
			{
				this->netlist->movableCells = atoi(sm[2].str().c_str());
			}
			else if (std::regex_match(line, sm, reg_NumFcells))
			{
				this->netlist->fixedCells = atoi(sm[2].str().c_str());
			}
			else if (std::regex_match(line, sm, reg_NumNets))
			{
				this->netlist->numNets = atoi(sm[2].str().c_str());
				this->netlist->nets.reserve(this->netlist->numNets);
			}
			else if (std::regex_match(line, sm, reg_Width))
			{
				this->netlist->width = atoi(sm[2].str().c_str());
			}
			else if (std::regex_match(line, sm, reg_Height))
			{
				this->netlist->height = atoi(sm[2].str().c_str());
			}
			else if (std::regex_match(line, sm, reg_Net))
			{
				net = new CNet(atoi(sm[1].str().c_str()));
				this->netlist->nets.push_back(net);
			}
			else if (std::regex_match(line, sm, reg_Cell))
			{
				cellID = atoi(sm[1].str().c_str());
				if ((allCells_it = this->allCells->find(cellID)) == this->allCells->end())
				{
					if (sm[6].str() == "F")
						cell = new CCell(cellID, atoi(sm[2].str().c_str()), atoi(sm[3].str().c_str()), atoi(sm[4].str().c_str()), atoi(sm[5].str().c_str()), false, net->netID, atoi(sm[8].str().c_str()), atoi(sm[9].str().c_str()));
					else if (sm[6].str() == "M")
						cell = new CCell(cellID, atoi(sm[2].str().c_str()), atoi(sm[3].str().c_str()), atoi(sm[4].str().c_str()), atoi(sm[5].str().c_str()), true, net->netID);
					else
						; // TODO: Throw some exception.
					allCells_it = this->allCells->insert({ cell->cellID, cell }).first;
				}
				allCells_it->second->nets.push_back(net->netID);
				net->cells.push_back(allCells_it->second);
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