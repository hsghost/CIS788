/*
/
*/

#include <chrono>
#include <random>
#include <cmath>
#include "CAnnealing.h"

int CAnnealing::Move1(int movingPoint)
{
	return (this->slicingTree->swapNodes(movingPoint, true));
}

int CAnnealing::Move2(int movingPoint)
{
	return (this->slicingTree->complementCuts(movingPoint));
}

int CAnnealing::Move3(int movingPoint)
{
	return (this->slicingTree->swapNodes(movingPoint, false));
}

CAnnealing::CAnnealing(CSlicingTree *slicingTree, unsigned M, double T, double alpha, double terminationRatio) :
slicingTree(slicingTree), M(M), T(T), alpha(alpha), terminationRatio(terminationRatio)
{
	this->E = this->slicingTree->getMinArea();
	this->seed = std::chrono::system_clock::now().time_since_epoch().count();
	CAnnealing::moves[0] = &CAnnealing::Move1;
	CAnnealing::moves[1] = &CAnnealing::Move2;
	CAnnealing::moves[2] = &CAnnealing::Move3;
}

CAnnealing::~CAnnealing()
{
	this->slicingTree = NULL;
}

int CAnnealing::metropolis()
{
	int error = 0, rejections = 0;
	int mv, op;
	double deltaE;

	std::uniform_int_distribution<int> mvDist(0, 2);
	auto mvDice = std::bind(mvDist, this->generator);
	std::uniform_int_distribution<int> opDist(0, this->slicingTree->getNumNodes() - 2);
	auto opDice = std::bind(opDist, this->generator);
	std::uniform_real_distribution<double> rDist(0, 1);
	auto rDice = std::bind(rDist, this->generator);

	for (unsigned i = 0; i < this->M; i++)
	{
		mv = mvDice();

		do
		{
			error |= (this->*(CAnnealing::moves[mv]))(op = opDice());
		} while (!error);
		
		if ((deltaE = this->slicingTree->getMinArea() - this->E) >= 0)
		{
			if (rDice() >= std::exp(-deltaE / this->T))
			{
				rejections++;
				error = (this->*(CAnnealing::moves[mv]))(op);
			}
		}
		
		this->E = this->slicingTree->getMinArea();
	}

	if (error)
		return (-1);
	else if (rejections / this->M > this->terminationRatio)
	{
		return (1);
	}
	else return (0);
}

int CAnnealing::doAnnealing()
{
	int error = 0;

	while (!(error = this->metropolis()))
	{
		this->T *= this->alpha;
	}

	if (error < 0) return (-1);
	else return (0);
}
