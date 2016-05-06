/*
/
*/

#include <iostream>
#include "CModulelistFileParser.h"
#include "CAnnealing.h"
#include "CDisplayAdaptor.h"

#define NUM_MOVES 200
#define STARTING_TEMPERATURE 2000
#define COOLING_RATIO 0.95
#define TERMINATION_RATIO 0.05

int main()
{
	CModulelist *modulelist = new CModulelist();

	std::ifstream *modulistFile = new std::ifstream();
	modulistFile->open("modulelist.txt");
	CModulelistFileParser *modulelistFileParser = new CModulelistFileParser(modulistFile, modulelist);
	modulelistFileParser->parseFile();
	delete modulelistFileParser;
	modulelistFileParser = NULL;
	delete modulistFile;
	modulistFile = NULL;

	CSlicingTree *slicingTree = new CSlicingTree(modulelist);

	CAnnealing *annealing = new CAnnealing(slicingTree, NUM_MOVES, STARTING_TEMPERATURE, COOLING_RATIO, TERMINATION_RATIO);
	annealing->doAnnealing();

	return (0);

}
