/*
/
*/

#include "hw1.h"

int main()
{
	COddTree *oddTree = new COddTree();

	std::string *inputExp = new std::string();
	std::cout << "Please enter a valid expression: " << std::endl << "> ";
	std::cin >> *inputExp;
	std::cout << std::endl;

	COddTreeBuilder *oddTreeBuilder = new COddTreeBuilder(oddTree);
	oddTreeBuilder->buildOddTree(inputExp);
	delete oddTreeBuilder;
	oddTreeBuilder = NULL;

	delete inputExp;
	inputExp = NULL;

	CNetlist *netlist = new CNetlist();

	COddTreeParser *oddTreeParser = new COddTreeParser(oddTree, &gateSequence, netlist);
	oddTreeParser->parseOddTree();
	delete oddTreeParser;
	oddTreeParser = NULL;

	delete oddTree;
	oddTree = NULL;

	CDisplayAdaptor *displayAdaptor = new CDisplayAdaptor(&gateSequence, netlist, &netlistString);
	displayAdaptor->trimGateSequence();
	displayAdaptor->serializeNetlist();
	delete displayAdaptor;
	displayAdaptor = NULL;

	delete netlist;
	netlist = NULL;

	std::cout << "Polysilicon Gate Sequence: " << gateSequence << std::endl;
	std::cout << std::endl;
	std::cout << netlistString << std::endl;

	system("PAUSE");

	return (0);
}