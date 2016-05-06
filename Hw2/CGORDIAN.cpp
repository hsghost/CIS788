/*
/
*/

#include "CGORDIAN.h"

CGORDIAN::CGORDIAN(CNetlist2 *netlist, AllCells *allCells, MatrixSd &mat_C, MatrixSd &vec_dx, MatrixSd &vec_dy) :
netlist(netlist), allCells(allCells), mat_C(mat_C), vec_dx(vec_dx), vec_dy(vec_dy)
{
	;
}

CGORDIAN::~CGORDIAN()
{
	this->netlist = NULL;
	this->allCells = NULL;
}

int CGORDIAN::doGORDIAN()
{
	mat_C.setZero();
	vec_dx.setZero();
	vec_dy.setZero();

	int numNets = this->netlist->getNumNets();
	CNet *currentNet;
	CCell *currentCell;
	CCell *currentSibling;
	size_t numCurrentNetCells;
	int currentCellIndex;
	int currentSiblingIndex;
	double currentC;
	GTup tup;
	std::unordered_map<GTup, GTri, GC_hash> nonZerosC;
	std::unordered_map<int, GTri> nonZerosdx;
	std::unordered_map<int, GTri> nonZerosdy;
	double e;


	for (int i = 0; i < numNets; i++)
	{
		currentNet = this->netlist->getNets()[i];
		numCurrentNetCells = currentNet->getCells().size();
		e = currentNet->getNetWeight() * 2 / numCurrentNetCells;
		
		for (unsigned j = 0; j < numCurrentNetCells; j++)
		{
			currentCell = currentNet->getCells()[j];
			if (currentCell->is_movable())
			{
				currentCellIndex = currentCell->getCellID() - 1;
				currentC = e * (numCurrentNetCells - 1);
				if (nonZerosC.find(tup = std::make_tuple(currentCellIndex, currentCellIndex)) == nonZerosC.end())
					nonZerosC[tup] = GTri(currentCellIndex, currentCellIndex, currentC);
				else
					nonZerosC[tup] = GTri(currentCellIndex, currentCellIndex, nonZerosC[tup].value() + currentC);
				if (nonZerosdx.find(currentCellIndex) == nonZerosdx.end())
					nonZerosdx[currentCellIndex] = GTri(currentCellIndex, 0, currentCell->getCellPinPosition()[0] * currentC);
				else
					nonZerosdx[currentCellIndex] = GTri(currentCellIndex, 0, nonZerosdx[currentCellIndex].value() + currentCell->getCellPinPosition()[0] * currentC);
				if (nonZerosdy.find(currentCellIndex) == nonZerosdy.end())
					nonZerosdy[currentCellIndex] = GTri(currentCellIndex, 0, currentCell->getCellPinPosition()[1] * currentC);
				else
					nonZerosdy[currentCellIndex] = GTri(currentCellIndex, 0, nonZerosdy[currentCellIndex].value() + currentCell->getCellPinPosition()[1] * currentC);
				for (unsigned k = 0; k < numCurrentNetCells; k++)
				{
					currentSibling = currentNet->getCells()[k];
					currentSiblingIndex = currentSibling->getCellID() - 1;

					if (currentSibling != currentCell)
					{
						if (currentSibling->is_movable())
						{
							if (nonZerosC.find(tup = std::make_tuple(currentCellIndex, currentSiblingIndex)) == nonZerosC.end())
								nonZerosC[tup] = GTri(currentCellIndex, currentSiblingIndex, -e);
							else
								nonZerosC[tup] = GTri(currentCellIndex, currentSiblingIndex, nonZerosC[tup].value() - e);
							if (nonZerosdx.find(currentCellIndex) == nonZerosdx.end())
								nonZerosdx[currentCellIndex] = GTri(currentCellIndex, 0, -(currentCell->getCellPinPosition()[0]) * e);
							else
								nonZerosdx[currentCellIndex] = GTri(currentCellIndex, 0, nonZerosdx[currentCellIndex].value() - currentCell->getCellPinPosition()[0] * e);
							if (nonZerosdy.find(currentCellIndex) == nonZerosdy.end())
								nonZerosdy[currentCellIndex] = GTri(currentCellIndex, 0, -(currentCell->getCellPinPosition()[1]) * e);
							else
								nonZerosdy[currentCellIndex] = GTri(currentCellIndex, 0, nonZerosdy[currentCellIndex].value() - currentCell->getCellPinPosition()[1] * e);
						}
						else
						{
							if (nonZerosdx.find(currentCellIndex) == nonZerosdx.end())
								nonZerosdx[currentCellIndex] = GTri(currentCellIndex, 0, (-e) * (currentSibling->getCellPosition()[0] + currentSibling->getCellPinPosition()[0]));
							else
								nonZerosdx[currentCellIndex] = GTri(currentCellIndex, 0, nonZerosdx[currentCellIndex].value() - e * (currentSibling->getCellPosition()[0] + currentSibling->getCellPinPosition()[0]));
							if (nonZerosdy.find(currentCellIndex) == nonZerosdy.end())
								nonZerosdy[currentCellIndex] = GTri(currentCellIndex, 0, (-e) * (currentSibling->getCellPosition()[1] + currentSibling->getCellPinPosition()[0]));
							else
								nonZerosdy[currentCellIndex] = GTri(currentCellIndex, 0, nonZerosdy[currentCellIndex].value() - e * (currentSibling->getCellPosition()[0] + currentSibling->getCellPinPosition()[1]));
						}
					}
				}
			}
		}
	}

	std::vector<GTri> v_nonZerosC;
	v_nonZerosC.reserve(nonZerosC.size());
	for (auto kv : nonZerosC)
		v_nonZerosC.push_back(kv.second);
	std::vector<GTri> v_nonZerosdx;
	v_nonZerosdx.reserve(nonZerosdx.size());
	for (auto kv : nonZerosdx)
		v_nonZerosdx.push_back(kv.second);
	std::vector<GTri> v_nonZerosdy;
	v_nonZerosdy.reserve(nonZerosdy.size());
	for (auto kv : nonZerosdy)
		v_nonZerosdy.push_back(kv.second);

	mat_C.setFromTriplets(v_nonZerosC.begin(), v_nonZerosC.end());
	vec_dx.setFromTriplets(v_nonZerosdx.begin(), v_nonZerosdx.end());
	vec_dy.setFromTriplets(v_nonZerosdy.begin(), v_nonZerosdy.end());

	return (0);
}

int CGORDIAN::shuffleMatrices(CGShuffledMatrices *sh_Mtx, std::vector<int> order)
{
	int i = 0;

	for (std::vector<int>::iterator it = order.begin(); it != order.end(); ++it)
	{
		sh_Mtx->mat_C_Sh.col(i) = this->mat_C.col(*it);
		sh_Mtx->vec_dx_Sh.row(i) = this->vec_dx.row(*it);
		sh_Mtx->vec_dy_Sh.row(i) = this->vec_dy.row(*it);
		// sh_Mtx->mat_Ax_Sh.col(i) = this->mat_Ax.col(*it);
		// sh_Mtx->mat_Dx.col(i) = this->mat_Ax
		// sh_Mtx
	}

	return (0);
}

int CGORDIAN::calculatePlacement()
{
	this->vec_ux.setZero();
	this->vec_uy.setZero();
	std::vector<GTri> nonZerosux;
	nonZerosux.reserve(this->numPartitions);
	std::vector<GTri> nonZerosuy;
	nonZerosuy.reserve(this->numPartitions);
	std::vector<GTri> nonZerosDx;
	nonZerosDx.reserve(this->numPartitions);
	std::vector<GTri> nonZerosBx;
	nonZerosBx.reserve(this->numCells - this->numPartitions);
	std::vector<GTri> nonZerosDy;
	nonZerosDy.reserve(this->numCells);
	std::vector<GTri> nonZerosBy;
	nonZerosBy.reserve(this->numCells - this->numPartitions);
	std::vector<int> order;
	order.reserve(this->numCells);

	int partitionCenterX, partitionCenterY;
	int partitionIndex = 0;

	int currentCellSize;
	int maxCellSize;

	int i = 0;

	for (std::list<CGPartition*>::iterator pIt = this->partitions.begin(); pIt != this->partitions.end(); ++pIt)
	{
		maxCellSize = (*pIt)->weight;

		if ((partitionCenterX = (*pIt)->centerCoordinate[0]) != 0)
			nonZerosux.push_back(GTri(partitionIndex, 0, partitionCenterX));
		if ((partitionCenterY = (*pIt)->centerCoordinate[1]) != 0)
			nonZerosux.push_back(GTri(partitionIndex, 0, partitionCenterY));

		for (std::list<CCell*>::iterator cIt = (*pIt)->cells.begin(); cIt != (*pIt)->cells.end(); ++cIt)
		{
			if (maxCellSize < (currentCellSize = (*cIt)->cellDimentions[0] * (*cIt)->cellDimentions[1]))
			{
				order.push_back(order[partitionIndex]);
				order[partitionIndex] = (*cIt)->cellID - 1;

				nonZerosBx.push_back(nonZerosDx[partitionIndex]);
				nonZerosDx[partitionIndex] = GTri(partitionIndex, partitionIndex, currentCellSize / (*pIt)->weight);

				maxCellSize = currentCellSize;
			}
			else
			{
				order.push_back((*cIt)->cellID - 1);

				nonZerosBx.push_back(GTri(partitionIndex, i, currentCellSize / (*pIt)->weight));

				// if ()
			}

			i++;
		}

		partitionIndex++;
	}

	this->vec_ux.setFromTriplets(nonZerosux.begin(), nonZerosux.end());
	this->vec_uy.setFromTriplets(nonZerosuy.begin(), nonZerosuy.end());

	return (0);
}
