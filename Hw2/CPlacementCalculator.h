/*
/
*/

#include <Eigen/Dense>
#include <vector>
#include "CNetlist2.h"
#include "GMatrices.h"

#ifndef __CPlacementCalculator_H__
#define __CPlacementCalculator_H__

class CPlacementCalculator
{
public:
	CPlacementCalculator(CNetlist2 *netlist, AllCells *allCells, MatrixSd &mat_C, MatrixSd &vec_dx, MatrixSd &vec_dy);
	~CPlacementCalculator();

private:
	CNetlist2 *netlist;
	AllCells *allCells;
	MatrixSd &mat_C;
	MatrixSd &vec_dx;
	MatrixSd &vec_dy;
	MatrixSd Z;
	MatrixSd D, B;
	MatrixSd x0;
	MatrixSd x;
	MatrixSd c;

public:
	int CalculatePlacement();
};

#endif