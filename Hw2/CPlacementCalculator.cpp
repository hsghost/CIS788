/*
/
*/

#include "CPlacementCalculator.h"

CPlacementCalculator::CPlacementCalculator(CNetlist2 *netlist, AllCells *allCells, Eigen::MatrixXd &mat_C, Eigen::VectorXd &vec_dx, Eigen::VectorXd &vec_dy) :
netlist(netlist), allCells(allCells), mat_C(mat_C), vec_dx(vec_dx), vec_dy(vec_dy)
{
	;
}

CPlacementCalculator::~CPlacementCalculator()
{
	this->netlist = NULL;
	this->allCells = NULL;
}
