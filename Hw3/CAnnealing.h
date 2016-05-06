/*
/
*/

#include <vector>
#include <functional>
#include <random>
#include "CSlicingTree.h"

#ifndef __CAnnealing_H__
#define __CAnnealing_H__

class CAnnealing;
typedef int (CAnnealing::*Moves)(int movingPoint);

class CAnnealing
{
public:
	CAnnealing(CSlicingTree *slicingTree, unsigned M, double T, double alpha, double terminationRatio);
	~CAnnealing();

private:
	CSlicingTree *slicingTree;
	unsigned M;
	double T, alpha;
	double E;
	double terminationRatio;
	unsigned seed;
	std::default_random_engine generator;

	struct RawData
	{
		double minArea;
		double tempreture;
		double probability;
	};
	std::vector<RawData> rawData;

	int Move1(int movingPoint),
		Move2(int movingPoint),
		Move3(int movingPoint);
	Moves moves[3];
	int metropolis();

public:
	int doAnnealing();
};

#endif