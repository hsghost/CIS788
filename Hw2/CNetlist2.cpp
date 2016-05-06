/*
/
*/

#include <vector>
#include "CNetlist2.h"

CCell::CCell(int cellID, int cellWidth, int cellHeight, int pinX, int pinY, bool movable, int netID) :
cellID(cellID), movable(movable)
{
	this->cellDimentions[0] = cellWidth;
	this->cellDimentions[1] = cellHeight;
	this->cellPinPosition[0] = pinX;
	this->cellPinPosition[1] = pinY;
}

CCell::CCell(int cellID, int cellWidth, int cellHeight, int pinX, int pinY, bool movable, int netID, int fixedCoordinateX, int fixedCoordinateY) :
cellID(cellID), movable(movable)
{
	this->cellDimentions[0] = cellWidth;
	this->cellDimentions[1] = cellHeight;
	this->cellPinPosition[0] = pinX;
	this->cellPinPosition[1] = pinY;
	this->cellPosition[0] = fixedCoordinateX;
	this->cellPosition[1] = fixedCoordinateY;
}

int CCell::getCellID()
{
	return (this->cellID);
}

int* CCell::getCellPinPosition()
{
	return (this->cellPinPosition);
}

int* CCell::getCellPosition()
{
	return (this->cellPosition);
}

bool CCell::is_movable()
{
	return (this->movable);
}

CNet::CNet(int netID, int netWeight) :
netID(netID), netWeight(netWeight)
{
	;
}

CNet::~CNet()
{
	for (int i = 0; i < this->cells.size(); i++)
	{
		// delete this->cells[i];
		this->cells[i] = NULL;
	}
}

int CNet::getNetID()
{
	return (this->netID);
}

int CNet::getNetWeight()
{
	return (this->netWeight);
}

int* CNet::getNetCenter()
{
	return (this->netCenter);
}

std::vector<CCell*> CNet::getCells()
{
	return (this->cells);
}


CNetlist2::CNetlist2() :
numCells(0), movableCells(0), fixedCells(0), numNets(0), width(0), height(0)
{
	;
}

CNetlist2::~CNetlist2()
{
	for (int i = 0; i < this->nets.size(); i++)
	{
		delete this->nets[i];
		this->nets[i] = NULL;
	}
}

int CNetlist2::getMovableCells()
{
	return (this->movableCells);
}

int CNetlist2::getNumCells()
{
	return (this->numCells);
}

int CNetlist2::getNumNets()
{
	return (this->numNets);
}

std::vector<CNet*> CNetlist2::getNets()
{
	return (this->nets);
}
