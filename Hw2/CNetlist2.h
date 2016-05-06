/*
/
*/

#include <vector>
#include <unordered_map>

#ifndef __CNetlist2_H__
#define __CNetlist2_H__

#define DEFAULT_NET_WEIGHT 1

class CCell
{
	friend class CNetlistFileParser;
	friend class CGORDIAN;

public:
	CCell(int cellID, int cellWidth, int cellHeight, int pinX, int pinY, bool movable, int netID);
	CCell(int cellID, int cellWidth, int cellHeight, int pinX, int pinY, bool movable, int netID, int fixedCoordinateX, int fixedCoordinateY);

private:
	int cellID;
	int cellDimentions[2];
	int cellPinPosition[2];
	int cellPosition[2];
	bool movable;
	std::vector<int> nets;

public:
	int getCellID();
	int* getCellPinPosition();
	int* getCellPosition();
	bool is_movable();
};

typedef std::unordered_map<int, CCell*> AllCells;

class CNet
{
	friend class CNetlistFileParser;
	friend class CGORDIAN;

public:
	CNet(int netID, int netWeight = DEFAULT_NET_WEIGHT);
	~CNet();

private:
	int netID;
	int netWeight;
	int netCenter[2];
	std::vector<CCell*> cells;

public:
	int getNetID();
	int getNetWeight();
	int* getNetCenter();
	std::vector<CCell*> getCells();
};

class CNetlist2
{
	friend class CNetlistFileParser;

public:
	CNetlist2();
	~CNetlist2();

private:
	int numCells;
	int movableCells;
	int fixedCells;
	int numNets;
	int width;
	int height;
	std::vector<CNet*> nets;

public:
	int getNumCells();
	int getNumNets();
	int getMovableCells();
	std::vector<CNet*> getNets();
};

#endif