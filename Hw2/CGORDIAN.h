/*
/
*/

#include <Eigen/Sparse>
#include <Eigen/Dense>
#include "GMatrices.h"
#include "CNetlist2.h"

#ifndef __CGORDIAN_H__
#define __CGORDIAN_H__

class CGPartition
{
	friend class CGORDIAN;

public:
	CGPartition();
	~CGPartition();

private:
	int centerCoordinate[2];
	int leftTopCoordinate[2];
	int rightBottomCoordinate[2];
	int weight;
	std::list<CCell*> cells;

};

class CGShuffledMatrices
{
	friend class CGORDIAN;
public:
	CGShuffledMatrices();
	~CGShuffledMatrices();

private:
	MatrixSd mat_C_Sh;
	MatrixSd vec_dx_Sh, vec_dy_Sh;
	MatrixSd vec_x0_Sh, vec_y0_Sh;
	MatrixSd vec_x_Sh, vec_y_Sh;
	MatrixSd vec_ux_Sh, vec_uy_Sh;
};

class CGORDIAN
{
public:
	CGORDIAN(CNetlist2 *netlist, AllCells *allCells, MatrixSd &mat_C, MatrixSd &vec_dx, MatrixSd &vec_dy);
	~CGORDIAN();

private:
	CNetlist2 *netlist;
	AllCells *allCells;
	MatrixSd &mat_C;
	MatrixSd &vec_dx, &vec_dy;
	MatrixSd mat_Zx, mat_Zy;
	MatrixSd mat_Dx, mat_Bx, mat_Dy, mat_By;
	MatrixSd vec_x0, vec_y0;
	MatrixSd vec_x, vec_y;
	MatrixSd vec_cx, vec_cy;
	MatrixSd vec_ux, vec_uy;
	std::list<CGPartition*> partitions;
	int numCells;
	int numNets;
	int numPartitions;

public:
	int doGORDIAN();
	int calculateCoodinate();
	int calculatePlacement();
	int doCut();
	int shuffleMatrices(CGShuffledMatrices *sh_Mtx, std::vector<int> order);
};

#endif
