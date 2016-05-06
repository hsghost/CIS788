// regex_match example
#include <iostream>
#include "CNetlist2.h"
#include "CNetlistFileParser.h"
#include "CGORDIAN.h"

int main()
{
	CNetlist2 *netlist = new CNetlist2();
	AllCells *allCells = new AllCells();

	std::ifstream *netlistFile = new std::ifstream();
	netlistFile->open("netlist.txt");
	CNetlistFileParser *netlistFileParser = new CNetlistFileParser(netlistFile, netlist, allCells);
	netlistFileParser->ParseFile();
	delete netlistFileParser;
	netlistFileParser = NULL;
	delete netlistFile;
	netlistFile = NULL;

	int numCells = netlist->getNumCells();

	MatrixSd mat_C(numCells, numCells);
	MatrixSd vec_dx(numCells, 1);
	MatrixSd vec_dy(numCells, 1);
	mat_C.reserve(numCells * 2);
	vec_dx.reserve(numCells * 2 / 3 + 1);
	vec_dy.reserve(numCells * 2 / 3 + 1);

	CGORDIAN *GORDIAN = new CGORDIAN(netlist, allCells, mat_C, vec_dx, vec_dy);
	GORDIAN->doGORDIAN();
	delete GORDIAN;
	GORDIAN = NULL;

	delete netlist;
	netlist = NULL;
	for (AllCells::iterator it = allCells->begin(); it != allCells->end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
	delete allCells;
	allCells = NULL;

#ifdef _DEBUG
	Eigen::MatrixXd d_mat_C = Eigen::MatrixXd(mat_C);
	Eigen::MatrixXd d_vec_dx = Eigen::MatrixXd(vec_dx);
	Eigen::MatrixXd d_vec_dy = Eigen::MatrixXd(vec_dy);

	std::cout << d_mat_C << std::endl << std::endl;
	std::cout << d_vec_dx << std::endl << std::endl;
	std::cout << d_vec_dy << std::endl << std::endl;
#endif

	std::cout << std::endl;
	std::cin.get();

	return (0);
}
